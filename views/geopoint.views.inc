<?php

/**
 * @file
 * Providing extra functionality for the Model UI via views.
 */

/**
 * Implements of hook_views_data().
 */
//function geopoint_views_data() {
//	$data['geopoint']['radius'] = array(
//		'group' => t('Geopoint'),
//		'title' => t('Geopoint radius filter'),
//		'help' => t('Filter geopoints by radius'),
//		'filter' => array('handler' => 'geopoint_handler_filter_radius'),
//	);
//
//	return $data;
//}

/**
 * Implements hook_views_data()
 */
function geopoint_views_data_alter(&$data) {


	$data['geopoint']['link_geopoint'] = array(
		'field' => array(
			'title' => t('Link'),
			'help' => t('Provide a link to the geopoint.'),
			'handler' => 'geopoint_handler_link_field',
		),
	);

  $data['geopoint']['link_geopoint'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the geopoint.'),
      'handler' => 'geopoint_handler_link_field',
    ),
  );
  $data['geopoint']['edit_geopoint'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the geopoint.'),
      'handler' => 'geopoint_handler_edit_link_field',
    ),
  );
  $data['geopoint']['delete_geopoint'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the geopoint.'),
      'handler' => 'geopoint_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows geopoints/geopoint/%geopoint_id/op
  $data['geopoint']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this geopoint.'),
      'handler' => 'geopoint_handler_geopoint_operations_field',
    ),
  );

	$index_field = geopoint_index_get_field();
	foreach ($index_field as $item) {
		$data['geopoint'][$item.'_geopoint'] = array(
			'field' => array(
				'title' => t($item),
				'help' => t('Provide a '.$item.' to the geopoint.'),
				'handler' => 'geopoint_handler_index_field',
			),
		);
	}

	$data['geopoint']['pid']['filter'] = array(
		'title' => t('Geopoint radius filter'),
		'handler' => 'geopoint_handler_radius_filter',
	);
}


/**
 * Implements hook_views_default_views().
 */
function geopoint_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'geopoints';
  $view->description = 'A list of all geopoints';
  $view->tag = 'geopoints';
  $view->base_table = 'geopoint';
  $view->human_name = 'geopoints';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'geopoints';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create any geopoint type';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'pid' => 'pid',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'pid' => array(
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
  $handler->display->display_options['empty']['area']['content'] = 'No geopoints have been created yet';
  /* Field: geopoint: geopoint ID */
  $handler->display->display_options['fields']['pid']['id'] = 'pid';
  $handler->display->display_options['fields']['pid']['table'] = 'geopoint';
  $handler->display->display_options['fields']['pid']['field'] = 'pid';
  $handler->display->display_options['fields']['pid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['pid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['pid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['pid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['pid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['pid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['pid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['pid']['empty_zero'] = 0;
  /* Field: geopoint: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'geopoint';
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

	/* Field: geopoint: Short Name */
	$handler->display->display_options['fields']['short']['id'] = 'short';
	$handler->display->display_options['fields']['short']['table'] = 'geopoint';
	$handler->display->display_options['fields']['short']['field'] = 'short';
	$handler->display->display_options['fields']['short']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['short']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['short']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['short']['alter']['external'] = 0;
	$handler->display->display_options['fields']['short']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['short']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['short']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['short']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['short']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['short']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['short']['alter']['html'] = 0;
	$handler->display->display_options['fields']['short']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['short']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['short']['hide_empty'] = 0;
	$handler->display->display_options['fields']['short']['empty_zero'] = 0;

	/* Field: geopoint: Type */
	$handler->display->display_options['fields']['type']['id'] = 'type';
	$handler->display->display_options['fields']['type']['table'] = 'geopoint';
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

  /* Field: geopoint: Link */
  $handler->display->display_options['fields']['link_geopoint']['id'] = 'link_geopoint';
  $handler->display->display_options['fields']['link_geopoint']['table'] = 'geopoint';
  $handler->display->display_options['fields']['link_geopoint']['field'] = 'link_geopoint';
  $handler->display->display_options['fields']['link_geopoint']['label'] = 'View';
  $handler->display->display_options['fields']['link_geopoint']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['link_geopoint']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['link_geopoint']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['link_geopoint']['alter']['external'] = 0;
  $handler->display->display_options['fields']['link_geopoint']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['link_geopoint']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['link_geopoint']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['link_geopoint']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['link_geopoint']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['link_geopoint']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['link_geopoint']['alter']['html'] = 0;
  $handler->display->display_options['fields']['link_geopoint']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['link_geopoint']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['link_geopoint']['hide_empty'] = 0;
  $handler->display->display_options['fields']['link_geopoint']['empty_zero'] = 0;
  /* Field: geopoint: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'geopoint';
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
  $handler = $view->new_display('page', 'Page', 'geopoints_admin_page');
  $handler->display->display_options['path'] = 'admin/content/geopoints/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'geopoints';
  $handler->display->display_options['tab_options']['description'] = 'Manage geopoints';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['geopoints'] = array(
    t('Master'),
    t('geopoints'),
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
    t('geopoint ID'),
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
