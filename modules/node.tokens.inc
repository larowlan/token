<?php
// $Id$

function node_tokens($type, $tokens, $data, $safe_for_html = TRUE) {
  $replacements = array();

  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];

    foreach($tokens as $name => $original) {
      if ($name == 'nid') $replacements[$original] = $node->nid;
      elseif ($name == 'vid') $replacements[$original] = $node->vid;
      elseif ($name == 'tnid') $replacements[$original] = $node->tnid;
      elseif ($name == 'uid') $replacements[$original] = $node->uid;
      elseif ($name == 'name') $replacements[$original] = check_plain($node->name);
      elseif ($name == 'title') $replacements[$original] = check_plain($node->title);
      elseif ($name == 'type') $replacements[$original] = $node->type;
      elseif ($name == 'type-name') $replacements[$original] = node_get_types('name', $node->type);
      elseif ($name == 'language') $replacements[$original] = filter_xss_admin($node->language);
      elseif ($name == 'url') $replacements[$original] = url('node/' . $node->nid);
      elseif ($name == 'edit-url') $replacements[$original] = url('node/' . $node->nid . '/edit');
      elseif ($name == 'body') {
        if ($safe_for_html) $replacements[$original] = check_markup($node->body, $node->format);
        else $replacements[$original] = $node->body;
      }
    }
    
    if ($author_tokens = token_match_prefix($tokens, 'author')) {
      $author = user_load($node->uid);
      $replacements += module_invoke_all('tokens', 'user', $author_tokens, array('user' => $author));
    }

    if ($created_tokens = token_match_prefix($tokens, 'created')) {
      $replacements += module_invoke_all('tokens', 'date', $created_tokens, array('date' => $node->created));
    }

    if ($changed_tokens = token_match_prefix($tokens, 'changed')) {
      $replacements += module_invoke_all('tokens', 'date', $changed_tokens, array('date' => $node->changed));
    }
  }
  
  return $replacements;
}


function node_token_info() {
  $data = array();

  // Basic ids, foreign keys, and metadata for nodes
  $data['node']['nid']['name'] = t('Node ID');
  $data['node']['nid']['description'] = t('The unique ID of the node.');

  $data['node']['vid']['name'] = t('Revision ID');
  $data['node']['vid']['description'] = t("The unique ID of the node's latest revision.");

  $data['node']['tnid']['name'] = t('Translation set ID');
  $data['node']['tnid']['description'] = t('The unique ID of the original-language version of this node, if one exists.');

  $data['node']['uid']['name'] = t('User ID');
  $data['node']['uid']['description'] = t('The unique ID of the user who posted the node.');
  
  $data['node']['type']['name'] = t('Content type');
  $data['node']['type']['description'] = t('The type of the node.');

  $data['node']['type-name']['name'] = t('Content type name');
  $data['node']['type-name']['description'] = t('The human-readable name of the node type.');

  
  
  // Content tokens for the node
  $data['node']['title']['name'] = t('Title');
  $data['node']['title']['description'] = t('The title of the node.');
  
  $data['node']['body']['name'] = t('Content');
  $data['node']['body']['description'] = t('The formatted content of the node itself.');

  $data['node']['language']['name'] = t('Language');
  $data['node']['language']['description'] = t('The language the node is written in.');

  $data['node']['url']['name'] = t('URL');
  $data['node']['url']['description'] = t('The URL of the node.');

  $data['node']['edit-url']['name'] = t('Edit URL');
  $data['node']['edit-url']['description'] = t("The URL of the node's edit page.");


  // Chained tokens for comments
  $data['node']['created']['name'] = t('Date created');
  $data['node']['created']['description'] = t('The date the node was posted.');
  $data['node']['created']['references'] = 'date';

  $data['node']['changed']['name'] = t('Date changed');
  $data['node']['changed']['description'] = t('The date the node was most recently updated.');
  $data['node']['changed']['references'] = 'date';

  $data['node']['author']['name'] = t('Author');
  $data['node']['author']['description'] = t('The author of the node.');
  $data['node']['author']['references'] = 'user';

  return $data;
}