<?php
// $Id$

function system_tokens($type, $tokens, $data, $sanitize = TRUE) {
  $replacements = array();
  
  if ($type == 'site') {
    foreach($tokens as $name => $original) {
      switch ($name) {
        case 'name':
          $site_name = variable_get('site_name');
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
          $replacements[$original] = url('<front>', array('absolute' => TRUE));
          break;
        case 'login-url':
          $replacements[$original] = url('user', array('absolute' => TRUE));
          break;
      }
    }
  }
  
  elseif ($type == 'date') {
    if (empty($data['date'])) {
      $date = REQUEST_TIME;
    } else {
      $date = $data['date'];
    }

    foreach($tokens as $name => $original) {
      switch ($name) {
        case 'small':
          $replacements[$original] = format_date($date, 'small');
          break;
        case 'medium':
          $replacements[$original] = format_date($date, 'medium');
          break;
        case 'large':
          $replacements[$original] = format_date($date, 'large');
          break;
        case 'since':
          $replacements[$original] = format_interval((REQUEST_TIME - $date), 2);
          break;
        case 'raw':
          $replacements[$original] = filter_xss($date);
          break;
      }
    }

    if ($created_tokens = token_match_prefix($tokens, 'custom')) {
      foreach($created_tokens as $name => $original) {
        $replacements[$original] = format_date($date, 'custom', $name);
      }
    }
  }

  elseif ($type == 'file' && !empty($data['file'])) {
    $file = $data['file'];

    foreach($tokens as $name => $original) {
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
          $replacements[$original] = url(file_create_url($file->filepath), array('absolute' => TRUE));
          break;
        
        // These tokens are default variations on the chained tokens handled below.
        case 'node':
          if ($nid = $file->nid) {
            $node = node_load($file->nid);
            $replacements[$original] = $sanitize ? filter_xss($node->title) : $node->title;
          }
          break;
        case 'timestamp':
          $replacements[$original] = format_date($file->timestamp);
          break;
        case 'owner':
          $account = user_load($file->uid);
          $replacements[$original] = $sanitize ? filter_xss($user->name) : $user->name;
          break;
      }
    }

    if ($node_tokens = token_match_prefix($tokens, 'node')) {
      $node = node_load($file->nid);
      $replacements += module_invoke_all('tokens', 'node', $node_tokens, array('node' => $node));
    }

    if ($date_tokens = token_match_prefix($tokens, 'timestamp')) {
      $replacements += module_invoke_all('tokens', 'date', $date_tokens, array('date' => $file->timestamp));
    }

    if (($owner_tokens = token_match_prefix($tokens, 'owner')) && $account = user_load($file->uid)) {
      $replacements += module_invoke_all('tokens', 'user', $owner_tokens, array('user' => $account));
    }
  }

  return $replacements;
}



