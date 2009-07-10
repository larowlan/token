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
      
      // These tokens are default variations on the chained tokens handled below.
      elseif ($name == 'author') $replacements[$original] = check_plain($node->name);
      elseif ($name == 'created') $replacements[$original] = format_date($node->created);
      elseif ($name == 'changed') $replacements[$original] = format_date($node->changed);
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
  
  // Metadata for token types.
  $data['types']['node']['name'] = t('Nodes');
  $data['types']['node']['description'] = t('Tokens related to individual nodes.');

  // Basic ids, foreign keys, and metadata for nodes
  $data['tokens']['node']['nid']['name'] = t('Node ID');
  $data['tokens']['node']['nid']['description'] = t('The unique ID of the node.');

  $data['tokens']['node']['vid']['name'] = t('Revision ID');
  $data['tokens']['node']['vid']['description'] = t("The unique ID of the node's latest revision.");

  $data['tokens']['node']['tnid']['name'] = t('Translation set ID');
  $data['tokens']['node']['tnid']['description'] = t('The unique ID of the original-language version of this node, if one exists.');

  $data['tokens']['node']['uid']['name'] = t('User ID');
  $data['tokens']['node']['uid']['description'] = t('The unique ID of the user who posted the node.');
  
  $data['tokens']['node']['type']['name'] = t('Content type');
  $data['tokens']['node']['type']['description'] = t('The type of the node.');

  $data['tokens']['node']['type-name']['name'] = t('Content type name');
  $data['tokens']['node']['type-name']['description'] = t('The human-readable name of the node type.');

  
  
  // Content tokens for the node
  $data['tokens']['node']['title']['name'] = t('Title');
  $data['tokens']['node']['title']['description'] = t('The title of the node.');
  
  $data['tokens']['node']['body']['name'] = t('Content');
  $data['tokens']['node']['body']['description'] = t('The formatted content of the node itself.');

  $data['tokens']['node']['language']['name'] = t('Language');
  $data['tokens']['node']['language']['description'] = t('The language the node is written in.');

  $data['tokens']['node']['url']['name'] = t('URL');
  $data['tokens']['node']['url']['description'] = t('The URL of the node.');

  $data['tokens']['node']['edit-url']['name'] = t('Edit URL');
  $data['tokens']['node']['edit-url']['description'] = t("The URL of the node's edit page.");


  // Chained tokens for nodes
  $data['tokens']['node']['created']['name'] = t('Date created');
  $data['tokens']['node']['created']['description'] = t('The date the node was posted.');
  $data['tokens']['node']['created']['references'] = 'date';

  $data['tokens']['node']['changed']['name'] = t('Date changed');
  $data['tokens']['node']['changed']['description'] = t('The date the node was most recently updated.');
  $data['tokens']['node']['changed']['references'] = 'date';

  $data['tokens']['node']['author']['name'] = t('Author');
  $data['tokens']['node']['author']['description'] = t('The author of the node.');
  $data['tokens']['node']['author']['references'] = 'user';

  return $data;
}