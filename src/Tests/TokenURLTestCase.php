<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenURLTestCase.
 */
namespace Drupal\token\Tests;
use Drupal\Core\Url;

/**
 * Tests url tokens.
 *
 * @group token
 */
class TokenURLTestCase extends TokenTestBase {
  protected static $modules = array('path', 'token', 'token_test', 'node');

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    parent::setUp();
    $this->saveAlias('node/1', 'first-node');
  }

  function testURLTokens() {
    $host = \Drupal::request()->getHttpHost();
    $tokens = array(
      'absolute' => "http://{$host}/first-node",
      'relative' => base_path() . 'first-node',
      'path' => 'first-node',
      'brief' => "{$host}/first-node",
      'args:value:0' => 'first-node',
      'args:value:1' => NULL,
      'args:value:N' => NULL,
      'unaliased' => "http://{$host}/node/1",
      'unaliased:relative' => base_path() . 'node/1',
      'unaliased:path' => 'node/1',
      'unaliased:brief' => "{$host}/node/1",
      'unaliased:args:value:0' => 'node',
      'unaliased:args:value:1' => '1',
      'unaliased:args:value:2' => NULL,
      // Deprecated tokens.
      'alias' => 'first-node',
    );
    $this->assertTokens('url', array('url' => new Url('entity.node.canonical', array('node' => 1))), $tokens);
  }
}
