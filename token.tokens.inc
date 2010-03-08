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
