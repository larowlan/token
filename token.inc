<?php
// $Id$

/**
 * General function to include the files that token relies on for the real work.
 *
 **/
function token_include() {
  $path = drupal_get_path('module', 'token');
  require_once("$path/token_node.inc");
  require_once("$path/token_user.inc");
  if (module_exists('content')) {
    require_once("$path/token_cck.inc");
  }
  if (module_exists('taxonomy')) {
    require_once("$path/token_taxonomy.inc");
  }
  if (module_exists('comment')) {
    require_once("$path/token_comment.inc");
  }

}


/**
 * Return the value of $original, with all instances of placeholder
 * tokens replaced by their proper values. To replace mutliple types
 * at once see token_replace_multiple().
 *
 * @param original
 *  A string, or an array of strings, to perform token substitutions
 *  on.
 * @param type
 *   A flag indicating the class of substitution tokens to use. If an
 *   object is passed in the second param, 'type' should contain the
 *   object's type. For example, 'node', 'comment', or 'user'. If no
 *   type is specified, only 'global' site-wide substitution tokens are
 *   built.
 * @param object
 *   Optionally, the object to use for building substitution values.
 *   A node, comment, user, etc.
 * @param leading
 *    Character(s) to prepend to the token key before searching for
 *    matches. Defaults to an open-bracket.
 * @param trailing
 *    Character(s) to append to the token key before searching for
 *    matches. Defaults to a close-bracket.
 * @return The modified version of $original, with all substitutions
 *   made.
 **/
function token_replace($original, $type = 'global', $object = NULL, $leading = '[', $trailing = ']') {
  $full = token_get_values($type, $object);
  return _token_replace_tokens($original, $full->tokens, $full->values, $leading, $trailing);
}

/**
 * Return the value of $original, with all instances of placeholder
 * tokens replaced by their proper values. Contrary to token_replace(),
 * this function supports replacing mutiple types.
 *
 * @param original
 *  A string, or an array of strings, to perform token substitutions
 *  on.
 * @param types
 *   An array of substitution classes and optional objects. The key is
 *   a flag indicating the class of substitution tokens to use.
 *   If an object is passed as value, the key should contain the
 *   object's type. For example, 'node', 'comment', or 'user'. The
 *   object will be used for building substitution values. If no type
 *   is specified, only 'global' site-wide substitution tokens are built.
 * @param leading
 *    Character(s) to prepend to the token key before searching for
 *    matches. Defaults to an open-bracket.
 * @param trailing
 *    Character(s) to append to the token key before searching for
 *    matches. Defaults to a close-bracket.
 * @return The modified version of $original, with all substitutions
 *   made.
 **/
function token_replace_multiple($original, $types = array('global' => NULL), $leading = '[', $trailing = ']') {
  $full = new stdClass();
  $full->tokens = $full->values = array();
  foreach ($types as $type => $object) {
    $temp = token_get_values($type, $object);
    $full->tokens = array_merge($full->tokens, $temp->tokens);
    $full->values = array_merge($full->values, $temp->values);
  }
  return _token_replace_tokens($original, $full->tokens, $full->values, $leading, $trailing);
}

// Internally used function to replace tokens.
function _token_replace_tokens($original, $tokens, $values, $leading, $trailing) {
  $tokens = token_prepare_tokens($tokens, $leading, $trailing);
  return str_replace($tokens, $values, $original);
}

/**
 * Return a list of valid substitution tokens and their values for
 * the specified type.
 *
 * @param type
 *   A flag indicating the class of substitution tokens to use. If an
 *   object is passed in the second param, 'type' should contain the
 *   object's type. For example, 'node', 'comment', or 'user'. If no
 *   type is specified, only 'global' site-wide substitution tokens are
 *   built.
 * @param object
 *   Optionally, the object to use for building substitution values.
 *   A node, comment, user, etc.
 * @return
 *   A keyed array containing the substitution tokens and the substition
 *   values for the passed-in type and object.
 */
