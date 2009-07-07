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

    foreach($tokens as $name => $original) {
      if ($name == 'uid') $replacements[$original] = $account->uid;
      elseif ($name == 'name') $replacements[$original] = check_plain($account->name);
      elseif ($name == 'mail') $replacements[$original] = check_plain($account->mail);
    }
  }
  
  return $replacements;
}