<?php

/**
 * @file
 * Class definition for the node Author user type.
 */

class AuthorType extends NodeUserType {

  protected function loadUser($node) {
    return user_load($node->uid);
  }

}
