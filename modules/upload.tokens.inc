<?php
// $Id$

function upload_tokens($type, $tokens, $data, $safe_for_html = TRUE) {
  $replacements = array();

  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];
    if (($upload_tokens = token_match_prefix($tokens, 'upload')) && !empty($node->files) && $upload = array_shift($node->files)) {
      $replacements += module_invoke_all('tokens', 'file', $upload_tokens, array('file' => $upload));
    }
  }

  return $replacements;
}

function upload_token_info() {
  $data = array();

  // Upload-related chained tokens for nodes
  $data['tokens']['node']['upload']['name'] = t('File attachment');
  $data['tokens']['node']['upload']['description'] = t('The first file attached to a node, if one exists.');
  $data['tokens']['ndoe']['upload']['references'] = 'file';

  return $data;
}