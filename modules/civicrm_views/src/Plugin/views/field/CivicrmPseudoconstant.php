<?php

namespace Drupal\civicrm_views\Plugin\views\field;

use Drupal\civicrm\Civicrm;
use Drupal\Core\Database\Connection;
use Drupal\core\form\FormStateInterface;
use Drupal\views\Plugin\views\field\FieldPluginBase;
use Drupal\views\ResultRow;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * @Todo: offer to display raw value or human friendly value
 *
 * @ingroup views_field_handlers
 * @ViewsField("civicrm_pseudoconstant")
 */
class CivicrmPseudoconstant extends FieldPluginBase {
  protected $pseudovalues = array();
  protected $html_type='';

  public function __construct(array $configuration, $plugin_id, $plugin_definition, Civicrm $civicrm,Connection $conn) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);

    $civicrm->initialize();
    $this->html_type=$this->definition['pseudo info']['html_type'];
    if(key_exists('pseudo arguments',$this->definition)){
      if(is_array($this->definition['pseudo arguments']) && key_exists('custom_field_id', $this->definition['pseudo arguments'])){
        $query=$conn->query('select value k,label v from civicrm_option_value where option_group_id=:og_id',[':og_id'=>$this->definition['pseudo info']['pseudoconstant']]);
        $this->pseudovalues=$query->fetchAllKeyed();
      }else{
        $this->pseudovalues = call_user_func_array($this->definition['pseudo callback'], $this->definition['pseudo arguments']);
      }
    }
  }

  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('civicrm'),
      $container->get('database')
    );
  }

  protected function defineOptions() {
    $options = parent::defineOptions();

    if($this->html_type=='File'){
      $options['file_display_format'] = array('default' => 'url');
    }

    if(key_exists('pseudo arguments',$this->definition)){
      $options['pseudoconstant_format'] = array('default' => 'raw');
    }

    if($this->html_type=='Multi-Select'){
      $options['value_separator'] = array('default' => ' , ');
    }
    return $options;
  }

  public function buildOptionsForm(&$form, FormStateInterface $form_state) {
    if($this->html_type=='File'){
      $form['file_display_format'] = array(
        '#type' => 'select',
        '#title' => t('Display format'),
        '#description' => t("Choose how to display this file."),
        '#options' => array(
          'raw' => t('Raw value (file id)'),
          'url' => t('Entity File URL (limited with permission)'),
          'image' => t('Entity File with img tag (limited with permission)'),
          'url_real' => t('Image URL (public accessable URL, image only)'),
          'image_real' => t('Image Tag (public accessable URL, image only)'),
        ),
        '#default_value' => isset($this->options['file_display_format']) ? $this->options['file_display_format'] : 'url',
      );
    }

    if(key_exists('pseudo arguments',$this->definition)){
      $form['pseudoconstant_format'] = array(
        '#type' => 'radios',
        '#title' => t('Display format'),
        '#description' => t("Choose how to display this field. 'Raw' will display this field as it is stored in the database, eg. as a number. 'Human friendly' will attempt to turn this raw value into something meaningful."),
        '#options' => array(
          'raw' => t('Raw value'),
          'pseudoconstant' => t('Human friendly'),
        ),
        '#default_value' => isset($this->options['pseudoconstant_format']) ? $this->options['pseudoconstant_format'] : 'raw',
      );
    }

    if($this->html_type=='Multi-Select'){
      $form['value_separator'] = [
        '#title' => $this->t('Value separator'),
        '#type' => 'textfield',
        '#size' => 120,
        '#default_value' => $this->options['value_separator'],
        '#description' => $this->t('Separator between values'),
      ];
    }

    parent::buildOptionsForm($form, $form_state);
  }

  public function render(ResultRow $values) {
    $val = $this->getValue($values);
    if($this->html_type=='File'){
      return $this->renderFile($values);
    }

    $values = $this->sanitizeValue($this->getValue($values));

    if (isset($this->options['pseudoconstant_format']) && $this->options['pseudoconstant_format'] == 'pseudoconstant') {
      $values = str_replace(array_keys($this->pseudovalues),array_values($this->pseudovalues),$values);

      if($this->html_type=='Multi-Select'){
        $output=$values;
        if($this->options['value_separator']){
          $output=str_replace("",$this->options['value_separator'],trim($values," \n\r\t\v\0"));
        }
        return ['#markup'=>$output];
      }

    }

    // Return raw value either if pseudoconstant_format is set to raw or, for some reason,
    // the raw value doesn't exist as a key in the $this->pseudovalues array.
    return $values;
  }

  protected function renderFile(ResultRow $values){
    $file_id = $this->getValue($values);
    $entity = \CRM_Core_BAO_File::getEntity($file_id);
    if (intval($file_id) && intval($entity['entity_id'])) {
      if ($this->options['file_display_format'] == 'raw') {
        return $file_id;
      }
      $entity_file = \CRM_Core_BAO_File::getEntityFile($entity['entity_table'], $entity['entity_id']);
      $file = $entity_file[$file_id];
      switch ($this->options['file_display_format']){
        case 'url':
          return $file['url'];
          break;
        case 'image':
          if ($file['img']) { // mimetype already checked
            return ['#markup'=> $file['img']];
          }
          break;
        case 'url_real':
          if ($file['img']) { // mimetype already checked, caution this will be accessable by public
            return $file['url_real'];
          }
          break;
        case 'image_real':
          if ($file['img']) { // mimetype already checked, caution this will be accessable by public
            return ['#markup'=> $file['img_real']];
          }
          break;
        case 'raw':
        default:
          return $file_id;
      }
    }
  }
}
