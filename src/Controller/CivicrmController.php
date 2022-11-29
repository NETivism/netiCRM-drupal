<?php

/**
 * @file
 * Contains \Drupal\civicrm\Controller\CivicrmController
 */

namespace Drupal\civicrm\Controller;

use Drupal\user\Entity\User;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\HttpKernel\Exception;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Render\Markup;
use Drupal\Core\Render\HtmlResponse;
use Drupal\civicrm\CivicrmPageState;
use Drupal\civicrm\Civicrm;

class CivicrmController extends ControllerBase {
  protected $civicrm;
  protected $civicrmPageState;

  public function __construct(Civicrm $civicrm, CivicrmPageState $civicrmPageState) {
    $this->civicrm = $civicrm;
    $this->civicrmPageState = $civicrmPageState;
  }

  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('civicrm'),
      $container->get('civicrm.page_state')
    );
  }

  public function main($args, $extra) {
    if ($extra) {
      $args = array_merge($args, explode(':', $extra));
    }

    // CiviCRM's Invoke.php has hardwired in the expectation that the query parameter 'q' is being used.
    // We recreate that parameter. Ideally in the future, this data should be passed in explicitly and not tied
    // to an environment variable.
    $_GET['q'] = implode('/', $args);

    // Need to disable the page cache.
    \Drupal::service('page_cache_kill_switch')->trigger();

    // Synchronize the Drupal user with the Contacts database (dev/drupal#107)
    if (!$this->currentUser()->isAnonymous()) {
      $this->civicrm->synchronizeUser(User::load($this->currentUser()->id()));
    }

    // correct exception handling will display drupal themed page
    try {
      $content = $this->civicrm->invoke($args);
    }
    catch (\CRM_Core_Exception $e) {
      // force content to white
      $content = '';

      // this will save civicrm session correctly
      $message = $e->getMessage();
      if (\CRM_Core_Config::singleton()->userFrameworkLogging) {
        \Drupal::logger('civicrm')->error(strip_tags($message));
      }

      $data = $e->getErrorData();
      $code = $e->getErrorCode();

      switch ($code) {
        case \CRM_Core_Error::NO_ERROR:
          // this will terminate response, and trigger event(KernelEvents::TERMINATE) correctly
          // CRM already output header and content to client
          // do not send real response here in symfony
          // Ideally in the future, CRM should not output header and content directly
          $kernel = \Drupal::getContainer()->get('kernel');
          $request = \Drupal::request();
          $response = HtmlResponse::create('', 200);
          $response->send();
          $kernel->terminate($request, $response);
          exit();
          break;
        case \CRM_Core_Error::STATUS_BOUNCE:
          \CRM_Core_Session::setStatus(FALSE, FALSE, 'warning'); // remove duplicate status display in CRM
          \CRM_Utils_System::civiBeforeShutdown();
          if (!empty($data['redirect'])) {
            $url = $data['redirect'];
          }
          $message = Markup::create($message);
          \Drupal::messenger()->addWarning($message);
          if ($url) {
            $response = RedirectResponse::create($url, 302); // CRM_Core_Error::STATUS_BOUNCE should also 302
            return $response;
          }
          break;

        case \CRM_Core_Error::FATAL_ERROR:
        case \CRM_Core_Error::DATABASE_ERROR:
          \CRM_Core_Session::setStatus(FALSE, FALSE, 'error');  // remove duplicate status display in CRM
          \CRM_Utils_System::civiBeforeShutdown();
          if ($data['content']) {
            $content = $data['content']; // this will fetch fatal.tpl from crm
          }
          else {
            $message = Markup::create($message);
            \Drupal::messenger()->addError($message);
          }

          # method 1, drupal way
          // throw new Exception\ServiceUnavailableHttpException(); // do not throw this, will not display drupal theme

          # method 2, crm fatal message template(not good)
          // $response = HtmlResponse::create($content, $code);
          // return $response;

          # method 3, drupal theme warpped fatal message, but not good at http status code
          // doing nothing, $build will include content
          break;

        default:
          \CRM_Utils_System::civiBeforeShutdown();
          throw new Exception\AccessDeniedHttpException();
          $content = '';
          break;
      }
    }
    // From \Drupal\Core\Form\FormBuilder->buildForm, the Response always throw EnforcedResponseException.
    catch(\Drupal\Core\Form\EnforcedResponseException $e) {
      $e->getResponse()->send();
    }
    // not one of CRM_Core_Exception
    catch (\Exception $e) {
      \Drupal::logger('civicrm')->error($e->getMessage());
      throw new Exception\AccessDeniedHttpException();
    }

    if ($this->civicrmPageState->isAccessDenied()) {
      throw new Exception\AccessDeniedHttpException();
    }

    // Start doing rendering if everything is fine

    // inline javascript
    $page_state = \Drupal::service('civicrm.page_state');
    $javascripts = $page_state->getJs();
    if (!empty($javascripts['inline'])) {
      foreach($javascripts['inline'] as $js) {
        $script = [
          '#type'   => 'html_tag',
          '#tag' => 'script',
          '#value' => Markup::create($js),
        ];
        $rendered = \Drupal::service('renderer')->render($script);
        $content .= $rendered;
      }
    }

    // We set the CiviCRM markup as safe and assume all XSSset (an other) issues have already
    // been taken care of.
    $build = array(
      '#markup' => Markup::create($content),
      '#cache' => [
        'max-age' => 0,
      ],
    );

    // Override default title value if one has been set in the course
    // of calling \CRM_Core_Invoke::invoke().
    if ($title = $this->civicrmPageState->getTitle()) {
      // Mark the pageTitle as safe so markup is not escaped by Drupal.
      // This handles the case where, eg. the page title is surrounded by <span id="crm-remove-title" style=display: none">
      // Todo: This is a naughty way to do this. Better to have CiviCRM passing us no markup whatsoever.
      $build['#title'] = Markup::create($title);
    }

    return $build;
  }

}
