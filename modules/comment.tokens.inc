<?php

function comment_tokens($type, $tokens, $data, $safe_for_html = TRUE) {
  $replacements = array();

  foreach($tokens as $token => $raw) {
    switch ($type) {
      case 'comment':
        if (empty($data['comment'])) {
          return;
        } else {
          $comment = $data['comment'];
        }
        if ($token == 'subject') $replacements[$raw] = check_plain($comment->subject);
        if ($token == 'name') $replacements[$raw] = check_plain($comment->name);
        if ($token == 'email') $replacements[$raw] = check_plain($comment->email);
        break;
    }
  }
  
  return $replacements;
}