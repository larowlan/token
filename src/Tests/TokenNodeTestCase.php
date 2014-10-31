<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenNodeTestCase.
 */

namespace Drupal\token\Tests;

/**
 * Test the node and content type tokens.
 *
 * @group token
 */
class TokenNodeTestCase extends TokenTestBase {
  protected $profile = 'standard';

  function testNodeTokens() {
    $source_node = $this->drupalCreateNode(array('revision_log' => $this->randomMachineName(), 'path' => array('alias' => 'content/source-node')));
    $tokens = array(
      'source' => NULL,
      'source:nid' => NULL,
      'log' => $source_node->revision_log->value,
      'url:path' => 'content/source-node',
      'url:absolute' => \Drupal::url('entity.node.canonical', ['node' => $source_node->id()], array('absolute' => TRUE)),
      'url:relative' => \Drupal::url('entity.node.canonical', ['node' => $source_node->id()], array('absolute' => FALSE)),
      'url:unaliased:path' => "node/{$source_node->id()}",
      'content-type' => 'Basic page',
      'content-type:name' => 'Basic page',
      'content-type:machine-name' => 'page',
      'content-type:description' => "Use <em>basic pages</em> for your static content, such as an 'About us' page.",
      'content-type:node-count' => 1,
      'content-type:edit-url' => \Drupal::url('entity.node_type.edit_form', ['node_type' => 'page'], array('absolute' => TRUE)),
      // Deprecated tokens.
      'type' => 'page',
      'type-name' => 'Basic page',
      'url:alias' => 'content/source-node',
    );
    $this->assertTokens('node', array('node' => $source_node), $tokens);

    $translated_node = $this->drupalCreateNode(array('tnid' => $source_node->id(), 'type' => 'article'));
    $tokens = array(
      'source' => $source_node->label(),
      'source:nid' => $source_node->id(),
      'log' => '',
      'url:path' => "node/{$translated_node->id()}",
      'url:absolute' => \Drupal::url('entity.node.canonical', ['node' => $translated_node->id()], array('absolute' => TRUE)),
      'url:relative' => \Drupal::url('entity.node.canonical', ['node' => $translated_node->id()], array('absolute' => FALSE)),
      'url:unaliased:path' => "node/{$translated_node->id()}",
      'content-type' => 'Article',
      'content-type:name' => 'Article',
      'content-type:machine-name' => 'article',
      'content-type:description' => "Use <em>articles</em> for time-sensitive content like news, press releases or blog posts.",
      'content-type:node-count' => 1,
      'content-type:edit-url' => \Drupal::url('entity.node_type.edit_form', ['node_type' => 'article'], array('absolute' => TRUE)),
      // Deprecated tokens.
      'type' => 'article',
      'type-name' => 'Article',
      'url:alias' => "node/{$translated_node->id()}",
    );
    $this->assertTokens('node', array('node' => $translated_node), $tokens);
  }
}
