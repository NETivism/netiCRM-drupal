<?php

namespace Drupal\civicrm_views\Plugin\views\filter;

use Drupal\civicrm\Civicrm;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\views\Plugin\views\filter\FilterPluginBase;

/**
 * Filter handler for contribution page goal achievement.
 *
 * @ingroup views_filter_handlers
 *
 * @ViewsFilter("civicrm_contribution_page_goal_filter")
 */
class CivicrmContributionPageGoalFilter extends FilterPluginBase {

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
    $options['filter_type'] = ['default' => 'achieved'];
    $options['achievement_status'] = ['default' => 'yes'];
    $options['percent_min'] = ['default' => 0];
    $options['percent_max'] = ['default' => 100];
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function buildOptionsForm(&$form, FormStateInterface $form_state) {
    parent::buildOptionsForm($form, $form_state);

    $form['filter_type'] = [
      '#weight' => -10,
      '#type' => 'select',
      '#title' => $this->t('Filter by'),
      '#options' => [
        'achieved' => $this->t('Achievement Status (Yes/No)'),
        'percent' => $this->t('Achievement Percentage Range'),
      ],
      '#default_value' => $this->options['filter_type'] ?? 'achieved',
      '#description' => $this->t('Choose how to filter goal achievement.'),
    ];

    $form['achievement_status'] = [
      '#type' => 'select',
      '#title' => $this->t('Achievement Status'),
      '#options' => [
        'yes' => $this->t('Yes - Goal achieved'),
        'no' => $this->t('No - Goal not achieved'),
      ],
      '#default_value' => $this->options['achievement_status'] ?? 'yes',
      '#states' => [
        'visible' => [
          ':input[name="options[filter_type]"]' => ['value' => 'achieved'],
        ],
      ],
    ];

    $form['percent_min'] = [
      '#type' => 'number',
      '#title' => $this->t('Minimum percentage'),
      '#default_value' => $this->options['percent_min'] ?? 0,
      '#min' => 0,
      '#max' => 100,
      '#states' => [
        'visible' => [
          ':input[name="options[filter_type]"]' => ['value' => 'percent'],
        ],
      ],
    ];

    $form['percent_max'] = [
      '#type' => 'number',
      '#title' => $this->t('Maximum percentage'),
      '#default_value' => $this->options['percent_max'] ?? 100,
      '#min' => 0,
      '#max' => 100,
      '#states' => [
        'visible' => [
          ':input[name="options[filter_type]"]' => ['value' => 'percent'],
        ],
      ],
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    // We don't add anything to the query here since goal achievement
    // is calculated dynamically. We'll filter in postExecute instead.
    $this->ensureMyTable();
  }

  /**
   * {@inheritdoc}
   */
  public function postExecute(&$values) {
    // Filter results based on goal achievement
    if (empty($values)) {
      return;
    }

    $filter_type = $this->options['filter_type'] ?? 'achieved';
    $achievement_status = $this->options['achievement_status'] ?? 'yes';

    foreach ($values as $key => $row) {
      $pageId = $row->civicrm_contribution_page_id ?? $row->id;

      if (empty($pageId)) {
        unset($values[$key]);
        continue;
      }

      // Get goal achievement data
      $achieved = \CRM_Contribute_BAO_ContributionPage::goalAchieved($pageId);

      if (empty($achieved)) {
        // If no goal data, remove this row
        unset($values[$key]);
        continue;
      }

      $should_keep = TRUE;

      if ($filter_type === 'achieved') {
        if ($achievement_status === 'yes' && !$achieved['achieved']) {
          $should_keep = FALSE;
        }
        elseif ($achievement_status === 'no' && $achieved['achieved']) {
          $should_keep = FALSE;
        }
      }
      elseif ($filter_type === 'percent') {
        $percent = $achieved['percent'] ?? 0;
        $min = $this->options['percent_min'] ?? 0;
        $max = $this->options['percent_max'] ?? 100;

        if ($percent < $min || $percent > $max) {
          $should_keep = FALSE;
        }
      }

      if (!$should_keep) {
        unset($values[$key]);
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function canExpose() {
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function adminSummary() {
    if (!empty($this->options['exposed'])) {
      return $this->t('exposed');
    }

    $filter_type = $this->options['filter_type'] ?? 'achieved';
    if ($filter_type === 'achieved') {
      $achievement_status = $this->options['achievement_status'] ?? 'yes';
      return $this->t('Achievement: @status', ['@status' => $achievement_status]);
    }
    else {
      $min = $this->options['percent_min'] ?? 0;
      $max = $this->options['percent_max'] ?? 100;
      return $this->t('Percentage: @min% - @max%', ['@min' => $min, '@max' => $max]);
    }
  }
}
