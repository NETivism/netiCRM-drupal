<?php
/*
 +--------------------------------------------------------------------+
 | CiviCRM version 4.4                                                |
 +--------------------------------------------------------------------+
 | Copyright CiviCRM LLC (c) 2004-2013                                |
 +--------------------------------------------------------------------+
 | This file is a part of CiviCRM.                                    |
 |                                                                    |
 | CiviCRM is free software; you can copy, modify, and distribute it  |
 | under the terms of the GNU Affero General Public License           |
 | Version 3, 19 November 2007.                                       |
 |                                                                    |
 | CiviCRM is distributed in the hope that it will be useful, but     |
 | WITHOUT ANY WARRANTY; without even the implied warranty of         |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.               |
 | See the GNU Affero General Public License for more details.        |
 |                                                                    |
 | You should have received a copy of the GNU Affero General Public   |
 | License along with this program; if not, contact CiviCRM LLC       |
 | at info[AT]civicrm[DOT]org. If you have questions about the        |
 | GNU Affero General Public License or the licensing of CiviCRM,     |
 | see the CiviCRM license FAQ at http://civicrm.org/licensing        |
 +--------------------------------------------------------------------+
*/

/**
 *
 * @package CRM
 * @copyright CiviCRM LLC (c) 2004-2013
 * @copyright DharmaTech  (c) 2009
 * $Id$
 *
 */

require_once 'Engage/Report/Form/List.php';

/**
 *  Generate a phone call list report
 */
class Engage_Report_Form_CallList extends Engage_Report_Form_List {
  public function __construct() {

    parent::__construct();

    $this->_columns = [
      'civicrm_phone' =>
      [
        'dao' => 'CRM_Core_DAO_Phone',
        'fields' =>
        [
          'phone' => ['default' => TRUE,
            'required' => TRUE,
          ]],
        'grouping' => 'location-fields',
        'order_bys' =>
        ['phone' => ['title' => ts('Phone'),
            'required' => TRUE,
          ]],
      ],
      'civicrm_address' =>
      [
        'dao' => 'CRM_Core_DAO_Address',
        'fields' =>
        [
          'street_address' =>
          ['default' => TRUE],
          'city' =>
          ['default' => TRUE],
          'postal_code' =>
          [
            'title' => 'Zip',
            'default' => TRUE,
          ],
          'state_province_id' =>
          ['title' => ts('State/Province'),
            'default' => TRUE,
          ],
          'country_id' =>
          ['title' => ts('Country'),
          ],
        ],
        'filters' =>
        [
          'street_address' => NULL,
          'city' => NULL,
          'postal_code' => ['title' => 'Zip'],
        ],
        'grouping' => 'location-fields',
      ],
      'civicrm_email' =>
      [
        'dao' => 'CRM_Core_DAO_Email',
        'fields' =>
        ['email' => NULL],
        'grouping' => 'location-fields',
      ],
      'civicrm_contact' =>
      [
        'dao' => 'CRM_Contact_DAO_Contact',
        'fields' =>
        [
          'id' =>
          ['title' => ts('Contact ID'),
            'required' => TRUE,
          ],
          'display_name' =>
          ['title' => ts('Contact Name'),
            'required' => TRUE,
            'no_repeat' => TRUE,
          ],
          'gender_id' =>
          ['title' => ts('Sex'),
            'required' => TRUE,
          ],
          'birth_date' =>
          ['title' => ts('Age'),
            'required' => TRUE,
            'type' => CRM_Report_FORM::OP_INT,
          ],
        ],
        'filters' =>
        [
          'sort_name' =>
          ['title' => ts('Contact Name'),
            'operator' => 'like',
          ],
        ],
        'grouping' => 'contact-fields',
      ],
      $this->_demoTable =>
      [
        'dao' => 'CRM_Contact_DAO_Contact',
        'fields' =>
        [
          $this->_demoLangCol =>
          [
            'type' => CRM_Report_FORM::OP_STRING,
            'required' => TRUE,
            'title' => ts('Language'),
          ],
        ],
        'filters' =>
        [
          $this->_demoLangCol =>
          [
            'title' => ts('Language'),
            'operatorType' => CRM_Report_FORM::OP_SELECT,
            'type' => CRM_Report_FORM::OP_STRING,
            'methodName' => 'selector',
            'options' => $this->_languages,
          ],
        ],
        'grouping' => 'contact-fields',
      ],
      $this->_coreInfoTable =>
      [
        'dao' => 'CRM_Contact_DAO_Contact',
        'fields' =>
        [
          $this->_coreTypeCol =>
          [
            'type' => CRM_Report_FORM::OP_STRING,
            'required' => TRUE,
            'title' => ts('Constituent Type'),
          ],
          $this->_coreOtherCol =>
          [
            'no_display' => TRUE,
            'type' => CRM_Report_Form::OP_STRING,
            'required' => TRUE,
          ],
        ],
        'grouping' => 'contact-fields',
      ],
      'civicrm_group' =>
      [
        'dao' => 'CRM_Contact_DAO_GroupContact',
        'alias' => 'cgroup',
        'filters' =>
        [
          'gid' =>
          [
            'name' => 'group_id',
            'title' => ts('Group'),
            'operatorType' => CRM_Report_Form::OP_MULTISELECT,
            'group' => TRUE,
            'options' => CRM_Core_PseudoConstant::group(),
          ],
        ],
      ],
    ];
  }

