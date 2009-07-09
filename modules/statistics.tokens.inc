<?php
// $Id$

// statistics_count_content_views

function statistics_tokens($type, $tokens, $data, $safe_for_html = TRUE) {
  $replacements = array();

  if ($type == 'node' & !empty($data['node'])) {
    $node = $data['node'];

    foreach($tokens as $name => $original) {
      if ($name == 'views') {
        $statistics = statistics_get($node->nid);
        $replacements[$original] = $statistics['totalviews'];
      }
    }
  }

  return $replacements;
}


function statistics_token_info() {
  $data = array();

  $data['tokens']['node']['views']['name'] = t('Number of views');
  $data['tokens']['node']['views']['description'] = t('The number of visitors who have read the node.');

  return $data;
}