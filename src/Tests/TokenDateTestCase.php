<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenDateTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Tests date tokens.
 *
 * @group token
 */
class TokenDateTestCase extends TokenTestBase {

  function testDateTokens() {
    $tokens = array(
      'token_test' => '1984',
      'invalid_format' => NULL,
    );

    $this->assertTokens('date', array('date' => 453859200), $tokens);
  }
}
