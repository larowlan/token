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

    $block_content = BlockContent::create(array(
      'type' => $label,
      'label' => '[current-page:title] block title',
      'info' => 'Test token title block',
      'body[value]' => 'This is the test token title block.',
    ));
    $block_content->save();

    $block = $this->drupalPlaceBlock('block_content:' . $block_content->uuid(), array(
      'label' => '[user:name]',
    ));
    $this->drupalGet($block->getSystemPath());
    // Ensure token validation is working on the block.
    $this->assertText('The Block title is using the following invalid tokens: [user:name].');

    // Create the block for real now with a valid title.
    $settings = $block->get('settings');
    $settings['label'] = '[current-page:title] block title';
    $block->set('settings', $settings);
    $block->save();

    // Ensure that tokens are not double-escaped when output as a block title.
    $node = $this->drupalCreateNode(array('title' => "Site's first node"));
    $this->drupalGet('node/' . $node->id());
    // The apostraphe should only be escaped once via check_plain().
    $this->assertRaw("Site&#039;s first node block title");
  }
}
