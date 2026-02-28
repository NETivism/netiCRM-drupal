<?php

function _civicrm_contribution_page_data(&$data, $enabled) {

  $data['civicrm_contribution_page']['table']['group'] = t('CiviCRM Contribution Page');

  $data['civicrm_contribution_page']['table']['base'] = [
    // Governs the whole mozilla
    'field' => 'id',
    'title' => 'CiviCRM '. t('Contribution Pages'),
    'help' => '',
  ];
  //CiviCRM Contribution Page - FIELDS

  //Numeric Contribution ID
  $data['civicrm_contribution_page']['id'] = [
    'title' => t('Contribution Page ID'),
    'help' => t('The numeric ID of the Contribution Page'),
    'field' => [
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  // Contribution Page Title
  $data['civicrm_contribution_page']['title'] = [
    'title' => t('Title'),
    'help' => t('The Contribution Page\'s Title'),
    'field' => [
      'handler' => 'civicrm_handler_field_contribution_page_link',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  // Contribution Page Intro Text
  $data['civicrm_contribution_page']['intro_text'] = [
    'title' => t('Intro Text'),
    'help' => t('The Contribution Page\'s intro'),
    'field' => [
      'handler' => 'civicrm_handler_field_markup',
      'click sortable' => TRUE,
    ],
  ];
  //Contribution Type
  $data['civicrm_contribution_page']['contribution_type'] = [
    'title' => t('Contribution Type'),
    'real field' => 'contribution_type_id',
    'help' => t('The Type of Contribution made, ie Member Dues, Event Fees etc'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'financialType',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'financialType',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  //Numeric: IS Active
  $data['civicrm_contribution_page']['is_active'] = [
    'title' => t('Is Active'),
    'field' => [
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => FALSE,
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  //BOOLEAN : is_monetary
  $data['civicrm_contribution_page']['is_monetary'] = [
    'title' => t('Execute real-time monetary transactions'),
    'field' => [
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_boolean_operator',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  //BOOLEAN : is_internal
  $data['civicrm_contribution_page']['is_internal'] = [
    'title' => t('Is Internal'),
    'field' => [
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_boolean_operator',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  //BOOLEAN : is_monetary
  $data['civicrm_contribution_page']['is_recur'] = [
    'title' => t('Recurring contributions'),
    'field' => [
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_boolean_operator',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  //BOOLEAN : IS Pay later
  $data['civicrm_contribution_page']['is_pay_later'] = [
    'title' => t('Is Pay Later'),
    'field' => [
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'views_handler_filter_boolean_operator',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  //Contribution Page Start DATE
  $data['civicrm_contribution_page']['start_date'] = [
    'title' => t('Start Date'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];

  civicrm_views_add_date_arguments($data['civicrm_contribution_page'], [
    'title' => 'Start Date',
    'name' => 'start_date',
  ]);
  //CONTRIBUTION END DATE
  $data['civicrm_contribution_page']['end_date'] = [
    'title' => t('End Date'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];
  civicrm_views_add_date_arguments($data['civicrm_contribution'], [
    'title' => 'End Date',
    'name' => 'end_date',
  ]);
  // CONTRIBUTION PAGE GOAL AMOUNT
  $data['civicrm_contribution_page']['goal_amount'] = [
    'title' => t('Goal Amount'),
    'field' => [
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
}
