<?php
/**
 * @file
 * civicrm_event_og_config_sample.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function civicrm_event_og_config_sample_field_default_fields() {
  $fields = [];

  // Exported field: 'node-og_event-body'.
  $fields['node-og_event-body'] = [
    'field_config' => [
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => [
        0 => 'node',
      ],
      'field_name' => 'body',
      'foreign keys' => [
        'format' => [
          'columns' => [
            'format' => 'format',
          ],
          'table' => 'filter_format',
        ],
      ],
      'indexes' => [
        'format' => [
          0 => 'format',
        ],
      ],
      'locked' => '0',
      'module' => 'text',
      'settings' => [
      ],
      'translatable' => '0',
      'type' => 'text_with_summary',
    ],
    'field_instance' => [
      'bundle' => 'og_event',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => [
        'default' => [
          'label' => 'hidden',
          'module' => 'text',
          'settings' => [],
          'type' => 'text_default',
          'weight' => '1',
        ],
        'entityreference_view_widget' => [
          'label' => 'above',
          'settings' => [],
          'type' => 'hidden',
          'weight' => 0,
        ],
        'teaser' => [
          'label' => 'hidden',
          'module' => 'text',
          'settings' => [
            'trim_length' => 600,
          ],
          'type' => 'text_summary_or_trimmed',
          'weight' => 1,
        ],
      ],
      'entity_type' => 'node',
      'field_name' => 'body',
      'label' => 'Body',
      'required' => FALSE,
      'settings' => [
        'display_summary' => TRUE,
        'text_processing' => 1,
        'user_register_form' => FALSE,
      ],
      'widget' => [
        'module' => 'text',
        'settings' => [
          'rows' => 20,
          'summary_rows' => 5,
        ],
        'type' => 'text_textarea_with_summary',
        'weight' => '1',
      ],
    ],
  ];

  // Exported field: 'node-og_event-field_civicrm_event'.
  $fields['node-og_event-field_civicrm_event'] = [
    'field_config' => [
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => [],
      'field_name' => 'field_civicrm_event',
      'foreign keys' => [
        'node' => [
          'columns' => [
            'target_id' => 'nid',
          ],
          'table' => 'node',
        ],
      ],
      'indexes' => [
        'target_id' => [
          0 => 'target_id',
        ],
      ],
      'locked' => '0',
      'module' => 'entityreference',
      'settings' => [
        'handler' => 'base',
        'handler_settings' => [
          'behaviors' => [
            'views-select-list' => [
              'status' => 0,
            ],
          ],
          'sort' => [
            'type' => 'none',
          ],
          'target_bundles' => [],
        ],
        'target_type' => 'civicrm_event',
      ],
      'translatable' => '0',
      'type' => 'entityreference',
    ],
    'field_instance' => [
      'bundle' => 'og_event',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => [
        'default' => [
          'label' => 'above',
          'module' => 'entityreference',
          'settings' => [
            'link' => FALSE,
          ],
          'type' => 'entityreference_label',
          'weight' => '5',
        ],
        'entityreference_view_widget' => [
          'label' => 'above',
          'settings' => [],
          'type' => 'hidden',
          'weight' => 0,
        ],
        'teaser' => [
          'label' => 'above',
          'settings' => [],
          'type' => 'hidden',
          'weight' => 0,
        ],
      ],
      'entity_type' => 'node',
      'field_name' => 'field_civicrm_event',
      'label' => 'CiviCRM Event',
      'required' => FALSE,
      'settings' => [
        'user_register_form' => FALSE,
      ],
      'widget' => [
        'module' => 'entityreference',
        'settings' => [
          'match_operator' => 'CONTAINS',
          'path' => '',
          'size' => 60,
        ],
        'type' => 'entityreference_autocomplete',
        'weight' => '3',
      ],
    ],
  ];

  // Exported field: 'node-og_event-group_group'.
  $fields['node-og_event-group_group'] = [
    'field_config' => [
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => [],
      'field_name' => 'group_group',
      'foreign keys' => [],
      'indexes' => [
        'value' => [
          0 => 'value',
        ],
      ],
      'locked' => '0',
      'module' => 'list',
      'no_ui' => TRUE,
      'settings' => [
        'allowed_values' => [
          0 => 'Not a group type',
          1 => 'Group type',
        ],
        'allowed_values_function' => '',
      ],
      'translatable' => '0',
      'type' => 'list_boolean',
    ],
    'field_instance' => [
      'bundle' => 'og_event',
      'default_value' => [
        0 => [
          'value' => 1,
        ],
      ],
      'deleted' => '0',
      'description' => 'Determine if this is an OG group.',
      'display' => [
        'default' => [
          'label' => 'above',
          'module' => 'og_ui',
          'settings' => [
            'field_name' => FALSE,
          ],
          'type' => 'og_group_subscribe',
          'weight' => 6,
        ],
        'entityreference_view_widget' => [
          'label' => 'above',
          'settings' => [],
          'type' => 'hidden',
          'weight' => 0,
        ],
        'teaser' => [
          'label' => 'above',
          'settings' => [],
          'type' => 'hidden',
          'weight' => 0,
        ],
      ],
      'display_label' => 1,
      'entity_type' => 'node',
      'field_name' => 'group_group',
      'label' => 'Group',
      'required' => FALSE,
      'settings' => [
        'user_register_form' => FALSE,
      ],
      'view modes' => [
        'full' => [
          'custom settings' => FALSE,
          'label' => 'Full',
          'type' => 'og_group_subscribe',
        ],
        'teaser' => [
          'custom settings' => FALSE,
          'label' => 'Teaser',
          'type' => 'og_group_subscribe',
        ],
      ],
      'widget' => [
        'module' => 'options',
        'settings' => [
          'display_label' => 0,
          'og_hide' => TRUE,
        ],
        'type' => 'options_onoff',
        'weight' => 0,
      ],
    ],
  ];

  // Translatables
  // Included for use with string extractors like potx.
  t('Body');
  t('CiviCRM Event');
  t('Determine if this is an OG group.');
  t('Group');

  return $fields;
}
