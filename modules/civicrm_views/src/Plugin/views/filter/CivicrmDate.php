<?php
namespace Drupal\civicrm_views\Plugin\views\filter;

use Drupal\views\Plugin\views\filter\Date;
use Drupal\Core\Form\FormStateInterface;

/**
 * @ingroup views_filter_handlers
 * @ViewsFilter("civicrm_date")
 */
class CivicrmDate extends Date{

  protected function valueForm(&$form, FormStateInterface $form_state) {
    parent::valueForm($form, $form_state);
    if (isset($form['value']) && !empty($form['value']['operator']) && $form['value']['operator']['#type'] == 'radios' ) {
      $form['value']['operator']['#options']['regular_expression'] = ts('Regular expression');
      $form['value']['operator']['#options']['not_regular_expression'] = ts('Negated regular expression');
    }
  }
  public function operators() {
    return [
      '<' => [
        'title' => $this->t('Is less than'),
        'method' => 'opSimple',
        'short' => $this->t('<'),
        'values' => 1,
      ],
      '<=' => [
        'title' => $this->t('Is less than or equal to'),
        'method' => 'opSimple',
        'short' => $this->t('<='),
        'values' => 1,
      ],
      '=' => [
        'title' => $this->t('Is equal to'),
        'method' => 'opSimple',
        'short' => $this->t('='),
        'values' => 1,
      ],
      '!=' => [
        'title' => $this->t('Is not equal to'),
        'method' => 'opSimple',
        'short' => $this->t('!='),
        'values' => 1,
      ],
      '>=' => [
        'title' => $this->t('Is greater than or equal to'),
        'method' => 'opSimple',
        'short' => $this->t('>='),
        'values' => 1,
      ],
      '>' => [
        'title' => $this->t('Is greater than'),
        'method' => 'opSimple',
        'short' => $this->t('>'),
        'values' => 1,
      ],
      'between' => [
        'title' => $this->t('Is between'),
        'method' => 'opBetween',
        'short' => $this->t('between'),
        'values' => 2,
      ],
      'regular_expression' => [
        'title' => $this->t('Regular expression'),
        'short' => $this->t('regex'),
        'method' => 'opRegex',
        'values' => 1,
      ],
      'not_regular_expression' => [
        'title' => $this->t('Negated regular expression'),
        'short' => $this->t('not regex'),
        'method' => 'opNotRegex',
        'values' => 1,
      ],
    ];
  }

  protected function opSimple($field){
    if (!empty($this->value['type']) && $this->value['type'] == 'offset') {
      $value = intval(strtotime($this->value['value'], 0));
      // keep sign
      $value = time() + sprintf('%+d', $value);
    }
    else {
      $value = intval(strtotime($this->value['value']));
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
      if (!preg_match('/^[+\-]\d+/', $this->value['min'])) {
        // support strtotime relative string
        $a = strtotime($this->value['min']);
      }
      else {
        // keep sign
        $a = $now + sprintf('%+d', $a);
      }
      if (!preg_match('/^[+\-]\d+/', $this->value['max'])) {
        // support strtotime relative string
        $b = strtotime($this->value['max']);
      }
      else {
        // keep sign
        $b = $now + sprintf('%+d', $b);
      }
    }

    if($a){
      $a = $this->formatDate($a);
      $this->query->addWhere($this->options['group'], $field, $a, '>=');
    }
    if($b){
      $b = $this->formatDate($b);
      $this->query->addWhere($this->options['group'], $field, $b, '<=');
    }

  }

  public function acceptExposedInput($input){
    $rc=parent::acceptExposedInput($input);
    if($this->operator=='between'){
      if ($this->value['min'] == '' && $this->value['max'] == '') {
        return FALSE;
      }else{
        return TRUE;
      }
    }
    return $rc;
  }

  public function validateExposed(&$form, FormStateInterface $form_state) {
    if (empty($this->options['exposed'])) {
      return;
    }

    if (empty($this->options['expose']['required'])) {
      // Who cares what the value is if it's exposed and non-required.
      return;
    }
    $value = &$form_state->getValue($this->options['expose']['identifier']);
    if (!empty($this->options['expose']['use_operator']) && !empty($this->options['expose']['operator_id'])) {
      $operator = &$form_state->getValue($this->options['expose']['operator_id']);
    } else {
      $operator = $this->operator;
    }

    $this->validateValidTime($this->options['expose']['identifier'], $form_state, $operator, $value);

  }

  /**
   * Validate that the time values convert to something usable.
   */
  public function validateValidTime(&$form, FormStateInterface $form_state, $operator, $value){
    $operators = $this->operators();
    if ($operators[$operator]['values'] == 1) {
      $convert = strtotime($value['value']);
      if (!empty($form['value']) && ($convert == -1 || $convert === FALSE)) {
        $form_state->setError($form['value'], $this->t('Invalid date format.'));
      }
    } elseif ($operators[$operator]['values'] == 2) {
      $min = strtotime($value['min']);
      // if ($min == -1 || $min === FALSE) {
        // $form_state->setError($form['min'], $this->t('Invalid date format.'));
      // }
      $max = strtotime($value['max']);
      // if ($max == -1 || $max === FALSE) {
        // $form_state->setError($form['max'], $this->t('Invalid date format.'));
      // }
      if($min===false && $max===false){
        $form_state->setError($form['max'], $this->t('Invalid date format.'));
      }
    }
  }

  protected function formatDate($unixtime){
    return date("Y-m-d H:i:s", $unixtime);
  }

}