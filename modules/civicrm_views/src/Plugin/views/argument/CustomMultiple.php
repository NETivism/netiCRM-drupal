<?php
/**
 * Created by Ken on 2021/11/1 上午 01:27
 */

namespace Drupal\civicrm_views\Plugin\views\argument;


use Drupal\civicrm\Civicrm;
use Drupal\Core\Database\Database;
use Drupal\Core\Form\FormStateInterface;
use Drupal\views\Plugin\views\argument\ArgumentPluginBase;
use Drupal\views\Plugin\views\display\DisplayPluginBase;
use Drupal\views\ViewExecutable;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 *
 * @ingroup views_argument_handlers
 *
 * @ViewsArgument("civicrm_custom_multiple")
 */
class CustomMultiple extends ArgumentPluginBase {

  public function __construct(array $configuration, $plugin_id, $plugin_definition,Civicrm $civicrm) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $civicrm->initialize();
  }

  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,$plugin_id,$plugin_definition,$container->get('civicrm')
    );

  }


  public function init(ViewExecutable $view, DisplayPluginBase $display, array &$options = NULL) {
    parent::init($view, $display, $options);
    $this->html_type=$this->definition['pseudo info']['html_type'];
  }

  protected function defineOptions() {
    $options= parent::defineOptions();
    $options['glossary'] = ['default' => FALSE, 'bool' => TRUE];
    $options['limit'] = ['default' => 0];
    $options['case'] = ['default' => 'none'];
    $options['path_case'] = ['default' => 'none'];
    $options['transform_dash'] = ['default' => FALSE, 'bool' => TRUE];
    $options['break_phrase'] = ['default' => FALSE, 'bool' => TRUE];

    return $options;
  }

  public function buildOptionsForm(&$form, FormStateInterface $form_state) {
    parent::buildOptionsForm($form, $form_state);
    // allow + for or, , for and
    $form['break_phrase'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow multiple values'),
      '#description' => t('If selected, users can enter multiple values in the form of 1+2+3 (for OR) or 1,2,3 (for AND).'),
      '#default_value' => !empty($this->options['break_phrase']),
      '#group' => 'options][more',
    );
  }

  public function query($group_by = FALSE) {
    $argument = $this->argument;
    if (!empty($this->options['break_phrase'])) {
      $this->unpackArgumentValue();
    }else{
      $this->value=[$argument];
      $this->operator = 'or';
    }

    if ($this->options['case'] != 'none' && Database::getConnection()->databaseType() == 'pgsql') {
      foreach ($this->value as $key => $value) {
        $this->value[$key] = mb_strtolower($value);
      }
    }

    if (!empty($this->definition['many to one'])) {
      if (!empty($this->options['glossary'])) {
        $this->helper->formula = TRUE;
      }
      $this->helper->ensureMyTable();
      $this->helper->addFilter();
      return;
    }

    $this->ensureMyTable();
    $formula = FALSE;
    $field = "$this->tableAlias.$this->realField";
    $placeholder = $this->placeholder();
    $conditions = $placeholders = [];
    if (count($this->value)) {
      $count = 0;
      foreach($this->value as $value) {
        $count++;
        $conditions[$count] = 'FIND_IN_SET('.$placeholder.$count.', REPLACE('.$field.", '".\CRM_Core_DAO::VALUE_SEPARATOR."', ','))";
        $placeholders[$placeholder.$count] = $value;
      }
      $where = implode(' '.$this->operator.' ', $conditions);
      $this->query->addWhereExpression(0, $where, $placeholders);
    }
  }


}
