<?php
// $Id$

function system_tokens($type, $tokens, $data, $safe_for_html = TRUE) {
  $replacements = array();
  
  if ($type == 'site') {
    foreach($tokens as $name => $original) {
      if ($name == 'name') $replacements[$original] = variable_get('site_name');
      elseif ($name == 'mission') $replacements[$original] = variable_get('mission_statement');
    }
  }
  
  elseif ($type == 'date') {
    if (empty($data['date'])) {
      $date = REQUEST_TIME;
    } else {
      $date = $data['date'];
    }

    foreach($tokens as $name => $raw) {
      if ($name == 'small') $replacements[$original] = format_date($date, 'small');
      elseif ($name == 'medium') $replacements[$original] = format_date($date, 'medium');
      elseif ($name == 'large') $replacements[$original] = format_date($date, 'large');
    }
  }

  return $replacements;
}