<?php

/**
 * @file
 * Definition of views_handler_argument_custom_multiple.
 */

/**
 * Basic argument handler to support multiple value field in civicrm
 * limits.
 *
 * @ingroup views_argument_handlers
 */
class views_handler_argument_custom_multiple extends views_handler_argument {
  function init(&$view, &$options) {
    parent::init($view, $options);
    if (!empty($this->definition['many to one'])) {
      $this->helper = new views_many_to_one_helper($this);

      // Ensure defaults for these, during summaries and stuff:
      $this->operator = 'or';
      $this->value = array();
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['glossary'] = array('default' => FALSE, 'bool' => TRUE);
    $options['limit'] = array('default' => 0);
    $options['case'] = array('default' => 'none');
    $options['path_case'] = array('default' => 'none');
    $options['transform_dash'] = array('default' => FALSE, 'bool' => TRUE);
    $options['break_phrase'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // allow + for or, , for and
    $form['break_phrase'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow multiple values'),
      '#description' => t('If selected, users can enter multiple values in the form of 1+2+3 (for OR) or 1,2,3 (for AND).'),
      '#default_value' => !empty($this->options['break_phrase']),
      '#fieldset' => 'more',
    );
  }

  /**
   * Build the query based upon the formula
   */
  function query($group_by = FALSE) {
    $argument = $this->argument;

    if (!empty($this->options['break_phrase'])) {
      views_break_phrase_string($argument, $this);
    }
    else {
      $this->value = array($argument);
      $this->operator = 'or';
    }

    if (!empty($this->definition['many to one'])) {
      $this->helper->ensure_my_table();
      $this->helper->add_filter();
      return;
    }

    $this->ensure_my_table();
    $formula = FALSE;
    $field = "$this->table_alias.$this->real_field";
    $placeholder = $this->placeholder();
    $conditions = $placeholders = array();
    if (count($this->value)) {
      $count = 0;
      foreach($this->value as $value) {
        $count++;
        $conditions[$count] = "FIND_IN_SET($placeholder".$count.", REPLACE("."$this->table_alias.$this->real_field".", '', ','))";
        $placeholders[$placeholder.$count] = $value;
      }
      $where = implode(' '.$this->operator.' ', $conditions);
      $this->query->add_where_expression(0, $where, $placeholders);
    }
  }
}
