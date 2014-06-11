<?php

/**
 * @file
 * Contains \Drupal\token\Controller\TokenAutocompleteController.
 */

namespace Drupal\token\Controller;

use Drupal\Component\Serialization\Json;
use Drupal\Core\Ajax\AjaxResponse;
use Drupal\Core\Ajax\OpenModalDialogCommand;
use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\HttpFoundation\Response;

/**
 * Returns tree responses for tokens.
 */
class TokenTreeController extends ControllerBase {

  /**
   * Page callback to output a token tree as an empty page.
   */
  function outputTree() {
    $build['#title'] = $this->t('Available tokens');

    $options = isset($_GET['options']) ? Json::decode($_GET['options']) : array();

    // Force the dialog option to be false so we're not creating a dialog within
    // a dialog.
    $options['dialog'] = FALSE;
    $build['tree']['#markup'] = _theme('token_tree', $options);

    return $build;
  }

}
