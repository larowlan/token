<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenBlockTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Tests block tokens.
 */
class TokenBlockTestCase extends TokenTestBase {
  protected static $modules = array('path', 'token', 'token_test', 'block');

  public static function getInfo() {
    return array(
      'name' => 'Block token tests',
      'description' => 'Test the block title token replacement.',
      'group' => 'Token',
    );
  }

  public function setUp($modules = array()) {
    $this->admin_user = $this->drupalCreateUser(array('access content', 'administer blocks'));
    $this->drupalLogin($this->admin_user);
  }

  public function testBlockTitleTokens() {
    $edit['title'] = '[current-page:title] block title';
    $edit['info'] = 'Test token title block';
    $edit['body[value]'] = 'This is the test token title block.';
    $edit['regions[bartik]'] = 'sidebar_first';
    $this->drupalPost('admin/structure/block/add', $edit, 'Save block');

    $this->drupalGet('node');
    $this->assertText('Welcome to ' . variable_get('site_name', 'Drupal') . ' block title');

    // Ensure that tokens are not double-escaped when output as a block title.
    $node = $this->drupalCreateNode(array('title' => "Site's first node"));
    $this->drupalGet('node/' . $node->nid);
    // The apostraphe should only be escaped once via check_plain().
    $this->assertRaw("Site&#039;s first node block title");
  }
}
