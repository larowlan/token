<?php
// $Id$

function system_tokens($type, $tokens, $data, $safe_for_html = TRUE) {
  $replacements = array();
  
  if ($type == 'site') {
    foreach($tokens as $token => $raw) {
      if ($token == 'name') $replacements[$raw] = variable_get('site_name');
      elseif ($token == 'mission') $replacements[$raw] = variable_get('mission_statement');
    }
  }
  
  elseif ($type == 'date') {
    if (empty($data['date'])) {
      $date = REQUEST_TIME;
    } else {
      $date = $data['date'];
    }

    foreach($tokens as $token => $raw) {
      if ($token == 'small') $replacements[$raw] = format_date($date, 'small');
      elseif ($token == 'medium') $replacements[$raw] = format_date($date, 'medium');
      elseif ($token == 'large') $replacements[$raw] = format_date($date, 'large');
    }
  }

  return $replacements;
}