  public function preProcess() {
    parent::preProcess();
    $reportDate = CRM_Utils_Date::customFormat(date('Y-m-d H:i'));
    $this->assign('reportTitle', "{$this->_orgName} - Call List <br /> {$reportDate}");
  }

  /**
   *  Generate WHERE clauses for SQL SELECT
   */
  public function where() {
    //  Don't list anybody who doesn't have a phone
    //  or has do_not_phone = 1
    $clauses = [
      "{$this->_aliases['civicrm_contact']}.do_not_phone != 1",
      "NOT ISNULL({$this->_aliases['civicrm_phone']}.phone)",
    ];

    foreach ($this->_columns as $tableName => $table) {
      //echo "where: table name $tableName<br>";

      //  Treatment of normal filters
      if (array_key_exists('filters', $table)) {
        foreach ($table['filters'] as $fieldName => $field) {
          //echo "&nbsp;&nbsp;&nbsp;field name $fieldName<br>";
          $clause = NULL;

          if (CRM_Utils_Array::value('type', $field) & CRM_Utils_Type::T_DATE) {
            $relative = CRM_Utils_Array::value("{$fieldName}_relative", $this->_params);
            $from     = CRM_Utils_Array::value("{$fieldName}_from", $this->_params);
            $to       = CRM_Utils_Array::value("{$fieldName}_to", $this->_params);

            $clause = $this->dateClause($field['name'], $relative, $from, $to);
          }
          elseif ($fieldName == $this->_demoLangCol) {
            if (!empty($this->_params[$this->_demoLangCol . '_value'])) {
              $clause = "{$field['dbAlias']}='" . $this->_params[$this->_demoLangCol . '_value'] . "'";
            }
          }
          else {
            $op = CRM_Utils_Array::value("{$fieldName}_op", $this->_params);
            if ($op == 'mand') {
              $clause = TRUE;
            }
            elseif ($op) {
              $clause = $this->whereClause($field,
                $op,
                CRM_Utils_Array::value("{$fieldName}_value", $this->_params),
                CRM_Utils_Array::value("{$fieldName}_min", $this->_params),
                CRM_Utils_Array::value("{$fieldName}_max", $this->_params)
              );
            }
          }
          //echo "&nbsp;&nbsp;&nbsp;clause=";
          //var_dump($clause);
          if (!empty($clause)) {
            if (!empty($field['group'])) {
              $clauses[] = $this->engageWhereGroupClause($clause);
            }
            else {
              $clauses[] = $clause;
            }
          }
        }
      }
    }

    if (empty($clauses)) {
      $this->_where = "WHERE ( 1 ) ";
    }
    else {
      $this->_where = "WHERE " . implode(' AND ', $clauses);
    }
    //echo $this->_where . "<br>";
  }

  /**
   *  Process submitted form
   */
  public function postProcess() {
    parent::postProcess();
  }

  /**
   *  Convert contact type info from fields separated by \x01 to a
   *  string of fields separated by commas
   */
  public function type2str($type) {
    $typeArray = explode("\x01", $type);
    foreach ($typeArray as $key => $value) {
      if (empty($value)) {
        unset($typeArray[$key]);
      }
    }
    return implode(', ', $typeArray);
  }

