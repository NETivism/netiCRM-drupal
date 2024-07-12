<?php

namespace Drupal\civicrm_views\Plugin\views\field;

use Drupal\civicrm\Civicrm;
use Drupal\core\form\FormStateInterface;
use Drupal\views\ResultRow;
use Drupal\views\Plugin\views\field\FieldPluginBase;

/**
 * @ingroup views_field_handlers
 *
 * @ViewsField("civicrm_standard")
 */
class CivicrmStandard extends FieldPluginBase {
  public function __construct(array $configuration, $plugin_id, $plugin_definition) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $civicrm = new Civicrm();
    $civicrm->initialize();
  }

  protected function defineOptions() {
    $options = parent::defineOptions();
    $options['mask_output'] = array('default' => 'standard');
    $options['mask_start'] = array('default' => '0');
    $options['mask_end'] = array('default' => '0');
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function buildOptionsForm(&$form, FormStateInterface $form_state) {
    parent::buildOptionsForm($form, $form_state);
    $form['mask_output'] = [
      '#type' => 'select',
      '#title' => $this->t('Mask output'),
      '#options' => [
        'standard' => ts('Complete display'),
        'partial' => ts('Partial hide'),
        'mask' => ts('Complete hide'),
      ],
      '#default_value' => $this->options['mask_output'] ?? 'standard',
    ];
    $form['mask_start'] = [
      '#type' => 'number',
      '#title' => $this->t('Mask start'),
      '#default_value' => $this->options['mask_start'] ?? '0',
      '#description' => $this->t('Start of mask position, using zero as first character.'),
      '#states' => [
        'visible' => [
          ':input[name="options[mask_output]"]' => ['value' => 'partial']
        ]
      ],
    ];
    $form['mask_end'] = [
      '#type' => 'number',
      '#title' => $this->t('Mask end'),
      '#default_value' => $this->options['mask_end'] ?? '0',
      '#description' => $this->t('End of mask position, calc from end of string.'),
      '#states' => [
        'visible' => [
          ':input[name="options[mask_output]"]' => ['value' => 'partial']
        ]
      ],
    ];
  }


  public function render(ResultRow $values) {
    $value = $this->getValue($values);
    if ($this->options['mask_output'] == 'partial') {
      if (empty($this->options['mask_start']) && empty($this->options['mask_end'])) {
        return \CRM_Utils_String::mask($value, 'auto');
      }
      else {
        return \CRM_Utils_String::mask($value, 'custom', (int)$this->options['mask_start'], (int)$this->options['mask_end']);
      }
    }
    elseif ($this->options['mask_output'] == 'mask') {
      return \CRM_Utils_String::mask($value, 'custom', 0, 0);
    }
    return $value;
  }
}
