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

  // Metadata for token types.
  $data['types']['site']['name'] = t('Site information');
  $data['types']['site']['description'] = t('Tokens for site-wide settings and other global information.');

  $data['types']['date']['name'] = t('Dates');
  $data['types']['date']['description'] = t('Tokens related to times and dates.');


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

  return $data;
}