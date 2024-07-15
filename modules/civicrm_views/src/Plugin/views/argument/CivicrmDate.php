<?php

namespace Drupal\civicrm_views\Plugin\views\argument;


use Drupal\civicrm\Civicrm;
use Drupal\views\Plugin\views\argument\ArgumentPluginBase;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 *
 * @ingroup views_argument_handlers
 *
 * @ViewsArgument("civicrm_date")
 */
class CivicrmDate extends ArgumentPluginBase {

  public function __construct(array $configuration, $plugin_id, $plugin_definition,Civicrm $civicrm) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $civicrm->initialize();
  }

  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,$plugin_id,$plugin_definition,$container->get('civicrm')
    );

  }

  public function query($group_by = FALSE) {
    $this->ensureMyTable();
    $field = "$this->tableAlias.$this->realField";
    $value = $this->argument;
    $placeholder = $this->placeholder();
    $this->query->addWhereExpression($this->options['group'], "$field = $placeholder", array( $placeholder => $value));
  }
}
