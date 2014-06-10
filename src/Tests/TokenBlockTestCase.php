<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenBlockTestCase.
 */
namespace Drupal\token\Tests;
use Drupal\block_content\Entity\BlockContent;
use Drupal\block_content\Entity\BlockContentType;

/**
 * Tests block tokens.
 *
 * @group Token
 */
class TokenBlockTestCase extends TokenTestBase {
  protected static $modules = array('path', 'token', 'token_test', 'block', 'node', 'views', 'block_content');

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
    $bundle = BlockContentType::create(array(
      'id' => $label,
      'label' => $label,
      'revision' => FALSE
    ));
    $bundle->save();

    $block = BlockContent::create(array(
      'type' => $label,
      'label' => '[user:name]',
      'info' => 'Test token title block',
      'body[value]' => 'This is the test token title block.',
    ));
    $block->save();
    $this->drupalGet($block->getSystemPath());
    // Ensure token validation is working on the block.
    $this->assertText('The Block title is using the following invalid tokens: [user:name].');

    // Create the block for real now with a valid title.
    $block->label = '[current-page:title] block title';
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
