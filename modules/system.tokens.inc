<?php
// $Id$

function system_tokens($type, $tokens, $data, $safe_for_html = TRUE) {
  $replacements = array();
  
  if ($type == 'site') {
    foreach($tokens as $name => $original) {
      if ($name == 'name') $replacements[$original] = variable_get('site_name');
      elseif ($name == 'slogan') $replacements[$original] = variable_get('site_slogan');
      elseif ($name == 'mission') $replacements[$original] = variable_get('mission_statement');
      elseif ($name == 'mail') $replacements[$original] = variable_get('site_mail');
      elseif ($name == 'url') $replacements[$original] = url('front');
      elseif ($name == 'login-url') $replacements[$original] = url('user');
    }
  }
  
  elseif ($type == 'date') {
    if (!empty($data['date'])) {
      $date = REQUEST_TIME;
    } else {
      $date = $data['date'];
    }

    foreach($tokens as $name => $raw) {
      if ($name == 'small') $replacements[$original] = format_date($date, 'small');
      elseif ($name == 'medium') $replacements[$original] = format_date($date, 'medium');
      elseif ($name == 'large') $replacements[$original] = format_date($date, 'large');
      elseif ($name == 'since') $replacements[$original] = format_interval($date, 2);
      elseif ($name == 'raw') $replacements[$original] = check_plain($date);
    }

    if ($created_tokens = token_match_prefix($tokens, 'custom')) {
      foreach($created_tokens as $name => $original) {
        $replacements[$original] = format_date($date, 'custom', $name);
      }
    }
  }

  return $replacements;
}



function system_token_info() {
  $data = array();

  // Sitewide variables and metadata.
  $data['site']['name']['name'] = t("Name");
  $data['site']['name']['description'] = t("The name of the site.");

  $data['site']['slogan']['name'] = t("Slogan");
  $data['site']['slogan']['description'] = t("The slogan of the site.");

  $data['site']['mission']['name'] = t("Mission");
  $data['site']['mission']['description'] = t("The optional 'mission' of the site.");

  $data['site']['mail']['name'] = t("Email");
  $data['site']['mail']['description'] = t("The administrative email address for the site.");

  $data['site']['url']['name'] = t("URL");
  $data['site']['url']['description'] = t("The URL of the site's front page.");

  $data['site']['login-url']['name'] = t("Login page");
  $data['site']['login-url']['description'] = t("The URL of the site's login page.");
  

  // Date formatting tokens.
  $data['date']['small']['name'] = t("Small format");
  $data['date']['small']['description'] = t("A date in 'small' format. (%date)", array('%date' => format_date(REQUEST_TIME, 'small')));

  $data['date']['medium']['name'] = t("Medium format");
  $data['date']['medium']['description'] = t("A date in 'medium' format. (%date)", array('%date' => format_date(REQUEST_TIME, 'medium')));

  $data['date']['large']['name'] = t("Large format");
  $data['date']['large']['description'] = t("A date in 'large' format. (%date)", array('%date' => format_date(REQUEST_TIME, 'large')));

  $data['date']['custom']['name'] = t("Custom format");
  $data['date']['custom']['description'] = t("A date in a custom format. See !php-date for details.", array('!php-date' => l(t('the PHP documentation'), 'http://php.net/manual/en/function.date.php')));

  $data['date']['since']['name'] = t("Time-since");
  $data['date']['since']['description'] = t("A data in 'time-since' format. (%date)", array('%date' => format_interval(REQUEST_TIME - 360, 2)));

  $data['date']['raw']['name'] = t("Raw timestamp");
  $data['date']['raw']['description'] = t("A date in UNIX timestamp format (%date)", array('%date' => REQUEST_TIME));

  return $data;
}