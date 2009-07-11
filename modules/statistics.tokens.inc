<?php
// $Id$

/**
 * @file
 * Builds placeholder replacement tokens for node visitor statistics.
 */

/**
 * Implement hook_tokens().
 */
function statistics_tokens($type, $tokens, $data, $language = NULL, $sanitize = TRUE) {
  $replacements = array();

  if ($type == 'node' & !empty($data['node'])) {
    $node = $data['node'];

    foreach ($tokens as $name => $original) {
      if ($name == 'views') {
        $statistics = statistics_get($node->nid);
        $replacements[$original] = $statistics['totalviews'];
      }
      elseif ($name == 'views-today') {
        $statistics = statistics_get($node->nid);
        $replacements[$original] = $statistics['dayviews'];
      }
      elseif ($name == 'last-view') {
        $statistics = statistics_get($node->nid);
        $replacements[$original] = format_date($statistics['timestamp']);
      }
    }

    if ($created_tokens = token_match_prefix($tokens, 'last-view')) {
      $statistics = statistics_get($node->nid);
      $replacements += module_invoke_all('tokens', 'date', $created_tokens, array('date' => $statistics['timestamp']), $language, $sanitize);
    }
  }

  return $replacements;
}

/**
 * Implement hook_token_info().
 */
function statistics_token_info() {
  $data = array();

  $data['tokens']['node']['views']['name'] = t('Number of views');
  $data['tokens']['node']['views']['description'] = t('The number of visitors who have read the node.');

  $data['tokens']['node']['day-views']['name'] = t('Views today');
  $data['tokens']['node']['day-views']['description'] = t('The number of visitors who have read the node today.');

  $data['tokens']['node']['last-view']['name'] = t('Last view');
  $data['tokens']['node']['last-view']['description'] = t('The date on which a visitor last read the node.');
  $data['tokens']['node']['last-view']['references'] = 'date';

  return $data;
}
