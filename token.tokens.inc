<?php
// $Id$

/**
 * @file
 * Token callbacks for the token module.
 */

/**
 * Implements hook_token_info_alter().
 */
function token_token_info_alter(&$info) {
  // Force 'date' type tokens to require input and add a 'current-date' type.
  // @todo Remove when http://drupal.org/node/943028 is fixed.
  $info['types']['date']['needs-data'] = 'date';
  $info['types']['current-date'] = array(
    'name' => t('Current date'),
    'description' => t('Tokens related to the current date and time.'),
    'type' => 'date',
  );

  // Add a 'dynamic' key to any tokens that have chained but dynamic tokens.
  $info['tokens']['date']['custom']['dynamic'] = TRUE;

  // @todo Remove [node:tnid] now that we have [node:source:nid]?
  //unset($info['tokens']['node']['tnid']);

  // @todo Change [node:type-name] to [node:machine-name]?

  // Add [token:path] tokens for any URI-able entities.
  $entities = entity_get_info();
  foreach ($entities as $entity => $entity_info) {
    if (!isset($entity_info['token type'])) {
      continue;
    }
    $token_type = $entity_info['token type'];

    // Add [entity:path] tokens if they do not already exist.
    if (!isset($info['tokens'][$token_type]['path']) && (!empty($entity_info['uri callback']) || !empty($entity_info['bundles'][$entity]['uri callback']))) {
      $info['tokens'][$token_type]['path'] = array(
        'name' => t('Path'),
        'description' => t('The URL alias of the @entity.', array('@entity' => drupal_strtolower($entity_info['label']))),
      );
    }
  }
}

/**
 * Implements hook_token_info().
 */
