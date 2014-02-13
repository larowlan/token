<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenRandomTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Tests random tokens.
 */
class TokenRandomTestCase extends TokenTestBase {
  public static function getInfo() {
    return array(
      'name' => 'Random token tests',
      'description' => 'Test the random tokens.',
      'group' => 'Token',
    );
  }

  function testRandomTokens() {
    $tokens = array(
      'number' => '[0-9]{1,}',
      'hash:md5' => '[0-9a-f]{32}',
      'hash:sha1' => '[0-9a-f]{40}',
      'hash:sha256' => '[0-9a-f]{64}',
      'hash:invalid-algo' => NULL,
    );

    $first_set = $this->assertTokens('random', array(), $tokens, array('regex' => TRUE));
    $second_set = $this->assertTokens('random', array(), $tokens, array('regex' => TRUE));
    foreach ($first_set as $token => $value) {
      $this->assertNotIdentical($first_set[$token], $second_set[$token]);
    }
  }
}
