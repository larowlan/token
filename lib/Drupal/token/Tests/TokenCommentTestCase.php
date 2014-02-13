<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenCommentTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Tests comment tokens.
 */
class TokenCommentTestCase extends TokenTestBase {
  public static function getInfo() {
    return array(
      'name' => 'Comment token tests',
      'description' => 'Test the comment tokens.',
      'group' => 'Token',
    );
  }

  public function setUp($modules = array()) {
    $modules[] = 'comment';
    parent::setUp($modules);
  }

  function testCommentTokens() {
    $node = $this->drupalCreateNode(array('comment' => COMMENT_NODE_OPEN));

    $parent_comment = new stdClass;
    $parent_comment->nid = $node->nid;
    $parent_comment->pid = 0;
    $parent_comment->cid = NULL;
    $parent_comment->uid = 0;
    $parent_comment->name = 'anonymous user';
    $parent_comment->mail = 'anonymous@example.com';
    $parent_comment->subject = $this->randomName();
    $parent_comment->timestamp = mt_rand($node->created, REQUEST_TIME);
    $parent_comment->language = LANGUAGE_NOT_SPECIFIED;
    $parent_comment->body[LANGUAGE_NOT_SPECIFIED][0] = $this->randomName();
    comment_save($parent_comment);

    $tokens = array(
      'url' => url('comment/' . $parent_comment->cid, array('fragment' => 'comment-' . $parent_comment->cid, 'absolute' => TRUE)),
      'url:absolute' => url('comment/' . $parent_comment->cid, array('fragment' => 'comment-' . $parent_comment->cid, 'absolute' => TRUE)),
      'url:relative' => url('comment/' . $parent_comment->cid, array('fragment' => 'comment-' . $parent_comment->cid, 'absolute' => FALSE)),
      'url:path' => 'comment/' . $parent_comment->cid,
      'parent:url:absolute' => NULL,
    );
    $this->assertTokens('comment', array('comment' => $parent_comment), $tokens);

    $comment = new stdClass();
    $comment->nid = $node->nid;
    $comment->pid = $parent_comment->cid;
    $comment->cid = NULL;
    $comment->uid = 1;
    $comment->subject = $this->randomName();
    $comment->timestamp = mt_rand($parent_comment->created, REQUEST_TIME);
    $comment->language = LANGUAGE_NOT_SPECIFIED;
    $comment->body[LANGUAGE_NOT_SPECIFIED][0] = $this->randomName();
    comment_save($comment);

    $tokens = array(
      'url' => url('comment/' . $comment->cid, array('fragment' => 'comment-' . $comment->cid, 'absolute' => TRUE)),
      'url:absolute' => url('comment/' . $comment->cid, array('fragment' => 'comment-' . $comment->cid, 'absolute' => TRUE)),
      'url:relative' => url('comment/' . $comment->cid, array('fragment' => 'comment-' . $comment->cid, 'absolute' => FALSE)),
      'url:path' => 'comment/' . $comment->cid,
      'parent:url:absolute' => url('comment/' . $parent_comment->cid, array('fragment' => 'comment-' . $parent_comment->cid, 'absolute' => TRUE)),
    );
    $this->assertTokens('comment', array('comment' => $comment), $tokens);
  }
}
