<?php

function _civicrm_contribution_page_data(&$data, $enabled) {

  $data['civicrm_contribution_page']['table']['group'] = t('CiviCRM Contribution Page');

  $data['civicrm_contribution_page']['table']['base'] = array(
    // Governs the whole mozilla
    'field' => 'id',
    'title' => 'CiviCRM '. t('Contribution Pages'),
    'help' => '',
  );
  //CiviCRM Contribution Page - FIELDS

  //Numeric Contribution ID
  $data['civicrm_contribution_page']['id'] = array(
    'title' => t('Contribution Page ID'),
    'help' => t('The numeric ID of the Contribution Page'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // Contribution Page Title
  $data['civicrm_contribution_page']['title'] = array(
    'title' => t('Title'),
    'help' => t('The Contribution Page\'s Title'),
    'field' => array(
      'handler' => 'civicrm_handler_field_contribution_page_link',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // Contribution Page Intro Text
  $data['civicrm_contribution_page']['intro_text'] = array(
    'title' => t('Intro Text'),
    'help' => t('The Contribution Page\'s intro'),
    'field' => array(
      'handler' => 'civicrm_handler_field_markup',
      'click sortable' => TRUE,
    ),
  );
  //Contribution Type
  $data['civicrm_contribution_page']['contribution_type'] = array(
    'title' => t('Contribution Type'),
    'real field' => 'contribution_type_id',
    'help' => t('The Type of Contribution made, ie Member Dues, Event Fees etc'),
    'field' => array(
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'financialType',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'financialType',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  //Numeric: IS Active 
  $data['civicrm_contribution_page']['is_active'] = array(
    'title' => t('Is Active'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => FALSE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  //BOOLEAN : is_monetary
  $data['civicrm_contribution_page']['is_monetary'] = array(
    'title' => t('Execute real-time monetary transactions'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  //BOOLEAN : is_internal
  $data['civicrm_contribution_page']['is_internal'] = array(
    'title' => t('Is Internal'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  //BOOLEAN : is_monetary
  $data['civicrm_contribution_page']['is_recur'] = array(
    'title' => t('Recurring contributions'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  //BOOLEAN : IS Pay later
  $data['civicrm_contribution_page']['is_pay_later'] = array(
    'title' => t('Is Pay Later'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  //Contribution Page Start DATE
  $data['civicrm_contribution_page']['start_date'] = array(
    'title' => t('Start Date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  civicrm_views_add_date_arguments($data['civicrm_contribution_page'], array(
    'title' => 'Start Date',
    'name' => 'start_date',
  ));
  //CONTRIBUTION END DATE
  $data['civicrm_contribution_page']['end_date'] = array(
    'title' => t('End Date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_contribution'], array(
    'title' => 'End Date',
    'name' => 'end_date',
  ));
  // CONTRIBUTION PAGE GOAL AMOUNT
  $data['civicrm_contribution_page']['goal_amount'] = array(
    'title' => t('Goal Amount'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
}
