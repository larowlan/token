<?php

function node_tokens($type, $tokens, $data, $safe_for_html = TRUE) {
  $replacements = array();
  $handoff = array('tokens' => array(), 'data' => array());

  foreach($tokens as $token => $raw) {
    switch ($type) {
      case 'node':
        if (empty($data['node'])) {
          return;
        } else {
          $node = $data['node'];
        }
        if ($token == 'nid') $replacements[$raw] = $node->nid;
        if ($token == 'uid') $replacements[$raw] = $node->uid;
        if ($token == 'name') $replacements[$raw] = check_plain($node->name);
        if ($token == 'title') $replacements[$raw] = check_plain($node->title);
        
        // If we find a node token like 'author:foo', it's a subtoken and we want
        // to load the user, then get its tokens. This allows us to do things like
        // [node:author:email] and [node:author:profile-homepage], since they are
        // just delegated to the 'user' token handling cases.
        $subtoken = split(':', $token, 2);
        if (count($subtoken) > 1) {
          if ($subtoken[0] == 'author') {
            $handoff['tokens']['user'][$subtoken[1]] = $raw;
            if (empty($handoff['data']['user'])) {
              $handoff['data']['user'] = user_load($node->uid);
            }
          }
        }
        break;
    }
  }
    
  // God this is ugly and horrible. Why? WHY? God, help.
  if (!empty($handoff['tokens'])) {
    foreach($handoff['tokens'] as $handoff_type => $handoff_tokens) {
      $delegated_tokens = module_invoke_all('tokens', $handoff_type, $handoff_tokens, $handoff['data']);
      foreach ($delegated_tokens as $raw => $value) {
        $replacements[$raw] = $value;
      }
    }
  }
  
  return $replacements;
}