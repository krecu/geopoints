<?php

/**
 * @file
 * Model type editing UI.
 */

/**
 * UI controller.
 */
class GeoPointTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
		$items[$this->path]['description'] = 'Manage geopoint entity types, including adding
		and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the model type editing form.
 */
function geopoint_type_form($form, &$form_state, $geopoint_type, $op = 'edit') {

  if ($op == 'clone') {
		$geopoint_type->label .= ' (cloned)';
		$geopoint_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $geopoint_type->label,
    '#description' => t('The human-readable name of this geopoint type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($geopoint_type->type) ? $geopoint_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $model_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'geopoint_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this geopoint type. It must only contain lowercase letters, numbers, and underscores.'),
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
function geopoint_type_form_submit(&$form, &$form_state) {
	$geopoint_type = entity_ui_form_submit_build_entity($form, $form_state);
	$geopoint_type->save();
  $form_state['redirect'] = 'admin/structure/geopoint';
}

/**
 * Form API submit callback for the delete button.
 */
function geopoint_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/geopoint/manage/' . $form_state['geopoint_type']->type . '/delete';
}