function system_token_info() {
  $data = array();

  // Metadata for token types.
  $data['types']['site']['name'] = t('Site information');
  $data['types']['site']['description'] = t('Tokens for site-wide settings and other global information.');

  $data['types']['date']['name'] = t('Dates');
  $data['types']['date']['description'] = t('Tokens related to times and dates.');

  $data['types']['file']['name'] = t('Files');
  $data['types']['file']['description'] = t('Tokens related to uploaded files.');


  // Sitewide variables and metadata.
  $data['tokens']['site']['name']['name'] = t("Name");
  $data['tokens']['site']['name']['description'] = t("The name of the site.");

  $data['tokens']['site']['slogan']['name'] = t("Slogan");
  $data['tokens']['site']['slogan']['description'] = t("The slogan of the site.");

  $data['tokens']['site']['mission']['name'] = t("Mission");
  $data['tokens']['site']['mission']['description'] = t("The optional 'mission' of the site.");

  $data['tokens']['site']['mail']['name'] = t("Email");
  $data['tokens']['site']['mail']['description'] = t("The administrative email address for the site.");

  $data['tokens']['site']['url']['name'] = t("URL");
  $data['tokens']['site']['url']['description'] = t("The URL of the site's front page.");

  $data['tokens']['site']['login-url']['name'] = t("Login page");
  $data['tokens']['site']['login-url']['description'] = t("The URL of the site's login page.");
  

  // Date formatting tokens.
  $data['tokens']['date']['small']['name'] = t("Small format");
  $data['tokens']['date']['small']['description'] = t("A date in 'small' format. (%date)", array('%date' => format_date(REQUEST_TIME, 'small')));

  $data['tokens']['date']['medium']['name'] = t("Medium format");
  $data['tokens']['date']['medium']['description'] = t("A date in 'medium' format. (%date)", array('%date' => format_date(REQUEST_TIME, 'medium')));

  $data['tokens']['date']['large']['name'] = t("Large format");
  $data['tokens']['date']['large']['description'] = t("A date in 'large' format. (%date)", array('%date' => format_date(REQUEST_TIME, 'large')));

  $data['tokens']['date']['custom']['name'] = t("Custom format");
  $data['tokens']['date']['custom']['description'] = t("A date in a custom format. See !php-date for details.", array('!php-date' => l(t('the PHP documentation'), 'http://php.net/manual/en/function.date.php')));

  $data['tokens']['date']['since']['name'] = t("Time-since");
  $data['tokens']['date']['since']['description'] = t("A data in 'time-since' format. (%date)", array('%date' => format_interval(REQUEST_TIME - 360, 2)));

  $data['tokens']['date']['raw']['name'] = t("Raw timestamp");
  $data['tokens']['date']['raw']['description'] = t("A date in UNIX timestamp format (%date)", array('%date' => REQUEST_TIME));


  // File related tokens.
  $data['tokens']['file']['fid']['name'] = t("File ID");
  $data['tokens']['file']['fid']['description'] = t("The unique ID of the uploaded file.");
  
  $data['tokens']['file']['uid']['name'] = t("User ID");
  $data['tokens']['file']['uid']['description'] = t("The unique ID of the user who owns the file.");

  $data['tokens']['file']['nid']['name'] = t("Node ID");
  $data['tokens']['file']['nid']['description'] = t("The unique ID of the node the file is attached to.");
  
  $data['tokens']['file']['name']['name'] = t("File name");
  $data['tokens']['file']['name']['description'] = t("The name of the file on disk.");

  $data['tokens']['file']['description']['name'] = t("Description");
  $data['tokens']['file']['description']['description'] = t("An optional human-readable description of the file.");
  
  $data['tokens']['file']['path']['name'] = t("Path");
  $data['tokens']['file']['path']['description'] = t("The location of the file on disk.");
  
  $data['tokens']['file']['mime']['name'] = t("MIME type");
  $data['tokens']['file']['mime']['description'] = t("The MIME type of the file.");
  
  $data['tokens']['file']['size']['name'] = t("File size");
  $data['tokens']['file']['size']['description'] = t("The size of the file, in kilobytes.");

  $data['tokens']['file']['path']['name'] = t("URL");
  $data['tokens']['file']['path']['description'] = t("The web-accessible URL for the file.");


  // Chained tokens for files
  $data['tokens']['file']['timestamp']['name'] = t('Timestamp');
  $data['tokens']['file']['timestamp']['description'] = t('The date the file was most recently changed.');
  $data['tokens']['file']['timestamp']['references'] = 'date';

  $data['tokens']['file']['node']['name'] = t('Node');
  $data['tokens']['file']['node']['description'] = t('The node the file is attached to.');
  $data['tokens']['file']['node']['references'] = 'date';

  $data['tokens']['file']['owner']['name'] = t('Owner');
  $data['tokens']['file']['owner']['description'] = t('The user who originally uploaded the file.');
  $data['tokens']['file']['owner']['references'] = 'user';

  return $data;
}
