<?php

namespace Drupal\civicrm_views\Plugin\views\field;

use Drupal\civicrm\Civicrm;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\core\form\FormStateInterface;
use Drupal\views\ResultRow;
use Drupal\views\Plugin\views\field\FieldPluginBase;
use Drupal\Core\Link;
use Drupal\Core\Url as CoreUrl;

/**
 * @ingroup views_field_handlers
 *
 * @ViewsField("civicrm_event_link")
 */
class CivicrmEventLink extends FieldPluginBase
{

  public function __construct(array $configuration, $plugin_id, $plugin_definition, Civicrm $civicrm)
  {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $civicrm->initialize();
  }

  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition)
  {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('civicrm')
    );
  }

  /**
   * {@inheritdoc}
   */
  protected function getDefaultLabel()
  {
    return $this->options['link_to_civicrm_event'] == 'page' ? ts('Event Info') : ts('Online Registration');
  }

  /**
   * {@inheritdoc}
   */
  protected function defineOptions()
  {
    $options = parent::defineOptions();
    $options['output_url_as_text'] = ['default' => FALSE];
    $options['absolute'] = ['default' => FALSE];
    $options['link_text'] = ['default' => ''];
    $options['link_to_civicrm_event'] = ['default' => 'page'];
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function buildOptionsForm(&$form, FormStateInterface $form_state)
  {
    parent::buildOptionsForm($form, $form_state);

    // The path is set by ::renderLink() so we do not allow to set it.
    $form['alter'] += ['path' => [], 'query' => [], 'external' => []];
    $form['alter']['path'] += ['#access' => FALSE];
    $form['alter']['path_case'] += ['#access' => FALSE];
    $form['alter']['query'] += ['#access' => FALSE];
    $form['alter']['external'] += ['#access' => FALSE];
    $form['alter']['prefix'] += ['#access' => FALSE];
    $form['alter']['suffix'] += ['#access' => FALSE];
    $form['alter']['absolute'] += ['#access' => FALSE];
    $form['alter']['replace_spaces'] += ['#access' => FALSE];

    $form['link_to_civicrm_event'] = array(
      '#type' => 'select',
      '#title' => $this->t('Choose where to link this field'),
      '#options' => array(
        // 'none' => $this->t('Don\'t Link this Field'),
        'page' => $this->t('Link to Event Page'),
        'registration' => t('Link to Event Registration'),
        // 'config' => $this->t('Link to Event Configuration'),
        // 'participants' => t('Link to Event Participants'),
        // 'custom' => $this->t('Link to a Custom Node'),
      ),
      '#default_value' => $this->options['link_to_civicrm_event'] ?? 'page',
    );

    $form['link_text'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Text to display'),
      '#default_value' => $this->options['link_text'],
      '#description' => 'Token is available'
    ];
  }


  public function render(ResultRow $row)
  {
    // ksm($row);
    // $access = $this->checkUrlAccess($row);
    // $build = ['#markup' => $access->isAllowed() ? $this->renderLink($row) : ''];
    // BubbleableMetadata::createFromObject($access)->applyTo($build);
    $build = ['#markup' => $this->renderLink($row)];

    return $build;
  }


  public function renderLink(ResultRow $row)
  {

    $event_id = $row->id;

    // $this->options['alter']['make_link'] = TRUE;

    $text = !empty($this->options['link_text']) ? $this->sanitizeValue($this->tokenizeValue($this->options['link_text'])) : $this->getDefaultLabel();
    switch ($this->options['link_to_civicrm_event']) {
      case 'registration':
        $link = Link::fromTextAndUrl($text, CoreUrl::fromUserInput('/civicrm/event/register', [
          'query' => [
            'reset' => 1,
            'id' => $event_id
          ],
          'attributes' => $this->prepareLinkAttr()
        ]))->toString();
        break;
      default:
        $link = Link::fromTextAndUrl($text, CoreUrl::fromUserInput('/civicrm/event/info', [
          'query' => [
            'reset' => 1,
            'id' => $event_id
          ],
          'attributes' => $this->prepareLinkAttr()
        ]))->toString();
        break;
    }

    return $link;
  }

  protected function prepareLinkAttr()  {
    $attr=[];
    if (!$this->options['alter']['make_link']) {
      return [];
    }
    $altOpts = $this->options['alter'];

    if($altOpts['target']){
      $attr['target'] = $this->tokenizeValue($altOpts['target']);
    }
    if ($altOpts['link_class']) {
      $attr['class'] = $this->tokenizeValue($altOpts['link_class']);
    }
    if ($altOpts['alt']) {
      $attr['title'] = $this->tokenizeValue($altOpts['alt']);
    }
    if ($altOpts['rel']) {
      $attr['rel'] = $this->tokenizeValue($altOpts['rel']);
    }

    return $attr;
  }

  /**
   * Gets the current active user.
   *
   * @todo: https://www.drupal.org/node/2105123 put this method in
   *   \Drupal\Core\Plugin\PluginBase instead.
   *
   * @return \Drupal\Core\Session\AccountInterface
   *   The current user.
   */
  protected function currentUser()
  {
    if (!$this->currentUser) {
      $this->currentUser = \Drupal::currentUser();
    }
    return $this->currentUser;
  }

  /**
   * @todo Add access control here.
   * Checks access to the link route.
   *
   * @param \Drupal\views\ResultRow $row
   *   A view result row.
   *
   * @return \Drupal\Core\Access\AccessResultInterface
   *   The access result.
   */
  protected function checkUrlAccess(ResultRow $row)
  {
    return true;
    // $url = $this->getUrlInfo($row);
    // return $this->accessManager->checkNamedRoute($url->getRouteName(), $url->getRouteParameters(), $this->currentUser(), TRUE);
  }
}
