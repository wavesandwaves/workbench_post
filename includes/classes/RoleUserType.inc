<?php

/**
 * @file
 * Class definition for the Role User type, users that are defined by their role
 */

class RoleUserType extends UserType implements EmailUserInterface {

  private $rid;

  function __construct($rid) {
    $user_label = user_role_load($rid)->name;
    parent::__construct($user_label);
  }

  private function createEmails($config) {
    $users = $this->loadUsers($this->rid);
    $emails = array();

    foreach ($users as $user_object) {
      $emails[] = $this->buildEmail($user_object, $config);
    }

    return $emails;
  }

  private function loadUsers($rid, $active_user = TRUE) {
    // TODO: load users
    $users = array();

    return $users;
  }

  public function email($node, $config) {
    // TODO check user has permission against node ?
    return $this->create_emails($config);
  }
}
