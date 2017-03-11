<?php

/**
 * @file
 * Hook_menu and hook_form implementations for Workbench Post admin interface
 */

include_once  'workbench_post.utils.inc';

/**
 * Implements hook_menu
 */
function workbench_post_menu() {

  $items = array();

  $items['admin/config/workbench/post'] = array(
      'title' => 'Workbench Post',
      'description' => 'Configure simple email notifications for Workbench moderation transitions.',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('workbench_post_form_users'),
      'theme callback' => 'variable_get',
      'theme arguments' => array('seven'),
      'access arguments' => array('administer workbench post'),
      'weight' => 100,
      'type' => MENU_NORMAL_ITEM,
  );

  $items['admin/config/workbench/post/emails'] = array(
    'title' => 'Emails',
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $items['admin/config/workbench/post/settings'] = array(
    'title' => 'Settings',
    'description' => 'Configure Workbench Post settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('workbench_post_form_settings'),
    'theme callback' => 'variable_get',
    'theme arguments' => array('seven'),
    'access arguments' => array('administer workbench post'),
    'weight' => 0,
    'type' => MENU_LOCAL_TASK,
  );

  return $items;
}

/**
 * Create email configuration options based on user types
 */
function workbench_post_create_user_email_content_fields(&$form, $user_type, $moderation_transition_options) {

  $user_string = $user_type->user_string;
  $user_label  = $user_type->user_label;

  $form['data'][$user_string]['emails']['#tree'] = TRUE;

    foreach ($moderation_transition_options as $transition => $transitionlabel) {

      $form['data'][$user_string]['emails'][$transition] = array(
          '#type' => 'fieldset',
          '#title' => t('Email to ' . $user_label . ': ' . $transitionlabel),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#states' => array(
              'visible' => array(
                  ':input[name="data[' . $user_string . '][user_notifications][' . $transition . ']"]' => array('checked' => TRUE),
              ),
          ),
      );
      $form['data'][$user_string]['emails'][$transition]['subject'] = array(
          '#type' => 'textfield',
          '#default_value' => $user_type->getEmailSubject($transition),
          '#title' => t('Email subject'),
          '#description' => t('The subject of the email that will be sent to the ' . $user_label . '.'),
          '#size' => 40,
          '#maxlength' => 120,
          '#required' => TRUE,
      );
      $form['data'][$user_string]['emails'][$transition]['body'] = array(
          '#type' => 'textarea',
          '#rows' => 10,
          '#columns' => 40,
          '#default_value' => $user_type->getEmailBody($transition),
          '#title' => t('Email text'),
          '#required' => TRUE,
          '#description' => t('The text of the email that will be sent to the ' . $user_label . '. ' . l(t('Tokens'), '/admin/help/token') . ' can be used in this email,
                       including global tokens and context-specific tokens of node and user'),
      );
    }
}

/**
 * @param $form
 * @param $user_type
 * @param $moderation_transition_options
 */
function workbench_post_create_user_notification_fields(&$form, $user_type, $moderation_transition_options) {

  $form['data'][$user_type->user_string]['user_notifications'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Transitions'),
    '#options' => $moderation_transition_options,
    '#default_value' => $user_type->user_notifications,
  );

}

/**
 * Implements hook_form
 *
 * Creates email content form for Workbench Post 'emails' tab
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */
function workbench_post_form_users($form, &$form_state) {

  $user_types = workbench_post_get_config_user_types();
  $moderation_transition_options = workbench_post_get_moderation_transition_options();

  foreach ($user_types as $user_type) {

    $form['data']['#tree'] = TRUE;
    $form['data'][$user_type->user_string] = array(
      '#type' => 'fieldset',
      '#title' => t('Email notifications for ' . $user_type->user_label),
    );

    workbench_post_create_user_notification_fields($form, $user_type, $moderation_transition_options);

    workbench_post_create_user_email_content_fields($form, $user_type, $moderation_transition_options);
  }

  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save settings'),
  );

  return $form;
}

/**
 * Implements hook_form_submit
 *
 * @param $form
 * @param $form_state
 */
function workbench_post_form_users_submit($form, &$form_state) {

  $user_types = workbench_post_get_user_types();
  $moderation_transition_options = workbench_post_get_moderation_transition_options();
  $transitions = Array();

  foreach ($user_types as $user_type ) {
    // add email and notification config to user type objects
    $user_type->user_notifications = $form_state['values']['data'][$user_type->user_string]['user_notifications'];
    $user_type->emails = $form_state['values']['data'][$user_type->user_string]['emails'];
    // build array of transitions we will send emails for
    foreach ($user_type->user_notifications as $transition) {
      if ($transition) {
        $transitions[$transition] = $transition;
      }
    }
  }
  // save configured user type objects to custom table
  workbench_post_set_config('workbench_post_users', $user_types);

  // save transitions we are using in variables array
  foreach ($moderation_transition_options as $transition => $transitionlabel) {
    if ($transitions[$transition]) {
      variable_set('workbench_post_' . $transition, TRUE);
    }
    else {
      variable_set('workbench_post_' . $transition, FALSE);
    }
  }

  drupal_set_message(t('The settings have been saved'));
}

/**
 * Implements hook_form
 *
 * Creates configuration form for Workbench Post 'settings' tab
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */
function workbench_post_form_settings($form, &$form_state) {

  $default_user_types = workbench_post_get_settings_default_user_types();
  $user_types = workbench_post_get_all_user_types();
  $config_email = workbench_post_get_email_from();

  $user_type_options = array_map(function($user_type) {
    return $user_type->user_label;
  }, $user_types);

  $form['data']['configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration'),
  );
  $form['data']['configuration']['user_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('User types to configure'),
    '#description' => t('Select which users to configure emails for'),
    '#options' => $user_type_options,
    '#default_value' => $default_user_types,
  );
  $form['data']['configuration']['email_from'] = array(
    '#type' => 'textfield',
    '#default_value' => $config_email,
    '#title' => t('Email from'),
    '#description' => t('The address your email will be sent from.'),
    '#size' => 40,
    '#maxlength' => 120,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;

}

/**
 * Implements hook_form_submit
 *
 * @param $form
 * @param $form_state
 */
function workbench_post_form_settings_submit($form, &$form_state) {

  $user_types = $form_state['values']['user_types'];
  $email_from = $form_state['values']['email_from'];

  workbench_post_set_config('user_types', $user_types);
  workbench_post_set_config('email_from', $email_from);

  drupal_set_message(t('The settings have been saved'));
}
