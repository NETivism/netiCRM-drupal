<?php
/*
 +--------------------------------------------------------------------+
 | CiviCRM version 4.3                                                |
 +--------------------------------------------------------------------+
 | This file is a part of CiviCRM.                                    |
 |                                                                    |
 | CiviCRM is free software; you can copy, modify, and distribute it  |
 | under the terms of the GNU Affero General Public License           |
 | Version 3, 19 November 2007 and the CiviCRM Licensing Exception.   |
 |                                                                    |
 | CiviCRM is distributed in the hope that it will be useful, but     |
 | WITHOUT ANY WARRANTY; without even the implied warranty of         |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.               |
 | See the GNU Affero General Public License for more details.        |
 |                                                                    |
 | You should have received a copy of the GNU Affero General Public   |
 | License and the CiviCRM Licensing Exception along                  |
 | with this program; if not, contact CiviCRM LLC                     |
 | at info[AT]civicrm[DOT]org. If you have questions about the        |
 | GNU Affero General Public License or the licensing of CiviCRM,     |
 | see the CiviCRM license FAQ at http://civicrm.org/licensing        |
 +--------------------------------------------------------------------+
*/


/*
 * Copyright (C) 2009 James Rakich
 * Licensed to CiviCRM under the Academic Free License version 3.0.
 *
 * civicrm_handler_field_event_price_set.inc
 * Gathers Price Set Information for the event to be displayed in a list (inline, numbered or unnumbered)
 *
 */
class civicrm_handler_field_event_available_places extends views_handler_field {
  function construct() {
    parent::construct();
    civicrm_initialize();
    require_once('api/v3/Event.php');
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['max_participant_type'] = array(
      'default' => 'number',
    );
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['max_participant_type'] = array(
      '#type' => 'select',
      '#title' => 'Value Type',
      '#default_value' => $this->options['max_participant_type'],
      '#options' => array(
        'number' => 'Max Participants Limit',
        'available_places' => 'Available Places',
        'is_full' => 'Is Full',
      ),
    );
    parent::options_form($form, $form_state);
  }

  function get_value($values, $field = NULL) {
    $type = !empty($this->options['max_participant_type']) ? $this->options['max_participant_type'] : 'number';
    switch ($type) {
      case 'available_places':
      case 'is_full':
        if (!empty($values->id)) {
          $event_id = $values->id;
          $events = array();
          _civicrm_api3_event_getisfull($events, $event_id);
          $event = $events[$event_id];
          return !empty($event[$type]) ? (int) $event[$type] : 0;
        }
        break;
      case 'number':
      default:
        return (int) $values->{$this->field_alias};
    }
  }

  function render($values) {
    $value = $this->get_value($values);
    $type = !empty($this->options['max_participant_type']) ? $this->options['max_participant_type'] : 'number';
    switch ($type) {
      case 'available_places':
        if (empty($values->{$this->field_alias})) {
          return $value ? $value : t('Unlimited');
        }
        else {
          return (int) $value;
        }
      case 'is_full':
        return $value ? t('Is Full') : '';
      case 'number':
      default:
        return (int) $value;
    }
  }
}

