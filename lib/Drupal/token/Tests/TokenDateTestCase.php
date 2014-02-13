<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenDateTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Tests date tokens.
 */
class TokenDateTestCase extends TokenTestBase {
  public static function getInfo() {
    return array(
      'name' => 'Date token tests',
      'description' => 'Test the date tokens.',
      'group' => 'Token',
    );
  }

  function testDateTokens() {
    $tokens = array(
      'token_test' => '1984',
      'invalid_format' => NULL,
    );

    $this->assertTokens('date', array('date' => 453859200), $tokens);
  }
}
