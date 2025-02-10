<?php

namespace Drupal\civicrm_views\Plugin\views\argument;


use Drupal\civicrm\Civicrm;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\views\Plugin\views\argument\NumericArgument;
use Drupal\Core\Form\FormStateInterface;

/**
 * @ViewsArgument("civicrm_custom_label")
 */
class CustomLabel extends NumericArgument {
  protected $_custom_labels;
  public function __construct(array $configuration, $plugin_id, $plugin_definition) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    civicrm_initialize();
    if (!empty($configuration['label callback']) && !empty($configuration['label arguments'])) {
      $this->_custom_labels = call_user_func_array($configuration['label callback'], $configuration['label arguments']);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function summaryName($data) {
    $value = (int) $data->{$this->name_alias};
    if (!empty($this->_custom_labels) && isset($this->_custom_labels[$value])) {
      return $this->_custom_labels[$value];
    }
    return parent::summaryName($data);
  }
}