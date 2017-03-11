INTRODUCTION
============

The Workbench Post module provides an admin interface for configuring email
notifications for Workbench Moderation state changes.  This module has been
tested with Workbench Moderation versions 1.3 and 3.0


REQUIREMENTS
------------

This module requires the following modules:

 *  Workbench Moderation (https://www.drupal.org/project/workbench_moderation)
 *  Token (https://www.drupal.org/project/token)


INSTALLATION
------------

Install as you would normally install a contributed Drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7 for further
information.


CONFIGURATION
-------------

 * Configure user permissions in Administration » People » Permissions:

    -  Administer Workbench Post notification settings

       The administrative menu requires this permission to be visible. Without
       this permission users will not be able to configure emails.

 * Configure emails and notification settings for users at Administration »
   Configuration » Workbench Post » Emails

 * Configure which type of users will receive emails and other settings at
   Administration » Configuration » Workbench Post » Settings
