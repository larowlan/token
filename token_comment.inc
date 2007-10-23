<?php
// $Id$

/**
 * Implementation of hook_token_values()
 */
function comment_token_values($type, $object = NULL) {
  $values = array();
  switch ($type) {
    case 'comment':

      // Cast to an object just in case fussy drupal gave us an array
      $comment = (object)$object;

      $values['comment-nid']            = $comment->nid;
      $values['comment-title']          = check_plain($comment->subject);
      $values['comment-body']           = check_markup($comment->comment, $comment->format);
      $values['comment-author-name']    = check_plain($comment->name);
      $values['comment-author-uid']     = $comment->uid;

      // Raw counterparts of user supplied data.
      $values['comment-title-raw']       = $comment->subject;
      $values['comment-body-raw']        = $comment->comment;
      $values['comment-author-name-raw'] = $comment->name;

      // Included in case a consuming module wants to format the body
      $values['comment-body-format']     = $comment->format;


      $values['comment-yyyy']           = date('Y', $comment->timestamp);
      $values['comment-yy']             = date('y', $comment->timestamp);
      $values['comment-month']          = date('F', $comment->timestamp);
      $values['comment-mon']            = date('M', $comment->timestamp);
      $values['comment-mm']             = date('m', $comment->timestamp);
      $values['comment-m']              = date('n', $comment->timestamp);
      $values['comment-ww']             = date('W', $comment->timestamp);
      $values['comment-date']           = date('N', $comment->timestamp);
      $values['comment-day']            = date('l', $comment->timestamp);
      $values['comment-ddd']            = date('D', $comment->timestamp);
      $values['comment-dd']             = date('d', $comment->timestamp);
      $values['comment-d']              = date('j', $comment->timestamp);
      break;
  }

  return $values;
}

/**
 * Implementation of hook_token_list()
 */
function comment_token_list($type = 'all') {
  if ($type == 'comment' || $type == 'all') {
    $tokens['comment']['comment-nid']             = t('Comment ID');
    $tokens['comment']['comment-title']           = t('Comment title');
    $tokens['comment']['comment-title-raw']       = t('Comment title. WARNING - raw user input.');
    $tokens['comment']['comment-body']            = t('Comment body');
    $tokens['comment']['comment-body-raw']        = t('Comment body. WARNING - raw user input.');

    $tokens['comment']['comment-author-uid']      = t("Comment author's user id");
    $tokens['comment']['comment-author-name']     = t("Comment author's user name");
    $tokens['comment']['comment-author-name-raw'] = t("Comment author's user name. WARNING - raw user input.");

    $tokens['comment']['comment-yyyy']            = t("Comment creation year (four digit)");
    $tokens['comment']['comment-yy']              = t("Comment creation year (two digit)");
    $tokens['comment']['comment-month']           = t("Comment creation month (full word)");
    $tokens['comment']['comment-mon']             = t("Comment creation month (abbreviated)");
    $tokens['comment']['comment-mm']              = t("Comment creation month (two digit, zero padded)");
    $tokens['comment']['comment-m']               = t("Comment creation month (one or two digit)");
    $tokens['comment']['comment-ww']              = t("Comment creation week (two digit)");
    $tokens['comment']['comment-date']            = t("Comment creation date (day of month)");
    $tokens['comment']['comment-day']             = t("Comment creation day (full word)");
    $tokens['comment']['comment-ddd']             = t("Comment creation day (abbreviation)");
    $tokens['comment']['comment-dd']              = t("Comment creation day (two digit, zero-padded)");
    $tokens['comment']['comment-d']               = t("Comment creation day (one or two digit)");

    return $tokens;
  }
}
