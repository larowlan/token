<?php
// $Id$

function taxonomy_tokens($type, $tokens, $data, $safe_for_html = TRUE) {
  $replacements = array();

  if ($type == 'term') {
    if (empty($data['term'])) {
      return;
    } else {
      $term = $data['term'];
    }

    foreach($tokens as $name => $original) {
      if ($name == 'tid') $replacements[$original] = $term->tid;
      elseif ($name == 'vid') $replacements[$original] = $term->vid;
      elseif ($name == 'name') $replacements[$original] = check_plain($term->name);
      elseif ($name == 'description') $replacements[$original] = check_plain($term->description);
      elseif ($name == 'node-count') {
        $sql = "SELECT COUNT (1) FROM {taxonomy_term_node} tn WHERE tn.tid = :tid";
        $count = db_result(db_query($sql, array(':tid' => $term->tid)));
        $replacements[$original] = check_plain($count);
      }
    }
    
    if ($vocabulary_tokens = token_match_prefix($tokens, 'vocabulary')) {
      $vocabulary = taxonomy_vocabulary_load($term->vid);
      $replacements += module_invoke_all('tokens', 'vocabulary', $vocabulary_tokens, array('vocabulary' => $vocabulary));
    }
  }

  elseif ($type == 'vocabulary') {
    if (empty($data['vocabulary'])) {
      return;
    } else {
      $vocabulary = $data['vocabulary'];
    }

    foreach($tokens as $name => $original) {
      if ($name == 'vid') $replacements[$original] = $vocabulary->vid;
      elseif ($name == 'name') $replacements[$original] = check_plain($vocabulary->name);
      elseif ($name == 'description') $replacements[$original] = check_plain($vocabulary->description);
      elseif ($name == 'term-count') {
        $sql = "SELECT COUNT (1) FROM {taxonomy_term_data} td WHERE td.vid = :vid";
        $count = db_result(db_query($sql, array(':vid' => $vocabulary->vid)));
        $replacements[$original] = check_plain($count);
      }
      elseif ($name == 'node-count') {
        $sql = "SELECT COUNT (1) FROM {taxonomy_term_node} tn LEFT JOIN {taxonomy_term_data} td ON tn.tid = td.tid WHERE td.vid = :vid";
        $count = db_result(db_query($sql, array(':vid' => $vocabulary->vid)));
        $replacements[$original] = check_plain($count);
      }
    }
  }

  return $replacements;
}