<?php
// $Id$

/**
 * Implementation of hook_token_values()
 */
function user_token_values($type, $object = NULL) {
  switch ($type) {
    case 'user':
      if (isset($object)) {
        $account = $object;
      }
      else {
        global $user;
        $account = $user;
      }

      $values['[user]']      = $user->uid ? $user->name : variable_get('anonymous', 'Anonymous');
      $values['[uid]']       = $user->uid;
      $values['[mail]']      = $user->uid ? $user->mail : '';
      $values['[reg-date]']  = $user->uid ? date('F j, Y', $user->created) : '';
      $values['[reg-since]'] = $user->uid ? format_interval($user->created) : '';
      $values['[log-date]']  = $user->uid ? date('F j, Y', $user->access) : '';
      $values['[log-since]'] = $user->uid ? format_interval($user->access) : '';

      break;
  }
  return $values;
}

/**
 * Implementation of hook_token_list()
 */
function user_token_list($type = 'all') {
  if ($type == 'user' || $type == 'all') {
    $tokens['user']['[user]']      = t("User's name");
    $tokens['user']['[uid]']       = t("User's ID");
    $tokens['user']['[mail]']      = t("User's email address");
 
    $tokens['user']['[reg-date]']  = t("User's registration date');
    $tokens['user']['[reg-since]'] = t("Days since the user registered");
    $tokens['user']['[log-date]']  = t("User's last login date");
    $tokens['user']['[log-since]'] = t("Days since the user's last login");

    return $tokens;
  }
}