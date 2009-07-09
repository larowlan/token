<?php
// $Id$

function comment_tokens($type, $tokens, $data, $safe_for_html = TRUE) {
  $replacements = array();

  if ($type == 'comment' && !empty($data['comment'])) {
    $comment = $data['comment'];

    foreach($tokens as $name => $original) {
      if ($name == 'cid') $replacements[$original] = $comment->cid;
      elseif ($name == 'nid') $replacements[$original] = $comment->nid;
      elseif ($name == 'uid') $replacements[$original] = $comment->uid;
      elseif ($name == 'pid') $replacements[$original] = $comment->pid;
      elseif ($name == 'hostname') $replacements[$original] = check_plain($comment->hostname);
      elseif ($name == 'name') $replacements[$original] = check_plain($comment->name);
      elseif ($name == 'mail') $replacements[$original] = check_plain($comment->mail);
      elseif ($name == 'homepage') $replacements[$original] = check_url($comment->homepage);
      elseif ($name == 'title') $replacements[$original] = check_plain($comment->subject);
      elseif ($name == 'body') {
        if ($safe_for_html) $replacements[$original] = check_markup($comment->comment, $comment->format);
        else $replacements[$original] = $comment->comment;
      }
      elseif ($name == 'url') $replacements[$original] = url('comment/' . $comment->cid, array('fragment' => 'comment-' . $comment->cid));
      elseif ($name == 'edit-url') $replacements[$original] = url('comment/edit/' . $comment->cid);
    }
    
    if ($node_tokens = token_match_prefix($tokens, 'node')) {
      $node = node_load($comment->nid);
      $replacements += module_invoke_all('tokens', 'node', $node_tokens, array('node' => $node));
    }

    if ($date_tokens = token_match_prefix($tokens, 'created')) {
      $replacements += module_invoke_all('tokens', 'date', $date_tokens, array('date' => $comment->timestamp));
    }

    if ($parent_tokens = token_match_prefix($tokens, 'parent') && $parent = comment_load($comment->pid)) {
      $replacements += module_invoke_all('tokens', 'comment', $parent_tokens, array('comment' => $parent));
    }

    if ($author_tokens = token_match_prefix($tokens, 'author') && $account = user_load($comment->uid)) {
      $replacements += module_invoke_all('tokens', 'user', $author_tokens, array('user' => $account));
    }
  }
  elseif ($type == 'node' & !empty($data['node'])) {
    $node = $data['node'];

    foreach($tokens as $name => $original) {
      if ($name == 'comment-count') $replacements[$original] = $node->comment_count;
    }
  }

  return $replacements;
}

function comment_token_info() {
  $data = array();

  // Metadata for token types.
  $data['types']['comment']['name'] = t('Comments');
  $data['types']['comment']['description'] = t('Tokens for comments posted on the site.');


  // Comment-related tokens for nodes
  $data['tokens']['node']['comment-count']['name'] = t('Comment count');
  $data['tokens']['node']['comment-count']['description'] = t('The number of comments posted on a node.');


  // Basic ids and foreign keys for comments
  $data['tokens']['comment']['cid']['name'] = t('Comment ID');
  $data['tokens']['comment']['cid']['description'] = t('The unique ID of the comment.');

  $data['tokens']['comment']['pid']['name'] = t('Parent ID');
  $data['tokens']['comment']['pid']['description'] = t("The unique ID of the comment's parent, if comment threading is active.");

  $data['tokens']['comment']['nid']['name'] = t('Node ID');
  $data['tokens']['comment']['nid']['description'] = t('The unique ID of the node the comment was posted to.');

  $data['tokens']['comment']['uid']['name'] = t('User ID');
  $data['tokens']['comment']['uid']['description'] = t('The unique ID of the user who posted the comment.');


  // Poster identity information for comments
  $data['tokens']['comment']['hostname']['name'] = t('IP Address');
  $data['tokens']['comment']['hostname']['description'] = t('The IP address of the computer the comment was posted from.');

  $data['tokens']['comment']['name']['name'] = t('Name');
  $data['tokens']['comment']['name']['description'] = t("The name left by the comment author.");

  $data['tokens']['comment']['mail']['name'] = t('Email address');
  $data['tokens']['comment']['mail']['description'] = t("The email address left by the comment author.");

  $data['tokens']['comment']['homepage']['name'] = t('Home page');
  $data['tokens']['comment']['homepage']['description'] = t("The home page URL left by the comment author.");
  
  
  // Content tokens for the comment
  $data['tokens']['comment']['title']['name'] = t('Title');
  $data['tokens']['comment']['title']['description'] = t('The title of the comment.');
  
  $data['tokens']['comment']['body']['name'] = t('Content');
  $data['tokens']['comment']['body']['description'] = t('The formatted content of the comment itself.');

  $data['tokens']['comment']['url']['name'] = t('URL');
  $data['tokens']['comment']['url']['description'] = t('The URL of the comment.');

  $data['tokens']['comment']['edit-url']['name'] = t('Edit URL');
  $data['tokens']['comment']['edit-url']['description'] = t("The URL of the comment's edit page.");


  // Chained tokens for comments
  $data['tokens']['comment']['created']['name'] = t('Date created');
  $data['tokens']['comment']['created']['description'] = t('The date the comment was posted.');
  $data['tokens']['comment']['created']['references'] = 'date';

  $data['tokens']['comment']['parent']['name'] = t('Parent');
  $data['tokens']['comment']['parent']['description'] = t("The comment's parent, if comment threading is active.");
  $data['tokens']['comment']['parent']['references'] = 'comment';

  $data['tokens']['comment']['node']['name'] = t('Node');
  $data['tokens']['comment']['node']['description'] = t('The node the comment was posted to.');
  $data['tokens']['comment']['node']['references'] = 'node';

  $data['tokens']['comment']['author']['name'] = t('Author');
  $data['tokens']['comment']['author']['description'] = t('The author of the comment, if they were logged in.');
  $data['tokens']['comment']['author']['references'] = 'user';

  return $data;
}