<?php

/**
 * @file
 * Defines Workbench Post utility functions
 */

/**
 * Returns an array of user type objects based on the user
 * types defined in 'User types to configure' on Settings tab
 *
 * If the user type object has already been configured we return that
 * from the database.  Otherwise we return a new user object with default settings
 *
 * @return array
 */
function workbench_post_get_config_user_types() {
  $config = workbench_post_get_config('workbench_post_users');
  $config = $config ? $config : workbench_post_get_user_types();
  $defaults = workbench_post_get_settings_default_user_types();
  $all_users = workbench_post_get_all_user_types();

  $config_users = array_map(function($user_type) use ($config, $all_users) {
    // map the default form config options to saved user type objects from db
    if ($config[$user_type]) {
      return $config[$user_type];
    };
    // maybe we've added a new user to configure since last saving to the database
    // so map the default form config options to default user objects
    if ($all_users[$user_type]) {
      return $all_users[$user_type];
    }
    return NULL;
  }, $defaults);

  return $config_users;
}

/**
 * Returns an array of user type objects based on the user
 * types defined in 'User types to configure' on Settings tab
 *
 * This creates new user type objects with default settings
 *
 * @return array
 */
function workbench_post_get_user_types() {
  $default_users = workbench_post_get_settings_default_user_types();
  $all_users = workbench_post_get_all_user_types();
  $users = array_filter($all_users, function($user) use ($default_users) {
    return $default_users[$user->user_string];
  });
  return $users;
}


/**
 * Returns default values for 'User types to configure' on Settings tab
 *
 * @return array of strings
 */
function workbench_post_get_settings_default_user_types() {
  $users = workbench_post_get_config('user_types');
  if ($users) {
    $users = array_filter($users, function($item) {
      return $item;
    });
    return $users;
  }
  else {
    $node_users_by_string = array_map(function($user_type) {
      return $user_type->user_string;
    }, workbench_post_get_node_user_types());

    return $node_users_by_string;
  }
}

/**
 * Returns a Role User objects for all authenticated user roles
 * defined on site
 *
 * @return array
 */
function workbench_post_get_role_user_types() {
  $roles = user_roles(TRUE);
  $users = array();
  foreach ($roles as $rid => $label) {
    $role_user  = new RoleUserType($rid);
    $users[$role_user->user_string] = $role_user;
  }
  return $users;
}

/**
 * Returns a Node User object for each user type that is defined
 * by its relationship to a node - Author, and Revision Author
 *
 * @return array
 */
function workbench_post_get_node_user_types() {
  $revision_author = new RevisionAuthorType('revision author');
  $author = new AuthorType('author');

  $users = array();
  $users[$revision_author->user_string] = $revision_author;
  $users[$author->user_string] = $author;

  return $users;
}

/***
 * Returns user type object for all Node Users and Role Users
 *
 * @return array
 */
function workbench_post_get_all_user_types() {
  $node_user_types = workbench_post_get_node_user_types();
  // TODO: uncomment to add role user options once they are properly supported
  //$role_user_types = _get_role_user_types();
  //$users = array_merge($node_user_types, $role_user_types);
  $users = $node_user_types;
  return $users;
}


/**
 * Returns moderation transition options
 *
 * @return array
 */
function workbench_post_get_moderation_transition_options() {

  $transition_options = workbench_moderation_transitions();
  $moderation_transition_options = array();

  foreach ($transition_options as $option) {
    $transition_string = $option->from_name . '_to_' . $option->to_name;
    $transition_label = str_replace('_', ' ', $transition_string);
    $moderation_transition_options[$transition_string] = $transition_label;
  }

  return $moderation_transition_options;
}

/**
 * Gets the default email subject line for a given transition
 *
 * @param $transition
 * @return string
 */
function workbench_post_get_default_subject($transition) {
  $default = constant(strtoupper('DEFAULT_SUBJECT_' . $transition));
  return $default ? $default : '';
}

/**
 * Get the default email body for a given transition and user type (most user
 * types use same default text)
 *
 * @param $user_string
 * @param $transition
 * @return string
 */
function workbench_post_get_default_body($user_string, $transition) {
  $default = constant(strtoupper('DEFAULT_BODY_' . $user_string . '_' . $transition));
  $fallback = constant(strtoupper('DEFAULT_BODY_' . $transition));
  $fallback = $fallback ? $fallback : '';
  return $default ? $default : $fallback;
}

/**
 * Save form data to the custom table
 *
 * @param $name
 * @param $data
 */
function workbench_post_set_config($name, $data) {
  $data = serialize($data);

  db_merge('workbench_post')
    ->key(array('data_name' => $name))
    ->fields(array(
      'data' => $data,
    ))
    ->execute();
}

/**
 * Get form data from the custom table
 *
 * @param $name
 * @return mixed
 */
function workbench_post_get_config($name) {
  $result = db_select('workbench_post', 'w')
    ->condition('data_name', $name , '=')
    ->fields('w', array('data'))
    ->execute()
    ->fetchAssoc();

  return unserialize($result['data']);
}

/**
 * Get sending email config from database, return site mail if not defined.
 *
 * @return mixed|null
 */
function workbench_post_get_email_from() {
  $email_from = workbench_post_get_config('email_from');
  return $email_from ? $email_from : variable_get('site_mail', '');
}
