<?php

namespace Drupal\civicrm;

use Drupal\Core\Config\ConfigException;
use Drupal\Core\Session\AccountInterface;

class Civicrm {

  public $initialized = FALSE;

  /**
   * Initialize CiviCRM. Call this function from other modules too if
   * they use the CiviCRM API.
   */
  public function initialize() {
    if ($this->initialized) {
      return;
    }

    // refs #31356, force session start for anonymous user
    $current_user = \Drupal::currentUser();
    if ($current_user->isAnonymous()) {
      // Add something to $_SESSION to force session start and session ID will persist.
      // See https://www.drupal.org/node/2865991.
      $_SESSION["CiviCRM_Anonymous"] = TRUE;
    }
    else {
      if (isset($_SESSION['CiviCRM_Anonymous'])) {
        unset($_SESSION['CiviCRM_Anonymous']);
      }
    }

    // refs #31213
    // include_path will conflict when using autoload
    // we need to make sure remove all PEAR related include path on autoload
    $includePaths = explode(PATH_SEPARATOR, get_include_path());
    foreach($includePaths as $idx => $path) {
      if (strstr($path, 'pear')) {
        unset($includePaths[$idx]);
      }
    }
    set_include_path(implode(PATH_SEPARATOR, $includePaths));

    // Get ready for problems
    $docLinkInstall = "http://wiki.civicrm.org/confluence/display/CRMDOC/Drupal+Installation+Guide";
    $docLinkTrouble = "http://wiki.civicrm.org/confluence/display/CRMDOC/Installation+and+Configuration+Trouble-shooting";
    $forumLink      = "http://forum.civicrm.org/index.php/board,6.0.html";

    $errorMsgAdd = t("Please review the <a href='!1'>Drupal Installation Guide</a> and the <a href='!2'>Trouble-shooting page</a> for assistance. If you still need help installing, you can often find solutions to your issue by searching for the error message in the <a href='!3'>installation support section of the community forum</a>.</strong></p>",
      array('!1' => $docLinkInstall, '!2' => $docLinkTrouble, '!3' => $forumLink)
    );

    $settingsFile = \Drupal::service('kernel')->getSitePath() . '/civicrm.settings.php';
    if (!defined('CIVICRM_SETTINGS_PATH')) {
      define('CIVICRM_SETTINGS_PATH', $settingsFile);
    }

    $output = include_once $settingsFile;
    if ($output == FALSE) {
      $msg = t("The CiviCRM settings file (civicrm.settings.php) was not found in the expected location ") .
        "(" . $settingsFile . "). " . $errorMsgAdd;
      throw new ConfigException($msg);
    }

    // This does pretty much all of the civicrm initialization
    $included = include_once 'CRM/Core/Config.php';
    if ($included == FALSE) {
      $msg = t("The path for including CiviCRM code files is not set properly. Most likely there is an error in the <em>civicrm_root</em> setting in your CiviCRM settings file (!1).",
          array('!1' => $settingsFile)
        ) . t("civicrm_root is currently set to: <em>!1</em>.", array('!1' => $civicrm_root)) . $errorMsgAdd;
      throw new ConfigException($msg);
    }

    // Initialize the system by creating a config object
    \CRM_Core_Config::singleton();

    // Mark CiviCRM as initialized.
    $this->initialized = TRUE;
  }

  public function isInitialized() {
    return $this->initialized;
  }

  public function invoke($args) {
    $this->initialize();

    // Civicrm will echo/print directly to stdout. We need to capture it
    // so that we can return the output as a renderable array.
    ob_start();
    $content = \CRM_Core_Invoke::invoke($args);
    $output = ob_get_clean();
    return !empty($content) ? $content : $output;
  }

  /**
   * Synchronize a Drupal account with CiviCRM. This is a wrapper for CRM_Core_BAO_UFMatch::synchronize().
   *
   * @param AccountInterface $account
   * @param string $contact_type
   */
  public function synchronizeUser(AccountInterface $account, $contact_type = 'Individual') {
    $this->initialize();
    \CRM_Core_BAO_UFMatch::synchronize($account, FALSE, 'Drupal', $this->getCtype($contact_type));
  }

  /**
   * Function to get the contact type
   *
   * @param string $default Default contact type
   *
   * @return $ctype Contact type
   *
   * @Todo: Document what this function is doing and why.
   */
  public function getCtype($default = 'Individual') {
    if (!empty($_REQUEST['ctype'])) {
      $ctype = $_REQUEST['ctype'];
    }
    else if (!empty($_REQUEST['edit']['ctype'])) {
      $ctype = $_REQUEST['edit']['ctype'];
    }
    else {
      $ctype = $default;
    }

    if (!in_array($ctype, array('Individual', 'Organization', 'Household'))) {
      $ctype = $default;
    }
    return $ctype;
  }
}