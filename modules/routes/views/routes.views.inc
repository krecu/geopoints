<?php

/**
 * Implements hook_views_data()
 */
function routes_views_data_alter(&$data) {

  $data['routes']['link_routes'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the routes.'),
      'handler' => 'routes_handler_link_field',
    ),
  );
  $data['routes']['edit_routes'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the routes.'),
      'handler' => 'routes_handler_edit_link_field',
    ),
  );
  $data['routes']['delete_routes'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the routes.'),
      'handler' => 'routes_handler_delete_link_field',
    ),
  );
  $data['routes']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this routes.'),
      'handler' => 'routes_handler_routes_operations_field',
    ),
  );

}


/**
 * Implements hook_views_default_views().
 */
function routes_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'routes';
  $view->description = 'A list of all routes';
  $view->tag = 'routes';
  $view->base_table = 'routes';
  $view->human_name = 'routes';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'routes';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create any routes type';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'rid' => 'rid',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'rid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty ';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'No routes have been created yet';

  /* Field: routes: routes ID */
  $handler->display->display_options['fields']['rid']['id'] = 'rid';
  $handler->display->display_options['fields']['rid']['table'] = 'routes';
  $handler->display->display_options['fields']['rid']['field'] = 'rid';
  $handler->display->display_options['fields']['rid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['rid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['rid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['rid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['rid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['rid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['rid']['empty_zero'] = 0;

  /* Field: routes: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'routes';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;

	/* Field: routes: duration */
	$handler->display->display_options['fields']['duration']['id'] = 'duration';
	$handler->display->display_options['fields']['duration']['table'] = 'routes';
	$handler->display->display_options['fields']['duration']['field'] = 'duration';
	$handler->display->display_options['fields']['duration']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['duration']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['duration']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['duration']['alter']['external'] = 0;
	$handler->display->display_options['fields']['duration']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['duration']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['duration']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['duration']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['duration']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['duration']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['duration']['alter']['html'] = 0;
	$handler->display->display_options['fields']['duration']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['duration']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['duration']['hide_empty'] = 0;
	$handler->display->display_options['fields']['duration']['empty_zero'] = 0;

	/* Field: routes: distance */
	$handler->display->display_options['fields']['distance']['id'] = 'distance';
	$handler->display->display_options['fields']['distance']['table'] = 'routes';
	$handler->display->display_options['fields']['distance']['field'] = 'distance';
	$handler->display->display_options['fields']['distance']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['distance']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['distance']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['distance']['alter']['external'] = 0;
	$handler->display->display_options['fields']['distance']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['distance']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['distance']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['distance']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['distance']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['distance']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['distance']['alter']['html'] = 0;
	$handler->display->display_options['fields']['distance']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['distance']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['distance']['hide_empty'] = 0;
	$handler->display->display_options['fields']['distance']['empty_zero'] = 0;

	/* Field: routes: Type */
	$handler->display->display_options['fields']['type']['id'] = 'type';
	$handler->display->display_options['fields']['type']['table'] = 'routes';
	$handler->display->display_options['fields']['type']['field'] = 'type';
	$handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['type']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['type']['alter']['external'] = 0;
	$handler->display->display_options['fields']['type']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['type']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['type']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['type']['alter']['html'] = 0;
	$handler->display->display_options['fields']['type']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['type']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['type']['hide_empty'] = 0;
	$handler->display->display_options['fields']['type']['empty_zero'] = 0;

  /* Field: routes: Link */
  $handler->display->display_options['fields']['link_routes']['id'] = 'link_routes';
  $handler->display->display_options['fields']['link_routes']['table'] = 'routes';
  $handler->display->display_options['fields']['link_routes']['field'] = 'link_routes';
  $handler->display->display_options['fields']['link_routes']['label'] = 'View';
  $handler->display->display_options['fields']['link_routes']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['link_routes']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['link_routes']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['link_routes']['alter']['external'] = 0;
  $handler->display->display_options['fields']['link_routes']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['link_routes']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['link_routes']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['link_routes']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['link_routes']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['link_routes']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['link_routes']['alter']['html'] = 0;
  $handler->display->display_options['fields']['link_routes']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['link_routes']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['link_routes']['hide_empty'] = 0;
  $handler->display->display_options['fields']['link_routes']['empty_zero'] = 0;

  /* Field: routes: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'routes';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['external'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['html'] = 0;
  $handler->display->display_options['fields']['operations']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['operations']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['operations']['hide_empty'] = 0;
  $handler->display->display_options['fields']['operations']['empty_zero'] = 0;
  
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'routes_admin_page');
  $handler->display->display_options['path'] = 'admin/content/routes/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'Routes';
  $handler->display->display_options['tab_options']['description'] = 'Manage routes';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['routes'] = array(
    t('Master'),
    t('routes'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('Empty '),
    t('No geopoints have been created yet'),
    t('routes ID'),
    t('.'),
    t(','),
    t('Name'),
    t('View'),
    t('Operations links'),
    t('Page'),
  );
  $views[] = $view;
  return $views;

}