  public function alterDisplay(&$rows) {
    // custom code to alter rows
    $genderList = CRM_Core_PseudoConstant::get('CRM_Contact_DAO_Contact', 'gender_id');
    $entryFound = FALSE;
    foreach ($rows as $rowNum => $row) {
      // handle state province
      if (array_key_exists('civicrm_address_state_province_id', $row)) {
        if ($value = $row['civicrm_address_state_province_id']) {
          $rows[$rowNum]['civicrm_address_state_province_id'] = CRM_Core_PseudoConstant::stateProvince($value);
        }
        $entryFound = TRUE;
      }

      // handle country
      if (array_key_exists('civicrm_address_country_id', $row)) {
        if ($value = $row['civicrm_address_country_id']) {
          $rows[$rowNum]['civicrm_address_country_id'] = CRM_Core_PseudoConstant::country($value);
        }
        $entryFound = TRUE;
      }

      // Handle contactType
      if (!empty($row[$this->_coreInfoTable . '_' . $this->_coreTypeCol])) {
        $rows[$rowNum][$this->_coreInfoTable . '_' . $this->_coreTypeCol] = $this->type2str($rows[$rowNum][$this->_coreInfoTable . '_' . $this->_coreTypeCol]);
        $entryFound = TRUE;
      }

      // date of birth to age
      if (!empty($row['civicrm_contact_birth_date'])) {
        $rows[$rowNum]['civicrm_contact_birth_date'] = $this->dob2age($row['civicrm_contact_birth_date']);
        $entryFound = TRUE;
      }

      // gender label
      if (!empty($row['civicrm_contact_gender_id'])) {
        $rows[$rowNum]['civicrm_contact_gender_id'] = $genderList[$row['civicrm_contact_gender_id']];
        $entryFound = TRUE;
      }

      if (($this->_outputMode != 'html') && !empty($row[$this->_coreInfoTable . '_' . $this->_coreOtherCol])) {
        $rows[$rowNum]['civicrm_contact_display_name'] .= "<br />" . $row[$this->_coreInfoTable . '_' . $this->_coreOtherCol];
        $entryFound = TRUE;
      }

      // skip looking further in rows, if first row itself doesn't
      // have the column we need
      if (!$entryFound) {
        break;
      }
    }

    $columnOrder = [
      'civicrm_phone_phone',
      'civicrm_contact_display_name',
      'civicrm_address_street_address',
      'civicrm_contact_birth_date',
      'civicrm_contact_gender_id',
      $this->_demoTable . '_' . $this->_demoLangCol,
      $this->_coreInfoTable . '_' . $this->_coreTypeCol,
      'civicrm_contact_id',
    ];
    if ($this->_outputMode == 'print' || $this->_outputMode == 'pdf') {
      $this->_columnHeaders = [
        'civicrm_phone_phone' => [
          'title' => 'PHONE',
          'type' => CRM_Utils_Type::T_STRING,
          'class' => 'width=68',
        ],
        'civicrm_contact_display_name' => [
          'title' => 'NAME',
          'type' => CRM_Utils_Type::T_STRING,
          'class' => 'width=83',
        ],
        'civicrm_address_street_address' => [
          'title' => 'ADDRESS',
          'type' => CRM_Utils_Type::T_STRING,
          'class' => 'width=117',
        ],
        'civicrm_contact_birth_date' => [
          'title' => 'AGE',
          'type' => CRM_Utils_Type::T_STRING,
          'class' => 'width=25',
        ],
        'civicrm_contact_gender_id' => [
          'title' => 'SEX',
          'type' => CRM_Utils_Type::T_STRING,
          'class' => 'width=18',
        ],
        $this->_demoTable . '_' . $this->_demoLangCol =>
        [
          'title' => 'Lang',
          'type' => CRM_Utils_Type::T_STRING,
          'class' => 'width=27',
        ],
        $this->_coreInfoTable . '_' . $this->_coreTypeCol =>
        [
          'title' => 'Contact Type',
          'type' => CRM_Utils_Type::T_STRING,
          'class' => 'width=48',
        ],
        'notes' => [
          'title' => 'NOTES',
          'type' => CRM_Utils_Type::T_STRING,
          'class' => 'width=48',
        ],
        'response_codes' => [
          'title' => 'RESPONSE CODES',
          'type' => CRM_Utils_Type::T_STRING,
          'class' => 'width=91',
        ],
        'status' => [
          'title' => 'STATUS',
          'type' => CRM_Utils_Type::T_STRING,
          'class' => 'width=70',
        ],
        'civicrm_contact_id' => [
          'title' => 'ID',
          'type' => CRM_Utils_Type::T_STRING,
          'class' => 'width=100',
        ],
      ];
      $newRows = [];
      foreach ($columnOrder as $col) {
        foreach ($rows as $rowNum => $row) {
          $newRows[$rowNum][$col] = $row[$col];
          $newRows[$rowNum]['notes'] = '&nbsp;';
          $newRows[$rowNum]['status'] = 'NH&nbsp;MV&nbsp;D&nbsp;WN';
          $newRows[$rowNum]['response_codes'] = '
        Q1&nbsp;&nbsp;&nbsp;&nbsp;Y&nbsp;&nbsp;&nbsp;&nbsp;N&nbsp;&nbsp;&nbsp;&nbsp;U&nbsp;&nbsp;&nbsp;&nbsp;D<br />
        Q2&nbsp;&nbsp;&nbsp;&nbsp;Y&nbsp;&nbsp;&nbsp;&nbsp;N&nbsp;&nbsp;&nbsp;&nbsp;U&nbsp;&nbsp;&nbsp;&nbsp;D<br />
        Q3&nbsp;&nbsp;&nbsp;&nbsp;Y&nbsp;&nbsp;&nbsp;&nbsp;N&nbsp;&nbsp;&nbsp;&nbsp;U&nbsp;&nbsp;&nbsp;&nbsp;D<br />
        Q4&nbsp;&nbsp;&nbsp;&nbsp;Y&nbsp;&nbsp;&nbsp;&nbsp;N&nbsp;&nbsp;&nbsp;&nbsp;U&nbsp;&nbsp;&nbsp;&nbsp;D';
        }
      }
      $rows = $newRows;
      $this->assign('pageTotal', ceil((count($newRows) / 7)));
    }
    else {
      // make sure column order is same as in print mode
      $tempHeaders = $this->_columnHeaders;
      $this->_columnHeaders = [];
      foreach ($columnOrder as $col) {
        if (array_key_exists($col, $tempHeaders)) {
          $this->_columnHeaders[$col] = $tempHeaders[$col];
          unset($tempHeaders[$col]);
        }
      }
      $this->_columnHeaders = $this->_columnHeaders + $tempHeaders;
    }
  }
}

