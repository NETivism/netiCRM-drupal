<?php

namespace Drupal\civicrm_views\Plugin\views\field;

use Drupal\civicrm\Civicrm;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\views\ResultRow;
use Drupal\views\Plugin\views\field\FieldPluginBase;

/**
 * @ingroup views_field_handlers
 *
 * @ViewsField("civicrm_event_registration_text")
 */
class CivicrmEventRegistrationText extends FieldPluginBase {
  public function __construct(array $configuration, $plugin_id, $plugin_definition, Civicrm $civicrm){
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $civicrm->initialize();
  }

  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition){
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('civicrm')
    );
  }

  public function render(ResultRow $row){
    $v=$this->getValue($row);
    $build = ['#markup' => $v??ts('Register Now')];
    return $build;
  }

}