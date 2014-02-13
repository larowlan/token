<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenFileTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Tests file tokens.
 */
class TokenFileTestCase extends TokenTestBase {
  public static function getInfo() {
    return array(
      'name' => 'File token tests',
      'description' => 'Test the file tokens.',
      'group' => 'Token',
    );
  }

  function testFileTokens() {
    // Create a test file object.
    $file = new stdClass();
    $file->fid = 1;
    $file->filename = 'test.png';
    $file->filesize = 100;
    $file->uri = 'public://images/test.png';
    $file->filemime = 'image/png';

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
