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

  // Comment-related tokens for nodes
  $data['node']['comment-count']['name'] = t('Comment count');
  $data['node']['comment-count']['description'] = t('The number of comments posted on a node.');


  // Basic ids and foreign keys for comments
  $data['comment']['cid']['name'] = t('Comment ID');
  $data['comment']['cid']['description'] = t('The unique ID of the comment.');

  $data['comment']['pid']['name'] = t('Parent ID');
  $data['comment']['pid']['description'] = t("The unique ID of the comment's parent, if comment threading is active.");

  $data['comment']['nid']['name'] = t('Node ID');
  $data['comment']['nid']['description'] = t('The unique ID of the node the comment was posted to.');

  $data['comment']['uid']['name'] = t('User ID');
  $data['comment']['uid']['description'] = t('The unique ID of the user who posted the comment.');


  // Poster identity information for comments
  $data['comment']['hostname']['name'] = t('IP Address');
  $data['comment']['hostname']['description'] = t('The IP address of the computer the comment was posted from.');

  $data['comment']['name']['name'] = t('Name');
  $data['comment']['name']['description'] = t("The name left by the comment author.");

  $data['comment']['mail']['name'] = t('Email address');
  $data['comment']['mail']['description'] = t("The email address left by the comment author.");

  $data['comment']['homepage']['name'] = t('Home page');
  $data['comment']['homepage']['description'] = t("The home page URL left by the comment author.");
  
  
  // Content tokens for the comment
  $data['comment']['title']['name'] = t('Title');
  $data['comment']['title']['description'] = t('The title of the comment.');
  
  $data['comment']['body']['name'] = t('Content');
  $data['comment']['body']['description'] = t('The formatted content of the comment itself.');

  $data['comment']['url']['name'] = t('URL');
  $data['comment']['url']['description'] = t('The URL of the comment.');

  $data['comment']['edit-url']['name'] = t('Edit URL');
  $data['comment']['edit-url']['description'] = t("The URL of the comment's edit page.");


  // Chained tokens for comments
  $data['comment']['created']['name'] = t('Date created');
  $data['comment']['created']['description'] = t('The date the comment was posted.');
  $data['comment']['created']['references'] = 'date';

  $data['comment']['parent']['name'] = t('Parent');
  $data['comment']['parent']['description'] = t("The comment's parent, if comment threading is active.");
  $data['comment']['parent']['references'] = 'comment';

  $data['comment']['node']['name'] = t('Node');
  $data['comment']['node']['description'] = t('The node the comment was posted to.');
  $data['comment']['node']['references'] = 'node';

  $data['comment']['author']['name'] = t('Author');
  $data['comment']['author']['description'] = t('The author of the comment, if they were logged in.');
  $data['comment']['author']['references'] = 'user';

  return $data;
}