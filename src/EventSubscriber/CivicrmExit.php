<?php
namespace Drupal\civicrm\EventSubscriber;

use Symfony\Component\HttpKernel\KernelEvents;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;

class CivicrmExit implements EventSubscriberInterface {

  public function beforeShutdown() {
    $isCRMInit = \Drupal::service('civicrm')->isInitialized();
    if ($isCRMInit) {
      if (!\CRM_Utils_System::$_shutdowned) {
        \CRM_Utils_System::civiBeforeShutdown();
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public static function getSubscribedEvents() {
    $events[KernelEvents::TERMINATE][] = array('beforeShutdown');
    return $events;
  }

}