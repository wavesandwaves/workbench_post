<?php

/**
 * @file
 * Base class for User Types
 */

class UserType {

  public $user_label;
  public $user_string;
  public $user_notifications;
  public $emails;

  function __construct($user_label, $user_notifications = NULL, $emails = NULL) {
    $this->user_label = t($user_label);
    $this->user_string = strtolower(str_replace(' ', '_', $this->user_label));
    $this->user_notifications = $user_notifications;
    $this->emails = $emails;
  }

  protected function buildEmail($user_object, $transition_name) {
    $email = array(
      'user_object' => $user_object,
      'email_to' => $user_object->mail,
      'subject' => $this->getEmailSubject($transition_name),
      'body' => $this->getEmailBody($transition_name),
    );

    return $email;
  }

  public function getEmailSubject($transition) {
    return isset($this->emails[$transition]['subject'])
      ? $this->emails[$transition]['subject']
      : workbench_post_get_default_subject($transition);
  }

  public function getEmailBody($transition) {
    return isset($this->emails[$transition]['body'])
      ? $this->emails[$transition]['body']
      : workbench_post_get_default_body($this->user_string, $transition);
  }

  public function notifyOnTransition($transition_name) {
    if ($this->user_notifications) {
      return $this->user_notifications[$transition_name];
    }
    return FALSE;
  }

}
