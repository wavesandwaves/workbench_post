<?php

/**
 * @file
 * Class definition for the node Revision Author user type.
 */

class RevisionAuthorType extends NodeUserType  {

  protected function loadUser($node) {
    $node_revision = node_load($node->nid, $node->vid);
    return user_load($node_revision->revision_uid);
  }

}
