<?php
// $Id$

/**
 * @file
 * Token callbacks for the token module.
 */

/**
 * Implements hook_token_info_alter().
 */
function token_token_info_alter(&$data) {
  // Add a 'dynamic' key to any tokens that have chained but dynamic tokens.
  $data['tokens']['date']['custom']['dynamic'] = TRUE;
}

/**
 * Implements hook_token_info().
 */
function token_token_info() {
  $info['tokens']['node']['source'] = array(
    'name' => t('Translation source node'),
    'description' => t("The source node for this current node's translation set."),
    'type' => 'node',
  );
  $info['tokens']['node']['log'] = array(
    'name' => t('Revision log message'),
    'description' => t('The explanation of the most recent changes made to the node.'),
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function token_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);

  // Node tokens.
  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'source':
          if (!empty($node->tnid) && $source_node = node_load($node->tnid)) {
            $title = $source_node->title;
            $replacements[$original] = $sanitize ? filter_xss($title) : $title;
          }
          break;
        case 'log':
          $replacements[$original] = $sanitize ? filter_xss($node->log) : $node->log;
          break;
      }
    }

    // Chained token relationships.
    if (!empty($node->tnid) && ($source_node = node_load($node->tnid)) && $source_tokens = token_find_with_prefix($tokens, 'source')) {
      $replacements += token_generate('node', $source_tokens, array('node' => $source_node), $options);
    }
  }

  return $replacements;
}

/**
 * Implements hook_token_info() on behalf of menu.module.
 */
function menu_token_info() {
  $info['types']['menu-link'] = array(
    'name' => t('Menu links'),
    'description' => t('Tokens related to menu links.'),
    'needs-data' => 'menu-link',
  );
  $info['tokens']['menu-link']['mlid'] = array(
    'name' => t('Link ID'),
    'description' => t('The unique ID of the menu link.'),
  );
  $info['tokens']['menu-link']['title'] = array(
    'name' => t('Title'),
    'description' => t('The title of the menu link.'),
  );
  $info['tokens']['menu-link']['menu'] = array(
    'name' => t('Menu'),
    'description' => t("The machine name of the link's menu."),
  );
  $info['tokens']['menu-link']['menu-name'] = array(
    'name' => t('Menu name'),
    'description' => t("The human-readable name of the link's menu."),
  );
  $info['tokens']['menu-link']['path'] = array(
    'name' => t('Path'),
    'description' => t('The path for the menu link.'),
  );
  $info['tokens']['menu-link']['url'] = array(
    'name' => t('URL'),
    'description' => t('The URL of the menu link.'),
  );
  $info['tokens']['menu-link']['parent'] = array(
    'name' => t('Parent'),
    'description' => t("The menu link's parent."),
    'type' => 'menu-link',
  );

  $info['tokens']['node']['menu-link'] = array(
    'name' => t('Menu link'),
    'description' => t("The menu link for this node."),
    'type' => 'menu-link',
  );
  //$info['tokens']['menu-link']['node'] = array(
  //  'name' => t('Menu node'),
  //  'description' => t('The node associated with the menu link item.'),
  //  'type' => 'node',
  //);
  return $info;
}

/**
 * Implements hook_tokens() on behalf of menu.module.
 */
function menu_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);

  // Node tokens.
  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];

    if (!isset($node->menu)) {
      // Nodes do not have their menu links loaded via menu_node_load().
      menu_node_prepare($node);
    }

    if (!empty($node->menu['mlid'])) {
      $link = $node->menu;
      if (!isset($link['title'])) {
        // Links may be impartially loaded, so ensure we have a full link.
        $link = menu_link_load($node->menu['mlid']);
      }

      foreach ($tokens as $name => $original) {
        switch ($name) {
          case 'menu-link':
            $replacements[$original] = $sanitize ? check_plain($link['title']) : $link['title'];
            break;
        }
      }

      // Chained token relationships.
      if ($menu_tokens = token_find_with_prefix($tokens, 'menu-link')) {
        $replacements += token_generate('menu-link', $menu_tokens, array('menu-link' => $link), $options);
      }
    }
  }

  // Menu link tokens.
  if ($type == 'menu-link' && !empty($data['menu-link'])) {
    $link = (array) $data['menu-link'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'mlid':
          $replacements[$original] = $link['mlid'];
          break;
        case 'title':
          $replacements[$original] = $sanitize ? check_plain($link['title']) : $link['title'];
          break;
        case 'menu':
          $replacements[$original] = $link['menu_name'];
          break;
        case 'menu-name':
          $menus = menu_get_menus();
          if (isset($menus[$link['menu_name']])) {
            $replacements[$original] = $menus[$link['menu_name']];
          }
          break;
        case 'path':
          $replacements[$original] = $link['href'];
          break;
        case 'url':
          $replacements[$original] = url($link['href']);
          break;
        case 'parent':
          if (!empty($link['plid']) && $parent = menu_link_load($link['plid'])) {
            $replacements[$original] = $sanitize ? check_plain($parent['title']) : $parent['title'];
          }
          break;
        //case 'node':
        //  if (preg_match('|node/(\d+)|', $link['link_path'], $matches) && ($node = node_load($matches[1]))) {
        //    $replacements[$original] = $sanitize ? check_plain($node->title) : $node->title;
        //  }
        //  break;
      }
    }

    // Chained token relationships.
    if (!empty($link['plid']) && ($parent = menu_link_load($link['plid'])) && $source_tokens = token_find_with_prefix($tokens, 'parent')) {
      $replacements += token_generate('menu-link', $source_tokens, array('menu-link' => $parent), $options);
    }
    //if (preg_match('|node/(\d+)|', $link['link_path'], $matches) && ($node = node_load($matches[1])) && $node_tokens = token_find_with_prefix($tokens, 'node')) {
    //  $replacements += token_generate('node', $node_tokens, array('node' => $node), $options);
    //}
  }

  return $replacements;
}
