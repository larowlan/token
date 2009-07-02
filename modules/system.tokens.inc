<?php

function system_tokens($type, $tokens, $data, $safe_for_html = TRUE) {
  $replacements = array();

  foreach($tokens as $token => $raw) {
    switch ($type) {
      case 'site':
        if ($token == 'name') $replacements[$raw] = variable_get('site_name');
        if ($token == 'mission') $replacements[$raw] = variable_get('mission_statement');
        break;
    }
  }
  
  return $replacements;
}