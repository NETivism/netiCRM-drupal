<?php
/**
 * Field handler to provide simple renderer that allows linking to an Contribution Page or it's Management Page.
 */
class civicrm_handler_field_contribution_page_link extends views_handler_field {

  /**
   * Constructor to provide additional field to add.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['id'] = 'id';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_civicrm_contribution_page'] = array('default' => 'none');
    $options['custom_link'] = array('default' => '');
    return $options;
  }

  /**
   * Provide link to node option
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_civicrm_contribution_page'] = array(
      '#type' => 'select',
      '#title' => t('Choose where to link this field'),
      '#options' => array(
        'none' => t('Don\'t Link this Field'),
        'page' => t('Link to Contribution Page'),
        'config' => t('Link to Contribution Page Configuration'),
        'custom' => t('Link to a Custom Node'),
      ),
      '#default_value' => !empty($this->options['link_to_civicrm_contribution_page']),
    );

    $form['custom_link'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom Link to a Drupal Node'),
      '#description' => t('Link to a Drupal Node or View, from the base path. The Contribution ID will be appended to the end of the link.'),
      '#default_value' => isset($this->options['custom_link']) ? $this->options['custom_link'] : '',
      '#dependency' => array('link_to_civicrm_contribution_page' => array('custom')),
    );
  }

  /**
   * Render whatever the data is as a link to the node.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {

    $link_choice = $this->options['link_to_civicrm_contribution_page'];

    $custom_link = NULL;
    if ($link_choice == 'custom') {
      $custom_link = $this->options['custom_link'];
    }


    switch ($link_choice) {

      // LINKING TO Contribution PAGE (PUBLICLY ACCESSIBLE)
      case 'page':
        if (user_access('make online contributions') && $data !== NULL && $data !== '') {
          return civicrm_views_href($data,
            'civicrm/contribute/transact',
            "reset=1&id={$values->{$this->aliases['id']}}"
          );
        }

      // LINKING TO Contribution Page config
      case 'config':
        if (user_access('access CiviContribute') && $data !== NULL && $data !== '') {
          return civicrm_views_href($data,
            'civicrm/admin/contribute',
            "action=update&id={$values->{$this->aliases['id']}}&reset=1"
          );
        }

      // CUSTOM NODE LINKAGE, GOOD FOR LINKING TO OTHER VIEWS WITH AN ARGUMENT
      case 'custom':
        if ($custom_link !== NULL && $custom_link !== '' && $data !== NULL && $data !== '') {
          return l($data, $custom_link . "{$values->{$this->aliases['id']}}", array('html' => TRUE));
        }

      // IF THE OTHER CASES AREN'T IN PLAY, THEN JUST PRINT THE DATA
      default:
        return $data;
    }
  }

  function render($values) {
    return $this->render_link(check_plain($values->{$this->field_alias}), $values);
  }
}

