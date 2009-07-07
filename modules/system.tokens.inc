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