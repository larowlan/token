<?php

/**
 * @file
 * Contains \Drupal\token\Controller\TokenCacheController.
 */

namespace Drupal\token\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\DependencyInjection\ContainerInjectionInterface;
use Drupal\Core\Url;
use Symfony\Cmf\Component\Routing\RouteObjectInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\HttpFoundation\Request;

/**
 * Clears cache for tokens.
 */
class TokenCacheController extends ControllerBase implements ContainerInjectionInterface  {

  /**
   * @var \Drupal\Core\Access\CsrfTokenGenerator
   */
  private $csrfToken;

  /**
   * Constructs a new TokenAutocompleteController.
   */
  public function __construct($csrf_token) {
    $this->csrfToken = $csrf_token;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('csrf_token')
    );
  }

  public function flush(Request $request) {
    $url = Url::fromRoute('<current>');
    if (!$request->query->has('token') || ! $this->csrfToken->validate($request->query->get('token'), $url->toString())) {
      return MENU_NOT_FOUND;
    }

    token_clear_cache();
    drupal_set_message(t('Token registry caches cleared.'));
    return $this->redirect($request->attributes->get(RouteObjectInterface::ROUTE_NAME), $request->attributes->get('_raw_variables', array()));
  }

}
