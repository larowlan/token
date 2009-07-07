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