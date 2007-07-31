<?php
// $Id$

/**
 * Implementation of hook_token_values()
 */
function taxonomy_token_values($type, $object = NULL) {
  switch ($type) {
    case 'taxonomy':
      $category = $object;
      
      $vid = $category->vid;
      $vocabulary = taxonomy_get_vocabulary($vid);
      $values['vid'] = $vid;
      $values['vocab'] = $vocabulary->name;
      $values['cat'] = $category->name;
      $values['tid'] = $category->tid;
      
      break;
  }
  return $values;
}

/**
 * Implementation of hook_token_list()
 */
function taxonomy_token_list($type = 'all') {
  if ($type == 'taxonomy' || $type == 'all') {
    $tokens['taxonomy']['tid'] = t("The id number of the category's parent vocabulary.");
    $tokens['taxonomy']['vocab'] = t("The vocabulary that the page's first category belongs to.");
    $tokens['taxonomy']['cat'] = t('The name of the category.');
    $tokens['taxonomy']['tid'] = t('The id number of the category.');

    return $tokens;
  }
}