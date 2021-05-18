<?php

namespace Drupal\civicrm_views\Plugin\views\field;

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
class CivicrmEventLInk extends FieldPluginBase {



  /**
   * {@inheritdoc}
   */
  protected function getDefaultLabel(){
    return $this->options['link_to_civicrm_event']=='page'?$this->t('View'): $this->t('Regiter');
  }

  /**
   * {@inheritdoc}
   */
  protected function defineOptions(){
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
  public function buildOptionsForm(&$form, FormStateInterface $form_state){
    parent::buildOptionsForm($form, $form_state);

    // The path is set by ::renderLink() so we do not allow to set it.
    $form['alter'] += ['path' => [], 'query' => [], 'external' => []];
    $form['alter']['path'] += ['#access' => FALSE];
    $form['alter']['query'] += ['#access' => FALSE];
    $form['alter']['external'] += ['#access' => FALSE];

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
      '#default_value' => $this->options['link_to_civicrm_event']??'page',
    );

    $form['link_text'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Text to display'),
      '#default_value' => $this->options['link_text'],
      '#description'=>'Token is available'
    ];
  }


  public function render(ResultRow $row){
    // ksm($row);
    // $access = $this->checkUrlAccess($row);
    // $build = ['#markup' => $access->isAllowed() ? $this->renderLink($row) : ''];
    // BubbleableMetadata::createFromObject($access)->applyTo($build);
    $build = ['#markup' => $this->renderLink($row)];

    return $build;
  }


public function renderLink(ResultRow $row){
    // $value = $this->getValue($row,'id');
    $event_id=$row->id;
    ksm($row,$row->id, $this->options);
    $this->options['alter']['make_link'] = TRUE;
    // $this->options['alter']['url'] = $this->getUrlInfo($row);
    // $text = !empty($this->options['text']) ? $this->sanitizeValue($this->options['text']) : $this->getDefaultLabel();
    // return $text;
    // $this->addLangcode($row);
    $text = !empty($this->options['link_text']) ? $this->sanitizeValue($this->tokenizeValue($this->options['link_text'])) : $this->getDefaultLabel();
    switch ($this->options['link_to_civicrm_event']){
      case 'registration':
        $link = Link::fromTextAndUrl($text, CoreUrl::fromUserInput('/civicrm/event/register', ['query' => [
          'reset' => 1,
          'id' => $event_id
        ]]))->toString();
        break;
      default:
        $link= Link::fromTextAndUrl($text, CoreUrl::fromUserInput('/civicrm/event/info',['query'=>[
          'reset'=>1,
          'id'=> $event_id
        ]]))->toString();
        break;
    }
    ksm($link);

    // $text = !empty($this->options['text']) ? $this->sanitizeValue($this->options['text']) : $this->getDefaultLabel();
    // return $text;

    return $link;
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
  protected function currentUser(){
    if (!$this->currentUser) {
      $this->currentUser = \Drupal::currentUser();
    }
    return $this->currentUser;
  }

  /**
   * Checks access to the link route.
   *
   * @param \Drupal\views\ResultRow $row
   *   A view result row.
   *
   * @return \Drupal\Core\Access\AccessResultInterface
   *   The access result.
   */
  protected function checkUrlAccess(ResultRow $row){
    return true;
    // $url = $this->getUrlInfo($row);
    // return $this->accessManager->checkNamedRoute($url->getRouteName(), $url->getRouteParameters(), $this->currentUser(), TRUE);
  }

}