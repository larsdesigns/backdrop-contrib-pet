<?php

/**
 * @file
 * Rules integration for the PET module.
 */

/**
 * Implements hook_rules_action_info().
 */
function pet_rules_action_info() {
  $actions = array(
    'pet_action_send_pet' => array(
      'label' => t('Send PET mail'),
      'group' => t('Previewable email templates'),
      // When declaring parameters, site builders will by default get a form to
      // select fitting data. In this case, we require the site builder to
      // select a user object. The 'type' could be any data type known to Rules,
      // which means any entity type known to Entity API or any additional data
      // type declared by hook_rules_data_info – by default including things
      // like 'integer', 'text' and more. See findme
      'parameter' => array(
        'pet_name' => array(
          'type' => 'text',
          'label' => t('The previewable email template to use'),
          'options list' => 'pet_pets_list',
          'description' => t('The template that will be sent for this action. You can see the full list or add a new one from <a href="@url">this page</a>.', array('@url' => url('admin/structure/pets'))),
        ),
        'account' => array(
          'type' => 'user',
          'label' => t('User for token substitutions'),
          'description' => t('If your template includes user tokens, this user will be used for them.'),
        ),
        'node' => array(
          'type' => 'node',
          'label' => t('Node for token substitutions'),
          'description' => t('If your template includes node tokens, this node will be used for them.'),
        ),
      ),
    ),
  );

  return $actions;
}

/**
 * Callback for eponymous rules action.
 */
function pet_action_send_pet($pet_name, $account, $node) {
  flog_it($account, '$account');
  flog_it($node, '$node');
  $pet = pet_load($pet_name);
  flog_it($pet, '$pet');
  $params = array(
    'pet_from' => variable_get('site_mail', ini_get('sendmail_from')),
    'pet_to' => 'daspin@gmail.com',
    'pet_uid' => $account->uid,
    'pet_nid' => $node->nid,
  );
  pet_send_one_mail($pet, $params);
}

/**
 * Return list of all PETs for rules configuration.
 */
function pet_pets_list() {
  $list = array();
  foreach (pet_load_multiple(FALSE) as $pet) {
    $list[$pet->name] = $pet->title;
  }
  return $list;
}