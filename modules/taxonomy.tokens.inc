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

        case 'url':
          $replacements[$original] = url('taxonomy/term/' . $term->tid, array('absolute' => TRUE));
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

        case 'parent':
          $parents = taxonomy_get_parents($term->tid);
          $parent = array_pop($parents);
          $replacements[$original] = filter_xss($parent->name);
          break;
      }
    }

    if ($vocabulary_tokens = token_find_with_prefix($tokens, 'vocabulary')) {
      $vocabulary = taxonomy_vocabulary_load($term->vid);
      $replacements += module_invoke_all('tokens', 'vocabulary', $vocabulary_tokens, array('vocabulary' => $vocabulary), $language, $sanitize);
    }

    if ($vocabulary_tokens = token_find_with_prefix($tokens, 'parent')) {
      $parents = taxonomy_get_parents($term->tid);
      $parent = array_pop($parents);
      $replacements += module_invoke_all('tokens', 'term', $vocabulary_tokens, array('term' => $parent), $language, $sanitize);
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
  $results = array();

  // Metadata for token types.
  $types['term'] = array(
    'name' => t("Taxonomy terms"),
    'description' => t("Tokens related to taxonomy terms."),
  );

  $types['vocabulary'] = array(
    'name' => t("Vocabularies"),
    'description' => t("Tokens related to taxonomy vocabularies."),
  );


  // Taxonomy term related variables.
  $term['tid'] = array(
    'name' => t("Term ID"),
    'description' => t("The unique ID of the taxonomy term."),
  );

  $term['vid'] = array(
    'name' => t("Vocabulary ID"),
    'description' => t("The unique ID of the vocabulary the term belongs to."),
  );

  $term['name'] = array(
    'name' => t("Name"),
    'description' => t("The name of the taxonomy term."),
  );

  $term['description'] = array(
    'name' => t("Description"),
    'description' => t("The optional description of the taxonomy term."),
  );

  $term['node-count'] = array(
    'name' => t("Node count"),
    'description' => t("The number of nodes tagged with the taxonomy term."),
  );

  $term['url'] = array(
    'name' => t("URL"),
    'description' => t("The URL of the taxonomy term."),
  );


  // Taxonomy vocabulary related variables.
  $vocabulary['vid'] = array(
    'name' => t("Vocabulary ID"),
    'description' => t("The unique ID of the taxonomy vocabulary."),
  );

  $vocabulary['name'] = array(
    'name' => t("Name"),
    'description' => t("The name of the taxonomy vocabulary."),
  );

  $vocabulary['description'] = array(
    'name' => t("Description"),
    'description' => t("The optional description of the taxonomy vocabulary."),
  );

  $vocabulary['node-count'] = array(
    'name' => t("Node count"),
    'description' => t("The number of nodes tagged with terms belonging to the taxonomy vocabulary."),
  );

  $vocabulary['term-count'] = array(
    'name' => t("Node count"),
    'description' => t("The number of terms belonging to the taxonomy vocabulary."),
  );


  // Chained tokens for taxonomies
  $term['vocabulary'] = array(
    'name' => t("Vocabulary"),
    'description' => t("The vocabulary the taxonomy term belongs to."),
    'references' => 'vocabulary',
  );

  $term['parent'] = array(
    'name' => t("Parent term"),
    'description' => t("The parent term of the taxonomy term, if one exists."),
    'references' => 'term',
  );

  $results['tokens']['term'] = $term;
  $results['tokens']['vocabulary'] = $vocabulary;
  $results['types'] = $types;

  return $results;
}
