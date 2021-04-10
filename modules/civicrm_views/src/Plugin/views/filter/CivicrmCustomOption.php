<?php

namespace Drupal\civicrm_views\Plugin\views\filter;

use Drupal\views\ViewExecutable;
use Drupal\views\Plugin\views\display\DisplayPluginBase;
use Drupal\views\Plugin\views\filter\InOperator;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\civicrm\Civicrm;

/**
 * @ingroup views_filter_handlers
 * @ViewsFilter("civicrm_custom_option")
 */
class CivicrmCustomOption extends CivicrmInOperator {

  public function init(ViewExecutable $view, DisplayPluginBase $display, array &$options = NULL){
    parent::init($view, $display, $options);
    // require_once 'CRM/Core/BAO/CustomOption.php';
  }

  public function getValueOptions(){
    if (isset($this->valueOptions)) {
      return $this->valueOptions;
    }
    if(is_array($this->definition['options arguments']) && key_exists('custom_field_id', $this->definition['options arguments'])){
      $this->valueOptions=$this->getCustomFieldOptions($this->definition['options arguments']['custom_field_id']);
    }

    return $this->valueOptions;

  }

  /**
   * @todo: Cache using Drupal Cache API
   */
  protected function getCustomFieldOptions($custom_field_id, $ignore_cache = false){
    require_once 'CRM/Core/BAO/CustomOption.php';
    $options = [];
    $req_time = \Drupal::time()->getRequestTime();
    $cache_id = 'custom_' . $custom_field_id;

    if (
      key_exists($cache_id, $this->options_cache) &&
      !$ignore_cache &&
      ($req_time < $this->options_cache[$cache_id]['expired'])
    ) {
      $options = $this->options_cache[$cache_id]['options'];
    } else {
      $raw_options = \CRM_Core_BAO_CustomOption::getCustomOption($custom_field_id);
      foreach ($raw_options as $k => $v) {
        $options[$v['value']] = $v['label'];
      }
      $this->options_cache[$cache_id] = array(
        'options' => $options,
        'expired' => $req_time + 3600 //一小時快取
      );
    }
    return $options;
  }


}
