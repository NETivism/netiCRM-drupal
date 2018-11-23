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

/**
 * Field handler to provide amount raised by given Personal Campaign Page
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_field_pcp_raised_amount extends views_handler_field {
  function construct() {
    parent::construct();
    if (!civicrm_initialize()) {
      return;
    }
    require_once 'CRM/Utils/Money.php';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['raised_amount_type'] = array(
      'default' => 'amount',
    );
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['raised_amount_type'] = array(
      '#type' => 'select',
      '#title' => 'Raised Amount Type',
      '#default_value' => $this->options['raised_amount_type'],
      '#options' => array(
        'amount' => 'Amount',
        'amount_per_contact' => 'Amount per Contact',
        'num_contact' => 'Number of Contacts',
        'num_contribution' => 'Number of Contributions',
      ),
    );
    parent::options_form($form, $form_state);
  }

  function render($values) {
    $raised_amount_type = !empty($this->options['raised_amount_type']) ? $this->options['raised_amount_type'] : 'amount';
    // retrieve the value of the "real field"
    $pcp_id = $values->{$this->field_alias};
    // get the total amount raised
    $amount = CRM_Contribute_BAO_PCP::thermoMeter($pcp_id, $raised_amount_type);
    if (!$amount) {
      // supply a numeric string if function returns NULL or undesirable result
      $amount = '0';
    }
    if (strstr($raised_amount_type, 'num_')) {
      return (int)$amount;
    }
    else {
      return CRM_Utils_Money::format($amount);
    }
  }
}


