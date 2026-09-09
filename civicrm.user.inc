<?php

use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Url;
use Symfony\Component\HttpFoundation\RedirectResponse;

/**
 * Implements hook_user_login().
 */
function civicrm_user_login(AccountInterface $account) {
  # refs #36521, #42221, we should sync user anyway
  \Drupal::service('civicrm')->synchronizeUser($account);
}

/**
 * Implements hook_user_insert().
 */
function civicrm_user_insert(AccountInterface $account) {
  $civicrm = \Drupal::service('civicrm');
  $civicrm->initialize();
  $userSystem = CRM_Core_Config::singleton()->userSystem;
  $userSystemID = $userSystem->getBestUFID($account);
  $uniqId = $userSystem->getBestUFUniqueIdentifier($account);
  $registration = $civicrm->takeRegistrationInput($account->getEmail());
  if ($registration !== NULL) {
    $result = \CRM_Core_BAO_UFMatchRegistration::register($userSystemID, $uniqId, $registration['values'], $registration['uploads']);
    $ufMatch = $result['ufMatch'];
  }
  else {
    // Programmatic account creation has no validated registration profile.
    $ufMatch = \CRM_Core_BAO_UFMatch::synchronizeUFMatch($account, $userSystemID, $uniqId, 'Drupal', NULL, 'Individual');
  }
  if (!$ufMatch) {
    throw new \CRM_Core_Exception(ts('Unable to link the account to a contact.'));
  }
  // No raw POST/profile reprocessing or unconditional primary-email update.
  // The mapping transaction has already applied the permitted profile values.
}

/**
 * Implements hook_user_update().
 */
function civicrm_user_update(AccountInterface $account) {
  \Drupal::service('civicrm')->initialize();

  // Saving login timestamps/roles must not overwrite an existing contact's
  // primary email merely because it differs from the account's email.
  if (!isset($account->original) || $account->original->getEmail() === $account->getEmail()) {
    return;
  }

  // Update primary email address only when the Drupal email actually changes.
  $contact_id = \CRM_Core_BAO_UFMatch::getContactId($account->id());
  if ($contact_id) {
    $contact_email = \CRM_Contact_BAO_Contact::getPrimaryEmail($contact_id);
    if ($contact_email != $account->getEmail()) {
      \CRM_Core_BAO_Log::audit($account->id(), 'users.mail', $contact_email);
      \CRM_Core_BAO_UFMatch::updateContactEmail($contact_id, $account->getEmail());
    }
  }

  // @Todo Drupal 7 code cleared navigation menu if it detected a change in roles.
  // \CRM_Core_BAO_Navigation::resetNavigation($contact_id);
}

/**
 * Implements hook_user_delete().
 */
function civicrm_user_delete(AccountInterface $account) {
  \Drupal::service('civicrm')->initialize();
  \CRM_Core_BAO_UFMatch::deleteUser($account->id());
}
