<?php
// $Id$

/**
 * @file
 * Builds placeholder replacement tokens for taxonomy terms and vocabularies.
 */

/**
 * Implement hook_tokens().
 */
function taxonomy_tokens($type, $tokens, $data, $language = NULL, $sanitize = TRUE) {
  $replacements = array();

  if ($type == 'term' && !empty($data['term'])) {
    $term = $data['term'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'tid':
          $replacements[$original] = $term->tid;
          break;
        case 'vid':
          $replacements[$original] = $term->vid;
          break;
        case 'name':
          $replacements[$original] = $sanitize ? filter_xss($term->name) : $term->name;
          break;
        case 'description':
          $replacements[$original] = $sanitize ? filter_xss($term->description) : $term->description;
          break;
        case 'node-count':
          $sql = "SELECT COUNT (1) FROM {taxonomy_term_node} tn WHERE tn.tid = :tid";
          $count = db_result(db_query($sql, array(':tid' => $term->tid)));
          $replacements[$original] = $count;
          break;
        case 'vocabulary':
          $vocabulary = taxonomy_vocabulary_load($term->vid);
          $replacements[$original] = filter_xss($vocabulary->name);
          break;
      }
    }

    if ($vocabulary_tokens = token_match_prefix($tokens, 'vocabulary')) {
      $vocabulary = taxonomy_vocabulary_load($term->vid);
      $replacements += module_invoke_all('tokens', 'vocabulary', $vocabulary_tokens, array('vocabulary' => $vocabulary), $language, $sanitize);
    }
  }

  elseif ($type == 'vocabulary' && !empty($data['vocabulary'])) {
    $vocabulary = $data['vocabulary'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'vid':
          $replacements[$original] = $vocabulary->vid;
          break;
        case 'name':
          $replacements[$original] = $sanitize ? filter_xss($vocabulary->name) : $vocabulary->name;
          break;
        case 'description':
          $replacements[$original] = $sanitize ? filter_xss($vocabulary->description) : $vocabulary->description;
          break;
        case 'term-count':
          $sql = "SELECT COUNT (1) FROM {taxonomy_term_data} td WHERE td.vid = :vid";
          $count = db_result(db_query($sql, array(':vid' => $vocabulary->vid)));
          $replacements[$original] = $count;
          break;
        case 'node-count':
          $sql = "SELECT COUNT (1) FROM {taxonomy_term_node} tn LEFT JOIN {taxonomy_term_data} td ON tn.tid = td.tid WHERE td.vid = :vid";
          $count = db_result(db_query($sql, array(':vid' => $vocabulary->vid)));
          $replacements[$original] = $count;
          break;
      }
    }
  }

  return $replacements;
}

/**
 * Implement hook_token_info().
 */
function taxonomy_token_info() {
  $data = array();

  // Metadata for token types.
  $data['types']['term']['name'] = t('Taxonomy terms');
  $data['types']['term']['description'] = t('Tokens related to taxonomy terms.');

  $data['types']['vocabulary']['name'] = t('Vocabularies');
  $data['types']['vocabulary']['description'] = t('Tokens related to taxonomy vocabularies.');


  // Taxonomy term related variables.
  $data['tokens']['term']['tid']['name'] = t("Term ID");
  $data['tokens']['term']['tid']['description'] = t("The unique ID of the taxonomy term.");

  $data['tokens']['term']['vid']['name'] = t("Vocabulary ID");
  $data['tokens']['term']['vid']['description'] = t("The unique ID of the vocabulary the term belongs to.");

  $data['tokens']['term']['name']['name'] = t("Name");
  $data['tokens']['term']['name']['description'] = t("The name of the taxonomy term.");

  $data['tokens']['term']['description']['name'] = t("Description");
  $data['tokens']['term']['description']['description'] = t("The optional description of the taxonomy term.");

  $data['tokens']['term']['node-count']['name'] = t("Node count");
  $data['tokens']['term']['node-count']['description'] = t("The number of nodes tagged with the taxonomy term.");


  // Taxonomy vocabulary related variables.
  $data['tokens']['vocabulary']['vid']['name'] = t("Vocabulary ID");
  $data['tokens']['vocabulary']['vid']['description'] = t("The unique ID of the taxonomy vocabulary.");

  $data['tokens']['vocabulary']['name']['name'] = t("Name");
  $data['tokens']['vocabulary']['name']['description'] = t("The name of the taxonomy vocabulary.");

  $data['tokens']['vocabulary']['description']['name'] = t("Description");
  $data['tokens']['vocabulary']['description']['description'] = t("The optional description of the taxonomy vocabulary.");

  $data['tokens']['vocabulary']['node-count']['name'] = t("Node count");
  $data['tokens']['vocabulary']['node-count']['description'] = t("The number of nodes tagged with terms belonging to the taxonomy vocabulary.");

  $data['tokens']['vocabulary']['term-count']['name'] = t("Node count");
  $data['tokens']['vocabulary']['term-count']['description'] = t("The number of terms belonging to the taxonomy vocabulary.");


  // Chained tokens for taxonomies
  $data['tokens']['term']['vocabulary']['name'] = t('Vocabulary');
  $data['tokens']['term']['vocabulary']['description'] = t('The vocabulary the taxonomy term belongs to.');
  $data['tokens']['term']['vocabulary']['references'] = 'vocabulary';

  return $data;
}
