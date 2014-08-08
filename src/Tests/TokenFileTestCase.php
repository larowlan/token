<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenFileTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Tests file tokens.
 *
 * @group token
 */
class TokenFileTestCase extends TokenTestBase {
  protected static $modules = array('path', 'token', 'token_test', 'file');

  function testFileTokens() {
    // Create a test file object.
    $file = entity_create('file', array(
      'fid' => 1,
      'filename' => 'test.png',
      'filesize' => 100,
      'uri' => 'public://images/test.png',
      'filemime' => 'image/png',
    ));

    $tokens = array(
      'basename' => 'test.png',
      'extension' => 'png',
      'size-raw' => 100,
    );
    $this->assertTokens('file', array('file' => $file), $tokens);

    // Test a file with no extension and a fake name.
    $file->filename = 'Test PNG image';
    $file->uri = 'public://images/test';

    $tokens = array(
      'basename' => 'test',
      'extension' => '',
      'size-raw' => 100,
    );
    $this->assertTokens('file', array('file' => $file), $tokens);
  }
}
