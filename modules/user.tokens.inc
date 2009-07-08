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


function user_token_info() {
  $data = array();

  // Basic data for users
  $data['node']['uid']['name'] = t('User ID');
  $data['node']['uid']['description'] = t('The unique ID of the user account.');

  $data['node']['name']['name'] = t('Name');
  $data['node']['name']['description'] = t("The login name of the user account.");

  $data['node']['mail']['name'] = t('Email');
  $data['node']['mail']['description'] = t('The email address of the user account.');

  $data['node']['url']['name'] = t('URL');
  $data['node']['url']['description'] = t('The url of the account profile page.');

  $data['node']['edit-url']['name'] = t('Edit URL');
  $data['node']['edit-url']['description'] = t('The url of the account edit page.');


  // Chained tokens for users
  $data['comment']['last-login']['name'] = t('Last login');
  $data['comment']['last-login']['description'] = t('The date the user last logged in to the site.');
  $data['comment']['last-login']['references'] = 'date';

  $data['comment']['created']['name'] = t('Created');
  $data['comment']['created']['description'] = t('The date the user account was created.');
  $data['comment']['created']['references'] = 'date';


  return $data;
}