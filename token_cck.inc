<?php
// $Id$


// Two helper functions that generate appropriate tokens for CCK-added fields.
function content_token_values($type, $object = NULL) {
  $tokens = array();
  if ($type == 'node') {
    $node = $object;
    content_view($node);

    $field_types = _content_field_types();

    foreach (content_fields() as $field) {
      
      $items = $node->$field['field_name'];
      if (!empty($items)) {
        $function = $field_types[$field['type']]['module'] . '_token_values';
        if (function_exists($function)) {
          $sub_list = $function('field', $items);
          foreach($sub_list as $token => $value) {
            $tokens[$field['field_name'] .'-'. $token] = $value;
          }
        }
      }
    }
  }
  return $tokens;
}

function content_token_list($type = 'all') {
  if ($type == 'node' || $type == 'all') {
    $list = array();
    $field_types = _content_field_types();

    foreach (content_fields() as $field) {
      $sub_list = array();
      $function = $field_types[$field['type']]['module'] . '_token_list';
      if (function_exists($function)) {
        $sub_list = $function('field');
        foreach ($sub_list as $category => $token_list) {
          foreach ($token_list as $token => $description) {
            $list['CCK '. $category][$field['field_name'] .'-'. $token] = $description;
          }
        }
      }
    }

    return $list;
  }
}



if (module_exists('nodereference')) {
  function nodereference_token_list($type = 'all') {
    if ($type == 'field' || $type == 'all') {
      $tokens = array();

      $tokens['node reference']['nid']   = t("Referenced node ID.");
      $tokens['node reference']['title'] = t("Referenced node title.");
      $tokens['node reference']['link']  = t("Formatted html link to the node.");

      return $tokens;
    }
  }

  function nodereference_token_values($type, $object = NULL) {
    if ($type == 'field') {
      $item = $object[0];

      $tokens['nid']   = $item['nid'];
      $tokens['title'] = strip_tags($item['view']);
      $tokens['link']  = $item['view'];

      return $tokens;
    }
  }
}



if (module_exists('number')) {
  function number_token_list($type = 'all') {
    if ($type == 'field' || $type == 'all') {
      $tokens = array();

      $tokens['number']['raw']       = t("Raw number value.");
      $tokens['number']['formatted'] = t("Formatted number value.");

      return $tokens;
    }
  }

  function number_token_values($type, $object = NULL) {
    if ($type == 'field') {
      $item = $object[0];

      $tokens['raw']       = $item['value'];
      $tokens['formatted'] = $item['view'];

      return $tokens;
    }
  }
}



if (module_exists('text')) {
  function text_token_list($type = 'all') {
    if ($type == 'field' || $type == 'all') {
      $tokens = array();

      $tokens['text']['raw']       = t("Raw, unfiltered text.");
      $tokens['text']['formatted'] = t("Formatted and filtered text.");

      return $tokens;
    }
  }

  function text_token_values($type, $object = NULL) {
    if ($type == 'field') {
      $item = $object[0];
      $tokens['raw']  = $item['value'];
      $tokens['formatted'] = $item['view'];
      return $tokens;
    }
  }
}


if (module_exists('userreference')) {
  function userreference_token_list($type = 'all') {
    if ($type == 'field' || $type == 'all') {
      $tokens = array();

      $tokens['user reference']['uid']   = t("Referenced user ID");
      $tokens['user reference']['name']  = t("Referenced user name");
      $tokens['user reference']['link']  = t("Formatted html link to referenced user");

      return $tokens;
    }
  }

  function userreference_token_values($type, $object = NULL) {
    if ($type == 'field') {
      $item = $object[0];

      $tokens['uid']   = $item['uid'];
      $tokens['name']  = strip_tags($item['view']);
      $tokens['link']  = $item['view'];

      return $tokens;
    }
  }
}


if (module_exists('link')) {
  function link_token_list($type = 'all') {
    if ($type == 'field' || $type == 'all') {
      $tokens = array();

      $tokens['link']['url']    = t("Link URL");
      $tokens['link']['title']  = t("Link title");
      $tokens['link']['view']   = t("Formatted html link");

      return $tokens;
    }
  }

  function link_token_values($type, $object = NULL) {
    if ($type == 'field') {
      $item = $object[0];

      $tokens['url']    = $item['url'];
      $tokens['title']  = $item['title'];
      $tokens['view']   = $item['view'];

      return $tokens;
    }
  }
}



