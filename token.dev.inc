<?php
// $Id$

function token_test_form(&$form_state) {
  $form['help'] = array(
    '#prefix' => '<strong>',
    '#markup' => t('Enter tokens in this field. Optionally pick a node and/or comment to be added to the replacement context.'),
    '#prefix' => '</strong>',
  );
  
  $sql = "SELECT n.nid, n.title FROM {node} n WHERE n.status = 1 ORDER BY n.created DESC LIMIT 10";
  $results = db_query($sql);
  $nodes = array('' => t('-None-'));
  while ($node = db_fetch_object($results)) {
    $nodes[$node->nid] = check_plain($node->title);
  }
  $form['node'] = array(
    '#type' => 'select',
    '#title' => t('Node'),
    '#options' => $nodes,
    '#default_value' => empty($form_state['input']['node']) ? '' : $form_state['input']['node'],
  );

  $sql = "SELECT c.cid, c.subject FROM {comment} c WHERE c.status = 1 ORDER BY c.timestamp DESC LIMIT 10";
  $results = db_query($sql);
  $comments = array('' => t('-None-'));
  while ($comment = db_fetch_object($results)) {
    $comments[$comment->nid] = check_plain($comment->subject);
  }
  $form['comment'] = array(
    '#type' => 'select',
    '#title' => t('Comment'),
    '#options' => $comments,
    '#default_value' => empty($form_state['input']['comment']) ? '' : $form_state['input']['comment'],
  );

  $form['text'] = array(
    '#type' => 'textarea',
    '#title' => t('Enter your text here.'),
    '#default_value' => empty($form_state['input']['text']) ? 'Hi, [user:name]. Welcome to [site:name]!' : $form_state['input']['text'],
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Replace tokens'),
  );
  
  if (!empty($form_state['input']['text'])) {
    $v = $form_state['input'];
    $data = array();
    if (!empty($v['node'])) {
      $data['node'] = node_load($v['node']);
    }
  
    if (!empty($v['comment'])) {
      $data['comment'] = comment_load($v['comment']);
    }

    $form['results'] = array(
      '#type' => 'item',
      '#title' => t('Results'),
      '#markup' => token_replace($form_state['input']['text'], $data),
    );
  }
  else {
    dsm('wait what?');
  }

  return $form;
}

function token_test_form_submit(&$form, &$form_state) {
  if (!empty($form_state['input'])) {
    $form_state['redirect'] = FALSE;
  }
}