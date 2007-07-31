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
        $account = user_load(array('uid' => $user->uid));
      }

      $values['user']           = $account->uid ? $account->name : variable_get('anonymous', 'Anonymous');
      $values['uid']            = $account->uid;
      $values['mail']           = $account->uid ? $account->mail : '';
      $values['reg-date']       = $account->uid ? format_date($account->created, 'short') : '';
      $values['reg-since']      = $account->uid ? format_interval($account->created) : '';
      $values['log-date']       = $account->uid ? format_date($account->access, 'short') : '';
      $values['log-since']      = $account->uid ? format_interval($account->access) : '';
      $values['date-in-tz']     = $account->uid ? format_date(time(), 'short', '', $account->timezone) : '';
      $values['account-url']    = $account->uid ? url("user/$account->uid") : '';
      $values['account-edit']   = $account->uid ? url("user/$account->uid/edit") : '';

      break;
  }
  return $values;
}

/**
 * Implementation of hook_token_list()
 */
function user_token_list($type = 'all') {
  if ($type == 'user' || $type == 'all') {
    $tokens['user']['user']           = t("User's name");
    $tokens['user']['uid']            = t("User's ID");
    $tokens['user']['mail']           = t("User's email address");
 
    $tokens['user']['reg-date']       = t("User's registration date");
    $tokens['user']['reg-since']      = t("Days since the user registered");
    $tokens['user']['log-date']       = t("User's last login date");
    $tokens['user']['log-since']      = t("Days since the user's last login");
    $tokens['user']['date-in-tz']     = t("The current date in the user's timezone");
    $tokens['user']['account-url']    = t("The URL of the user's profile page.");
    $tokens['user']['account-edit']   = t("The URL the user's account editing page.");

    return $tokens;
  }
}