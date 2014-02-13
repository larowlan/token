<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenNodeTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Tests node tokens.
 */
class TokenNodeTestCase extends TokenTestBase {
  protected $profile = 'standard';

  public static function getInfo() {
    return array(
      'name' => 'Node and content type token tests',
      'description' => 'Test the node and content type tokens.',
      'group' => 'Token',
    );
  }

  function testNodeTokens() {
    $source_node = $this->drupalCreateNode(array('log' => $this->randomName(), 'path' => array('alias' => 'content/source-node')));
    $tokens = array(
      'source' => NULL,
      'source:nid' => NULL,
      'log' => $source_node->log,
      'url:path' => 'content/source-node',
      'url:absolute' => url("node/{$source_node->nid}", array('absolute' => TRUE)),
      'url:relative' => url("node/{$source_node->nid}", array('absolute' => FALSE)),
      'url:unaliased:path' => "node/{$source_node->nid}",
      'content-type' => 'Basic page',
      'content-type:name' => 'Basic page',
      'content-type:machine-name' => 'page',
      'content-type:description' => "Use <em>basic pages</em> for your static content, such as an 'About us' page.",
      'content-type:node-count' => 1,
      'content-type:edit-url' => url('admin/structure/types/manage/page', array('absolute' => TRUE)),
      // Deprecated tokens.
      'tnid' => 0,
      'type' => 'page',
      'type-name' => 'Basic page',
      'url:alias' => 'content/source-node',
    );
    $this->assertTokens('node', array('node' => $source_node), $tokens);

    $translated_node = $this->drupalCreateNode(array('tnid' => $source_node->nid, 'type' => 'article'));
    $tokens = array(
      'source' => $source_node->title,
      'source:nid' => $source_node->nid,
      'log' => '',
      'url:path' => "node/{$translated_node->nid}",
      'url:absolute' => url("node/{$translated_node->nid}", array('absolute' => TRUE)),
      'url:relative' => url("node/{$translated_node->nid}", array('absolute' => FALSE)),
      'url:unaliased:path' => "node/{$translated_node->nid}",
      'content-type' => 'Article',
      'content-type:name' => 'Article',
      'content-type:machine-name' => 'article',
      'content-type:description' => "Use <em>articles</em> for time-sensitive content like news, press releases or blog posts.",
      'content-type:node-count' => 1,
      'content-type:edit-url' => url('admin/structure/types/manage/article', array('absolute' => TRUE)),
      // Deprecated tokens.
      'type' => 'article',
      'type-name' => 'Article',
      'tnid' => $source_node->nid,
      'url:alias' => "node/{$translated_node->nid}",
    );
    $this->assertTokens('node', array('node' => $translated_node), $tokens);
  }
}
