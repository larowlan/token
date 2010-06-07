<?php
// $Id$

/**
 * @file
 * Token callbacks for the token module.
 */

/**
 * Implements hook_token_info_alter().
 */
function token_token_info_alter(&$data) {
  // Add a 'dynamic' key to any tokens that have chained but dynamic tokens.
  $data['tokens']['date']['custom']['dynamic'] = TRUE;
}

/**
 * Implements hook_token_info().
 */
function token_token_info() {
  $info['tokens']['node']['source'] = array(
    'name' => t('Translation source node'),
    'description' => t("The source node for this current node's translation set."),
    'type' => 'node',
  );
  $info['tokens']['node']['log'] = array(
    'name' => t('Revision log message'),
    'description' => t('The explanation of the most recent changes made to the node.'),
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function token_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);

  // Node tokens.
  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'source':
          if (!empty($node->tnid) && $source_node = node_load($node->tnid)) {
            $title = $source_node->title;
            $replacements[$original] = $sanitize ? filter_xss($title) : $title;
          }
          break;
        case 'log':
          $replacements[$original] = $sanitize ? filter_xss($node->log) : $node->log;
          break;
      }
    }

    // Chained token relationships.
    if (!empty($node->tnid) && ($source_node = node_load($node->tnid)) && $source_tokens = token_find_with_prefix($tokens, 'source')) {
      $replacements += token_generate('node', $source_tokens, array('node' => $source_node), $options);
    }
  }

  return $replacements;
}
