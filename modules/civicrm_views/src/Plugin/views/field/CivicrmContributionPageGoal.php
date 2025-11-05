<?php

namespace Drupal\civicrm_views\Plugin\views\field;

use Drupal\civicrm\Civicrm;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\core\form\FormStateInterface;
use Drupal\views\ResultRow;
use Drupal\views\Plugin\views\field\FieldPluginBase;

/**
 * Field handler for contribution page goal achievement fields.
 *
 * @ingroup views_field_handlers
 *
 * @ViewsField("civicrm_contribution_page_goal")
 */
class CivicrmContributionPageGoal extends FieldPluginBase {

  protected $civicrm;

  public function __construct(array $configuration, $plugin_id, $plugin_definition, Civicrm $civicrm) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->civicrm = $civicrm;
    $civicrm->initialize();
  }

  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('civicrm')
    );
  }

  /**
   * {@inheritdoc}
   */
  protected function defineOptions() {
    $options = parent::defineOptions();
    $options['goal_field'] = ['default' => 'percent'];
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function buildOptionsForm(&$form, FormStateInterface $form_state) {
    parent::buildOptionsForm($form, $form_state);

    $form['goal_field'] = [
      '#type' => 'select',
      '#title' => $this->t('Goal field to display'),
      '#options' => [
        'type' => $this->t('Goal Type'),
        'label' => $this->t('Goal Label'),
        'goal' => $this->t('Goal Target Amount/Count'),
        'achieved' => $this->t('Goal Achieved (Yes/No)'),
        'current' => $this->t('Current Achieved Amount/Count'),
        'percent' => $this->t('Achievement Percentage'),
      ],
      '#default_value' => $this->options['goal_field'] ?? 'percent',
      '#description' => $this->t('Select which goal field to display in this column.'),
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function render(ResultRow $values) {
    $pageId = $this->getValue($values, 'id');

    if (empty($pageId)) {
      return '';
    }

    // Get goal achievement data
    $achieved = \CRM_Contribute_BAO_ContributionPage::goalAchieved($pageId);

    if (empty($achieved)) {
      return '';
    }

    $field = $this->options['goal_field'];

    switch ($field) {
      case 'type':
        return $this->sanitizeValue($achieved['type'] ?? '');

      case 'label':
        return $this->sanitizeValue($achieved['label'] ?? '');

      case 'goal':
        $value = $achieved['goal'] ?? 0;
        if ($achieved['type'] === 'amount') {
          return \CRM_Utils_Money::format($value);
        }
        return $value;

      case 'achieved':
        return !empty($achieved['achieved']) ? $this->t('Yes') : $this->t('No');

      case 'current':
        $value = $achieved['current'] ?? 0;
        if ($achieved['type'] === 'amount') {
          return \CRM_Utils_Money::format($value);
        }
        return $value;

      case 'percent':
        $percent = $achieved['percent'] ?? 0;
        return number_format($percent, 2) . '%';

      default:
        return '';
    }
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    // Make sure the id field is available
    $this->ensureMyTable();
    $this->addAdditionalFields(['id']);
  }
}
