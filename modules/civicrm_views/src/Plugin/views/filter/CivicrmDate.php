<?php
namespace Drupal\civicrm_views\Plugin\views\filter;

use Drupal\views\Plugin\views\filter\Date;

/**
 * @ingroup views_filter_handlers
 * @ViewsFilter("civicrm_date")
 */
class CivicrmDate extends Date{



  protected function opSimple($field){

    $value = intval(strtotime($this->value['value'], 0));

    if (!empty($this->value['type']) && $this->value['type'] == 'offset') {
      // keep sign
      $value = time() + sprintf('%+d', $value);
    }

    $value = $this->formatDate($value);

    $this->query->addWhere($this->options['group'], $field, $value, $this->operator);

  }

  protected function opBetween($field){
    if ($this->operator == 'between') {
      $a = intval(strtotime($this->value['min'], 0));
      $b = intval(strtotime($this->value['max'], 0));
    } else {
      $a = intval(strtotime($this->value['max'], 0));
      $b = intval(strtotime($this->value['min'], 0));

      $this->query->setWhereGroup('OR', $this->options['group']);
    }

    if ($this->value['type'] == 'offset') {
      $now = time();
      // keep sign
      $a = $now + sprintf('%+d', $a);
      // keep sign
      $b = $now + sprintf('%+d', $b);
    }

    $a = $this->formatDate($a);
    $b = $this->formatDate($b);

    // %s is safe here because strtotime + format_date scrubbed the input
    $this->query->addWhere($this->options['group'], $field, $a, '>=');
    $this->query->addWhere($this->options['group'], $field, $b, '<=');

  }

  protected function formatDate($unixtime){
    return date("Y-m-d H:i:s", $unixtime);
  }

}