if (module_exists('imagefield')) {
  function imagefield_token_list($type = 'all') {
    if ($type == 'field' || $type == 'all') {
      $tokens = array();

      $tokens['image']['fid']       = t("Image file ID");
      $tokens['image']['title']     = t("Image title");
      $tokens['image']['alt']       = t("Image alt text");
      $tokens['image']['filename']  = t("Image file name");
      $tokens['image']['filepath']  = t("Image path");
      $tokens['image']['filemime']  = t("Image MIME type");
      $tokens['image']['filesize']  = t("Image file size");
      $tokens['image']['view']      = t("Fully formatted HTML image tag");

      return $tokens;
    }
  }

  function imagefield_token_values($type, $object = NULL) {
    if ($type == 'field') {
      $item = $object[0];

      $tokens['fid']      = $item['fid'];
      $tokens['title']    = $item['title'];
      $tokens['alt']      = $item['alt'];
      $tokens['filename'] = $item['filename'];
      $tokens['filepath'] = $item['filepath'];
      $tokens['filemime'] = $item['filemime'];
      $tokens['filesize'] = $item['filesize'];
      $tokens['view']     = $item['view'];

      return $tokens;
    }
  }
}



if (module_exists('date')) {
  function date_token_list($type = 'all') {
    if ($type == 'field' || $type == 'all') {
      $tokens = array();

      $tokens['date']['value']          = t("The raw date value.");
      $tokens['date']['view']           = t("The formatted date.");
      $tokens['date']['timestamp']      = t("The raw date timestamp.");
      $tokens['date']['yyyy']           = t("Date year (four digit)");
      $tokens['date']['yy']             = t("Date year (two digit)");
      $tokens['date']['month']          = t("Date month (full word)");
      $tokens['date']['mon']            = t("Date month (abbreviated)");
      $tokens['date']['mm']             = t("Date month (two digit, zero padded)");
      $tokens['date']['m']              = t("Date month (one or two digit)");
      $tokens['date']['ww']             = t("Date week (two digit)");
      $tokens['date']['date']           = t("Date date (day of month)");
      $tokens['date']['day']            = t("Date day (full word)");
      $tokens['date']['ddd']            = t("Date day (abbreviation)");
      $tokens['date']['dd']             = t("Date day (two digit, zero-padded)");
      $tokens['date']['d']              = t("Date day (one or two digit)");

      $tokens['date']['to-????']        = t("If the field has a to-date defined, the same tokens exist in the form: [to-????], where ???? is the normal token.");

      return $tokens;
    }
  }

  function date_token_values($type, $object = NULL) {
    if ($type == 'field') {
      include_once(drupal_get_path('module', 'date') .'/date.inc');
      $item = $object[0];

      $date = date_make_date(trim($item['value']));
      $timestamp = date_fuzzy_stamp($date);
      
      $tokens['value']          = $item['value'];
      $tokens['view']           = $item['view'];
      $tokens['timestamp']      = $timestamp;
      $tokens['yyyy']           = date_show_date($date, 'Y');
      $tokens['yy']             = date_show_date($date, 'y');
      $tokens['month']          = date_show_date($date, 'F');
      $tokens['mon']            = date_show_date($date, 'M');
      $tokens['mm']             = date_show_date($date, 'm');
      $tokens['m']              = date_show_date($date, 'n');
      $tokens['ww']             = date_show_date($date, 'W');
      $tokens['date']           = date_show_date($date, 'N');
      $tokens['day']            = date_show_date($date, 'l');
      $tokens['ddd']            = date_show_date($date, 'D');
      $tokens['dd']             = date_show_date($date, 'd');
      $tokens['d']              = date_show_date($date, 'j');

      if (!empty($item['value2'])) {
        $date = date_make_date(trim($item['value']));
        $timestamp = date_fuzzy_stamp($date);

        $tokens['to-value']          = $item['value'];
        $tokens['to-view']           = $item['view'];
        $tokens['to-timestamp']      = $timestamp;
        $tokens['to-yyyy']           = date_show_date($date, 'Y');
        $tokens['to-yy']             = date_show_date($date, 'y');
        $tokens['to-month']          = date_show_date($date, 'F');
        $tokens['to-mon']            = date_show_date($date, 'M');
        $tokens['to-mm']             = date_show_date($date, 'm');
        $tokens['to-m']              = date_show_date($date, 'n');
        $tokens['to-ww']             = date_show_date($date, 'W');
        $tokens['to-date']           = date_show_date($date, 'N');
        $tokens['to-day']            = date_show_date($date, 'l');
        $tokens['to-ddd']            = date_show_date($date, 'D');
        $tokens['to-dd']             = date_show_date($date, 'd');
        $tokens['to-d']              = date_show_date($date, 'j');
      }
      return $tokens;
    }
  }
}