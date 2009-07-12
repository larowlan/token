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

    if ($created_tokens = token_find_with_prefix($tokens, 'last-view')) {
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
  $results = array();

  $node['views'] = array(
    'name' => t("Number of views"),
    'description' => t("The number of visitors who have read the node."),
  );

  $node['day-views'] = array(
    'name' => t("Views today"),
    'description' => t("The number of visitors who have read the node today."),
  );

  $node['last-view'] = array(
    'name' => t("Last view"),
    'description' => t("The date on which a visitor last read the node."),
    'references' => 'date',
  );

  $results['tokens']['node'] = $node;
  return $results;
}
