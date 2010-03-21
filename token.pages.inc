<?php
// $Id$

/**
 * @file
 * User page callbacks for the token module.
 */

/**
 * Build a tree array of tokens used for themeing or information.
 *
 * @param $token_type
 *   The token type. Should not be an alias (e.g. use 'user' instead of
 *   'current-user').
 * @param $flat_tree
 *   A boolean if TRUE will only make a flat array of tokens, otherwise
 *   child tokens will be inside the 'children' parameter of a token.
 * @param $recursion_limit
 *   An integer with the maximum number of token levels to recurse.
 * @param $parents
 *   An optional array with the current parents of the tokens.
 */
function token_build_tree($token_type, $flat_tree = FALSE, $recursion_limit = 4, $parents = array()) {
  $info = token_get_info();
  if ($recursion_limit <= 0 || !isset($info['types'][$token_type])) {
    return array();
  }

  $tree = array();
  foreach ($info['tokens'][$token_type] as $token => $token_info) {
    // Build the raw token string.
    $token_parents = $parents;
    if (empty($token_parents)) {
      // If the parents array is currently empty, assume the token type is its
      // parent.
      $token_parents[] = $token_type;
    }
    $token_parents[] = $token;
    if (!empty($token_info['dynamic'])) {
      $token_parents[] = '?';
    }
    $raw_token = '[' . implode(':', $token_parents) . ']';
    $tree[$raw_token] = $token_info;

    // Add the token's parent as its raw token value.
    if (!empty($parents)) {
      $tree[$raw_token]['parent'] = '[' . implode(':', $parents) . ']';
    }

    // Fetch the child tokens.
    if (!empty($token_info['type'])) {
      $children = token_build_tree($token_info['type'], $flat_tree, $recursion_limit - 1, $token_parents);
      if ($flat_tree) {
        $tree = array_merge($tree, $children);
      }
      else {
        $tree[$raw_token]['children'] = $children;
      }
    }
  }

  return $tree;
}

/**
 * Provide a 'tree' display of nested tokens.
 */
function theme_token_tree($variables) {
  $token_types = $variables['token_types'];
  $info = token_get_info();

  if ($token_types == 'all') {
    $token_types = array_keys($info['types']);
  }
  elseif ($variables['global_types']) {
    $token_types = array_merge($token_types, token_get_global_token_types());
    array_unique($token_types);
  }

  // Check for token type validity and sort.
  $token_types = array_intersect($token_types, array_keys($info['types']));
  sort($token_types);
  $token_types = array('node');

  $header = array(
    t('Name'),
    t('Token'),
    t('Description'),
  );
  $rows = array();

  foreach ($token_types as $type) {
    $type_info = $info['types'][$type] += array('type' => $type);
    $parent = array();

    if (count($token_types) > 1) {
      $rows[] = _token_token_tree_format_row($type, $type_info, TRUE);
      $parent = array($type);
    }

    $tree = token_build_tree($type_info['type'], TRUE, $variables['recursion_limit'], $parent);
    foreach ($tree as $token => $token_info) {
      $rows[] = _token_token_tree_format_row($token, $token_info);
    }
  }

  if (count($rows)) {
    drupal_add_library('token', 'treeTable');
    drupal_add_js(drupal_get_path('module', 'token') . '/token.js');
    drupal_add_css(drupal_get_path('module', 'token') . '/token.css');
  }

  $table_options = array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('class' => array('token-tree')),
    'empty' => t('No tokens available.'),
  );
  if ($variables['click_insert']) {
    $table_options['caption'] = t('Click a token to insert it into the field you\'ve last clicked.');
    $table_options['attributes']['class'][] = 'token-click-insert';
  }

  return theme('table', $table_options);
}

/**
 * Build a row in the token tree.
 */
function _token_token_tree_format_row($token, array $token_info, $is_group = FALSE) {
  $row = array(
    'id' => _token_clean_css_identifier($token),
    'class' => array(),
    'data' => array(
      'name' => $token_info['name'],
      'token' => '',
      'description' => $token_info['description'],
    ),
  );

  if ($is_group) {
    // This is a token type/group.
    $row['class'][] = 'token-group';
  }
  else {
    // This is a token.
    $row['data']['token'] = array(
      'data' => $token,
      'class' => array('token-key'),
    );
    if (!empty($token_info['parent'])) {
      $row['class'][] = 'child-of-' . _token_clean_css_identifier($token_info['parent']);
    }
  }

  return $row;
}

/**
 * Wrapper function for drupal_clean_css_identifier() for use with tokens.
 *
 * This trims any brackets from the token and also cleans the colon character
 * to a hyphen.
 *
 * @see drupal_clean_css_identifier().
 */
function _token_clean_css_identifier($id) {
  return drupal_clean_css_identifier('token-' . trim($id, '[]'), array(' ' => '-', '_' => '-', '/' => '-', '[' => '-', ']' => '', ':' => '-'));
}

function token_autocomplete() {
  $args = func_get_args();
  $string = implode('/', $args);

  $token_info = token_info();

  preg_match_all('/\[([^\s\]:]*):?([^\s\]]*)?\]?/', $string, $matches);
  $types = $matches[1];
  $tokens = $matches[2];

  foreach ($types as $index => $type) {
    if (!empty($tokens[$index]) || isset($token_info['types'][$type])) {
      token_autocomplete_token($type, $tokens[$index]);
    }
    else {
      token_autocomplete_type($type);
    }
  }

}

function token_autocomplete_type($string = '') {
  $token_info = token_info();
  $types = $token_info['types'];
  $matches = array();

  foreach ($types as $type => $info) {
    if (!$string || strpos($type, $string) === 0) {
      $type_key = "[{$type}:";
       $matches[$type_key] = levenshtein($type, $string);
    }
  }

  if ($string) {
    asort($matches);
  }
  else {
    ksort($matches);
  }

  $matches = drupal_map_assoc(array_keys($matches));
  drupal_json_output($matches);
}

function token_autocomplete_token($type, $string = '') {
  $token_info = token_info();
  $tokens = $token_info['tokens'][$type];
  $matches = array();

  foreach ($tokens as $token => $token_info) {
    if (!$string || strpos($token, $string) === 0) {
      // Check if the token can be continued.
      $token_key = "[{$type}:{$token}" . (empty($token_info['type']) ? ']' : ':');
      $matches[$token_key] = levenshtein($token, $string);
    }
  }

  if ($string) {
    asort($matches);
  }
  else {
    ksort($matches);
  }

  $matches = drupal_map_assoc(array_keys($matches));
  drupal_json_output($matches);
}
