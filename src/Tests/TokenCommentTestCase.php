<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenCommentTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Tests comment tokens.
 *
 * @group token
 */
class TokenCommentTestCase extends TokenTestBase {
  protected static $modules = array('path', 'token', 'token_test', 'node', 'comment');

  function testCommentTokens() {
    $this->drupalCreateContentType(array('type' => 'page', 'name' => t('Page')));
    \Drupal::service('comment.manager')->addDefaultField('node', 'page');
    $node = $this->drupalCreateNode();

    $parent_comment = entity_create('comment', array(
      'entity_id' => $node->id(),
      'entity_type' => 'node',
      'field_name' => 'comment',
      'name' => 'anonymous user',
      'mail' => 'anonymous@example.com',
      'subject' => $this->randomName(),
      'body' => $this->randomName(),
    ));
    $parent_comment->save();

    $tokens = array(
      'url' => url('comment/' . $parent_comment->id(), array('fragment' => 'comment-' . $parent_comment->id(), 'absolute' => TRUE)),
      'url:absolute' => url('comment/' . $parent_comment->id(), array('fragment' => 'comment-' . $parent_comment->id(), 'absolute' => TRUE)),
      'url:relative' => url('comment/' . $parent_comment->id(), array('fragment' => 'comment-' . $parent_comment->id(), 'absolute' => FALSE)),
      'url:path' => 'comment/' . $parent_comment->id(),
      'parent:url:absolute' => NULL,
    );
    $this->assertTokens('comment', array('comment' => $parent_comment), $tokens);

    $comment = entity_create('comment', array(
      'entity_id' => $node->id(),
      'pid' => $parent_comment->id(),
      'entity_type' => 'node',
      'field_name' => 'comment',
      'uid' => 1,
      'name' => 'anonymous user',
      'mail' => 'anonymous@example.com',
      'subject' => $this->randomName(),
      'body' => $this->randomName(),
    ));
    $comment->save();

    $tokens = array(
      'url' => url('comment/' . $comment->id(), array('fragment' => 'comment-' . $comment->id(), 'absolute' => TRUE)),
      'url:absolute' => url('comment/' . $comment->id(), array('fragment' => 'comment-' . $comment->id(), 'absolute' => TRUE)),
      'url:relative' => url('comment/' . $comment->id(), array('fragment' => 'comment-' . $comment->id(), 'absolute' => FALSE)),
      'url:path' => 'comment/' . $comment->id(),
      'parent:url:absolute' => url('comment/' . $parent_comment->id(), array('fragment' => 'comment-' . $parent_comment->id(), 'absolute' => TRUE)),
    );
    $this->assertTokens('comment', array('comment' => $comment), $tokens);
  }
}
