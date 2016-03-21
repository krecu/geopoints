<?php

/**
 * UI controller.
 */
class RoutesUIController extends EntityDefaultUIController {
	public function hook_menu() {
		$items = array();
		$id_count = count(explode('/', $this->path));
		$wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;
		$items[$this->path] = array(
			'title' => 'Routes',
			'description' => 'Add edit and update routes.',
			'page callback' => 'system_admin_menu_block_page',
			'access arguments' => array('access administration pages'),
			'file path' => drupal_get_path('module', 'system'),
			'file' => 'system.admin.inc',
		);
		$items[$this->path]['type'] = MENU_LOCAL_TASK;
		$items[$this->path . '/add'] = array(
			'title' => 'Add a routes',
			'description' => 'Add a new routes',
			'page callback'  => 'routes_add_page',
			'access callback'  => 'routes_access',
			'access arguments' => array('edit'),
			'type' => MENU_NORMAL_ITEM,
			'weight' => 20,
			'file' => 'routes.admin.inc',
			'file path' => drupal_get_path('module', $this->entityInfo['module'])
		);
		foreach (routes_get_types() as $type) {
			$items[$this->path . '/add/' . $type->type] = array(
				'title' => 'Add ' . $type->label,
				'page callback' => 'routes_form_wrapper',
				'page arguments' => array(routes_create(array('type' => $type->type))),
				'access callback' => 'routes_access',
				'access arguments' => array('edit', 'edit ' . $type->type),
				'file' => 'routes.admin.inc',
				'file path' => drupal_get_path('module', $this->entityInfo['module'])
			);
		}

		$items[$this->path . '/routes/' . $wildcard] = array(
			'page callback' => 'routes_form_wrapper',
			'page arguments' => array($id_count + 1),
			'access callback' => 'routes_access',
			'access arguments' => array('edit', $id_count + 1),
			'weight' => 0,
			'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
			'file' => 'routes.admin.inc',
			'file path' => drupal_get_path('module', $this->entityInfo['module'])
		);

		$items[$this->path . '/' . $wildcard . '/edit'] = array(
			'title' => 'Edit',
			'type' => MENU_DEFAULT_LOCAL_TASK,
			'weight' => -10,
			'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
		);

		$items[$this->path . '/' . $wildcard . '/delete'] = array(
			'title' => 'Delete',
			'page callback' => 'routes_delete_form_wrapper',
			'page arguments' => array($id_count + 1),
			'access callback' => 'routes_access',
			'access arguments' => array('edit', $id_count + 1),
			'type' => MENU_LOCAL_TASK,
			'context' => MENU_CONTEXT_INLINE,
			'weight' => 10,
			'file' => 'routes.admin.inc',
			'file path' => drupal_get_path('module', $this->entityInfo['module'])
		);

		$items['routes/' . $wildcard] = array(
			//'title' => 'Title',
			'title callback' => 'routes_page_title',
			'title arguments' => array(1),
			'page callback' => 'routes_page_view',
			'page arguments' => array(1),
			'access callback' => 'routes_access',
			'access arguments' => array('view', 1),
			'type' => MENU_CALLBACK,
		);
		return $items;
	}

	public function addPage() {
		$item = menu_get_item();
		$content = system_admin_menu_block($item);

		if (count($content) == 1) {
			$item = array_shift($content);
			drupal_goto($item['href']);
		}
		return theme('routes_add_list', array('content' => $content));
	}

}


/**
 * Form callback wrapper: create or edit a routes.
 */
function routes_form_wrapper($routes) {
	routes_set_breadcrumb();
	return drupal_get_form('routes_edit_form', $routes);
}


/**
 * Form callback wrapper: delete a routes.
 */
function routes_delete_form_wrapper($routes) {
	routes_set_breadcrumb();
	return drupal_get_form('routes_delete_form', $routes);
}


/**
 * Form callback: create or edit a routes.
 */