function token_token_info() {
  // Node tokens.
  $info['tokens']['node']['source'] = array(
    'name' => t('Translation source node'),
    'description' => t("The source node for this current node's translation set."),
    'type' => 'node',
  );
  $info['tokens']['node']['log'] = array(
    'name' => t('Revision log message'),
    'description' => t('The explanation of the most recent changes made to the node.'),
  );

  // Taxonomy term and vocabulary tokens.
  if (module_exists('taxonomy')) {
    $info['tokens']['term']['edit-url'] = array(
      'name' => t('Edit URL'),
      'description' => t("The URL of the taxonomy term's edit page."),
    );

    $info['tokens']['vocabulary']['machine-name'] = array(
      'name' => t('Machine-readable name'),
      'description' => t('The unique machine-readable name of the vocabulary.'),
    );
    $info['tokens']['vocabulary']['edit-url'] = array(
      'name' => t('Edit URL'),
      'description' => t("The URL of the vocabulary's edit page."),
    );
  }

  // File tokens.
  $info['tokens']['file']['extension'] = array(
    'name' => t('Extension'),
    'description' => t('The extenion of the file.'),
  );

  // Add information on the restricted user tokens.
  $info['tokens']['user']['cancel-url'] = array(
    'name' => t('Account cancellation URL'),
    'description' => t('The URL of the confirm delete page for the user account.'),
    'restricted' => TRUE,
  );
  $info['tokens']['user']['one-time-login-url'] = array(
    'name' => t('One-time login URL'),
    'description' => t('The URL of the one-time login page for the user account.'),
    'restricted' => TRUE,
  );

  // Menu link tokens (work regardless if menu module is enabled or not).
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
    'description' => t('The URL alias of the menu link.'),
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
  $info['tokens']['menu-link']['root'] = array(
    'name' => t('Root'),
    'description' => t("The menu link's root."),
    'type' => 'menu-link',
  );

  // Current page tokens.
  $info['types']['current-page'] = array(
    'name' => t('Current page'),
    'description' => t('Tokens related to the current page request.'),
  );
  $info['tokens']['current-page']['title'] = array(
    'name' => t('Title'),
    'description' => t('The title of the current page.'),
  );
  $info['tokens']['current-page']['path'] = array(
    'name' => t('Path'),
    'description' => t('The URL alias of the current page.'),
  );
  $info['tokens']['current-page']['url'] = array(
    'name' => t('URL'),
    'description' => t('The URL of the current page.'),
  );
  $info['tokens']['current-page']['page-number'] = array(
    'name' => t('Page number'),
    'description' => t('The page number of the current page when viewing paged lists.'),
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function token_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  $url_options = array('absolute' => TRUE);
  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }

  $sanitize = !empty($options['sanitize']);

  // Current date tokens.
  // @todo Remove when http://drupal.org/node/943028 is fixed.
  if ($type == 'current-date') {
    $replacements += token_generate('date', $tokens, array('date' => REQUEST_TIME), $options);
  }

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
    if (!empty($node->tnid) && ($source_tokens = token_find_with_prefix($tokens, 'source')) && $source_node = node_load($node->tnid)) {
      $replacements += token_generate('node', $source_tokens, array('node' => $source_node), $options);
    }
  }

  // Taxonomy term tokens.
  if ($type == 'term' && !empty($data['term'])) {
    $term = $data['term'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'edit-url':
          $replacements[$original] = url("taxonomy/term/{$term->tid}/edit", $url_options);
          break;
      }
    }
  }

  // Vocabulary tokens.
  if ($type == 'vocabulary' && !empty($data['vocabulary'])) {
    $vocabulary = $data['vocabulary'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'machine-name':
          // Vocabulary machine names cannot have XSS-capable characters so
          // there is no need to escape it.
          $replacements[$original] = $vocabulary->machine_name;
          break;
        case 'edit-url':
          $replacements[$original] = url("admin/structure/taxonomy/{$vocabulary->machine_name}/edit", $url_options);
          break;
      }
    }
  }

  if ($type == 'file' && !empty($data['file'])) {
    $file = $data['file'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'extension':
          $extension = pathinfo($file->filename, PATHINFO_EXTENSION);
          $replacements[$original] = $sanitize ? check_plain($extension) : $extension;
          break;
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
          // This is a machine name so does not ever need to be sanitized.
          $replacements[$original] = $link['menu_name'];
          break;
        case 'menu-name':
          $menus = menu_get_menus();
          if (isset($menus[$link['menu_name']])) {
            $replacements[$original] = $sanitize ? check_plain($menus[$link['menu_name']]) : $menus[$link['menu_name']];
          }
          break;
        case 'path':
          $alias = drupal_get_path_alias($link['href'], $language_code);
          $replacements[$original] = $sanitize ? check_plain($alias) : $alias;
          break;
        case 'url':
          $replacements[$original] = url($link['href'], $url_options);
          break;
        case 'parent':
          if (!empty($link['plid']) && $parent = menu_link_load($link['plid'])) {
            $replacements[$original] = $sanitize ? check_plain($parent['title']) : $parent['title'];
          }
          break;
        case 'root';
          if (!empty($link['p1']) && $root = menu_link_load($link['p1'])) {
            $replacements[$original] = $sanitize ? check_plain($root['title']) : $root['title'];
          }
      }
    }

    // Chained token relationships.
    if (!empty($link['plid']) && ($source_tokens = token_find_with_prefix($tokens, 'parent')) && $parent = menu_link_load($link['plid'])) {
      $replacements += token_generate('menu-link', $source_tokens, array('menu-link' => $parent), $options);
    }
    if (!empty($link['p1']) && ($root_tokens = token_find_with_prefix($tokens, 'root')) && $root = menu_link_load($link['p1'])) {
      $replacements += token_generate('menu-link', $source_tokens, array('menu-link' => $root), $options);
    }
  }

  // Current page tokens.
  if ($type == 'current-page') {
    $current_path = current_path();

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'title':
          $replacements[$original] = drupal_get_title();
          break;
        case 'path':
          $alias = drupal_get_path_alias($current_path, $language_code);
          $replacements[$original] = $sanitize ? check_plain($alias) : $alias;
          break;
        case 'url':
          $replacements[$original] = url($current_path, $url_options);
          break;
        case 'page-number':
          if ($page = filter_input(INPUT_GET, 'page')) {
            // @see PagerDefault::execute()
            $pager_page_array = explode(',', $page);
            $page = $pager_page_array[0];
          }
          $replacements[$original] = (int) $page + 1;
          break;
      }
    }
  }

  // Entity tokens.
  if (!empty($data[$type]) && $entity_type = token_get_entity_mapping('token', $type)) {
    $entity = $data[$type];

    // Sometimes taxonomy terms are not properly loaded.
    // @see http://drupal.org/node/870528
    if ($entity_type == 'taxonomy_term' && !isset($entity->vocabulary_machine_name)) {
      $entity->vocabulary_machine_name = db_query("SELECT machine_name FROM {taxonomy_vocabulary} WHERE vid = :vid", array(':vid' => $entity->vid))->fetchField();
    }

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // @todo Support entity:label
        // @todo Support entity:url
        case 'path':
          if ($uri = entity_uri($entity_type, $entity) && $entity_type != 'file') {
            $alias = drupal_get_path_alias($uri['path'], $language_code);
            $replacements[$original] = $sanitize ? check_plain($alias) : $alias;
          }
          break;
      }
    }
  }

  // If $type is a token type, $data[$type] is empty but $data[$entity_type] is not, re-run token replacements.
  if (empty($data[$type]) && ($entity_type = token_get_entity_mapping('token', $type)) && $entity_type != $type && !empty($data[$entity_type])) {
    $data[$type] = $data[$entity_type];
    $replacements += module_invoke_all('tokens', $type, $tokens, $data, $options);
  }

  return $replacements;
}

/**
 * Implements hook_token_info() on behalf of book.module.
 */
function book_token_info() {
  $info['tokens']['node']['book'] = array(
    'name' => t('Book'),
    'description' => t('The book page associated with the node.'),
    'type' => 'menu-link',
  );
  return $info;
}

/**
 * Implements hook_tokens() on behalf of book.module.
 */
function book_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);

  // Node tokens.
  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];

    if (!empty($node->book['mlid'])) {
      $link = $node->book;
      if (!isset($link['title'])) {
        // Links may be impartially loaded, so ensure we have a full link.
        $link = book_link_load($link['mlid']);
      }

      foreach ($tokens as $name => $original) {
        switch ($name) {
          case 'book':
            $replacements[$original] = $sanitize ? check_plain($link['title']) : $link['title'];
            break;
        }
      }

      // Chained token relationships.
      if ($book_tokens = token_find_with_prefix($tokens, 'book')) {
        $replacements += token_generate('menu-link', $book_tokens, array('menu-link' => $link), $options);
      }
    }
  }

  return $replacements;
}

/**
 * Implements hook_token_info() on behalf of menu.module.
 */
function menu_token_info() {
  $info['tokens']['node']['menu-link'] = array(
    'name' => t('Menu link'),
    'description' => t("The menu link for this node."),
    'type' => 'menu-link',
  );
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

  return $replacements;
}
