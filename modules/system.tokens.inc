<?php
// $Id$

/**
 * @file
 * Builds placeholder replacement tokens system-wide data.
 *
 * This file handles tokens for the global 'site' token type, as well as
 * 'date' and 'file' tokens.
 */

/**
 * Implement hook_tokens().
 */
function system_tokens($type, $tokens, $data, $language = NULL, $sanitize = TRUE) {
  $url_options = array('absolute' => TRUE);
  if (isset($language)) {
    $url_options['language'] = $language;
  }

  $replacements = array();

  if ($type == 'site') {
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'name':
          $site_name = variable_get('site_name', 'Drupal');
          $replacements[$original] = $sanitize ? filter_xss($site_name) : $site_name;
          break;

        case 'slogan':
          $slogan = variable_get('site_slogan');
          $replacements[$original] = $sanitize ? filter_xss($slogan) : $slogan;
          break;

        case 'mission':
          $mission = variable_get('site_mission');
          $replacements[$original] = $sanitize ? filter_xss($mission) : $mission;
          break;

        case 'mail':
          $mail = variable_get('site_name');
          $replacements[$original] = variable_get('site_mail');
          break;

        case 'url':
          $replacements[$original] = url('<front>', $url_options);
          break;

        case 'login-url':
          $replacements[$original] = url('user', $url_options);
          break;
      }
    }
  }

  elseif ($type == 'date') {
    if (empty($data['date'])) {
      $date = REQUEST_TIME;
    }
    else {
      $date = $data['date'];
    }
    $langcode = (isset($language) ? $language->language : NULL);

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'raw':
          $replacements[$original] = filter_xss($date);
          break;

        case 'small':
          $replacements[$original] = format_date($date, 'small', '', NULL, $langcode);
          break;

        case 'medium':
          $replacements[$original] = format_date($date, 'medium', '', NULL, $langcode);
          break;

        case 'large':
          $replacements[$original] = format_date($date, 'large', '', NULL, $langcode);
          break;

        case 'since':
          $replacements[$original] = format_interval((REQUEST_TIME - $date), 2, $langcode);
          break;
      }
    }

    if ($created_tokens = token_find_with_prefix($tokens, 'custom')) {
      foreach ($created_tokens as $name => $original) {
        $replacements[$original] = format_date($date, 'custom', $name, NULL, $langcode);
      }
    }
  }

  elseif ($type == 'file' && !empty($data['file'])) {
    $file = $data['file'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Basic keys and values.
        case 'fid':
          $replacements[$original] = $file->fid;
          break;

        case 'uid':
          $replacements[$original] = $file->uid;
          break;

        case 'nid':
          $replacements[$original] = $file->nid;
          break;

        // Essential file data
        case 'name':
          $replacements[$original] = $sanitize ? check_plain($file->filename) : $file->filename;
          break;

        case 'description':
          $replacements[$original] = $sanitize ? filter_xss($file->description) : $file->description;
          break;

        case 'path':
          $replacements[$original] = $sanitize ? filter_xss($file->filepath) : $file->filepath;
          break;

        case 'mime':
          $replacements[$original] = $sanitize ? filter_xss($file->filemime) : $file->filemime;
          break;

        case 'size':
          $replacements[$original] = format_size($file->filesize);
          break;

        case 'url':
          $replacements[$original] = url(file_create_url($file->filepath), $url_options);
          break;

        // These tokens are default variations on the chained tokens handled below.
        case 'node':
          if ($nid = $file->nid) {
            $node = node_load($file->nid);
            $replacements[$original] = $sanitize ? filter_xss($node->title) : $node->title;
          }
          break;

        case 'timestamp':
          $replacements[$original] = format_date($file->timestamp, 'medium', '', NULL, (isset($language) ? $language->language : NULL));
          break;

        case 'owner':
          $account = user_load($file->uid);
          $replacements[$original] = $sanitize ? filter_xss($user->name) : $user->name;
          break;
      }
    }

    if ($node_tokens = token_find_with_prefix($tokens, 'node')) {
      $node = node_load($file->nid);
      $replacements += module_invoke_all('tokens', 'node', $node_tokens, array('node' => $node), $language, $sanitize);
    }

    if ($date_tokens = token_find_with_prefix($tokens, 'timestamp')) {
      $replacements += module_invoke_all('tokens', 'date', $date_tokens, array('date' => $file->timestamp), $language, $sanitize);
    }

    if (($owner_tokens = token_find_with_prefix($tokens, 'owner')) && $account = user_load($file->uid)) {
      $replacements += module_invoke_all('tokens', 'user', $owner_tokens, array('user' => $account), $language, $sanitize);
    }
  }

  return $replacements;
}