function routes_edit_form($form, &$form_state, $routes) {

	if ($path = libraries_get_path('geocomplete')) {
		drupal_add_js('http://maps.googleapis.com/maps/api/js?libraries=places&ln=ru&sensor=false', array('type' => 'external'));
		drupal_add_js($path . '/jquery.geocomplete.min.js');
		drupal_add_js('
		jQuery(document).ready(function($){
			$(".geocomplete").geocomplete();
		});
		', array('type' => 'inline'));
	}

	$form['name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name'),
		'#default_value' => isset($routes->name) ? $routes->name : '',
		'#maxlength' => 255,
		'#required' => TRUE,
		'#weight' => -10,
	);

	$form['pid1'] = array(
		'#attributes' => array('class' => array('geocomplete')),
		'#type' => 'textfield',
		'#title' => t('GeoPoint from'),
		'#default_value' => isset($routes->pid1->name) ? $routes->pid1->name : '',
		'#maxlength' => 255,
		'#required' => TRUE,
		'#weight' => -5,
	);
	$form['pid2'] = array(
		'#attributes' => array('class' => array('geocomplete')),
		'#type' => 'textfield',
		'#title' => t('GeoPoint to'),
		'#default_value' => isset($routes->pid2->name) ? $routes->pid2->name : '',
		'#maxlength' => 255,
		'#required' => TRUE,
		'#weight' => -4,
	);

	$form_state['routes'] = $routes;
	field_attach_form('routes', $routes, $form, $form_state);

	$form['actions'] = array(
		'#type' => 'container',
		'#attributes' => array('class' => array('form-actions')),
		'#weight' => 400,
	);
	$submit = array();

	if (!empty($form['#submit'])) {
		$submit += $form['#submit'];
	}

	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save'),
		'#submit' => $submit + array('routes_edit_form_submit'),
	);

	if (!empty($routes->name)) {
		$form['actions']['delete'] = array(
			'#type' => 'submit',
			'#value' => t('Delete'),
			'#suffix' => l(t('Cancel'), 'admin/content/routes'),
			'#submit' => $submit + array('routes_form_submit_delete'),
			'#weight' => 45,
		);
	}

	$form['#validate'][] = 'routes_edit_form_validate';
	return $form;
}


/**
 * Form API validate callback for the model form
 */
function routes_edit_form_validate(&$form, &$form_state) {
	$routes = $form_state['routes'];
	field_attach_form_validate('routes', $routes, $form, $form_state);
}


/**
 * Form API submit callback for the model form.
 */
function routes_edit_form_submit(&$form, &$form_state) {
	$routes = entity_ui_controller('routes')->entityFormSubmitBuildEntity($form, $form_state);
	$routes->save();
	$form_state['redirect'] = 'admin/content/routes';
}

/**
 * Form API submit callback for the delete button.
 */
function routes_form_submit_delete(&$form, &$form_state) {
	$form_state['redirect'] = 'admin/content/routes/' . $form_state['routes']->rid . '/delete';
}


/**
 * Form callback: confirmation form for deleting a model.
 */
function routes_delete_form($form, &$form_state, $routes) {
	$form_state['routes'] = $routes;

	$form['#submit'][] = 'routes_delete_form_submit';

	$form = confirm_form($form,
		t('Are you sure you want to delete model %name?', array('%name' => $routes->name)),
		'admin/content/routes',
		'<p>' . t('This action cannot be undone.') . '</p>',
		t('Delete'),
		t('Cancel'),
		'confirm'
	);

	return $form;
}

/**
 * Submit callback for routes_delete_form
 */
function routes_delete_form_submit($form, &$form_state) {
	$routes = $form_state['routes'];
	routes_delete($routes);
	drupal_set_message(t('The routes %name has been deleted.', array('%name' => $routes->name)));
	watchdog('routes', 'Deleted routes %name.', array('%name' => $routes->name));
	$form_state['redirect'] = 'admin/content/routes';
}



/**
 * Page to add routes Entities.
 */
function routes_add_page() {
	$controller = entity_ui_controller('routes');
	return $controller->addPage();
}


/**
 * Displays the list of available model types for model creation.
 */
function theme_routes_add_list($variables) {
	$content = $variables['content'];
	if ($content) {
		$output = '<dl class="routes-type-list">';
		foreach ($content as $item) {
			$output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
			$output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
		}
		$output .= '</dl>';
	}
	else {
		if (user_access('administer routes types')) {
			$output = '<p>' . t('Routes Entities cannot be added because you have not created any model types yet. Go to the <a href="@routes-type-list">model type creation page</a> to add a new model type.', array('@routes-type-list' => url('admin/structure/routes/add'))) . '</p>';
		}
		else {
			$output = '<p>' . t('No routes types have been created yet for you to use.') . '</p>';
		}
	}

	return $output;
}


/**
 * Sets the breadcrumb for administrative model pages.
 */
function routes_set_breadcrumb() {
	$breadcrumb = array(
		l(t('Home'), '<front>'),
		l(t('Administration'), 'admin'),
		l(t('Content'), 'admin/content'),
		l(t('Routes'), 'admin/content/routes'),
	);
	drupal_set_breadcrumb($breadcrumb);
}



