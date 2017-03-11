<?php

/**
 * @file
 * Interface definition for user types that send emails.
 */

interface EmailUserInterface {
  public function email($node, $config);
}
