<?php
/**
 * @file
 * relationship_to_role_sync.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function relationship_to_role_sync_user_default_roles() {
  $roles = [];

  // Exported role: Case Managers.
  $roles['Case Managers'] = [
    'name' => 'Case Managers',
    'weight' => '3',
  ];

  return $roles;
}
