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
 * Build the $data array for CiviContribute related tables
 * Includes the following tables
 * civicrm_contribute
 * civicrm_contribution_soft
 * civicrm_contribution_recur
 * civicrm_pcp
 */
function _civicrm_contribute_data(&$data, $enabled) {
  //----------------------------------------------------------------
  // CIVICRM Contributions are here with all the bling, base tabling it up.
  //----------------------------------------------------------------

  $data['civicrm_contribution']['table']['group'] = t('CiviCRM Contributions');

  $data['civicrm_contribution']['table']['base'] = [
    // Governs the whole mozilla
    'field' => 'id',
    'title' => t('CiviCRM Contributions'),
    'help' => t("View displays CiviCRM Contributions, with relation to contacts."),
  ];

  //CiviCRM recurring contributions - make this available as a base table too
  $data['civicrm_contribution_recur']['table']['base'] = [
    'field' => 'id',
    'title' => t('CiviCRM Recurring Contribution profile'),
    'help' => t("Recurring Contribution Profiles in CiviCRM."),
  ];

  //this join makes Contribution data available in view based on recurring . A separate relationship is defined elsewhere to make recurring data available to view based on contributi
  $data['civicrm_contribution']['table']['join']['civicrm_contribution_recur'] = [
    'left_field' => 'id',
    'field' => 'contribution_recur_id',
  ];
  //TABLE JOINS FOR CIVICRM CONTRIBUTIONS GO HERE!

  $data['civicrm_contribution']['table']['join']['civicrm_participant'] = [
    'left_table' => 'civicrm_participant_payment',
    'left_field' => 'contribution_id',
    'field' => 'id',
  ];

  $data['civicrm_contribution']['table']['join']['civicrm_pledge'] = [
    'left_table' => 'civicrm_pledge_payment',
    'left_field' => 'contribution_id',
    'field' => 'id',
  ];
  //CiviCRM Contributions - FIELDS

  //Numeric Contribution ID
  $data['civicrm_contribution']['id'] = [
    'title' => t('Contribution ID'),
    'help' => t('The numeric ID of the Contribution'),
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
  //Contact ID for relationship to contact record (brings in Contact custom fields)
  $data['civicrm_contribution']['contact_id'] = [
    'title' => t('Contact Record ID'),
    'help' => 'Bring Contact Record the complete contact record to the party',
    'relationship' => [
      'base' => 'civicrm_contact',
      'left_field' => 'id',
      'field' => 'contact_id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Contact, with custom fields'),
    ],
  ];
  //Financial Type
  $data['civicrm_contribution']['financial_type'] = [
    'title' => t('Financial Type'),
    'real field' => 'financial_type_id',
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

  //Contribution Page
  $data['civicrm_contribution']['contribution_page'] = [
    'title' => t('Contribution Page'),
    'real field' => 'contribution_page_id',
    'help' => t('The Page this Contribution came from'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'contributionPage',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'contributionPage',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  //Payment Instrument Used
  $data['civicrm_contribution']['payment_instrument'] = [
    'title' => t('Payment Instrument'),
    'real field' => 'payment_instrument_id',
    'help' => t('The Payment Instrument Used for the Contribution'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'paymentInstrument',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'paymentInstrument',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];



  //Contribution Receive Date
  $data['civicrm_contribution']['receive_date'] = [
    'title' => t('Receive Date'),
    'help' => t('The Date the Contribution was received / paid'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];

  civicrm_views_add_date_arguments($data['civicrm_contribution'], [
    'title' => 'Receive Date',
      'name' => 'receive_date',
    ]);
  //Non Deductible Amount of Contribution
  $data['civicrm_contribution']['non_deductible_amount'] = [
    'title' => t('Non Deductible Amount'),
    'help' => t('The Non Deductible Part of the Contribution'),
    'field' => [
      'handler' => 'civicrm_handler_field_money',
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

  $data['civicrm_contribution']['total_amount'] = [
    'title' => t('Total Amount'),
    'help' => t('The Total Contribution made by the Contact'),
    'field' => [
      'handler' => 'civicrm_handler_field_money',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_numeric',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  $data['civicrm_contribution']['fee_amount'] = [
    'title' => t('Transaction Fee Amount'),
    'help' => t('The fee charged by the Payment Processor for the transaction.'),
    'field' => [
      'handler' => 'civicrm_handler_field_money',
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

  $data['civicrm_contribution']['net_amount'] = [
    'title' => t('Net Amount'),
    'help' => t('The Full Contribution minus Payment Processor Fees.'),
    'field' => [
      'handler' => 'civicrm_handler_field_money',
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

  //Transaction ID
  $data['civicrm_contribution']['trxn_id'] = [
    'title' => t('Transaction ID'),
    'help' => t('Unique Transaction ID from Payment Processor / Bank etc'),
    'field' => [
      'handler' => 'views_handler_field',
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

  //Invoice ID
  $data['civicrm_contribution']['invoice_id'] = [
    'title' => t('Invoice ID'),
    'help' => t('Unique Invoice ID from system'),
    'field' => [
      'handler' => 'views_handler_field',
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

  //Currency
  $data['civicrm_contribution']['currency'] = [
    'title' => t('Currency'),
    'help' => t('Currency used for the Contribution'),
    'field' => [
      'handler' => 'views_handler_field',
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

  //Contribution Cancellation Date
  $data['civicrm_contribution']['cancel_date'] = [
    'title' => t('Cancellation Date'),
    'help' => t('The Date the Contribution was cancelled (if it was)'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];
  civicrm_views_add_date_arguments($data['civicrm_contribution'], [
    'title' => 'Cancel Date',
      'name' => 'cancel_date',
    ]);

  //Cancellation Reason
  $data['civicrm_contribution']['cancel_reason'] = [
    'title' => t('Cancellation Reason'),
    'help' => t('The Reason for the Contribution Cancellation'),
    'field' => [
      'handler' => 'views_handler_field',
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

  //Contribution Receipt Date
  $data['civicrm_contribution']['receipt_date'] = [
    'title' => t('Receipt Date'),
    'help' => t('The Date a receipt for the contribution was sent.'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];
  civicrm_views_add_date_arguments($data['civicrm_contribution'], [
    'title' => 'Receipt Date',
      'name' => 'receipt_date',
    ]);


  //Receipt ID
  $data['civicrm_contribution']['receipt_id'] = [
    'title' => t('Receipt ID'),
    'help' => t('Unique Receipt ID from system'),
    'field' => [
      'handler' => 'views_handler_field',
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


  //Contribution Thank You Date
  $data['civicrm_contribution']['thankyou_date'] = [
    'title' => t('Thank You Date'),
    'help' => t('The Date a thank you to the donor for the contribution was sent.'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];
  civicrm_views_add_date_arguments($data['civicrm_contribution'], [
    'title' => 'Thank You Date',
      'name' => 'thankyou_date',
    ]);

  //Contribution Source
  $data['civicrm_contribution']['source'] = [
    'title' => t('Source'),
    'help' => t('Where the Contribution came through.'),
    'field' => [
      'handler' => 'views_handler_field',
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

  //Contribution Status
  $data['civicrm_contribution']['contribution_status'] = [
    'title' => t('Contribution Status'),
    'real field' => 'contribution_status_id',
    'help' => t('The Status of this Contribution'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'contributionStatus',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'contributionStatus',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  //TODO: CONTRIBUTION RECURRING STUFF + HONOR_CONTACT_ID + HONOR_TYPE_ID

  //BOOLEAN : IS A TEST CONTRIBUTION
  $data['civicrm_contribution']['is_test'] = [
    'title' => t('Is Test'),
    'help' => t('Is the contribution a test entry?'),
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

  //BOOLEAN : IS SET TO PAY LATER
  $data['civicrm_contribution']['is_pay_later'] = [
    'title' => t('Is Pay Later'),
    'help' => t('Opted to Pay Contribution Later, and is still pending payment.'),
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
  $data['civicrm_contribution']['check_number'] = [
    'title' => t('Check Number'),
    'help' => t('Contribution\'s Check Number'),
    'field' => [
      'handler' => 'views_handler_field',
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
  if (isset($enabled['CiviCampaign'])) {
    $data['civicrm_contribution']['campaign_id'] = [
      'title' => t('Campaign'),
      'help' => t('Contribution -> Campaign'),
      'relationship' => [
        'base' => 'civicrm_campaign',
        'field' => 'id',
        'handler' => 'views_handler_relationship',
        'label' => t('Contribution -> Campaign'),
      ],
    ];
  }


  /*
     * Soft Credits
     */


  $data['civicrm_contribution_soft']['table']['group'] = t('CiviCRM  Soft Credits');


  //TABLE JOINS FOR CIVICRM CONTRIBUTIONS GO HERE!
  $data['civicrm_contribution_soft']['table']['join']['civicrm_contribution'] = [
    'left_table' => 'civicrm_contribution',
    'left_field' => 'id',
    'field' => 'contribution_id',
  ];
  //CiviCRM Soft Credits - FIELDS

  //Numeric Soft Credit ID
  $data['civicrm_contribution_soft']['id'] = [
    'title' => t('Contribution Soft Credit ID'),
    'help' => t('The numeric ID of the Soft Credit'),
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

  //Numeric Contact ID of the Soft Credit recipient
  $data['civicrm_contribution_soft']['contact_id'] = [
    'title' => t('Contact ID'),
    'help' => t('Contact ID of the Soft Credit recipient'),
    'field' => [
      'handler' => 'views_handler_field_numeric',
    ],
    'argument' => [
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ],
    'filter' => [
      'handler' => 'views_handler_filter_numeric',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'relationship' => [
      'base' => 'civicrm_contact',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('Soft Contribution -> Contact Record'),
    ],
  ];
  //Soft Credit Amount
  $data['civicrm_contribution_soft']['amount'] = [
    'title' => t('Amount'),
    'help' => t('The amount of the soft credit'),
    'field' => [
      'handler' => 'civicrm_handler_field_money',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  //Personal Campaign Page
  $data['civicrm_contribution_soft']['pcp_roll_nickname'] = [
    'title' => t('Personal Campaign Page Nickname'),
    'help' => t('The Page this Contribution came from'),
    'field' => [
      'handler' => 'views_handler_field',
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
  //Personal Campaign Page Personla Note
  $data['civicrm_contribution_soft']['pcp_personal_note'] = [
    'title' => t('Personal Campaign Page Personal Note'),
    'help' => t('The Personal note form PCP'),
    'field' => [
      'handler' => 'views_handler_field',
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


  /*
     * Soft Credits
     */


  $data['civicrm_contribution_recur']['table']['group'] = t('CiviCRM  Recurring Contributions');


  //TABLE JOINS FOR CIVICRM CONTRIBUTIONS GO HERE!
  $data['civicrm_contribution_recur']['table']['join']['civicrm_contribution'] = [
    'left_table' => 'civicrm_contribution',
    'left_field' => 'contribution_recur_id',
    'field' => 'id',
  ];

  //CiviCRM Soft Credits - FIELDS

  //Numeric Recurring Contribution ID
  $data['civicrm_contribution_recur']['id'] = [
    'title' => t('Recurring Contribution ID'),
    'help' => t('The numeric ID of the  Recurring Contribution'),
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

  $data['civicrm_contribution_recur']['contact_id'] = [
    'title' => t('Contact ID'),
    'help' => t('The contact who is paying the recurring pledge'),
    'field' => [
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ],
    'relationship' => [
      'base' => 'civicrm_contact',
      'left_field' => 'id',
      'field' => 'contact_id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Recurring payment Contact'),
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

  //Recurring Contribution Amount
  $data['civicrm_contribution_recur']['amount'] = [
    'title' => t('Amount'),
    'help' => t('The amount of the recurring contribution'),
    'field' => [
      'handler' => 'civicrm_handler_field_money',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  //Frequency of recurring contribution
  $data['civicrm_contribution_recur']['frequency_unit'] = [
    'title' => t('Frequency Unit'),
    'help' => t('Day, Week, Month, Year'),
    'field' => [
      'handler' => 'views_handler_field',
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
  //Frequency Interval
  $data['civicrm_contribution_recur']['frequency_interval'] = [
    'title' => t('Frequency Interval'),
    'help' => t('The Interval of the Frequency Unit'),
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
  //# of Installments
  $data['civicrm_contribution_recur']['installments'] = [
    'title' => t('Installments'),
    'help' => t('Number of Installments'),
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
  //Recurring Contribution Start Date
  $data['civicrm_contribution_recur']['start_date'] = [
    'title' => t('Start Date'),
    'help' => t('The Date the recurring contribution started.'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];
  civicrm_views_add_date_arguments($data['civicrm_contribution_recur'], [
    'title' => 'Recurring Contribution Start Date',
      'name' => 'start_date',
    ]);
  //Recurring Contribution Create Date
  $data['civicrm_contribution_recur']['create_date'] = [
    'title' => t('Create Date'),
    'help' => t('The Date the recurring contribution was created.'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];
  civicrm_views_add_date_arguments($data['civicrm_contribution_recur'], [
    'title' => 'Recurring Contribution Create Date',
      'name' => 'create_date',
    ]);
  //Recurring Contribution Modified Date
  $data['civicrm_contribution_recur']['modified_date'] = [
    'title' => t('Modified Date'),
    'help' => t('The Date the recurring contribution was modified.'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];
  civicrm_views_add_date_arguments($data['civicrm_contribution_recur'], [
    'title' => 'Recurring Contribution Modified Date',
      'name' => 'modified_date',
    ]);
  //Recurring Contribution Cancel Date
  $data['civicrm_contribution_recur']['cancel_date'] = [
    'title' => t('Cancel Date'),
    'help' => t('The Date the recurring contribution was cancelled.'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];
  civicrm_views_add_date_arguments($data['civicrm_contribution_recur'], [
    'title' => 'Recurring Contribution Cancel Date',
      'name' => 'cancel_date',
    ]);
  //Recurring Contribution End Date
  $data['civicrm_contribution_recur']['end_date'] = [
    'title' => t('End Date'),
    'help' => t('The Date the recurring contribution ended.'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];
  civicrm_views_add_date_arguments($data['civicrm_contribution_recur'], [
    'title' => 'Recurring Contribution End Date',
      'name' => 'end_date',
    ]);
  //Unique Transaction ID
  $data['civicrm_contribution_recur']['trxn_id'] = [
    'title' => t('Unique Transaction ID'),
    'help' => t('The Transaction ID of the  Recurring Contribution'),
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
  //Invoice ID
  $data['civicrm_contribution_recur']['invoice_id'] = [
    'title' => t('Invoice ID'),
    'help' => t('The Unique CiviCRM Generated Invoice ID'),
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
  //recurring contributions Status
  $data['civicrm_contribution_recur']['contribution_status'] = [
    'title' => t('Contribution Status'),
    'real field' => 'contribution_status_id',
    'help' => t('The Contribution\'s Status'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'contributionStatus',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'contributionStatus',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  //BOOLEAN : IS A TEST CONTRIBUTION
  $data['civicrm_contribution_recur']['is_test'] = [
    'title' => t('Is Test'),
    'help' => t('Is the recurring contribution a test entry?'),
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
  //Cycle contibution Day
  $data['civicrm_contribution_recur']['cycle_day'] = [
    'title' => t('Cycle Day'),
    'help' => t('What day in the contribution will the debit occur'),
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
  //Recurring Contribution End Date
  $data['civicrm_contribution_recur']['next_sched_contribution'] = [
    'title' => t('Next Contribution Date'),
    'help' => t('The Next Contribution Date.'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];
  civicrm_views_add_date_arguments($data['civicrm_contribution_recur'], [
    'title' => 'Recurring Contribution Next Date',
      'name' => 'next_sched_contribution',
    ]);
  //How many times has the contribution failed
  $data['civicrm_contribution_recur']['failure_count'] = [
    'title' => t('Failure Count'),
    'help' => t('How many times has theis recurring contribution failed?'),
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
  //when will the filed contribution be retried?
  $data['civicrm_contribution_recur']['failure_retry_date'] = [
    'title' => t('Failed retry date'),
    'help' => t('When will the failed contribution be retried?'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];
  civicrm_views_add_date_arguments($data['civicrm_contribution_recur'], [
    'title' => 'Recurring Contribution Failure Retry Date',
      'name' => 'failure_retry_date',
    ]);
  //BOOLEAN : Does thie recurring contribution auto-renew?
  $data['civicrm_contribution_recur']['auto_renew'] = [
    'title' => t('Auto-renew'),
    'help' => t('Will this recurring contribution automayically renew?'),
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
  $data['civicrm_pledge']['table']['base'] = [
    'field' => 'id',
    'title' => t('CiviCRM Pledges'),
    'help' => t("CiviCRM Pledges."),
  ];
  $data['civicrm_pledge']['table']['group'] = t('CiviCRM Pledges');
  $data['civicrm_pledge_payment']['table']['group'] = t('Pledge Payments');
  $data['civicrm_pledge']['id'] = [
    'title' => t('CiviCRM Pledge ID'),
    'help' => t('The numeric ID of the Pledge'),
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
  $data['civicrm_pledge']['amount'] = [
    'title' => t('Amount'),
    'help' => t('The total amount of the pledge'),
    'field' => [
      'handler' => 'civicrm_handler_field_money',
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
  //------------------------------------------------------
  // CiviCRM Personal Campaign Pages tables
  //------------------------------------------------------

  $data['civicrm_pcp']['table']['group'] = t('CiviCRM Personal Campaign Pages');

  $data['civicrm_pcp']['table']['base'] = [
    'field' => 'id',
    'title' => t('CiviCRM Personal Campaign Pages'),
    'help' => t("View displays CiviCRM Personal Campaign Pages, with connection to contacts."),
  ];

  $data['civicrm_pcp']['table']['join'] = [
    // Makes PCP fields available to Contact views.
    'civicrm_contact' => [
      'left_field' => 'id',
      'field' => 'contact_id',
    ],
  ];

  // PCP FIELDS

  // Numeric PCP ID
  $data['civicrm_pcp']['id'] = [
    'title' => t('PCP ID'),
    'help' => t('The numeric ID of the Personal Campaign Page.'),
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
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  // PCP Text Link
  $data['civicrm_pcp']['url'] = [
    'title' => t('URL'),
    'help' => t('Generates URLs for various PCP actions.'),
    'field' => [
      'handler' => 'civicrm_handler_field_link_pcp',
    ],
  ];

  // PCP status
  $data['civicrm_pcp']['status'] = [
    'title' => t('Page Status'),
    'real field' => 'status_id',
    'help' => t('The Status of this Personal Contribution Page'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'pcpStatus',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'pcpStatus',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  // PCP title
  $data['civicrm_pcp']['title'] = [
    'title' => t('Title'),
    'help' => t('The title of this Personal Contribution Page'),
    'field' => [
      'handler' => 'views_handler_field',
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

  // introductory text
  $data['civicrm_pcp']['intro_text'] = [
    'title' => t('Intro text'),
    'help' => t('Introductory welcome text'),
    'field' => [
      'handler' => 'views_handler_field',
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

  // page text
  $data['civicrm_pcp']['page_text'] = [
    'title' => t('Page text'),
    'help' => t('Page text (your message)'),
    'field' => [
      'handler' => 'views_handler_field',
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

  // text for donate link
  $data['civicrm_pcp']['donate_link_text'] = [
    'title' => t('Text for donate link'),
    'help' => t('Text for donate link'),
    'field' => [
      'handler' => 'views_handler_field',
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

  // contribution page
  $data['civicrm_pcp']['contribution_page'] = [
    'title' => t('Associated Contribution or Event Page'),
    'real field' => 'contribution_page_id',
    'help' => t('The Contribution or Event Page with which this Personal Campaign Page is associated.'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'contributionPage',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
      'numeric' => TRUE,
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'contributionPage',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  //BOOLEAN : IS Thermometer
  $data['civicrm_pcp']['is_thermometer'] = [
    'title' => t('Is Thermometer'),
    'help' => t('Does the page use a progress indicator?'),
    'field' => [
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'views_handler_filter_numeric',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  //BOOLEAN : Is Honor Roll
  $data['civicrm_pcp']['is_honor_roll'] = [
    'title' => t('Is Honor Roll'),
    'help' => t('Does the page use an Honor Roll?'),
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

  // fundraising goal
  $data['civicrm_pcp']['goal_amount'] = [
    'title' => t('Goal Amount'),
    'help' => t('Goal Amount.'),
    'field' => [
      'handler' => 'civicrm_handler_field_money',
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

  // amount raised
  $data['civicrm_pcp']['raised_amount'] = [
    'title' => t('Amount Raised'),
    'help' => t('Amount raised through this Personal Campaign Page'),
    'real field' => 'id',
    'field' => [
      'handler' => 'civicrm_handler_field_pcp_raised_amount',
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
      'handler' => 'civicrm_handler_sort_pcp_raised_amount',
    ],
  ];

  // referer (not really sure what this is, but...)
  $data['civicrm_pcp']['referer'] = [
    'title' => t('Referer'),
    'help' => t('Referer'),
    'field' => [
      'handler' => 'views_handler_field',
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

  //BOOLEAN : Is Active
  $data['civicrm_pcp']['is_active'] = [
    'title' => t('Is Active'),
    'help' => t('Is this page active?'),
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

  // currency
  $data['civicrm_pcp']['currency'] = [
    'title' => t('Currency'),
    'help' => t('Currency'),
    'field' => [
      'handler' => 'views_handler_field',
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

  $data['civicrm_pcp']['image_url'] = [
    'title' => t('Image URL'),
    'real field' => 'id',
    'help' => t('URL for the image the user uploaded when creating this page'),
    'field' => [
      'handler' => 'civicrm_handler_field_file',
    ],
  ];

  //Contribution Text Link
  $data['civicrm_contribution']['text_link'] = [
    'field' => [
      'title' => t('Text Link'),
      'help' => t('Creates a Text Link for various contribution actions.'),
      'handler' => 'civicrm_handler_field_link_contribution',
    ],
  ];
}

