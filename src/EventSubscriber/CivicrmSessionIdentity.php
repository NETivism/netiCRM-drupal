<?php

namespace Drupal\civicrm\EventSubscriber;

use Drupal\civicrm\Civicrm;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;
use Symfony\Component\HttpKernel\Event\RequestEvent;
use Symfony\Component\HttpKernel\KernelEvents;

/**
 * Invalidate stale CRM identities on the next request, including non-CRM pages.
 */
class CivicrmSessionIdentity implements EventSubscriberInterface {

  protected $civicrm;

  /**
   * @param \Drupal\civicrm\Civicrm $civicrm CiviCRM initialization service.
   */
  public function __construct(Civicrm $civicrm) {
    $this->civicrm = $civicrm;
  }

  /**
   * @param \Symfony\Component\HttpKernel\Event\RequestEvent $event Request whose existing CRM identity should be checked.
   * @return void
   */
  public function onRequest(RequestEvent $event) {
    if (!$event->isMainRequest()) {
      return;
    }
    $request = $event->getRequest();
    if (!$request->hasPreviousSession()) {
      return;
    }
    $scope = $request->getSession()->get('CiviCRM', []);
    if (is_array($scope) && (isset($scope['ufID']) || isset($scope['userID']))) {
      $initialized = $this->civicrm->isInitialized();
      $this->civicrm->initialize();
      if ($initialized) {
        \CRM_Core_BAO_UFMatch::refreshSession();
      }
    }
  }

  /**
   * @return array Request event listener configuration.
   */
  public static function getSubscribedEvents() {
    // Authentication is priority 300; routing/access checks start at 32.
    return [KernelEvents::REQUEST => ['onRequest', 33]];
  }
}
