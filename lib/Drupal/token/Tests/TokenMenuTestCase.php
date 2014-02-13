<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenMenuTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Tests menu tokens.
 */
class TokenMenuTestCase extends TokenTestBase {
  public static function getInfo() {
    return array(
      'name' => 'Menu link and menu token tests',
      'description' => 'Test the menu tokens.',
      'group' => 'Token',
    );
  }

  public function setUp($modules = array()) {
    $modules[] = 'menu';
    parent::setUp($modules);
  }

  function testMenuTokens() {
    // Add a root link.
    $root_link = array(
      'link_path' => 'root',
      'link_title' => 'Root link',
      'menu_name' => 'main-menu',
    );
    menu_link_save($root_link);

    // Add another link with the root link as the parent
    $parent_link = array(
      'link_path' => 'root/parent',
      'link_title' => 'Parent link',
      'menu_name' => 'main-menu',
      'plid' => $root_link['mlid'],
    );
    menu_link_save($parent_link);

    // Test menu link tokens.
    $tokens = array(
      'mlid' => $parent_link['mlid'],
      'title' => 'Parent link',
      'menu' => 'Main menu',
      'menu:name' => 'Main menu',
      'menu:machine-name' => 'main-menu',
      'menu:description' => 'The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.',
      'menu:menu-link-count' => 2,
      'menu:edit-url' => url("admin/structure/menu/manage/main-menu", array('absolute' => TRUE)),
      'url' => url('root/parent', array('absolute' => TRUE)),
      'url:absolute' => url('root/parent', array('absolute' => TRUE)),
      'url:relative' => url('root/parent', array('absolute' => FALSE)),
      'url:path' => 'root/parent',
      'url:alias' => 'root/parent',
      'edit-url' => url("admin/structure/menu/item/{$parent_link['mlid']}/edit", array('absolute' => TRUE)),
      'parent' => 'Root link',
      'parent:mlid' => $root_link['mlid'],
      'parent:title' => 'Root link',
      'parent:menu' => 'Main menu',
      'parent:parent' => NULL,
      'parents' => 'Root link',
      'parents:count' => 1,
      'parents:keys' => $root_link['mlid'],
      'root' => 'Root link',
      'root:mlid' => $root_link['mlid'],
      'root:parent' => NULL,
      'root:root' => NULL,
    );
    $this->assertTokens('menu-link', array('menu-link' => $parent_link), $tokens);

    // Add a node menu link
    $node_link = array(
      'enabled' => TRUE,
      'link_title' => 'Node link',
      'plid' => $parent_link['mlid'],
      'customized' => 0,
      'description' => '',
    );
    $node = $this->drupalCreateNode(array('menu' => $node_link));

    // Test [node:menu] tokens.
    $tokens = array(
      'menu-link' => 'Node link',
      'menu-link:mlid' => $node->menu['mlid'],
      'menu-link:title' => 'Node link',
      'menu-link:menu' => 'Main menu',
      'menu-link:url' => url('node/' . $node->nid, array('absolute' => TRUE)),
      'menu-link:url:path' => 'node/' . $node->nid,
      'menu-link:edit-url' => url("admin/structure/menu/item/{$node->menu['mlid']}/edit", array('absolute' => TRUE)),
      'menu-link:parent' => 'Parent link',
      'menu-link:parent:mlid' => $node->menu['plid'],
      'menu-link:parent:mlid' => $parent_link['mlid'],
      'menu-link:parents' => 'Root link, Parent link',
      'menu-link:parents:count' => 2,
      'menu-link:parents:keys' => $root_link['mlid'] . ', ' . $parent_link['mlid'],
      'menu-link:root' => 'Root link',
      'menu-link:root:mlid' => $root_link['mlid'],
    );
    $this->assertTokens('node', array('node' => $node), $tokens);

    // Reload the node which will not have $node->menu defined and re-test.
    $loaded_node = node_load($node->nid);
    $this->assertTokens('node', array('node' => $loaded_node), $tokens);

    // Regression test for http://drupal.org/node/1317926 to ensure the
    // original node object is not changed when calling menu_node_prepare().
    $this->assertTrue(!isset($loaded_node->menu), t('The $node->menu property was not modified during token replacement.'), 'Regression');
  }
}
