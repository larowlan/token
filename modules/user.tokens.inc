<?php
// $Id$

/**
 * @file
 * Builds placeholder replacement tokens for user-related data.
 */

/**
 * Implement hook_token_info().
 */
function user_token_info() {
  $type = array(
    'name' => t('Users'),
    'description' => t('Tokens related to individual user accounts.'),
  );

  $user['uid'] = array(
    'name' => t('User ID'),
    'description' => t("The unique ID of the user account."),
  );
  $user['name'] = array(
    'name' => t("Name"),
    'description' => t("The login name of the user account."),
  );
  $user['mail'] = array(
    'name' => t("Email"),
    'description' => t("The email address of the user account."),
  );
  $user['url'] = array(
    'name' => t("URL"),
    'description' => t("The URL of the account profile page."),
  );
  $user['edit-url'] = array(
    'name' => t("Edit URL"),
    'description' => t("The url of the account edit page."),
  );

  // Chained tokens for users.
  $user['current-user'] = array(
    'name' => t("Currently user"),
    'description' => t("The account of the currently logged in user."),
    'type' => 'user',
  );
  $user['last-login'] = array(
    'name' => t("Last login"),
    'description' => t("The date the user last logged in to the site."),
    'type' => 'date',
  );
  $user['created'] = array(
    'name' => t("Created"),
    'description' => t("The date the user account was created."),
    'type' => 'date',
  );

  return array(
    'types' => array('user' => $type),
    'tokens' => array('user' => $user),
  );
}

/**
 * Implement hook_tokens().
 */
function user_tokens($type, $tokens, array $data = array(), array $options = array()) {
  global $user;
  $url_options = array('absolute' => TRUE);
  if (isset($options['language'])) {
    $url_options['language'] = $language;
    $language_code = $language->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'user' && !empty($data['user'])) {
    $account = $data['user'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Basic user account information.
        case 'uid':
          $replacements[$original] = $account->uid;
          break;

        case 'name':
          $replacements[$original] = $sanitize ? check_plain($account->name) : $account->name;
          break;

        case 'mail':
          $replacements[$original] = $sanitize ? check_plain($account->mail) : $account->mail;
          break;

        case 'url':
          $replacements[$original] = url("user/$account->uid", $url_options);
          break;

        case 'edit-url':
          $replacements[$original] = url("user/$account->uid/edit", $url_options);
          break;

        // These tokens are default variations on the chained tokens handled below.
        case 'last-login':
          $replacements[$original] = format_date($account->login, 'medium', '', NULL, $language_code);
          break;

        case 'created':
          $replacements[$original] = format_date($account->created, 'medium', '', NULL, $language_code);
          break;
      }
    }

    if ($login_tokens = token_find_with_prefix($tokens, 'last-login')) {
      $replacements += module_invoke_all('tokens', 'date', $login_tokens, array('date' => $account->login), $options);
    }

    if ($registered_tokens = token_find_with_prefix($tokens, 'created')) {
      $replacements += module_invoke_all('tokens', 'date', $registered_tokens, array('date' => $account->created), $options);
    }
  }
  if ($type == 'current-user') {
    global $user;
    $replacements += module_invoke_all('tokens', 'user', $tokens, array('user' => $user), $options);
  }

  return $replacements;
}
