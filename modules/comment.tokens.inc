<?php
// $Id$

/**
 * @file
 * Builds placeholder replacement tokens for comment-related data.
 */

/**
 * Implement hook_tokens().
 */
function comment_tokens($type, $tokens, $data, $language = NULL, $sanitize = TRUE) {
  $url_options = array('absolute' => TRUE);
  if (isset($language)) {
    $url_options['language'] = $language;
  }

  $replacements = array();

  if ($type == 'comment' && !empty($data['comment'])) {
    $comment = $data['comment'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Simple key values on the comment.
        case 'cid':
          $replacements[$original] = $comment->cid;
          break;

        case 'nid':
          $replacements[$original] = $comment->nid;
          break;

        case 'uid':
          $replacements[$original] = $comment->uid;
          break;

        case 'pid':
          $replacements[$original] = $comment->pid;
          break;

        // Poster identity information for comments
        case 'hostname':
          $replacements[$original] = $sanitize ? check_plain($comment->hostname) : $comment->hostname;
          break;

        case 'name':
          $replacements[$original] = $sanitize ? filter_xss($comment->name) : $comment->name;
          break;

        case 'mail':
          $replacements[$original] = $sanitize ? check_plain($comment->email) : $comment->mail;
          break;

        case 'homepage':
          $replacements[$original] = $sanitize ? filter_xss_bad_protocol($comment->homepage) : $comment->homepage;
          break;

        case 'title':
          $replacements[$original] = $sanitize ? filter_xss($comment->subject) : $comment->subject;
          break;

        case 'body':
          $replacements[$original] = $sanitize ? check_markup($comment->comment, $comment->format) : $replacements[$original] = $comment->comment;
          break;

        // Comment related URLs.
        case 'url':
          $replacements[$original] = url('comment/' . $comment->cid, array('absolute' => TRUE, 'fragment' => 'comment-' . $comment->cid));
          break;

        case 'edit-url':
          $replacements[$original] = url('comment/edit/' . $comment->cid, array('absolute' => TRUE));
          break;

        // Default values for the chained tokens handled below.
        case 'author':
          $replacements[$original] = $sanitize ? filter_xss($comment->name) : $comment->name;
          break;

        case 'parent':
          if (!empty($comment->pid)) {
            $parent = comment_load($comment->pid);
            $replacements[$original] = $sanitize ? filter_xss($parent->subject) : $parent->subject;
          }
          break;

        case 'created':
          $replacements[$original] = format_date($comment->timestamp, 'medium', '', NULL, (isset($language) ? $language->language : NULL));
          break;

        case 'node':
          $node = node_load($comment->nid);
          $replacements[$original] = $sanitize ? filter_xss($node->title) : $node->title;
          break;
      }
    }

    // Chained token relationships.
    if ($node_tokens = token_find_with_prefix($tokens, 'node')) {
      $node = node_load($comment->nid);
      $replacements += module_invoke_all('tokens', 'node', $node_tokens, array('node' => $node), $language, $sanitize);
    }

    if ($date_tokens = token_find_with_prefix($tokens, 'created')) {
      $replacements += module_invoke_all('tokens', 'date', $date_tokens, array('date' => $comment->timestamp), $language, $sanitize);
    }

    if (($parent_tokens = token_find_with_prefix($tokens, 'parent')) && $parent = comment_load($comment->pid)) {
      $replacements += module_invoke_all('tokens', 'comment', $parent_tokens, array('comment' => $parent), $language, $sanitize);
    }

    if (($author_tokens = token_find_with_prefix($tokens, 'author')) && $account = user_load($comment->uid)) {
      $replacements += module_invoke_all('tokens', 'user', $author_tokens, array('user' => $account), $language, $sanitize);
    }
  }
  elseif ($type == 'node' & !empty($data['node'])) {
    $node = $data['node'];

    foreach ($tokens as $name => $original) {
      if ($name == 'comment-count') {
        $replacements[$original] = $node->comment_count;
      }
    }
  }

  return $replacements;
}

/**
 * Implement hook_token_info().
 */
function comment_token_info() {
  $results = array();

  // Metadata for token types.
  $type['name'] = t('Comments');
  $type['description'] = t('Tokens for comments posted on the site.');


  // Comment-related tokens for nodes
  $node['comment-count'] = array(
    'name' => t("Comment count"),
    'description' => t("The number of comments posted on a node."),
  );


  // Core comment tokens
  $comment['cid'] = array(
    'name' => t("Comment ID"),
    'description' => t("The unique ID of the comment."),
  );

  $comment['pid'] = array(
    'name' => t("Parent ID"),
    'description' => t("The unique ID of the comment's parent, if comment threading is active."),
  );

  $comment['nid'] = array(
    'name' => t("Node ID"),
    'description' => t("The unique ID of the node the comment was posted to."),
  );

  $comment['uid'] = array(
    'name' => t("User ID"),
    'description' => t("The unique ID of the user who posted the comment."),
  );

  $comment['hostname'] = array(
    'name' => t("IP Address"),
    'description' => t("The IP address of the computer the comment was posted from."),
  );

  $comment['name'] = array(
    'name' => t("Name"),
    'description' => t("The name left by the comment author."),
  );

  $comment['mail'] = array(
    'name' => t("Email address"),
    'description' => t("The email address left by the comment author."),
  );

  $comment['homepage'] = array(
    'name' => t("Home page"),
    'description' => t("The home page URL left by the comment author."),
  );

  $comment['title'] = array(
    'name' => t("Title"),
    'description' => t("The title of the comment."),
  );

  $comment['body'] = array(
    'name' => t("Content"),
    'description' => t("The formatted content of the comment itself."),
  );

  $comment['url'] = array(
    'name' => t("URL"),
    'description' => t("The URL of the comment."),
  );

  $comment['edit-url'] = array(
    'name' => t("Edit URL"),
    'description' => t("The URL of the comment's edit page."),
  );


  // Chained tokens for comments
  $comment['created'] = array(
    'name' => t("Date created"),
    'description' => t("The date the comment was posted."),
    'references' => 'date',
  );

  $comment['parent'] = array(
    'name' => t("Parent"),
    'description' => t("The comment's parent, if comment threading is active."),
    'references' => 'comment',
  );

  $comment['node'] = array(
    'name' => t("Node"),
    'description' => t("The node the comment was posted to."),
    'references' => 'node',
  );

  $comment['author'] = array(
    'name' => t("Author"),
    'description' => t("The author of the comment, if they were logged in."),
    'references' => 'user',
  );

  $results['types']['comment'] = $type;
  $results['tokens']['node'] = $node;
  $results['tokens']['comment'] = $comment;
  return $results;
}
