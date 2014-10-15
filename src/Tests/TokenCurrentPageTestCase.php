<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenCurrentPageTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Test the [current-page:*] tokens.
 *
 * @group token
 */
class TokenCurrentPageTestCase extends TokenTestBase {

  protected static $modules = array('path', 'token', 'token_test', 'node');

  function testCurrentPageTokens() {
    $this->drupalGet('user');
    $tokens = array(
      '[current-page:title]' => t('Log in'),
      '[current-page:url]' => \Drupal::url('user.page', [], array('absolute' => TRUE)),
      '[current-page:url:absolute]' => \Drupal::url('user.page', [], array('absolute' => TRUE)),
      '[current-page:url:relative]' => \Drupal::url('user.page', [], array('absolute' => FALSE)),
      '[current-page:url:path]' => 'user',
      '[current-page:url:args:value:0]' => 'user',
      '[current-page:url:args:value:1]' => NULL,
      '[current-page:url:unaliased]' => \Drupal::url('user.page', [], array('absolute' => TRUE, 'alias' => TRUE)),
      '[current-page:page-number]' => 1,
      '[current-page:query:foo]' => NULL,
      '[current-page:query:bar]' => NULL,
      '[current-page:query:q]' => 'user',
      // Deprecated tokens
      '[current-page:arg:0]' => 'user',
      '[current-page:arg:1]' => NULL,
    );
    $this->assertPageTokens('', $tokens);

    $this->drupalCreateContentType(array('type' => 'page'));
    $node = $this->drupalCreateNode(array('title' => 'Node title', 'path' => array('alias' => 'node-alias')));
    $tokens = array(
      '[current-page:title]' => 'Node title',
      '[current-page:url]' => \Drupal::url('entity.node.canonical', ['node' => $node->id()], array('absolute' => TRUE)),
      '[current-page:url:absolute]' => \Drupal::url('entity.node.canonical', ['node' => $node->id()], array('absolute' => TRUE)),
      '[current-page:url:relative]' => \Drupal::url('entity.node.canonical', ['node' => $node->id()], array('absolute' => FALSE)),
      '[current-page:url:alias]' => 'node-alias',
      '[current-page:url:args:value:0]' => 'node-alias',
      '[current-page:url:args:value:1]' => NULL,
      '[current-page:url:unaliased]' => \Drupal::url('entity.node.canonical', ['node' => $node->id()], array('absolute' => TRUE, 'alias' => TRUE)),
      '[current-page:url:unaliased:args:value:0]' => 'node',
      '[current-page:url:unaliased:args:value:1]' => $node->id(),
      '[current-page:url:unaliased:args:value:2]' => NULL,
      '[current-page:page-number]' => 1,
      '[current-page:query:foo]' => 'bar',
      '[current-page:query:bar]' => NULL,
      '[current-page:query:q]' => 'node/1',
      // Deprecated tokens
      '[current-page:arg:0]' => 'node',
      '[current-page:arg:1]' => 1,
      '[current-page:arg:2]' => NULL,
    );
    $this->assertPageTokens("node/{$node->id()}", $tokens, array(), array('url_options' => array('query' => array('foo' => 'bar'))));
  }
}
