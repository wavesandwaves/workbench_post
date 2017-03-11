<?php

/**
 * @file
 * Class definition for Node User types, user types that are defined by their
 * relationship to a given node
 */

abstract class NodeUserType extends UserType implements EmailUserInterface  {

  protected function createEmail($node, $transition_name) {
    $user_object = $this->loadUser($node);
    return $this->buildEmail($user_object, $transition_name);
  }

  public function email($node, $transition_name) {
    return $this->createEmail($node, $transition_name);
  }

  abstract protected function loadUser($node);
}