function token_get_values($type = 'global', $object = NULL, $flush = FALSE) {
  token_include();

  static $tokens;
  static $depth;

  // Flush the static token cache. Useful for processes that need to slog through
  // huge numbers of tokens in a single execution cycle. Flushing it will keep
  // them from burning through memory.
  if ($flush || !isset($tokens)) {
    $tokens = array();
  }

  // Simple recursion check. This is to avoid content_view()'s potential
  // for endless looping when a filter uses tokens, which load the content
  // view, which calls the filter, which uses tokens, which...
  if (!isset($depth)) {
    $depth = 1;
  }
  elseif ($depth = 1) {
    $depth++;
  }
  else {
    // We'll allow things to get two levels deep, but bail out after that
    // without performing any substitutions.
    $result = new stdClass();
    $result->tokens = array();
    $result->values = array();
    return $result;
  }

  $path = drupal_get_path('module', 'token');
  require_once("$path/token_node.inc");
  require_once("$path/token_comment.inc");
  require_once("$path/token_user.inc");
  if (module_exists('content')) {
    require_once("$path/token_cck.inc");
  }
  if (module_exists('taxonomy')) {
    require_once("$path/token_taxonomy.inc");
  }


  $id = _token_get_id($type, $object);
  if (isset($tokens[$type][$id])) {
    $tmp_tokens = $tokens[$type][$id];
  }
  else {
    $tmp_tokens = module_invoke_all('token_values', $type, $object);
    $tokens[$type][$id] = $tmp_tokens;
  }

  // Special-case global tokens, as we always want to be able to process
  // those substitutions.
  if (!isset($tokens['global']['default'])) {
    $tokens['global']['default'] = module_invoke_all('token_values', 'global');
  }

  $all = array_merge($tokens['global']['default'], $tokens[$type][$id]);

  $result = new stdClass();
  $result->tokens = array_keys($all);
  $result->values = array_values($all);

  $depth--;

  return $result;
}

/**
 * A helper function that retrieves all currently exposed tokens,
 * and merges them recursively. This is only necessary when building
 * the token listing -- during actual value replacement, only tokens
 * in a particular domain are requested and a normal array_marge() is
 * sufficient.
 *
 * @param type
 *   A flag indicating the class of substitution tokens to use. If an
 *   object is passed in the second param, 'type' should contain the
 *   object's type. For example, 'node', 'comment', or 'user'. If no
 *   type is specified, only 'global' site-wide substitution tokens are
 *   built.
 * @return
 *   The array of usable tokens and their descriptions, organized by
 *   token type.
 */
function token_get_list($type = 'all') {
  token_include();
  $return = array();
  foreach (module_implements('token_list') as $module) {
    $function = $module .'_token_list';
    $result = $function($type);
    if (is_array($result)) {
      foreach ($result as $category => $tokens) {
        foreach ($tokens as $token => $title) {
          $return[$category][$token] = $title;
        }
      }
    }
  }
  return $return;
}

/**
 * A helper function that transforms all the elements of an
 * array. Used to change the delimiter style from brackets to
 * percent symbols etc.
 *
 * @param tokens
 *    The array of tokens keys with no delimiting chacaters
 * @param leading
 *    Character(s) to prepend to the token key before searching for
 *    matches. Defaults to an open-bracket.
 * @param trailing
 *    Character(s) to append to the token key before searching for
 *    matches. Defaults to a close-bracket.
 *  @return
 *    The array of token keys, each wrapped in the specified
 *    delimiter style.
 */
function token_prepare_tokens($tokens = array(), $leading = '[', $trailing = ']') {
  foreach ($tokens as $key => $value) {
    $tokens[$key] = $leading . $value . $trailing;
  }
  return $tokens;
}

// Internal utility function used for static caching. There are
// almost certainly better ways to do this, but for the moment it's
// sufficient.
function _token_get_id($type = 'global', $object = NULL) {
  if (!isset($object)) {
    return "default";
  }
  switch ($type) {
    case 'node':
      return $object->nid;
    case 'comment':
      return $object->cid;
    case 'user':
      return $object->uid;
    default:
      return crc32(serialize($object));
  }
}
