<?php

namespace Drupal\civicrm\Routing;

use \Symfony\Component\Routing\Route;
use \Symfony\Component\Routing\RouteCollection;
use \Drupal\civicrm\CivicrmHelper;

class Routes {
  public function routes() {
    $collection = new RouteCollection();

    // Initialize CiviCRM.
    \Drupal::service('civicrm')->initialize();

    $items = \CRM_Core_Menu::items();

    // CiviCRM doesn't list optional path components. So we include 5 optional components for each route,
    // and let each default to empty string.
    $permissions = [];
    foreach ($items as $path => $item) {
      $requirement = [];

      if (!empty($item['access_arguments'])) {
        $perm = '';
        $operator = '+';
        if (!empty($item['access_arguments'][1])) {
          $operator = $item['access_arguments'][1] == 'and' ? ',' : '+';
        }
        if (is_array($item['access_arguments'][0])) {
          $perm = implode($operator, $item['access_arguments'][0]);
        }
        else {
          $perm = $item['access_arguments'][0];
        }
        $requirement['_permission'] = $perm;
        $permissions[$path] = $requirement['_permission'];
      }
      elseif ($item['access_callback'] == 1) {
        $requirement['_access'] = 'TRUE';
      }
      elseif ($item['is_public']) {
        $requirement['_access'] = 'TRUE';
      }
      else {
        $itemsLeft[$path] = $item;
        continue;
      }
      
      $route = new Route(
        '/' . $path . '/{extra}',
        [
          '_title' => isset($item['title']) ? $item['title'] : 'CiviCRM',
          '_controller' => 'Drupal\civicrm\Controller\CivicrmController::main',
          'args' => explode('/', $path),
          'extra' => '',
        ],
        $requirement
      );
      $route_name = CivicrmHelper::parseURL($path)['route_name'];
      $collection->add($route_name, $route);
    }

    // items that doesn't have permission setting
    // test parent path to get permission
    foreach ($itemsLeft as $path => $item) {
      $requirement = [];
      $tmpPath = $path;
      $strOccr = substr_count($tmpPath, '/');
      for($i = 0; $i < $strOccr; $i++) {
        $tmpPath =  substr($tmpPath, 0, strrpos($tmpPath, '/'));
        if (!empty($permissions[$tmpPath])) {
          $requirement['_permission'] = $permissions[$tmpPath];
          break;
        }
      }
      if (!empty($requirement)) {
        $route = new Route(
          '/' . $path . '/{extra}',
          [
            '_title' => isset($item['title']) ? $item['title'] : 'CiviCRM',
            '_controller' => 'Drupal\civicrm\Controller\CivicrmController::main',
            'args' => explode('/', $path),
            'extra' => '',
          ],
          $requirement
        );
        $route_name = CivicrmHelper::parseURL($path)['route_name'];
        $collection->add($route_name, $route);
      }
    }

    return $collection;
  }
}
