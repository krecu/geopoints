<?php

/**
 * @file
 * Model editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class GeoPointUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {

    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'GeoPoints',
      'description' => 'Add edit and update geopoints.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );

    // Change the overview menu type for the list of models.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;

    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a geopoint',
      'description' => 'Add a new geopoint',
      'page callback'  => 'geopoint_add_page',
      'access callback'  => 'geopoint_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'geopoint.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );

    // Add menu items to add each different type of entity.
    foreach (geopoint_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'geopoint_form_wrapper',
        'page arguments' => array(geopoint_create(array('type' => $type->type))),
        'access callback' => 'geopoint_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'geopoint.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing model entities
    $items[$this->path . '/geopoint/' . $wildcard] = array(
      'page callback' => 'geopoint_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'geopoint_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'geopoint.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items[$this->path . '/geopoint/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );

    $items[$this->path . '/geopoint/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'geopoint_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'geopoint_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'geopoint.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );

    // Menu item for viewing models
    $items['geopoint/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'geopoint_page_title',
      'title arguments' => array(1),
      'page callback' => 'geopoint_page_view',
      'page arguments' => array(1),
      'access callback' => 'geopoint_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }
  
  
  /**
   * Create the markup for the add Model Entities page within the class
   * so it can easily be extended/overriden.
   */ 
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }    
    return theme('geopoint_add_list', array('content' => $content));
  }
  
}


/**
 * Form callback wrapper: create or edit a model.
 *
 * @param $model
 *   The model object being edited by this form.
 *
 * @see model_edit_form()
 */
function geopoint_form_wrapper($geopoints) {
  // Add the breadcrumb for the form's location.
	geopoint_set_breadcrumb();
  return drupal_get_form('geopoint_edit_form', $geopoints);
}


/**
 * Form callback wrapper: delete a model.
 *
 * @param $model
 *   The model object being edited by this form.
 *
 * @see model_edit_form()
 */
function geopoint_delete_form_wrapper($geopoint) {
  // Add the breadcrumb for the form's location.
  //model_set_breadcrumb();
  return drupal_get_form('geopoint_delete_form', $geopoint);
}


/**
 * Form callback: create or edit a model.
 *
 * @param $model
 *   The model object to edit or for a create form an empty model object
 *     with only a model type defined.
 */
function geopoint_edit_form($form, &$form_state, $geopoint) {

	if ($path = libraries_get_path('geocomplete')) {
		drupal_add_js('http://maps.googleapis.com/maps/api/js?libraries=places&ln=ru&sensor=false', array('type' => 'external'));
		drupal_add_js($path . '/jquery.geocomplete.min.js');
		drupal_add_js('
		jQuery(document).ready(function($){
			$("#geocomplete").geocomplete();
		});
		', array('type' => 'inline'));
	}


  // Add the default field elements.
  $form['name'] = array(
		'#attributes' => array('id' => 'geocomplete'),
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => isset($geopoint->name) ? $geopoint->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );

	$form['latitude'] = array(
		'#type' => 'hidden',
		'#title' => t('Latitude'),
		'#default_value' => isset($geopoint->latitude) ? $geopoint->latitude : '',
		'#maxlength' => 255,
		'#weight' => -5,
	);

	$form['longitude'] = array(
		'#type' => 'hidden',
		'#title' => t('longitude'),
		'#default_value' => isset($geopoint->longitude) ? $geopoint->longitude : '',
		'#maxlength' => 255,
		'#weight' => -5,
	);

	$form['short'] = array(
		'#type' => 'hidden',
		'#title' => t('Short name'),
		'#default_value' => isset($geopoint->short) ? $geopoint->short : '',
		'#maxlength' => 255,
		'#weight' => -5,
	);

	// Add the field related form elements.
  $form_state['geopoint'] = $geopoint;
  field_attach_form('geopoint', $geopoint, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => $submit + array('geopoint_edit_form_submit'),
  );
  
  if (!empty($geopoint->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#suffix' => l(t('Cancel'), 'admin/content/geopoints'),
      '#submit' => $submit + array('geopoint_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'geopoint_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the model form
 */
function geopoint_edit_form_validate(&$form, &$form_state) {
	$geopoint = $form_state['geopoint'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('geopoint', $geopoint, $form, $form_state);
}


/**
 * Form API submit callback for the model form.
 * 
 * @todo remove hard-coded link
 */
function geopoint_edit_form_submit(&$form, &$form_state) {

	$geopoint = entity_ui_controller('geopoint')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the model and go back to the list of models

	$geopoint->save();
  $form_state['redirect'] = 'admin/content/geopoints';
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function geopoint_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/geopoints/geopoint/' . $form_state['geopoint']->pid . '/delete';
}


/**
 * Form callback: confirmation form for deleting a model.
 *
 * @param $model
 *   The model to delete
 *
 * @see confirm_form()
 */
function geopoint_delete_form($form, &$form_state, $geopoint) {
  $form_state['geopoint'] = $geopoint;

  $form['#submit'][] = 'geopoint_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete model %name?', array('%name' => $geopoint->name)),
    'admin/content/geopoint',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  
  return $form;
}

/**
 * Submit callback for model_delete_form
 */
function geopoint_delete_form_submit($form, &$form_state) {
  $geopoint = $form_state['geopoint'];

	geopoint_delete($geopoint);

  drupal_set_message(t('The geopoint %name has been deleted.', array('%name' => $geopoint->name)));
  watchdog('geopoint', 'Deleted model %name.', array('%name' => $geopoint->name));

  $form_state['redirect'] = 'admin/content/geopoints';
}



/**
 * Page to add geopoint Entities.
 *
 * @todo Pass this through a proper theme function
 */
function geopoint_add_page() {
  $controller = entity_ui_controller('geopoint');
  return $controller->addPage();
}


/**
 * Displays the list of available model types for model creation.
 *
 * @ingroup themeable
 */
function theme_geopoint_add_list($variables) {
  $content = $variables['content'];
  $output = 'sdfsdf';
  if ($content) {
    $output = '<dl class="geopoint-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer geopoint types')) {
      $output = '<p>' . t('Model Entities cannot be added because you have not created any model types yet. Go to the <a href="@create-geopoint-type">model type creation page</a> to add a new model type.', array('@create-geopoint-type' => url('admin/structure/geopoint/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No geopoint types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}





/**
 * Sets the breadcrumb for administrative model pages.
 */
function geopoint_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('GeoPoint'), 'admin/content/geopoints'),
  );

  drupal_set_breadcrumb($breadcrumb);
}



