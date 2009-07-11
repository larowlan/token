<?php
// $Id$

/**
 * @file
 * Builds placeholder replacement tokens for values specific to Poll nodes.
 */

/**
 * Implement hook_tokens().
 */
function poll_tokens($type, $tokens, $data, $sanitize = TRUE) {
  $replacements = array();

  if ($type == 'node' && !empty($data['node']) && $data['node']->type == 'poll') {
    $node = $data['node'];

    $total_votes = 0;
    $highest_votes = 0;
    foreach ($node->choice as $choice) {
      if ($choice['chvotes'] > $highest_votes) {
        $winner = $choice;
        $highest_votes = $choice['chvotes'];
      }
      $total_votes = $total_votes + $choice['chvotes'];
    }
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'poll-votes':
          $replacements[$original] = $total_votes;
          break;
        case 'poll-winner':
          if (isset($winner)) {
            $replacements[$original] = $sanitize ? filter_xss($winner['chtext']) : $winner['chtext'];
          }
          break;
        case 'poll-winner-votes':
          if (isset($winner)) {
            $replacements[$original] = $winner['chvotes'];
          }
          break;
        case 'poll-winner-percent':
          if (isset($winner)) {
            $percent = ($winner['chvotes'] / $total_votes) * 100;
            $replacements[$original] = number_format($percent, 0);
          }
          break;
        case 'poll-duration':
          $replacements[$original] = format_interval($node->runtime, 1);
          break;
      }
    }
  }

  return $replacements;
}

/**
 * Implement hook_token_info().
 */
function poll_token_info() {
  $data = array();

  // Poll results
  $data['tokens']['node']['poll-votes']['name'] = t('Poll votes');
  $data['tokens']['node']['poll-votes']['description'] = t('The number of votes that have been cast on a poll node.');

  $data['tokens']['node']['poll-winner']['name'] = t('Poll winner');
  $data['tokens']['node']['poll-winner']['description'] = t("The winning poll answer.");

  $data['tokens']['node']['poll-winner-votes']['name'] = t('Poll winner votes');
  $data['tokens']['node']['poll-winner-votes']['description'] = t('The number of votes received by the winning poll answer.');

  $data['tokens']['node']['poll-winner-percent']['name'] = t('Poll winner percent');
  $data['tokens']['node']['poll-winner-percent']['description'] = t('The percentage of votes received by the winning poll answer.');

  $data['tokens']['node']['poll-duration']['name'] = t('Poll duration');
  $data['tokens']['node']['poll-duration']['description'] = t('The length of time the poll node is set to run.');

  return $data;
}
