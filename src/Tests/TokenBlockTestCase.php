<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenBlockTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Tests block tokens.
 *
 * @group Token
 */
class TokenBlockTestCase extends TokenTestBase {
  protected static $modules = array('path', 'token', 'token_test', 'block', 'node', 'views', 'custom_block');

  public static function getInfo() {
    return array(
      'name' => 'Block token tests',
      'description' => 'Test the block title token replacement.',
      'group' => 'Token',
    );
  }

  public function setUp($modules = array()) {
    parent::setUp();
    $this->admin_user = $this->drupalCreateUser(array('access content', 'administer blocks'));
    $this->drupalLogin($this->admin_user);
  }

  public function testBlockTitleTokens() {
    $label = 'tokenblock';
    $bundle = entity_create('custom_block_type', array(
      'id' => $label,
      'label' => $label,
      'revision' => FALSE
    ));
    $bundle->save();

    /* @var \Drupal\custom_block\CustomBlockInterface $block */
    $block = entity_create('custom_block', array(
      'type' => $label,
      'label' => '[current-page:title] block title',
      'info' => 'Test token title block',
      'body[value]' => 'This is the test token title block.',
    ));
    $block->save();
    $this->drupalPlaceBlock('custom_block:' . $block->uuid(), array(
      'label' => '[current-page:title] block title',
    ));

    // Ensure that tokens are not double-escaped when output as a block title.
    $node = $this->drupalCreateNode(array('title' => "Site's first node"));
    $this->drupalGet('node/' . $node->id());
    // The apostraphe should only be escaped once via check_plain().
    $this->assertRaw("Site&#039;s first node block title");
  }
}