/**
 * Implement hook_token_info().
 */
function system_token_info() {
  $results = array();

  $types['site']['name'] = t('Site information');
  $types['site']['description'] = t('Tokens for site-wide settings and other global information.');

  $types['date']['name'] = t('Dates');
  $types['date']['description'] = t('Tokens related to times and dates.');

  $types['file']['name'] = t('Files');
  $types['file']['description'] = t('Tokens related to uploaded files.');


  $site['name']['name'] = t("Name");
  $site['name']['description'] = t("The name of the site.");

  $site['slogan']['name'] = t("Slogan");
  $site['slogan']['description'] = t("The slogan of the site.");

  $site['mission']['name'] = t("Mission");
  $site['mission']['description'] = t("The optional 'mission' of the site.");

  $site['mail']['name'] = t("Email");
  $site['mail']['description'] = t("The administrative email address for the site.");

  $site['url']['name'] = t("URL");
  $site['url']['description'] = t("The URL of the site's front page.");

  $site['login-url']['name'] = t("Login page");
  $site['login-url']['description'] = t("The URL of the site's login page.");


  $date['small']['name'] = t("Small format");
  $date['small']['description'] = t("A date in 'small' format. (%date)", array('%date' => format_date(REQUEST_TIME, 'small')));

  $date['medium']['name'] = t("Medium format");
  $date['medium']['description'] = t("A date in 'medium' format. (%date)", array('%date' => format_date(REQUEST_TIME, 'medium')));

  $date['large']['name'] = t("Large format");
  $date['large']['description'] = t("A date in 'large' format. (%date)", array('%date' => format_date(REQUEST_TIME, 'large')));

  $date['custom']['name'] = t("Custom format");
  $date['custom']['description'] = t("A date in a custom format. See !php-date for details.", array('!php-date' => l(t('the PHP documentation'), 'http://php.net/manual/en/function.date.php')));

  $date['since']['name'] = t("Time-since");
  $date['since']['description'] = t("A data in 'time-since' format. (%date)", array('%date' => format_interval(REQUEST_TIME - 360, 2)));

  $date['raw']['name'] = t("Raw timestamp");
  $date['raw']['description'] = t("A date in UNIX timestamp format (%date)", array('%date' => REQUEST_TIME));


  $file['fid']['name'] = t("File ID");
  $file['fid']['description'] = t("The unique ID of the uploaded file.");

  $file['uid']['name'] = t("User ID");
  $file['uid']['description'] = t("The unique ID of the user who owns the file.");

  $file['nid']['name'] = t("Node ID");
  $file['nid']['description'] = t("The unique ID of the node the file is attached to.");

  $file['name']['name'] = t("File name");
  $file['name']['description'] = t("The name of the file on disk.");

  $file['description']['name'] = t("Description");
  $file['description']['description'] = t("An optional human-readable description of the file.");

  $file['path']['name'] = t("Path");
  $file['path']['description'] = t("The location of the file on disk.");

  $file['mime']['name'] = t("MIME type");
  $file['mime']['description'] = t("The MIME type of the file.");

  $file['size']['name'] = t("File size");
  $file['size']['description'] = t("The size of the file, in kilobytes.");

  $file['path']['name'] = t("URL");
  $file['path']['description'] = t("The web-accessible URL for the file.");

  $file['timestamp']['name'] = t('Timestamp');
  $file['timestamp']['description'] = t('The date the file was most recently changed.');
  $file['timestamp']['references'] = 'date';

  $file['node']['name'] = t('Node');
  $file['node']['description'] = t('The node the file is attached to.');
  $file['node']['references'] = 'date';

  $file['owner']['name'] = t('Owner');
  $file['owner']['description'] = t('The user who originally uploaded the file.');
  $file['owner']['references'] = 'user';


  $results['types'] = $types;
  $results['tokens']['site'] = $site;
  $results['tokens']['date'] = $date;
  $results['tokens']['file'] = $file;
  return $results;
}
