<?php
// $Id$

function user_tokens($type, $tokens, $data, $safe_for_html = TRUE) {
  global $user;

  $replacements = array();

  if ($type == 'user') {
    if (!empty($data['user'])) {
      $account = $data['user'];
    } else {
      $account = $user;
    }

    foreach($tokens as $name => $original) {
      if ($name == 'uid') $replacements[$original] = $account->uid;
      elseif ($name == 'name') $replacements[$original] = check_plain($account->name);
      elseif ($name == 'mail') $replacements[$original] = check_plain($account->mail);
      elseif ($name == 'url') $replacements[$original] = url("user/$account->uid");
      elseif ($name == 'edit-url') $replacements[$original] = url("user/$account->uid/edit");
    }
    
    if ($login_tokens = token_match_prefix($tokens, 'last-login')) {
      $replacements += module_invoke_all('tokens', 'date', $login_tokens, array('date' => $node->created));
    }

    if ($registered_tokens = token_match_prefix($tokens, 'created')) {
      $replacements += module_invoke_all('tokens', 'date', $registered_tokens, array('date' => $node->created));
    }
  }
  
  return $replacements;
}