<?php

namespace Drupal\civicrm\Form;

use Drupal\civicrm\Civicrm;
use Drupal\Core\Access\AccessResult;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Markup;
use Drupal\Core\Session\AccountInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\Routing\Exception\ResourceNotFoundException;
use Drupal\Core\Cache\Cache;

class UserProfile extends FormBase  {
  protected $user;
  protected $profile;
  protected $contact_id;
  protected $uf_group;

  public function __construct(Civicrm $civicrm) {
    $civicrm->initialize();
  }

  static function create(ContainerInterface $container) {
    return new static(
      $container->get('civicrm')
    );
  }

  public function getFormId() {
    return 'civicrm_user_profile';
  }

  public function buildForm(array $form, FormStateInterface $form_state, AccountInterface $user = NULL, $profile = NULL) {
    // Make the controller state available to form overrides.
    $form_state->set('controller', $this);
    $this->user = $user;
    $this->profile = $profile;

    // Search for the profile form, otherwise generate a 404.
    $uf_groups = \CRM_Core_BAO_UFGroup::getModuleUFGroup('User Account');
    if (empty($uf_groups[$profile])) {
      throw new ResourceNotFoundException();
    }
    $this->uf_group = $uf_groups[$profile];

    // Grab the form html.
    // if there were param on submit, getEditHTML will add contact directly
    $this->contact_id = \CRM_Core_BAO_UFMatch::getContactId($user->id());
    $html = \CRM_Core_BAO_UFGroup::getEditHTML($this->contact_id, $this->uf_group['title']);
    // this dirty hack will get newly added contact
    // add drupal user id as uf_id to save them
    global $civicrm_profile_contact_id;
    if (!empty($civicrm_profile_contact_id)) {
      $params = array(
        'contact_id' => $civicrm_profile_contact_id,
        'uf_id' => $user->id(),
        'uf_name' => $user->get('mail')->value,
      );
      \CRM_Core_BAO_UFMatch::create($params);
    }

    $form['#title'] = $this->user->getAccountName();
    $form['form'] = array(
      '#type' => 'fieldset',
      '#title' => $this->uf_group['title'],
      'html' => array(
        '#markup' => Markup::create($html),
      ),
    );
    $form['actions'] = array(
      '#type' => 'actions',
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#button_type' => 'primary',
      ),
    );

    return $form;
  }

  public function validateForm(array &$form, FormStateInterface $form_state) {
    $errors = \CRM_Core_BAO_UFGroup::isValid($this->contact_id, $this->uf_group['title']);

    if (is_array($errors)) {
      foreach ($errors as $name => $error) {
        $form_state->setErrorByName($name, $error);
      }
    }
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    // Invalidate caches for user, so that latest profile information shows.
    Cache::invalidateTags(array('user:' . $this->user->id()));
    \Drupal::messenger()->addStatus(t("Profile successfully updated."));
    // CiviCRM will process form when redirect to getEditHTML, so we do nothing here
  }

  public function access($profile) {
    $uf_groups = \CRM_Core_BAO_UFGroup::getModuleUFGroup('User Account', 0, FALSE, \CRM_Core_Permission::EDIT);

    if (isset($uf_groups[$profile])) {
      return AccessResult::allowed();
    }
    return AccessResult::forbidden();
  }
}