<?php

/**
 * UI controller.
 */
class RoutesTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
		$items[$this->path]['description'] = 'Manage routes entity types, including adding
		and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the model type editing form.
 */
function routes_type_form($form, &$form_state, $routes_type, $op = 'edit') {

  if ($op == 'clone') {
		$routes_type->label .= ' (cloned)';
		$routes_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $routes_type->label,
    '#description' => t('The human-readable name of this routes type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($routes_type->type) ? $routes_type->type : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'routes_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this routes type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['data']['#tree'] = TRUE;

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function routes_type_form_submit(&$form, &$form_state) {
	$routes_type = entity_ui_form_submit_build_entity($form, $form_state);
	$routes_type->save();
  $form_state['redirect'] = 'admin/structure/routes';
}

/**
 * Form API submit callback for the delete button.
 */
function routes_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/routes/manage/' . $form_state['routes_type']->type . '/delete';
}
