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
 * Build the $data array for CiviMail related tables
 * Includes the following tables
 * civicrm_mailing
 * civicrm_mailing_event_queue
 * civicrm_mailing_header
 * civicrm_mailing_job
 * civicrm_mailing_event_forward
 * civicrm_mailing_event_trackable_url_open
 * civicrm_mailing_trackable_url
 * civicrm_mailing_event_bounce
 * civicrm_mailing_event_delivered
 * civicrm_mailing_event_opened
 * civicrm_mailing_event_reply
 * civicrm_mailing_event_unsubscribe
 */
function _civicrm_mail_data(&$data, $enabled) {
  /*
     * CivicRM Mailings tables
     */

  $data['civicrm_mailing']['table']['group'] = t('CiviCRM Mailings');

  $data['civicrm_mailing']['table']['base'] = [
    // Governs the whole mozilla
    'field' => 'id',
    'title' => t('CiviCRM Mailings'),
    'help' => t("View displays CiviCRM Mailing Results."),
  ];

  $data['civicrm_mailing_event_queue']['table']['join'] = [
    // Directly links to contact table - link A
    'civicrm_mailing' => [
      'left_table' => 'civicrm_mailing_job',
      'left_field' => 'id',
      'field' => 'job_id',
    ],
  ];



  //CiviCRM Mailing - FIELDS
  // expose the ID to Views to allow users to drill down to a specific mailing.
  $data['civicrm_mailing']['id'] = [
    'title' => t('ID'),
    'help' => t('The mailing\'s ID in the database.'),
    'argument' => [
      'handler' => 'views_handler_argument_numeric',
    ],
    'field' => [
      'handler' => 'views_handler_field',
    ],
  ];

  //Mailing Name
  $data['civicrm_mailing']['name'] = [
    'title' => t('Name'),
    'help' => t('Title of the mailing'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Mailing_PseudoConstant',
      'pseudo method' => 'completed',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Mailing_PseudoConstant',
      'pseudo method' => 'completed',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  //Mailing Subject line
  $data['civicrm_mailing']['subject'] = [
    'title' => t('Subject Line'),
    'help' => t('Subject Line of the mailing'),
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

  //Mailing Body plaintext
  $data['civicrm_mailing']['body_text'] = [
    'title' => t('Body - Text'),
    'help' => t('Plaintext mailing message'),
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

  //Mailing Body HTML
  $data['civicrm_mailing']['body_html'] = [
    'title' => t('Body - HTML'),
    'help' => t('HTML mailing message'),
    'field' => [
      'handler' => 'civicrm_handler_field_markup',
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

  //Mailing From e-mail
  $data['civicrm_mailing']['from_email'] = [
    'title' => t('From Email'),
    'help' => t('From Email of the mailing'),
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
  //Mailing reply to e-mail
  $data['civicrm_mailing']['replyto_email'] = [
    'title' => t('Reply to Email'),
    'help' => t('Reply Email of the mailing'),
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
  //Does the mailng track click throughs
  $data['civicrm_mailing']['url_tracking'] = [
    'title' => t('Track URLs?'),
    'help' => t('Does the mailing track click throughs?'),
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
  //Does the e-mail track opens
  $data['civicrm_mailing']['open_tracking'] = [
    'title' => t('Track Opens?'),
    'help' => t('Does the mailing track Open Rate?'),
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
  //has mailing been sent
  $data['civicrm_mailing']['is_completed'] = [
    'title' => t('Complete?'),
    'help' => t('Is the mailing completely sent?'),
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
  //is this mailing public
  $data['civicrm_mailing']['visibility'] = [
    'title' => t('Visibility'),
    'help' => t('Is this mailing public or not?'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_visibility',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  // Link to Campaign Table
  if (isset($enabled['CiviCampaign'])) {
    $data['civicrm_mailing']['campaign_id'] = [
      'title' => t('Campaign'),
      'help' => t('Mailing -> Campaign'),
      'relationship' => [
        'base' => 'civicrm_campaign',
        'field' => 'id',
        'handler' => 'views_handler_relationship',
        'label' => t('Mailing -> Campaign'),
      ],
    ];
  }

  // mailing_component table aliased as mailing_header
  $data['civicrm_mailing_header']['table']['group'] = ts('CiviCRM Mailings');

  $data['civicrm_mailing_header']['table']['join'] = [
    // link to civicrm_mailing
    'civicrm_mailing' => [
      'left_field' => 'header_id',
      'table' => 'civicrm_mailing_component',
      'field' => 'id',
    ],
  ];

  $data['civicrm_mailing_header']['body_html'] = [
    'title' => t('Mailing Header - HTML'),
    'help' => t('The HTML header for this mailing.'),
    'field' => [
      'handler' => 'civicrm_handler_field_markup',
      'click sortable' => FALSE,
    ],
  ];

  $data['civicrm_mailing_header']['body_text'] = [
    'title' => t('Mailing Header - Plaintext'),
    'help' => t('The plaintext header for this mailing.'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
    ],
  ];


  // mailing_component table aliased as mailing_footer
  $data['civicrm_mailing_footer']['table']['group'] = ts('CiviCRM Mailings');

  $data['civicrm_mailing_footer']['table']['join'] = [
    // link to civicrm_mailing
    'civicrm_mailing' => [
      'left_field' => 'footer_id',
      'table' => 'civicrm_mailing_component',
      'field' => 'id',
    ],
  ];

  $data['civicrm_mailing_footer']['body_html'] = [
    'title' => t('Mailing Footer - HTML'),
    'help' => t('The HTML footer for this mailing.'),
    'field' => [
      'handler' => 'civicrm_handler_field_markup',
      'click sortable' => FALSE,
    ],
  ];

  $data['civicrm_mailing_footer']['body_text'] = [
    'title' => t('Mailing Footer - Plaintext'),
    'help' => t('The plaintext footer for this mailing.'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
    ],
  ];


  //Mailing Job table
  $data['civicrm_mailing_job']['table']['group'] = ts('CiviCRM Mailing Job');

  $data['civicrm_mailing_job']['table']['join'] = [
    // link to civicrm_mailing
    'civicrm_mailing' => [
      'left_field' => 'id',
      'field' => 'mailing_id',
    ],
  ];

  //When is the mailing sceduled to go out
  $data['civicrm_mailing_job']['scheduled_date'] = [
    'title' => t('Scheduled Date'),
    'help' => t('The Mailing\'s Scheduled Date'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument_date',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];

  civicrm_views_add_date_arguments($data['civicrm_mailing_job'], [
    'title' => 'Scheduled Date',
      'name' => 'scheduled_date',
    ]);
  //When did the mailing start to send
  $data['civicrm_mailing_job']['start_date'] = [
    'title' => t('Start Date'),
    'help' => t('The Mailing\'s Start Date'),
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

  civicrm_views_add_date_arguments($data['civicrm_mailing_job'], [
    'title' => 'Start Date',
      'name' => 'start_date',
    ]);
  //Mailing finish date
  $data['civicrm_mailing_job']['end_date'] = [
    'title' => t('End Date'),
    'help' => t('The Mailing\'s End Date'),
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

  civicrm_views_add_date_arguments($data['civicrm_mailing_job'], [
    'title' => 'End Date',
      'name' => 'end_date',
    ]);


  //Is this a test mailing
  $data['civicrm_mailing_job']['is_test'] = [
    'title' => t('Is Test?'),
    'help' => t('Is this the test mailing?'),
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
  //Mailing table that tracks who forwarded the e-mail
  $data['civicrm_mailing_event_forward']['table']['group'] = ts('CiviCRM Mailing Forwards');

  $data['civicrm_mailing_event_forward']['table']['join'] = [
    // links through civicrm_mail_event_queue
    'civicrm_mailing' => [
      'left_table' => 'civicrm_mailing_event_queue',
      'left_field' => 'id',
      'field' => 'event_queue_id',
    ],
  ];
  //time stamp of eash individual forward
  $data['civicrm_mailing_event_forward']['time_stamp'] = [
    'title' => t('Date forwaded'),
    'help' => t('Date user forwarded the e-mail'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument_date',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];
  //Tracks who click through, traces each individual click through
  $data['civicrm_mailing_event_trackable_url_open']['table']['group'] = ts('CiviCRM Mailing Click Throughs');

  $data['civicrm_mailing_event_trackable_url_open']['table']['join'] = [
    // link to civicrm_mailing through civicrm_mailing_event_queue
    'civicrm_mailing' => [
      'left_table' => 'civicrm_mailing_event_queue',
      'left_field' => 'id',
      'field' => 'event_queue_id',
    ],
  ];
  //timestamp of click through
  $data['civicrm_mailing_event_trackable_url_open']['time_stamp'] = [
    'title' => t('Date of click through'),
    'help' => t('Date user clicked a link in the e-mail'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument_date',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];
  //ID of click through event
  $data['civicrm_mailing_event_trackable_url_open']['id'] = [
    'title' => t('Mailing click through ID'),
    'help' => t('The numeric ID click through record'),
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
  //Track the click through's URL
  $data['civicrm_mailing_trackable_url']['table']['group'] = ts('CiviCRM Mailing Click Throughs URL');

  $data['civicrm_mailing_trackable_url']['table']['join'] = [
    // link to civicrm_mailing through civicrm_mailing_event_queue
    'civicrm_mailing' => [
      'left_table' => 'civicrm_mailing_event_trackable_url_open',
      'left_field' => 'trackable_url_id',
      'field' => 'id',
    ],
  ];
  $data['civicrm_mailing_trackable_url']['url'] = [
    'title' => t('URL'),
    'help' => t('What URL was clicked'),
    'field' => [
      'handler' => 'views_handler_field_url',
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



  $data['civicrm_mailing_event_bounce']['table']['group'] = ts('CiviCRM Mailing Bounced');

  $data['civicrm_mailing_event_bounce']['table']['join'] = [
    // link to civicrm_mailing through civicrm_mailing_event_queue
    'civicrm_mailing' => [
      'left_table' => 'civicrm_mailing_event_queue',
      'left_field' => 'id',
      'field' => 'event_queue_id',
    ],
  ];

  $data['civicrm_mailing_event_bounce']['time_stamp'] = [
    'title' => t('Date of bounce'),
    'help' => t('Date user e-mail bounced'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument_date',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];
  $data['civicrm_mailing_event_bounce']['id'] = [
    'title' => t('Mailing bounced ID'),
    'help' => t('The numeric ID of the bounce record'),
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

  $data['civicrm_mailing_event_bounce']['bounce_reason'] = [
    'title' => t('Reason'),
    'help' => t('Reason the e-mail bounced'),
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

  $data['civicrm_mailing_event_delivered']['table']['group'] = ts('CiviCRM Mailing  Delivered');

  $data['civicrm_mailing_event_delivered']['table']['join'] = [
    // link to civicrm_mailing through civicrm_mailing_event_queue
    'civicrm_mailing' => [
      'left_table' => 'civicrm_mailing_event_queue',
      'left_field' => 'id',
      'field' => 'event_queue_id',
    ],
  ];

  $data['civicrm_mailing_event_delivered']['time_stamp'] = [
    'title' => t('Date of delivery'),
    'help' => t('Date user e-mail delivered'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument_date',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];
  $data['civicrm_mailing_event_delivered']['id'] = [
    'title' => t('Mailing delivered ID'),
    'help' => t('The numeric ID of the delivery record'),
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


  $data['civicrm_mailing_event_opened']['table']['group'] = ts('CiviCRM Mailing Opened');

  $data['civicrm_mailing_event_opened']['table']['join'] = [
    // link to civicrm_mailing through civicrm_mailing_event_queue
    'civicrm_mailing' => [
      'left_table' => 'civicrm_mailing_event_queue',
      'left_field' => 'id',
      'field' => 'event_queue_id',
    ],
  ];

  $data['civicrm_mailing_event_opened']['time_stamp'] = [
    'title' => t('Date e-mail opened'),
    'help' => t('Date e-mail opened'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument_date',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];
  $data['civicrm_mailing_event_opened']['id'] = [
    'title' => t('Mailing opened ID'),
    'help' => t('The numeric ID of the opened record'),
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


  $data['civicrm_mailing_event_reply']['table']['group'] = ts('CiviCRM Mailing Replied');

  $data['civicrm_mailing_event_reply']['table']['join'] = [
    // link to civicrm_mailing through civicrm_mailing_event_queue
    'civicrm_mailing' => [
      'left_table' => 'civicrm_mailing_event_queue',
      'left_field' => 'id',
      'field' => 'event_queue_id',
    ],
  ];

  $data['civicrm_mailing_event_reply']['time_stamp'] = [
    'title' => t('Date of reply'),
    'help' => t('Date user replied'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument_date',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];
  $data['civicrm_mailing_event_reply']['id'] = [
    'title' => t('Mailing reply ID'),
    'help' => t('The numeric ID of the reply record'),
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


  $data['civicrm_mailing_event_unsubscribe']['table']['group'] = ts('CiviCRM Mailing Unsubscribe');

  $data['civicrm_mailing_event_unsubcribe']['table']['join'] = [
    // link to civicrm_mailing through civicrm_mailing_event_queue
    'civicrm_mailing' => [
      'left_table' => 'civicrm_mailing_event_queue',
      'left_field' => 'id',
      'field' => 'event_queue_id',
    ],
  ];

  $data['civicrm_mailing_event_unsubscribe']['time_stamp'] = [
    'title' => t('Date of unsubscribe'),
    'help' => t('Date user unsubscribed'),
    'field' => [
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument_date',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_datetime',
    ],
    'sort' => [
      'handler' => 'civicrm_handler_sort_date',
    ],
  ];

  $data['civicrm_mailing_event_unsubscribe']['id'] = [
    'title' => t('Mailing unsubscribeded ID'),
    'help' => t('The numeric ID of the unsubscribe record'),
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

