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

  // Metadata for token types.
  $data['types']['user']['name'] = t('Users');
  $data['types']['user']['description'] = t('Tokens related to individual user accounts.');

  // Basic data for users
  $data['tokens']['user']['uid']['name'] = t('User ID');
  $data['tokens']['user']['uid']['description'] = t('The unique ID of the user account.');

  $data['tokens']['user']['name']['name'] = t('Name');
  $data['tokens']['user']['name']['description'] = t("The login name of the user account.");

  $data['tokens']['user']['mail']['name'] = t('Email');
  $data['tokens']['user']['mail']['description'] = t('The email address of the user account.');

  $data['tokens']['user']['url']['name'] = t('URL');
  $data['tokens']['user']['url']['description'] = t('The url of the account profile page.');

  $data['tokens']['user']['edit-url']['name'] = t('Edit URL');
  $data['tokens']['user']['edit-url']['description'] = t('The url of the account edit page.');


  // Chained tokens for users
  $data['tokens']['user']['last-login']['name'] = t('Last login');
  $data['tokens']['user']['last-login']['description'] = t('The date the user last logged in to the site.');
  $data['tokens']['user']['last-login']['references'] = 'date';

  $data['tokens']['user']['created']['name'] = t('Created');
  $data['tokens']['user']['created']['description'] = t('The date the user account was created.');
  $data['tokens']['user']['created']['references'] = 'date';


  return $data;
}