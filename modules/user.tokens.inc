<?php

function user_tokens($type, $tokens, $data, $safe_for_html = TRUE) {
  $replacements = array();

  foreach($tokens as $token => $raw) {
    switch ($type) {
      case 'user':
        if (empty($data['user'])) {
          return;
        } else {
          $account = $data['user'];
        }
        if ($token == 'uid') $replacements[$raw] = $account->uid;
        if ($token == 'name') $replacements[$raw] = check_plain($account->name);
        if ($token == 'email') $replacements[$raw] = check_plain($account->email);
        break;
    }
  }
  
  return $replacements;
}