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
    $root_link = entity_create('menu_link', array(
      'link_path' => 'admin',
      'link_title' => 'Administration',
      'menu_name' => 'main-menu',
    ));
    $root_link->save();

    // Add another link with the root link as the parent
    $parent_link = entity_create('menu_link', array(
      'link_path' => 'admin/config',
      'link_title' => 'Configuration',
      'menu_name' => 'main-menu',
      'plid' => $root_link['mlid'],
    ));
    $parent_link->save();

    // Test menu link tokens.
    $tokens = array(
      'mlid' => $parent_link['mlid'],
      'title' => 'Configuration',
      'menu' => 'Main menu',
      'menu:name' => 'Main menu',
      'menu:machine-name' => 'main-menu',
      'menu:description' => 'The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.',
      'menu:menu-link-count' => 2,
      'menu:edit-url' => url("admin/structure/menu/manage/main-menu", array('absolute' => TRUE)),
      'url' => url('admin/config', array('absolute' => TRUE)),
      'url:absolute' => url('admin/config', array('absolute' => TRUE)),
      'url:relative' => url('admin/config', array('absolute' => FALSE)),
      'url:path' => 'admin/config',
      'url:alias' => 'admin/config',
      'edit-url' => url("admin/structure/menu/item/{$parent_link['mlid']}/edit", array('absolute' => TRUE)),
      'parent' => 'Administration',
      'parent:mlid' => $root_link['mlid'],
      'parent:title' => 'Administration',
      'parent:menu' => 'Main menu',
      'parent:parent' => NULL,
      'parents' => 'Administration',
      'parents:count' => 1,
      'parents:keys' => $root_link['mlid'],
      'root' => 'Administration',
      'root:mlid' => $root_link['mlid'],
      'root:parent' => NULL,
      'root:root' => NULL,
    );
    $this->assertTokens('menu-link', array('menu-link' => $parent_link), $tokens);

    // Add a node.
    $node = $this->drupalCreateNode();

    // Allow main menu for this node type.
    \Drupal::config('menu.entity.node.' . $node->getType())->set('available_menus', array('main-menu'))->save();

    // Add a node menu link
    $node_link = entity_create('menu_link', array(
      'enabled' => TRUE,
      'link_path' => 'node/' . $node->id(),
      'link_title' => 'Node link',
      'plid' => $parent_link['mlid'],
      'customized' => 0,
      'menu_name' => 'main-menu',
      'description' => '',
    ));
    $node_link->save();

    // Test [node:menu] tokens.
    $tokens = array(
      'menu-link' => 'Node link',
      'menu-link:mlid' => $node->menu['mlid'],
      'menu-link:title' => 'Node link',
      'menu-link:menu' => 'Main menu',
      'menu-link:url' => url('node/' . $node->id(), array('absolute' => TRUE)),
      'menu-link:url:path' => 'node/' . $node->id(),
      'menu-link:edit-url' => url("admin/structure/menu/item/{$node_link->id()}/edit", array('absolute' => TRUE)),
      'menu-link:parent' => 'Configuration',
      'menu-link:parent:mlid' => $node->menu['plid'],
      'menu-link:parent:mlid' => $parent_link['mlid'],
      'menu-link:parents' => 'Administration, Configuration',
      'menu-link:parents:count' => 2,
      'menu-link:parents:keys' => $root_link['mlid'] . ', ' . $parent_link['mlid'],
      'menu-link:root' => 'Administration',
      'menu-link:root:mlid' => $root_link['mlid'],
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
