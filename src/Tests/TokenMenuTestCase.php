<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenMenuTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Tests menu tokens.
 *
 * @group token
 */
class TokenMenuTestCase extends TokenTestBase {
  protected static $modules = array('path', 'token', 'token_test', 'menu_ui', 'node');

  function testMenuTokens() {
    // Add a menu.
    $menu = entity_create('menu', array(
      'id' => 'main-menu',
      'label' => 'Main menu',
      'description' => 'The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.',
    ));
    $menu->save();
    // Add a root link.
    $root_link = entity_create('menu_link_content', array(
      'url' => 'admin',
      'title' => 'Administration',
      'menu_name' => 'main-menu',
      'bundle' => 'menu_link_content',
    ));
    $root_link->save();

    // Add another link with the root link as the parent
    $parent_link = entity_create('menu_link_content', array(
      'route_name' => 'system.admin_config',
      'title' => 'Configuration',
      'menu_name' => 'main-menu',
      'parent' => 'menu_link_content:' . $root_link->uuid(),
      'bundle' => 'menu_link_content',
    ));
    $parent_link->save();

    // Test menu link tokens.
    $tokens = array(
      'mlid' => $parent_link->id(),
      'title' => 'Configuration',
      'menu' => 'Main menu',
      'menu:name' => 'Main menu',
      'menu:machine-name' => 'main-menu',
      'menu:description' => 'The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.',
      'menu:menu-link-count' => 2,
      'menu:edit-url' => \Drupal::url('entity.menu.edit_form', ['menu' => 'main-menu'], array('absolute' => TRUE)),
      'url' => \Drupal::url('system.admin_config', [], array('absolute' => TRUE)),
      'url:absolute' => \Drupal::url('system.admin_config', [], array('absolute' => TRUE)),
      'url:relative' => \Drupal::url('system.admin_config', [], array('absolute' => FALSE)),
      'url:path' => 'admin/config',
      'url:alias' => 'admin/config',
      'edit-url' => \Drupal::url('entity.menu_link_content.canonical', ['menu_link_content' => $parent_link->id()], array('absolute' => TRUE)),
      'parent' => 'Administration',
      'parent:mlid' => $root_link->id(),
      'parent:title' => 'Administration',
      'parent:menu' => 'Main menu',
      'parent:parent' => NULL,
      'parents' => 'Administration',
      'parents:count' => 1,
      'parents:keys' => $root_link->id(),
      'root' => 'Administration',
      'root:mlid' => $root_link->id(),
      'root:parent' => NULL,
      'root:root' => NULL,
    );
    $this->assertTokens('menu-link', array('menu-link' => $parent_link), $tokens);

    // Add a node.
    $node = $this->drupalCreateNode();

    // Allow main menu for this node type.
    \Drupal::config('menu.entity.node.' . $node->getType())->set('available_menus', array('main-menu'))->save();

    // Add a node menu link
    $node_link = entity_create('menu_link_content', array(
      'link_path' => 'node/' . $node->id(),
      'title' => 'Node link',
      'parent' => 'menu_link_content:' . $parent_link->uuid(),
      'menu_name' => 'main-menu',
    ));
    $node_link->save();

    // Test [node:menu] tokens.
    $tokens = array(
      'menu-link' => 'Node link',
      'menu-link:mlid' => $node_link->id(),
      'menu-link:title' => 'Node link',
      'menu-link:menu' => 'Main menu',
      'menu-link:url' => $node->url('canonical', ['absolute' => TRUE]),
      'menu-link:url:path' => 'node/' . $node->id(),
      'menu-link:edit-url' => $node_link->url('canonical', ['absolute' => TRUE]),
      'menu-link:parent' => 'Configuration',
      'menu-link:parent:mlid' => $parent_link->id(),
      'menu-link:parents' => 'Administration, Configuration',
      'menu-link:parents:count' => 2,
      'menu-link:parents:keys' => $root_link->id() . ', ' . $parent_link->id(),
      'menu-link:root' => 'Administration',
      'menu-link:root:mlid' => $root_link->id(),
    );
    $this->assertTokens('node', array('node' => $node), $tokens);

    // Reload the node which will not have $node->menu defined and re-test.
    $loaded_node = node_load($node->id());
    $this->assertTokens('node', array('node' => $loaded_node), $tokens);

    // Regression test for http://drupal.org/node/1317926 to ensure the
    // original node object is not changed when calling menu_node_prepare().
    $this->assertTrue(!isset($loaded_node->menu), t('The $node->menu property was not modified during token replacement.'), 'Regression');
  }
}
