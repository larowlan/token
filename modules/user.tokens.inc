<?php
// $Id$

/**
 * @file
 * Builds placeholder replacement tokens for user-related data.
 */

/**
 * Implement hook_tokens().
 */
function user_tokens($type, $tokens, $data, $language = NULL, $sanitize = TRUE) {
  global $user;
  $url_options = array('absolute' => TRUE);
  if (isset($language)) {
    $url_options['language'] = $language;
  }

  $replacements = array();

  if ($type == 'user') {
    if (!empty($data['user'])) {
      $account = $data['user'];
    }
    else {
      $account = $user;
    }

    foreach ($tokens as $name => $original) {
      switch ($name) {

        // Basic user account information.
        case 'uid':
          $replacements[$original] = $account->uid;
          break;
        case 'name':
          $replacements[$original] = $sanitize ? filter_xss($account->name) : $account->name;
          break;
        case 'mail':
          $replacements[$original] = $sanitize ? filter_xss($account->mail) : $account->mail;
          break;
        case 'new-password':
          $replacements[$original] = $account->password;
          break;
        case 'url':
          $replacements[$original] = url("user/$account->uid", $url_options);
          break;
        case 'edit-url':
          $replacements[$original] = url("user/$account->uid/edit", $url_options);
          break;
        case 'confirm-cancel-url':
          $replacements[$original] = user_cancel_url($account);
          break;
        case 'password-reset-url':
          $replacements[$original] = user_pass_reset_url($account);
          break;

        // These tokens are default variations on the chained tokens handled below.
        case 'last-login':
          if (isset($language)) {
            $replacements[$original] = format_date($account->login, 'medium', '', NULL, $language->language);
          }
          else {
            $replacements[$original] = format_date($account->login);
          }
          break;
        case 'created':
          if (isset($language)) {
            $replacements[$original] = format_date($account->created, 'medium', '', NULL, $language->language);
          }
          else {
            $replacements[$original] = format_date($account->created);
          }
          break;
      }
    }

    if ($login_tokens = token_match_prefix($tokens, 'last-login')) {
      $replacements += module_invoke_all('tokens', 'date', $login_tokens, array('date' => $account->login), $language, $sanitize);
    }

    if ($registered_tokens = token_match_prefix($tokens, 'created')) {
      $replacements += module_invoke_all('tokens', 'date', $registered_tokens, array('date' => $account->created), $language, $sanitize);
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

  $data['tokens']['user']['new-password']['name'] = t('New password');
  $data['tokens']['user']['new-password']['description'] = t("A newly-assigned password given to the user account on creation.");

  $data['tokens']['user']['url']['name'] = t('URL');
  $data['tokens']['user']['url']['description'] = t('The url of the account profile page.');

  $data['tokens']['user']['edit-url']['name'] = t('Edit URL');
  $data['tokens']['user']['edit-url']['description'] = t('The url of the account edit page.');

  $data['tokens']['user']['confirm-cancel-url']['name'] = t('Account cancellation URL');
  $data['tokens']['user']['confirm-cancel-url']['description'] = t("The account cancellation URL for the user account. Visiting this URL will automatically cancel a user account: it cannot be undone.");

  $data['tokens']['user']['password-reset-url']['name'] = t('Password reset URL');
  $data['tokens']['user']['password-reset-url']['description'] = t('A single-use password reset URL for the user account.');


  // Chained tokens for users
  $data['tokens']['user']['last-login']['name'] = t('Last login');
  $data['tokens']['user']['last-login']['description'] = t('The date the user last logged in to the site.');
  $data['tokens']['user']['last-login']['references'] = 'date';

  $data['tokens']['user']['created']['name'] = t('Created');
  $data['tokens']['user']['created']['description'] = t('The date the user account was created.');
  $data['tokens']['user']['created']['references'] = 'date';


  return $data;
}
