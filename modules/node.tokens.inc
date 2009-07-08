<?php
// $Id$

function node_tokens($type, $tokens, $data, $safe_for_html = TRUE) {
  $replacements = array();

  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];

    foreach($tokens as $name => $original) {
      if ($name == 'nid') $replacements[$original] = $node->nid;
      elseif ($name == 'tnid') $replacements[$original] = $comment->tnid;
      elseif ($name == 'uid') $replacements[$original] = $node->uid;
      elseif ($name == 'name') $replacements[$original] = check_plain($node->name);
      elseif ($name == 'title') $replacements[$original] = check_plain($node->title);
      elseif ($name == 'type') $replacements[$original] = $node->type;
      elseif ($name == 'type-name') $replacements[$original] = node_get_types('name', $node->type);
      elseif ($name == 'language') $replacements[$original] = filter_xss_admin($node->language);
      elseif ($name == 'url') $replacements[$original] = url('node/' . $node->nid);
      elseif ($name == 'edit-url') $replacements[$original] = url('node/' . $node->nid . '/edit');
    }
    
    if ($author_tokens = token_match_prefix($tokens, 'author')) {
      $author = user_load($node->uid);
      $replacements += module_invoke_all('tokens', 'user', $author_tokens, array('user' => $author));
    }

    if ($created_tokens = token_match_prefix($tokens, 'created')) {
      $replacements += module_invoke_all('tokens', 'date', $created_tokens, array('date' => $node->created));
    }

    if ($changed_tokens = token_match_prefix($tokens, 'changed')) {
      $replacements += module_invoke_all('tokens', 'date', $changed_tokens, array('date' => $node->changed));
    }
  }
  
  return $replacements;
}