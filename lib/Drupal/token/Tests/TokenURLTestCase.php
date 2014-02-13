<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenURLTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Tests url tokens.
 */
class TokenURLTestCase extends TokenTestBase {
  public static function getInfo() {
    return array(
      'name' => 'URL token tests',
      'description' => 'Test the URL tokens.',
      'group' => 'Token',
    );
  }

  public function setUp($modules = array()) {
    parent::setUp($modules);
    $this->saveAlias('node/1', 'first-node');
  }

  function testURLTokens() {
    $tokens = array(
      'absolute' => 'http://example.com/first-node',
      'relative' => base_path() . 'first-node',
      'path' => 'first-node',
      'brief' => 'example.com/first-node',
      'args:value:0' => 'first-node',
      'args:value:1' => NULL,
      'args:value:N' => NULL,
      'unaliased' => 'http://example.com/node/1',
      'unaliased:relative' => base_path() . 'node/1',
      'unaliased:path' => 'node/1',
      'unaliased:brief' => 'example.com/node/1',
      'unaliased:args:value:0' => 'node',
      'unaliased:args:value:1' => '1',
      'unaliased:args:value:2' => NULL,
      // Deprecated tokens.
      'alias' => 'first-node',
    );
    $this->assertTokens('url', array('path' => 'node/1', 'options' => array('base_url' => 'http://example.com')), $tokens);
  }
}
