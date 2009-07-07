<?php

function comment_tokens($type, $tokens, $data, $safe_for_html = TRUE) {
  $replacements = array();

  if ($type == 'comment') {
    if (empty($data['comment'])) {
      return;
    } else {
      $comment = $data['comment'];
    }
    foreach($tokens as $name => $original) {
      if ($name == 'subject') $replacements[$original] = check_plain($comment->subject);
      elseif ($name == 'nid') $replacements[$original] = $comment->nid;
      elseif ($name == 'uid') $replacements[$original] = $comment->uid;
      elseif ($name == 'pid') $replacements[$original] = $comment->pid;
      elseif ($name == 'hostname') $replacements[$original] = check_plain($comment->hostname);
      elseif ($name == 'name') $replacements[$original] = check_plain($comment->name);
      elseif ($name == 'mail') $replacements[$original] = check_plain($comment->mail);
      elseif ($name == 'homepage') $replacements[$original] = check_plain($comment->homepage);
      elseif ($name == 'body') {
        if ($safe_for_html) $replacements[$original] = check_markup($comment->comment, $comment->format);
        else $replacements[$original] = $comment->comment;
      }
    }
    
    if ($node_tokens = token_match_prefix($tokens, 'node')) {
      $node = node_load($comment->nid);
      $replacements += module_invoke_all('tokens', 'node', $node_tokens, array('node' => $node));
    }

    if ($date_tokens = token_match_prefix($tokens, 'created')) {
      $replacements += module_invoke_all('tokens', 'date', $date_tokens, array('date' => $comment->timestamp));
    }

    if ($parent_tokens = token_match_prefix($tokens, 'parent') && $parent = comment_load($comment->pid)) {
      $replacements += module_invoke_all('tokens', 'comment', $parent_tokens, array('comment' => $parent));
    }
  }
  
  return $replacements;
}