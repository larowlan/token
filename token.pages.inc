<?php
// $Id$

/**
 * @file
 * User page callbacks for the token module.
 */

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

  $header = array(
    t('Name'),
    t('Token'),
    t('Description'),
  );
  $rows = array();

  foreach ($token_types as $type) {
    $type_info = $info['types'][$type] += array('type' => $type);
    $id = _token_clean_css_identifier($type);
    $row = array(
      'id' => $id,
      'class' => array('token-group'),
      'data' => array(
        $type_info['name'],
        '',
        $type_info['description'],
      ),
    );

    if (count($token_types) > 1) {
      $rows[] = $row;
    }
    else {
      $id = NULL;
    }
    $rows = array_merge($rows, token_browser_list_tokens($type_info['type'], $type, $id, $variables['recursion_limit']));
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

function token_browser_list_tokens($type, $prefix = NULL, $parent_id = NULL, $recursion_limit = 4) {
  // Since tokens could possibly recurse (e.g. terms have parents, and those
  // parent terms could have parents, etc.), we have to add a depth check.
  if ($recursion_limit <= 0) {
    return array();
  }

  $info = token_get_info();
  $rows = array();

  if (!isset($prefix)) {
    $prefix = $type;
  }

  foreach ($info['tokens'][$type] as $token => $token_info) {
    $key = "{$prefix}:{$token}";
    if (!empty($token_info['dynamic'])) {
      // Add a placeholder for dynamic tokens like date:custom.
      $key .= ':?';
    }
    $id = _token_clean_css_identifier($key);
    $row = array(
      'id' => $id,
      'class' => array(),
      'data' => array(
        $token_info['name'],
        array(
          'data' => "[{$key}]",
          'class' => 'token-key',
        ),
        $token_info['description'],
      ),
    );
    if (isset($parent_id)) {
      $row['class'][] = 'child-of-' . $parent_id;
    }
    $rows[] = $row;
    if (!empty($token_info['type'])) {
      $rows = array_merge($rows, token_browser_list_tokens($token_info['type'], $key, $id, $recursion_limit - 1));
    }
  }

  return $rows;
}

/**
 * Wrapper function for drupal_clean_css_identifier() that also removes ':'.
 *
 * @see drupal_clean_css_identifier().
 */
function _token_clean_css_identifier($id) {
  return drupal_clean_css_identifier('token-' . $id, array(' ' => '-', '_' => '-', '/' => '-', '[' => '-', ']' => '', ':' => '-'));
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
