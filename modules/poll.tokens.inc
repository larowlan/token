<?php
// $Id$

function poll_tokens($type, $tokens, $data, $safe_for_html = TRUE) {
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
    foreach($tokens as $name => $original) {
      if ($name == 'poll-votes') $replacements[$original] = $total_votes;
      elseif ($name == 'poll-winner' && isset($winner)) $replacements[$original] = check_plain($winner['chtext']);
      elseif ($name == 'poll-winner-votes' && isset($winner)) $replacements[$original] = $winner['chvotes'];
      elseif ($name == 'poll-winner-percent' && isset($winner)) {
        $percent = ($winner['chvotes'] / $total_votes) * 100;
        $replacements[$original] = number_format($percent, 0);
      }
      elseif ($name == 'poll-duration') $replacements[$original] = format_interval($node->runtime, 1);
    }
  }

  return $replacements;
}