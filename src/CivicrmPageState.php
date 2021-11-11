<?php

namespace Drupal\civicrm;

class CivicrmPageState {
  protected $title = '';
  protected $css = array();
  protected $js = array();
  protected $breadcrumbs = array();
  protected $accessDenied = FALSE;
  protected $html_headers = array();
  protected $html_headers_meta = array();

  public function setTitle($title) {
    $this->title = $title;
  }

  public function getTitle() {
    return $this->title;
  }

  public function addCSS(array $css) {
    $this->css[] = $css;
  }

  public function getCSS() {
    return $this->css;
  }

  public function addJS($script, $type) {
    $this->js[$type][] = $script;
  }

  public function getJS() {
    return $this->js;
  }

  public function addBreadcrumb($name, $url) {
    $this->breadcrumbs[$name] = $url;
  }

  public function resetBreadcrumbs() {
    $this->breadcrumbs = array();
  }

  public function getBreadcrumbs() {
    return $this->breadcrumbs;
  }

  /**
   * Add markup text to stacked variable: $html_headers.
   *
   * @param string $html The complete html tags string.
   *
   * @return void
   * @access public
   */
  public function addHtmlHeaderMarkup($html) {
    if (is_string($html)) {
      $this->html_headers[] = $html;
    }
  }

  /**
   * Save tag array to stacked variable: $html_headers_meta.
   *
   * @param string $meta The tag array, which likes: 
   * [
   *   '#tag' => 'style',
   *   '#attribute' => [
   *     'type' => 'text/css',
   *   ],
   *   '#value => '@import url(xxx.css)',
   * ]
   *
   * @return void
   * @access public
   */
  public function addHtmlHeaderMeta($meta) {
    if (is_array($meta)){
      $this->html_headers_meta[] = $meta;
    }
  }

  public function getHtmlHeaders() {
    return implode(' ', $this->html_headers);
  }

  public function getHtmlHeadersMeta() {
    return $this->html_headers_meta;
  }

  public function setAccessDenied() {
    $this->accessDenied = TRUE;
  }

  public function isAccessDenied() {
    return $this->accessDenied;
  }
}
