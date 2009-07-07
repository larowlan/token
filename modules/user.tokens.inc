<?php
// $Id$

function user_tokens($type, $tokens, $data, $safe_for_html = TRUE) {
  global $user;

  $replacements = array();

  if ($type == 'user') {
    if (empty($data['user'])) {
      $account = $data['user'];
    } else {
      $account = $user;
    }

    foreach($tokens as $token => $raw) {
      if ($token == 'uid') $replacements[$raw] = $account->uid;
      elseif ($token == 'name') $replacements[$raw] = check_plain($account->name);
      elseif ($token == 'mail') $replacements[$raw] = check_plain($account->mail);
    }
  }
  
  return $replacements;
}