<?php

/**
 * @file
 * Contains administrative pages for creating, editing, and deleting previewable email templates (PETs).
 */

/**
 * Generates the model type editing form.
 */
function pet_form($form, &$form_state, $pet, $op = 'edit') {
  if ($op == 'clone') {
    $pet->name .= '_cloned';
    $pet->title .= ' (cloned)';
  }
  
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $pet->title,
    '#description' => t('A short, descriptive title for this email template. It will be used in administrative interfaces, and in page titles and menu items.'),
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Name'),
    '#default_value' => $pet->name,
    '#description' => t('The machine-name for this email template. It may only contain lowercase letters, underscores, and numbers. It will be used in URLs and in all API calls.'),
    '#maxlength' => 64,
    '#machine_name' => array(
      'exists' => 'pet_load',
      'source' => array('title'),
    ),
    '#required' => TRUE,
    '#disabled' => isset($pet->name) && $op != 'clone',
  );
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $pet->subject,
    '#description' => t('The subject line of the email template. May include tokens of any token type specified below.'),
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  $form['mail_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => $pet->body,
    '#description' => t('The body of the email template. May include tokens of any token type specified below.'),
  );
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Additional options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#access' => user_access('administer previewable email templates'),
  );
  $form['advanced']['from_override'] = array(
    '#type' => 'textfield',
    '#title' => t('From override'),
    '#default_value' => $pet->from_override,
    '#description' => t('By default, the From: address is the site address, which is %site_mail and which is configurable on the core <a href="@site_url">site information page</a>. You may specify a different From: address here, which will override the system default for this PET.', array('%site_mail' => variable_get('site_mail', ini_get('sendmail_from')), '@site_url' => url('admin/settings/site-information'))),
    '#maxlength' => 255,
    '#required' => FALSE,
  );
  $form['advanced']['cc_default'] = array(
    '#type' => 'textarea',
    '#title' => t('CC default'),
    '#rows' => 3,
    '#default_value' => $pet->cc_default,
    '#description' => t('Emails to be copied by default for each mail sent to recipient. Enter emails separated by lines or commas.'),
    '#required' => FALSE,
  );
  $form['advanced']['bcc_default'] = array(
    '#type' => 'textarea',
    '#title' => t('BCC default'),
    '#rows' => 3,
    '#default_value' => $pet->bcc_default,
    '#description' => t('Emails to be blind copied by default for each mail sent to recipient. Enter emails separated by lines or commas.'),
    '#required' => FALSE,
  );
  $form['advanced']['recipient_callback'] = array(
    '#type' => 'textfield',
    '#title' => t('Recipient callback'),
    '#default_value' => $pet->recipient_callback,
    '#description' => t('The name of a function which will be called to retrieve a list of recipients. This function will be called if the query parameter uid=0 is in the URL. It will be called with one argument, the loaded node (if the PET takes one) or NULL if not. This function should return an array of recipients in the form uid|email, as in 136|bob@example.com. If the recipient has no uid, leave it blank but leave the pipe in. Providing the uid allows token substitution for the user.'),
    '#maxlength' => 255,
  );
  $form['advanced']['object_types'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom tokens'),
    '#default_value' => $pet->object_types,
    '#description' => t('List of custom token types this template can handle, one per line. Format is type-name|object-it-acts-on, e.g. my-token-type|user. For types that don\'t require an object, leave the object empty, as in HCI global|. All tokens of type user and node are automatically available to all templates.'),
  );
  $form['token_help'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Make sure that the tokens you choose are available to your template when in use.'),
  );
  $form['token_help']['help'] = array(
    '#value' => theme('token_help', 'all'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save template'),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/structure/pets',
  );

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function pet_form_submit(&$form, &$form_state) {
  $pet = entity_ui_form_submit_build_entity($form, $form_state);
  $pet->save();
  $form_state['redirect'] = 'admin/structure/pets';
}

/**
 * Form API submit callback for the delete button.
 */
function pet_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/registration_types/manage/' . $form_state['registration_type']->type . '/delete';
}
