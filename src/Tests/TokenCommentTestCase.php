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

    /** @var \Drupal\comment\Entity\Comment $parent_comment */
    $parent_comment = entity_create('comment', array(
      'entity_id' => $node->id(),
      'entity_type' => 'node',
      'field_name' => 'comment',
      'name' => 'anonymous user',
      'mail' => 'anonymous@example.com',
      'subject' => $this->randomMachineName(),
      'body' => $this->randomMachineName(),
    ));
    $parent_comment->save();

    // Fix http://example.com/index.php/comment/1 fails 'url:path' test.
    $parent_comment_path = $parent_comment->url();
    $parent_comment_path = ltrim($parent_comment_path, '/');

    $tokens = array(
      'url' => $parent_comment->urlInfo('canonical', ['fragment' => "comment-{$parent_comment->id()}"])->setAbsolute()->toString(),
      'url:absolute' => $parent_comment->urlInfo('canonical', ['fragment' => "comment-{$parent_comment->id()}"])->setAbsolute()->toString(),
      'url:relative' => $parent_comment->urlInfo('canonical', ['fragment' => "comment-{$parent_comment->id()}"])->toString(),
      'url:path' => $parent_comment_path,
      'parent:url:absolute' => NULL,
    );
    $this->assertTokens('comment', array('comment' => $parent_comment), $tokens);

    /** @var \Drupal\comment\Entity\Comment  $comment */
    $comment = entity_create('comment', array(
      'entity_id' => $node->id(),
      'pid' => $parent_comment->id(),
      'entity_type' => 'node',
      'field_name' => 'comment',
      'uid' => 1,
      'name' => 'anonymous user',
      'mail' => 'anonymous@example.com',
      'subject' => $this->randomMachineName(),
      'body' => $this->randomMachineName(),
    ));
    $comment->save();

    // Fix http://example.com/index.php/comment/1 fails 'url:path' test.
    $comment_path = \Drupal::url('entity.comment.canonical', array('comment' => $comment->id()));
    $comment_path = ltrim($comment_path, '/');

    $tokens = array(
      'url' => $comment->urlInfo('canonical', ['fragment' => "comment-{$comment->id()}"])->setAbsolute()->toString(),
      'url:absolute' => $comment->urlInfo('canonical', ['fragment' => "comment-{$comment->id()}"])->setAbsolute()->toString(),
      'url:relative' => $comment->urlInfo('canonical', ['fragment' => "comment-{$comment->id()}"])->toString(),
      'url:path' => $comment_path,
      'parent:url:absolute' => $parent_comment->urlInfo('canonical', ['fragment' => "comment-{$parent_comment->id()}"])->setAbsolute()->toString(),
    );
    $this->assertTokens('comment', array('comment' => $comment), $tokens);
  }
}
