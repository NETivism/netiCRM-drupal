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
 * Build the $data array for all the filed that are part of any CivicRM install
 * Includes the following tables
 * civicrm_contact
 * civicrm_address
 * civicrm_email
 * civicrm_website
 * civicrm_phone
 * civicrm_group
 * civicrm_tag
 * civicrm_relationship
 * civicrm_activity
 * civicrm_uf_match
 * All Custom fields
 */
function _civicrm_core_data(&$data, $enabled) {
  //------------------------------------------------------
  // CiviCRM Contacts Views Table
  //------------------------------------------------------

  $data['civicrm_contact']['table']['group'] = t('CiviCRM Contacts');

  $data['civicrm_contact']['table']['base'] = [
    // Governs the whole mozilla
    'field' => 'id',
    'title' => t('CiviCRM Contacts'),
    'help' => t("View displays CiviCRM Contacts, of people, organizations, etc."),
  ];

  //TABLE JOINS FOR CIVICRM CONTACTS GO HERE!

  $data['civicrm_contact']['table']['join'] = [
    // Directly links to participants.
    'civicrm_participant' => [
      'left_field' => 'contact_id',
      'field' => 'id',
    ],
    // Directly links to contribution
    'civicrm_contribution' => [
      'left_field' => 'contact_id',
      'field' => 'id',
    ],
    // Directly links to activity.
    'civicrm_activity' => [
      'left_field' => 'source_contact_id',
      'field' => 'id',
    ],
    // Directly links to membership.
    'civicrm_membership' => [
      'left_field' => 'contact_id',
      'field' => 'id',
    ],
    // Link to mailing.
    'civicrm_mailing' => [
      'left_table' => 'civicrm_mailing_event_queue',
      'left_field' => 'contact_id',
      'field' => 'id',
    ],
    // Link to grant.
    'civicrm_grant' => [
      'left_field' => 'contact_id',
      'field' => 'id',
    ],
    // makes contact fields available to PCP views.
    'civicrm_pcp' => [
      'left_field' => 'contact_id',
      'field' => 'id',
    ],
  ];

  // For other base tables, explain how we join -

  $data['civicrm_contact']['table']['join']['users'] = [
    'left_table' => 'civicrm_uf_match',
    'left_field' => 'contact_id',
    'field' => 'id',
  ];
  $data['civicrm_uf_match']['table']['join']['users'] = [
    'left_field' => 'uid',
    'field' => 'uf_id',
  ];

  //CiviCRM Contacts - FIELDS

  //Numeric Contact ID
  $data['civicrm_contact']['id'] = [
    'title' => t('Contact ID'),
    'help' => t('The numeric ID of the Contact'),
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
    'relationship' => [
      'base' => 'civicrm_contribution',
      'title' => 'Contribution Records',
      'left_field' => 'contact_id',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Contributions'),
    ],
  ];

  // Drupal ID of the contact (from uf_match.uf_id)
  $data['civicrm_contact']['drupal_id'] = [
    'real field' => 'id',
    'title' => t('Drupal ID'),
    'help' => t('Relates a CiviCRM Contact to the Druapl User Record'),
    'field' => [
      'handler' => 'civicrm_handler_field_drupalid',
      'click sortable' => TRUE,
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'relationship' => [
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'civicrm_handler_relationship_contact2users',
      'label' => t('Drupal User'),
    ],
  ];

  //SORT Name for the Contact (Last Name followed by First)
  $data['civicrm_contact']['sort_name'] = [
    'title' => t('Sort Name'),
    'help' => t('The Contact Name for sorting purposes, Last Name then First'),
    'field' => [
      'handler' => 'civicrm_handler_field_contact_link',
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

  //DISPLAY Name for the Contact (Full Name with Prefixes and Suffixes)
  $data['civicrm_contact']['display_name'] = [
    'title' => t('Display Name'),
    'help' => t('Full Name of the Contact with prefixes and suffixes'),
    'field' => [
      'handler' => 'civicrm_handler_field_contact_link',
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

  //Gender
  $data['civicrm_contact']['gender'] = [
    'title' => t('Gender'),
    'real field' => 'gender_id',
    'help' => t('Contact\'s Gender'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'gender',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'gender',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  // Prefix
  $data['civicrm_contact']['prefix_id'] = [
    // The item it appears as on the UI,
    'title' => t('Prefix'),
    // The help that appears on the UI,
    'help' => t('Prefix'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'individualPrefix',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'individualPrefix',
    ],
  ];
  // Suffix
  $data['civicrm_contact']['suffix_id'] = [
    // The item it appears as on the UI,
    'title' => t('Suffix'),
    // The help that appears on the UI,
    'help' => t('Suffix'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'individualSuffix',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'individualSuffix',
    ],
  ];

  //FIRST NAME
  $data['civicrm_contact']['first_name'] = [
    'title' => t('First Name'),
    'help' => t('First Name'),
    'field' => [
      'handler' => 'civicrm_handler_field_contact_link',
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

  //MIDDLE NAME
  $data['civicrm_contact']['middle_name'] = [
    'title' => t('Middle Name'),
    'help' => t('Middle Name'),
    'field' => [
      'handler' => 'civicrm_handler_field_contact_link',
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

  //LAST NAME
  $data['civicrm_contact']['last_name'] = [
    'title' => t('Last Name'),
    'help' => t('Last Name'),
    'field' => [
      'handler' => 'civicrm_handler_field_contact_link',
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

  //BOOLEAN : IS Deceased
  $data['civicrm_contact']['is_deceased'] = [
    'title' => t('Is Deceased'),
    'help' => t('Is the contact deceased?'),
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

  //THUMBSUP: Contact Permissions ie Do not email, do not this, do not that.

  //BOOLEAN : DO NOT EMAIL
  $data['civicrm_contact']['do_not_email'] = [
    'title' => t('Do Not E-mail'),
    'help' => t('Does this contact not want to be emailed?'),
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

  //BOOLEAN : DO NOT PHONE
  $data['civicrm_contact']['do_not_phone'] = [
    'title' => t('Do Not Phone'),
    'help' => t('Does this contact not want to be phoned?'),
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

  //BOOLEAN : DO NOT MAIL
  $data['civicrm_contact']['do_not_mail'] = [
    'title' => t('Do Not Mail'),
    'help' => t('Does this contact not want to be mailed?'),
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

  //BOOLEAN : DO NOT SMS
  $data['civicrm_contact']['do_not_sms'] = [
    'title' => t('Do Not Sms'),
    'help' => t('Does this contact not want to receive messages?'),
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

  //BOOLEAN : DO NOT TRADE
  $data['civicrm_contact']['do_not_trade'] = [
    'title' => t('Do Not Trade'),
    'help' => t('Does this contact not want trade?'),
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

  //BOOLEAN : Opted out of Bulk Mailing
  $data['civicrm_contact']['is_opt_out'] = [
    'title' => t('Opted Out of Bulk Mail'),
    'help' => t('Has this contact opted out of bulk mail?'),
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

  //BOOLEAN : Is Deleted
  $data['civicrm_contact']['is_deleted'] = [
    'title' => t('Is Deleted'),
    'help' => t('Has this contact been deleted?'),
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

  // Organization Name
  $data['civicrm_contact']['current_employer'] = [
    // The item it appears as on the UI,
    'title' => t('Organization Name'),
    'real field' => 'organization_name',
    // The help that appears on the UI,
    'help' => t('The display name of the contact\'s organizations.'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'argument' => [
      'handler' => 'views_handler_argument_string',
    ],
  ];

  // contact_type
  $data['civicrm_contact']['contact_type'] = [
    // The item it appears as on the UI,
    'title' => t('Contact Type'),
    // The help that appears on the UI,
    'help' => t('Contact Type'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Contact_BAO_ContactType',
      'pseudo method' => 'basicTypePairs',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'pseudo class' => 'CRM_Contact_BAO_ContactType',
      'pseudo method' => 'basicTypePairs',
    ],
  ];

  //Contact Sub Type
  $data['civicrm_contact']['contact_sub_type'] = [
    'title' => t('Contact Sub Type'),
    'help' => t('The Sub-type for the Contact'),
    'field' => [
      'handler' => 'civicrm_handler_field',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Contact_BAO_ContactType',
      'pseudo method' => 'subTypePairs',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'pseudo class' => 'CRM_Contact_BAO_ContactType',
      'pseudo method' => 'subTypePairs',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  //Birth Date
  $data['civicrm_contact']['birth_date'] = [
    'title' => t('Birth Date'),
    'help' => t('The contact\'s birth date.'),
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

  // add the other generic fields
  $objType = 'Individual';
  $fields = CRM_Contact_BAO_Contact::exportableFields($objType);

  $skipFields = [
    'contact_is_deleted' => 1,
    'current_employer_id' => 1,
  ];
  civicrm_views_add_fields($fields, $data['civicrm_contact'], 'civicrm_contact', $skipFields);

  // CHILLING OUT AT THE CIVICRM_ADDRESS TABLE

  $data['civicrm_address']['table']['group'] = t('CiviCRM Address');

  // Explain how this table joins to others.
  $data['civicrm_address']['table']['join'] = [
    // Directly links to contact table.
    'civicrm_contact' => [
      'left_field' => 'id',
      'field' => 'contact_id',
    ],
  ];

  //CiviCRM Address Using Contacts as a pitstop before going on to Participants
  $data['civicrm_address']['table']['join']['civicrm_participant'] = [
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  //CiviCRM Address Using Contacts as a pitstop before going on to Contributions
  $data['civicrm_address']['table']['join']['civicrm_contribution'] = [
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  //CiviCRM Address Using Contacts as a pitstop before going on to Activities
  $data['civicrm_address']['table']['join']['civicrm_activity'] = [
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  //CiviCRM Address Using Contacts as a pitstop before going on to PCP (makes address fields available to PCP views)
  $data['civicrm_address']['table']['join']['civicrm_pcp'] = [
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  //CiviCRM Address Using Locations as a pitstop before going on to Events
  $data['civicrm_address']['table']['join']['civicrm_event'] = [
    'left_table' => 'civicrm_loc_block',
    'left_field' => 'address_id',
    'field' => 'id',
  ];
  //CiviCRM Address Using Contacts as a pitstop before going on to Memberships
  $data['civicrm_address']['table']['join']['civicrm_membership'] = [
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  $data['civicrm_address']['table']['join']['users'] = [
    'left_table' => 'civicrm_uf_match',
    'left_field' => 'contact_id',
    'field' => 'contact_id',
  ];

  //BOOLEAN : IS PRIMARY ADDRESS
  $data['civicrm_address']['is_primary'] = [
    'title' => t('Is Primary Address'),
    'help' => t('Is this address the contact\'s primary address?'),
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

  //BOOLEAN : IS BILLING ADDRESS
  $data['civicrm_address']['is_billing'] = [
    'title' => t('Is Billing Address'),
    'help' => t('Is this address the contact\'s billing address?'),
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

  //ADDRESS'S LOCATION TYPE
  $data['civicrm_address']['location_type'] = [
    'title' => t('Location Type'),
    'real field' => 'location_type_id',
    'help' => t('The Location of the Address for the Contact'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'locationType',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'locationType',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  //FULL STREET ADDRESS
  $data['civicrm_address']['street_address'] = [
    'title' => t('Full Street Address'),
    'help' => t('Full Street Address without Country, State or Postcode'),
    'field' => [
      'handler' => 'civicrm_handler_field_address',
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

  //FULL STREET ADDRESS SUPPLEMENTAL 1
  $data['civicrm_address']['supplemental_address_1'] = [
    'title' => t('Supplemental Street Address'),
    'help' => t('Supplemental Street Address such as Apartment or Suite Number or Department'),
    'field' => [
      'handler' => 'civicrm_handler_field_address',
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

  //FULL STREET ADDRESS SUPPLEMENTAL 2
  $data['civicrm_address']['supplemental_address_2'] = [
    'title' => t('2nd Supplemental Street Address'),
    'help' => t('2nd Supplemental Street Address such as Apartment or Suite Number or Department'),
    'field' => [
      'handler' => 'civicrm_handler_field_address',
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

  //CITY OR SUBURB THE ADDRESS IS AT
  $data['civicrm_address']['city'] = [
    'title' => t('City / Suburb'),
    'help' => t('The City or Suburb the address is at'),
    'field' => [
      'handler' => 'civicrm_handler_field_address',
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

  //STATE OR PROVINCE
  $data['civicrm_address']['state_province'] = [
    'title' => t('State / Province'),
    'real field' => 'state_province_id',
    'help' => t('The State or Province of the Address'),
    'field' => [
      'handler' => 'civicrm_handler_field_state',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'stateProvince',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  //ZIP / POSTAL CODE
  $data['civicrm_address']['postal_code'] = [
    'title' => t('ZIP / Postal Code'),
    'help' => t('ZIP / Postal Code of the Address'),
    'field' => [
      'handler' => 'civicrm_handler_field_address',
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

  //Latitude or Geo Code 1
  $data['civicrm_address']['geo_code_1'] = [
    'title' => t('Latitude'),
    'help' => t('Latitude of the Address for mapping'),
    'field' => [
      'handler' => 'civicrm_handler_field_address',
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

  //Longitude or Geo Code 2
  $data['civicrm_address']['geo_code_2'] = [
    'title' => t('Longitude'),
    'help' => t('Longitude of the Address for mapping'),
    'field' => [
      'handler' => 'civicrm_handler_field_address',
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

  //COUNTRY
  $data['civicrm_address']['country'] = [
    'title' => t('Country'),
    'real field' => 'country_id',
    'help' => t('The Country of the Address'),
    'field' => [
      'handler' => 'civicrm_handler_field_country',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'country',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'country',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  //COUNTY
  $data['civicrm_address']['county'] = [
    'title' => t('County'),
    'real field' => 'county_id',
    'help' => t('The County of the Address'),
    'field' => [
      'handler' => 'civicrm_handler_field_county',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'county',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'county',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  //Address name
  $data['civicrm_address']['name'] = [
    'title' => t('Address Name'),
    'help' => t('Be sure addres name is enabled in CiviCRM'),
    'field' => [
      'handler' => 'civicrm_handler_field_address',
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

  $fields = CRM_Core_DAO_Address::fields();

  civicrm_views_add_fields($fields, $data['civicrm_address'], 'civicrm_address');

  // CHILLING OUT AT THE CIVICRM_EMAIL TABLE

  $data['civicrm_email']['table']['group'] = t('CiviCRM Email');

  // Explain how this table joins to others.
  $data['civicrm_email']['table']['join'] = [
    // Directly links to contact table.
    'civicrm_contact' => [
      'left_field' => 'id',
      'field' => 'contact_id',
    ],
  ];

  //CiviCRM Email Using Contacts as a pitstop before going on to Participants
  $data['civicrm_email']['table']['join']['civicrm_participant'] = [
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  //CiviCRM Email Using Contacts as a pitstop before going on to Contributions
  $data['civicrm_email']['table']['join']['civicrm_contribution'] = [
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  //CiviCRM Email Using Contacts as a pitstop before going on to Activities
  $data['civicrm_email']['table']['join']['civicrm_activity'] = [
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  //CiviCRM Email Using Locations as a pitstop before going on to Events
  $data['civicrm_email']['table']['join']['civicrm_event'] = [
    'left_table' => 'civicrm_loc_block',
    'left_field' => 'email_id',
    'field' => 'id',
  ];
  //CiviCRM Email Using Contacts as a pitstop before going on to Memberhsips
  $data['civicrm_email']['table']['join']['civicrm_membership'] = [
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  //Display E-mails in User Views
  $data['civicrm_email']['table']['join']['users'] = [
    'left_table' => 'civicrm_uf_match',
    'left_field' => 'contact_id',
    'field' => 'contact_id',
  ];

  //BOOLEAN : IS PRIMARY EMAIL
  $data['civicrm_email']['is_primary'] = [
    'title' => t('Is Primary Email'),
    'help' => t('Is this address the contact\'s primary email?'),
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

  //BOOLEAN : IS BILLING EMAIL
  $data['civicrm_email']['is_billing'] = [
    'title' => t('Is Billing Email'),
    'help' => t('Is this address the contact\'s billing email?'),
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

  //EMAIL'S LOCATION TYPE
  $data['civicrm_email']['location_type'] = [
    'title' => t('Location Type'),
    'real field' => 'location_type_id',
    'help' => t('The Location of the Address for the Contact'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'locationType',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'locationType',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  //FULL EMAIL ADDRESS
  $data['civicrm_email']['email'] = [
    'title' => t('Email Address'),
    'help' => t('Email Address'),
    'field' => [
      'handler' => 'civicrm_handler_field_email',
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

  // CHILLING OUT AT THE CIVICRM_PHONE TABLE

  $data['civicrm_phone']['table']['group'] = t('CiviCRM Phone Details');

  // Explain how this table joins to others.
  $data['civicrm_phone']['table']['join'] = [
    // Directly links to contact table.
    'civicrm_contact' => [
      'left_field' => 'id',
      'field' => 'contact_id',
    ],
  ];

  //CiviCRM Phone Using Contacts as a pitstop before going on to Participants
  $data['civicrm_phone']['table']['join']['civicrm_participant'] = [
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  //CiviCRM Phone Using Contacts as a pitstop before going on to Contributions
  $data['civicrm_phone']['table']['join']['civicrm_contribution'] = [
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  //CiviCRM Phone Using Contacts as a pitstop before going on to Activities
  $data['civicrm_phone']['table']['join']['civicrm_activity'] = [
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  //CiviCRM Phone Using Locations as a pitstop before going on to Events
  $data['civicrm_phone']['table']['join']['civicrm_event'] = [
    'left_table' => 'civicrm_loc_block',
    'left_field' => 'phone_id',
    'field' => 'id',
  ];
  //CiviCRM Phone Using Contacts as a pitstop before going on to Memberhsips
  $data['civicrm_phone']['table']['join']['civicrm_membership'] = [
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  //Display Phone Details in User Views
  $data['civicrm_phone']['table']['join']['users'] = [
    'left_table' => 'civicrm_uf_match',
    'left_field' => 'contact_id',
    'field' => 'contact_id',
  ];

  //BOOLEAN : IS PRIMARY EMAIL
  $data['civicrm_phone']['is_primary'] = [
    'title' => t('Is Primary Phone'),
    'help' => t('Is this the contact\'s primary phone?'),
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

  //BOOLEAN : IS BILLING EMAIL
  $data['civicrm_phone']['is_billing'] = [
    'title' => t('Is Billing Phone'),
    'help' => t('Is this the contact\'s billing phone?'),
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

  //PHONE'S LOCATION TYPE
  $data['civicrm_phone']['location_type'] = [
    'title' => t('Location Type'),
    'real field' => 'location_type_id',
    'help' => t('The Location of the Phone for the Contact'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'locationType',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'locationType',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  //FULL PHONE NUMBER
  $data['civicrm_phone']['phone'] = [
    'title' => t('Phone Number'),
    'help' => t('Full Phone Number'),
    'field' => [
      'handler' => 'civicrm_handler_field_phone',
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

  //CONTACT ID contact this number belongs to
  $data['civicrm_phone']['contact_id'] = [
    'title' => t('Contact ID'),
    'help' => t('Contact phone number belongs to '),
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
    'relationship' => [
      'base' => 'civicrm_contact',
      'base field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('Related Phone'),
    ],
  ];

  //TODO: Mobile Provider

  //PHONE TYPE  , Phone, Mobile, Fax or Pager
  $data['civicrm_phone']['phone_type'] = [
    'title' => t('Phone Type'),
    'real field' => 'phone_type_id',
    'help' => t('The Type, either Phone, Mobile, Fax or Pager'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'phoneType',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'phoneType',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  // CHILLING OUT AT THE CIVICRM_WEBSITE TABLE

  $data['civicrm_website']['table']['group'] = t('CiviCRM Website');

  // Explain how this table joins to others.
  $data['civicrm_website']['table']['join'] = [
    // Directly links to contact table.
    'civicrm_contact' => [
      'left_field' => 'id',
      'field' => 'contact_id',
    ],
  ];

  //CiviCRM Website Using Contacts as a pitstop before going on to Participants
  $data['civicrm_website']['table']['join']['civicrm_participant'] = [
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  //CiviCRM Website Using Contacts as a pitstop before going on to Contributions
  $data['civicrm_website']['table']['join']['civicrm_contribution'] = [
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  //CiviCRM Website Using Contacts as a pitstop before going on to Activities
  $data['civicrm_website']['table']['join']['civicrm_activity'] = [
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  //CiviCRM Website Using Contacts as a pitstop before going on to Memberhsips
  $data['civicrm_website']['table']['join']['civicrm_membership'] = [
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  //Display Websites in User Views
  $data['civicrm_website']['table']['join']['users'] = [
    'left_table' => 'civicrm_uf_match',
    'left_field' => 'contact_id',
    'field' => 'contact_id',
  ];

  //WEBSITE TYPE
  $data['civicrm_website']['website_type'] = [
    'title' => t('Website Type'),
    'real field' => 'website_type_id',
    'help' => t('The type of website for the contact'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'websiteType',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'websiteType',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  //FULL WEB ADDRESS
  $data['civicrm_website']['url'] = [
    'title' => t('Website URL'),
    'help' => t('Website URL'),
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
    'relationship' => [
      'base' => 'civicrm_contact',
      'base field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('Related Phone'),
    ],
  ];
  // CIVICRM Activities are here with all the activeness they can muster, base tabling it up.
  //----------------------------------------------------------------------------------------

  $data['civicrm_activity']['table']['group'] = t('CiviCRM Activities');

  $data['civicrm_activity']['table']['base'] = [
    // Governs the whole mozilla
    'field' => 'id',
    'title' => t('CiviCRM Activities'),
    'help' => t("View displays CiviCRM Activities"),
  ];

  //TABLE JOINS FOR CIVICRM ACTIVITIES GO HERE!

  //CiviCRM Activities - FIELDS

  //Numeric Activity ID
  $data['civicrm_activity']['id'] = [
    'title' => t('Activity ID'),
    'help' => t('The numeric ID of the Activity Instance'),
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

  $data['civicrm_activity']['source_contact_id'] = [
    'title' => t('Source Contact'),
    'help' => t('The contact who scheduled this activity'),
    'relationship' => [
      'base' => 'civicrm_contact',
      'field' => 'source_contact_id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Source Contact'),
    ],
  ];

  /*
   * This block is commented out for two reasons:
   *  1) The help text is misleading to users; source_record_id is NOT
   *     "the contact who made this activity."
   *  2) It doesn't work; the code attempts to join on the wrong table.
   *
   * TODO: Figure out how to tell Views to join to a table whose identity
   *       isn't known yet. Because the details of the "parent" activity
   *       reside in different tables depending on the value of
   *       activity_type_id, Views needs to be told which table to
   *       join to on the fly.
   *
   *  $data['civicrm_activity']['source_record_id'] = array(
   *    'title' => t('Source Record ID'),
   *    // 'help' => t('The contact who made this activity'),
   *    'help' => t('The parent transaction (e.g. Contribution, Event Registration) if it is not an Activity.'),
   *    'relationship' => array(
   *      'base' => 'civicrm_contact',
   *      'field' => 'id',
   *      'handler' => 'views_handler_relationship',
   *      'label' => t('CiviCRM Source Record Contact'),
   *    ),
   *  );
   */

  //Activity Type
  $data['civicrm_activity']['activity_type'] = [
    'title' => t('Activity Type'),
    'real field' => 'activity_type_id',
    'help' => t('The Type of Activity'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'activityType',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'activityType',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  //Activity Status
  $data['civicrm_activity']['status'] = [
    'title' => t('Activity Status'),
    'real field' => 'status_id',
    'help' => t('The Status of this Activity'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'activityStatus',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'activityStatus',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  //Activity Priority
  $data['civicrm_activity']['priority'] = [
    'title' => t('Priority'),
    'real field' => 'priority_id',
    'help' => t('The Priority of this Activity'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'priority',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'priority',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  //Parent ID
  $data['civicrm_activity']['parent_id'] = [
    'title' => t('Parent Activity ID'),
    'help' => t('The numeric ID of the Parent Activity '),
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

  //Activity Subject
  $data['civicrm_activity']['subject'] = [
    'title' => t('Subject'),
    'help' => t('A short description of the activity..'),
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

  //Scheduled Activity Date
  $data['civicrm_activity']['activity_date_time'] = [
    'title' => t('Scheduled Activity Date'),
    'help' => t('The Date and Time the activity is scheduled to happen.'),
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
  civicrm_views_add_date_arguments($data['civicrm_activity'], [
    'title' => 'Scheduled Activity Date',
      'name' => 'activity_date_time',
    ]);

  //Activity Duration
  $data['civicrm_activity']['duration'] = [
    'title' => t('Duration (min)'),
    'help' => t('The Activity\'s Duration in Minutes'),
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

  //Activity Location
  $data['civicrm_activity']['location'] = [
    'title' => t('Location'),
    'help' => t('Activity\'s Location in a text string.'),
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

  //Activity Details / Notes
  $data['civicrm_activity']['details'] = [
    'title' => t('Details'),
    'help' => t('Details and Notes regarding the activity.'),
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

  //BOOLEAN : IS A TEST ACTIVITY
  $data['civicrm_activity']['is_test'] = [
    'title' => t('Is Test'),
    'help' => t('Is the activity a test entry?'),
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
  //Activity Medium (ie Phone, e-mail etc...)
  $data['civicrm_activity']['medium'] = [
    'title' => t('Medium'),
    'real field' => 'medium_id',
    'help' => t('The Medium of this Activity (ie phone, e-mail, etc...'),
    'field' => [
      'handler' => 'civicrm_handler_field_encounter_medium',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_encounter_medium',
      'allow empty' => TRUE,
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  //BOOLEAN : IS AUTO ACTIVITY
  $data['civicrm_activity']['is_auto'] = [
    'title' => t('Is Automaticaly created'),
    'help' => t('Is the activity created by the system?'),
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
  // Result
  // TODO: Add in Filter handler that can be narrowed down to spefics record_source_id and Activity Type
  $data['civicrm_activity']['result'] = [
    'title' => t('Result'),
    'help' => t('Survey or Petition Result'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  // Link to Campaign Table
  if (isset($enabled['CiviCampaign'])) {
    $data['civicrm_activity']['campaign_id'] = [
      'title' => t('Campaign'),
      'help' => t('Activity -> Campaign'),
      'relationship' => [
        'base' => 'civicrm_campaign',
        'field' => 'id',
        'handler' => 'views_handler_relationship',
        'label' => t('Activity -> Campaign'),
      ],
    ];
  }
  $data['civicrm_activity']['engagement_level'] = [
    'title' => t('Engagement level'),
    'help' => t('The Engagement Level of this Activity for this contact'),
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Campaign_PseudoConstant',
      'pseudo method' => 'engagementLevel',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Campaign_PseudoConstant',
      'pseudo method' => 'engagementLevel',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  // CRM-11488
  $data['civicrm_activity']['is_current_revision'] = [
    'title' => t('Current Revision'),
    'help' => t('Use to dedupe Civicase revisions'),
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
  $data['civicrm_activity']['is_deleted'] = [
    'title' => t('Is Deleted'),
    'help' => t('If the current Activity is deleted. Also used to dedupe Civicase revisions'),
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

  // CHILLING OUT AT THE civicrm_activity_assignment TABLE

  $data['civicrm_activity_assignment']['table']['group'] = t('CiviCRM Activity Assignments');

  // Explain how this table joins to others.
  $data['civicrm_activity_assignment']['table']['join'] = [
    // Directly links to activity table
    'civicrm_activity' => [
      'left_field' => 'id',
      'field' => 'activity_id',
    ],
  ];

  $data['civicrm_activity_assignment']['assignee_contact_id'] = [
    'title' => t('Assignee Contact ID'),
    'help' => t('The contact who is assigned the activity.'),
    'relationship' => [
      'base' => 'civicrm_contact',
      'left_field' => 'id',
      'field' => 'assignee_contact_id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Assignee Contact ID'),
    ],
  ];

  // CHILLING OUT AT THE civicrm_activity_target TABLE

  $data['civicrm_activity_target']['table']['group'] = t('CiviCRM Activity Targets');

  // Explain how this table joins to others.
  $data['civicrm_activity_target']['table']['join'] = [
    // Directly links to activity table
    'civicrm_activity' => [
      'left_field' => 'id',
      'field' => 'activity_id',
    ],
  ];

  $data['civicrm_activity_target']['target_contact_id'] = [
    'title' => t('Target Contact ID'),
    'help' => t('The contact who is the activity\'s target.'),
    'relationship' => [
      'base' => 'civicrm_contact',
      'left_field' => 'id',
      'field' => 'target_contact_id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Target Contact ID'),
    ],
  ];
  //Activity Text Link
  $data['civicrm_activity']['text_link'] = [
    'field' => [
      'title' => t('Text Link'),
      'help' => t('Creates a Text Link for various activity actions.'),
      'handler' => 'civicrm_handler_field_link_activity',
    ],
  ];

  //TEXT LINKS THAT AREN'T PART OF THE TABLE
  //Contact Text Link
  $data['civicrm_contact']['text_link'] = [
    'field' => [
      'title' => t('Text Link'),
      'help' => t('Creates a Text Link for various contact actions.'),
      'handler' => 'civicrm_handler_field_link_contact',
    ],
  ];

  //----------------------------------------------------------------
  // CIVICRM Relationships are here with all the connections, base tabling it up.
  //----------------------------------------------------------------

  $data['civicrm_relationship']['table']['group'] = t('CiviCRM Relationships');

  $data['civicrm_relationship']['table']['base'] = [
    // Governs the whole mozilla
    'field' => 'id',
    'title' => t('CiviCRM Relationships'),
    'help' => t("View displays CiviCRM Relationships, with connection to contacts."),
  ];

  //TABLE JOINS FOR CIVICRM RELATIONSHIPS GO HERE!

  $data['civicrm_relationship']['table']['join'] = [
    // Directly links to contact table - link A
    'civicrm_contact' => [
      'left_field' => 'id',
      'field' => 'contact_id_a',
    ],
    // Directly links to contact table - link B also.
    //       'civicrm_contact' => array(
    //                    'left_field' => 'id',
    //                    'field' => 'contact_id_b',
    //                    ),
  ];

  //Display A->B relationships in User Views
  $data['civicrm_relationship']['table']['join']['users'] = [
    'left_table' => 'civicrm_uf_match',
    'left_field' => 'contact_id',
    'field' => 'contact_id_a',
  ];

  //CiviCRM Relationships - FIELDS

  //Numeric Relationship ID
  $data['civicrm_relationship']['id'] = [
    'title' => t('Relationship ID'),
    'help' => t('The numeric ID of the Relationship'),
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

  //Relationship Type
  $data['civicrm_relationship']['relationship_type'] = [
    'title' => t('Relationship Type A-to-B'),
    'real field' => 'relationship_type_id',
    'help' => t('The Type of Relationship, i.e. Employee of, Household Head for...'),
    'field' => [
      'handler' => 'civicrm_handler_field_relationship_type',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_relationship_type',
      'allow empty' => TRUE,
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  $data['civicrm_relationship']['contact_id_a'] = [
    'title' => t('Contact ID A'),
    'real field' => 'contact_id_a',
    'help' => t('The contact A'),
    'relationship' => [
      'base' => 'civicrm_contact',
      'field' => 'id',
      'relationship table' => 'civicrm_relationship',
      'relationship field' => 'contact_id_a',
      'other_field' => 'contact_id_b',
      'handler' => 'civicrm_handler_relationship',
      'label' => t('CiviCRM Contact A'),
    ],
  ];

  $data['civicrm_relationship']['contact_id_b'] = [
    'title' => t('Contact ID B'),
    'real field' => 'contact_id_b',
    'help' => t('The contact B'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'relationship' => [
      'base' => 'civicrm_contact',
      'field' => 'id',
      'relationship table' => 'civicrm_relationship',
      'relationship field' => 'contact_id_b',
      'other_field' => 'contact_id_a',
      'handler' => 'civicrm_handler_relationship',
      'label' => t('CiviCRM Contact B'),
    ],
  ];

  $data['civicrm_relationship']['description'] = [
    'title' => t('Description'),
    'help' => t('Description of the relationship'),
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

  //Relationship start date
  $data['civicrm_relationship']['start_date'] = [
    'title' => t('Start Date'),
    'help' => t('The Relationship\'s Start Date'),
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

  civicrm_views_add_date_arguments($data['civicrm_relationship'], [
    'title' => 'Start Date',
      'name' => 'start_date',
    ]);

  //Relationship end date
  $data['civicrm_relationship']['end_date'] = [
    'title' => t('End Date'),
    'help' => t('The Relationship\'s End Date'),
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

  civicrm_views_add_date_arguments($data['civicrm_replationship'], [
    'title' => 'End Date',
      'name' => 'end_date',
    ]);
  //BOOLEAN : IS RELATIONSHIP ACTIVE
  $data['civicrm_relationship']['is_active'] = [
    'title' => t('Is Relationship Active'),
    'help' => t('Is the Relationship active?'),
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

  //Relationship Text Link
  $data['civicrm_relationship']['text_link'] = [
    'field' => [
      'title' => t('Text Link'),
      'help' => t('Creates a Text Link for various relationship actions.'),
      'handler' => 'civicrm_handler_field_link_relationship',
    ],
  ];

  /*
     * Add support for CivicRM groups table
     */

  $data['civicrm_group']['table']['group'] = t('CiviCRM Groups');

  $data['civicrm_group']['table']['join']['civicrm_contact'] = [
    'left_table' => 'civicrm_group_contact',
    'left_field' => 'group_id',
    'field' => 'id',
  ];

  $data['civicrm_group_contact']['table']['join']['civicrm_contact'] = [
    // Directly links to group table
    'left_field' => 'id',
    'field' => 'contact_id',
  ];

  $data['civicrm_group_contact']['table']['group'] = t('CiviCRM Groups');
  $data['civicrm_group_status']['table']['group'] = t('Contact Group Status');

  $data['civicrm_group_contact']['status'] = [
    'title' => t('Contact Status'),
    'help' => t('The group status of the contact'),
    'field' => [
      'handler' => 'civicrm_handler_filter_group_status',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_group_status',
      'allow empty' => TRUE,
    ],
  ];

  // CiviCRM Groups - FIELDS

  // Numeric Group ID
  $data['civicrm_group_contact']['status'] = [
    'title' => t('Contact Status'),
    'help' => t('The group status of the contact'),
    'field' => [
      'handler' => 'civicrm_handler_filter_group_status',
      'click sortable' => TRUE,
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_group_status',
      'allow empty' => TRUE,
    ],
  ];

  //CiviCRM Groups - FIELDS

  //Numeric Group ID
  $data['civicrm_group']['id'] = [
    'title' => t('Group ID'),
    'help' => t('The numeric ID of the Group'),
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
  //Is Group Active?
  $data['civicrm_group']['is_active'] = [
    'title' => t('Is Active'),
    'help' => t('Is the Group Active'),
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

  $data['civicrm_group']['title'] = [
    'title' => t('Title'),
    'help' => t('Group Title'),
    'real field' => 'id',
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'allGroup',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'allGroup',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  $data['civicrm_group']['description'] = [
    'title' => t('Description'),
    'help' => t('Description of the Group'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  /*
     * Add support for CiviCRM tags
     */

  $data['civicrm_tag']['table']['group'] = t('CiviCRM Tags');

  $data['civicrm_tag']['table']['join']['civicrm_contact'] = [
    'left_table' => 'civicrm_entity_tag',
    'left_field' => 'tag_id',
    'field' => 'id',
  ];
  $data['civicrm_tag']['table']['join']['civicrm_activity'] = [
    'left_table' => 'civicrm_entity_tag',
    'left_field' => 'tag_id',
    'field' => 'id',
  ];

  $data['civicrm_entity_tag']['table']['join']['civicrm_contact'] = [
    // Directly links to tag table
    'left_field' => 'id',
    'field' => 'entity_id',
    'extra' => [
      [
        'field' => 'entity_table',
        'value' => 'civicrm_contact',
        'numeric' => FALSE,
      ],
    ],
  ];
  $data['civicrm_entity_tag']['table']['join']['civicrm_activity'] = [
    // Directly links to tag table
    'left_field' => 'id',
    'field' => 'entity_id',
    'extra' => [
      [
        'field' => 'entity_table',
        'value' => 'civicrm_activity',
        'numeric' => FALSE,
      ],
    ],
  ];
  //CiviCRM Groups - FIELDS

  //Numeric Group ID
  $data['civicrm_tag']['id'] = [
    'title' => t('Tag ID'),
    'help' => t('The numeric ID of the Tag'),
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

  $data['civicrm_tag']['name'] = [
    'title' => t('Name'),
    'help' => t('Name of Tag'),
    'real field' => 'id',
    'field' => [
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Member_PseudoConstant',
      'pseudo method' => 'tag',
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'pseudo class' => 'CRM_Member_PseudoConstant',
      'pseudo method' => 'tag',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];
  $data['civicrm_tag']['description'] = [
    'title' => t('Description'),
    'help' => t('Description of the Tag'),
    'field' => [
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ],
    'argument' => [
      'handler' => 'views_handler_argument',
    ],
    'sort' => [
      'handler' => 'views_handler_sort',
    ],
  ];

  /*
    * UF Match table - expose drupal ID
    */

  $data['civicrm_uf_match']['table']['group'] = t("CiviCRM Drupal match table");
  $data['civicrm_uf_match']['table']['join']['civicrm_contact'] = [
    'left_field' => 'id',
    'field' => 'contact_id'
  ];
  $data['civicrm_uf_match']['uf_id'] = [
    'title' => t('Drupal User ID'),
    'help' => t('The user id of the User matched to a CiviCRM Contact'),
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
  $data['civicrm_uf_match']['domain_id'] = [
    'title' => t('Domain ID'),
    'help' => t('Relates a CiviCRM contact to its domain'),
    'field' => [
      'handler' => 'views_handler_field_numeric',
    ],
    'filter' => [
      'handler' => 'civicrm_handler_filter_domain',
    ],
    'sort' => [
      'handler' => 'views_handler_sort_numeric',
    ],
  ];

  // Pull in all the Custom fields
  $query = "select id, extends, extends_entity_column_value, style from civicrm_custom_group where is_active = 1";

  $dao = CRM_Core_DAO::executeQuery($query);

  while ($dao->fetch()) {
    // call getTree using $dao->id as groupID and $dao->extends as entityType
    $extends_column_values = [];
    if (strstr($dao->extends_entity_column_value, CRM_Core_DAO::VALUE_SEPARATOR)) {
      $extends_column_values = trim($dao->extends_entity_column_value, CRM_Core_DAO::VALUE_SEPARATOR);
      $extends_column_values = explode(CRM_Core_DAO::VALUE_SEPARATOR, $extends_column_values);
    }
    else {
      $extends_column_values = $dao->extends_entity_column_value;
    }

    $data = civicrm_views_custom_data_cache($data, $dao->extends, $dao->id, $extends_column_values, $dao->style);
  }
}
