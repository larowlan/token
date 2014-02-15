<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenCurrentPageTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Tests current page tokens.
 */
class TokenCurrentPageTestCase extends TokenTestBase {
  public static function getInfo() {
    return array(
      'name' => 'Current page token tests',
      'description' => 'Test the [current-page:*] tokens.',
      'group' => 'Token',
    );
  }

  function testCurrentPageTokens() {
    $tokens = array(
      '[current-page:title]' => t('Welcome to @site-name', array('@site-name' => \Drupal::config('system.site')->get('name'))),
      '[current-page:url]' => url('node', array('absolute' => TRUE)),
      '[current-page:url:absolute]' => url('node', array('absolute' => TRUE)),
      '[current-page:url:relative]' => url('node', array('absolute' => FALSE)),
      '[current-page:url:path]' => 'node',
      '[current-page:url:args:value:0]' => 'node',
      '[current-page:url:args:value:1]' => NULL,
      '[current-page:url:unaliased]' => url('node', array('absolute' => TRUE, 'alias' => TRUE)),
      '[current-page:page-number]' => 1,
      '[current-page:query:foo]' => NULL,
      '[current-page:query:bar]' => NULL,
      '[current-page:query:q]' => 'node',
      // Deprecated tokens
      '[current-page:arg:0]' => 'node',
      '[current-page:arg:1]' => NULL,
    );
    $this->assertPageTokens('', $tokens);

    $node = $this->drupalCreateNode(array('title' => 'Node title', 'path' => array('alias' => 'node-alias')));
    $tokens = array(
      '[current-page:title]' => 'Node title',
      '[current-page:url]' => url("node/{$node->id()}", array('absolute' => TRUE)),
      '[current-page:url:absolute]' => url("node/{$node->id()}", array('absolute' => TRUE)),
      '[current-page:url:relative]' => url("node/{$node->id()}", array('absolute' => FALSE)),
      '[current-page:url:alias]' => 'node-alias',
      '[current-page:url:args:value:0]' => 'node-alias',
      '[current-page:url:args:value:1]' => NULL,
      '[current-page:url:unaliased]' => url("node/{$node->id()}", array('absolute' => TRUE, 'alias' => TRUE)),
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
