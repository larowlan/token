<?php
// $Id$

/**
 * Implementation of hook_token_values()
 */
function node_token_values($type, $object = NULL) {
  switch ($type) {
    case 'node':
      $node = $object;

      $values['[nid]']            = $node->nid;
      $values['[type]']           = $node->type;
      $values['[type-name]']      = node_get_types('name', $node->type);
      $values['[title]']          = check_plain($node->title);

      $values['[author-uid]']     = $node->uid;
      $values['[author-name]']    = $node->name;

      $values['[yyyy]']           = date('Y', $node->created);
      $values['[yy]']             = date('y', $node->created);
      $values['[month]']          = date('F', $node->created);
      $values['[mon]']            = date('M', $node->created);
      $values['[mm]']             = date('m', $node->created);
      $values['[m]']              = date('n', $node->created);
      $values['[ww]']             = date('W', $node->created);
      $values['[date]']           = date('N', $node->created);
      $values['[day]']            = date('l', $node->created);
      $values['[ddd]']            = date('D', $node->created);
      $values['[dd]']             = date('d', $node->created);
      $values['[d]']              = date('j', $node->created);

      $values['[mod-yyyy]']       = date('Y', $node->changed);
      $values['[mod-yy]']         = date('y', $node->changed);
      $values['[mod-month]']      = date('F', $node->changed);
      $values['[mod-mon]']        = date('M', $node->changed);
      $values['[mod-mm]']         = date('m', $node->changed);
      $values['[mod-m]']          = date('n', $node->changed);
      $values['[mod-ww]']         = date('W', $node->changed);
      $values['[mod-date]']       = date('N', $node->changed);
      $values['[mod-day]']        = date('l', $node->changed);
      $values['[mod-ddd]']        = date('D', $node->changed);
      $values['[mod-dd]']         = date('d', $node->changed);
      $values['[mod-d]']          = date('j', $node->changed);

      // And now taxonomy, which is a bit more work. This code is adapted from
      // pathauto's handling code; it's intended for compatability with it.
      if (module_exists('taxonomy') && is_array($node->taxonomy) && count($node->taxonomy) > 0) {
        foreach ($node->taxonomy as $firsttermid) {
          if ($firsttermid) {
            break;
          }
        }
        $term = taxonomy_get_term($firsttermid);
        $values['[term]'] = $term->name;
        $values['[term-id]'] = $term->tid;

        $vid = $term->vid;
        $vocabulary = taxonomy_get_vocabulary($vid);
        $values['[vocab]'] = $vocabulary->name;
        $values['[vocab-id]'] = $vocabulary->name;

        $catpath = '';
        $parents = taxonomy_get_parents_all($firsttermid);
        foreach ($parents as $parent) {
          $catpath = $parent->name.'/'.$catpath;
        }
        $values['[catpath]'] = $catpath;
      } 
      else {
        $values['[term]'] = '';
        $values['[term-id]'] = '';
        $values['[vocab]'] = '';
        $values['[vocab-id]'] = '';
        $values['[catpath]'] = '';
      }

      break;
  }
  return $values;
}

/**
 * Implementation of hook_token_list()
 */
function node_token_list($type = 'all') {
  $tokens['node']['[nid]']            = t('Node ID');
  $tokens['node']['[type]']           = t('Node type');
  $tokens['node']['[type-name]']      = t('Node type (user-friendly version)');

  $tokens['node']['[author-uid]']     = t("Node author's user id");
  $tokens['node']['[author-name]']    = t("Node author's user name");

  $tokens['node']['[term]']           = t("Name of top taxonomy term");
  $tokens['node']['[term-id]']        = t("ID of top taxonomy term");
  $tokens['node']['[vocab]']          = t("Name of top term's vocabulary");
  $tokens['node']['[vocab-id]']       = t("ID of top term's vocabulary");
  $tokens['node']['[catpath]']        = t("Full taxonomy tree for the topmost term");

  $tokens['node']['[yyyy]']           = t("Node creation year (four digit)");
  $tokens['node']['[yy]']             = t("Node creation year (two digit)");
  $tokens['node']['[month]']          = t("Node creation month (full word)");
  $tokens['node']['[mon]']            = t("Node creation month (abbreviated)");
  $tokens['node']['[mm]']             = t("Node creation month (two digit, zero padded)");
  $tokens['node']['[m]']              = t("Node creation month (one or two digit)");
  $tokens['node']['[ww]']             = t("Node creation week (two digit)");
  $tokens['node']['[date]']           = t("Node creation date (day of month)");
  $tokens['node']['[day]']            = t("Node creation day (full word)");
  $tokens['node']['[ddd]']            = t("Node creation day (abbreviation)");
  $tokens['node']['[dd]']             = t("Node creation day (two digit, zero-padded)");
  $tokens['node']['[d]']              = t("Node creation day (one or two digit)");
  $tokens['node']['[mod-????] etc.']  = t('All tokens for node creation dates can also be used with with the "mod-" prefix; doing so will use the modification date rather than the creation date.');

  return $tokens;
}