<?php

/**
 * Implements hook_path_alias_types().
 */
function routes_path_alias_types() {
	return array('routes/' => t('routes'));
}

/**
 * Implements hook_pathauto().
 */
function routes_pathauto($op) {
	switch ($op) {
		case 'settings':
			$settings = array();
			$settings['module'] = 'routes';
			$settings['token_type'] = 'routes';
			$settings['groupheader'] = t('Routes paths');
			$settings['patterndescr'] = t('Default path pattern');
			$settings['patterndefault'] = 'routes/[routes:name]';
			$settings['batch_update_callback'] = 'routes_pathauto_bulk_update_batch_process';
			$settings['batch_file'] = drupal_get_path('module', 'routes') . '/routes.pathauto.inc';

			foreach (routes_get_types() as $routes_type => $routes_name) {
				$settings['patternitems'][$routes_type] = t('Pattern for all @type paths', array('@type' => $routes_type));
			}

			return (object) $settings;
		default:
			break;
	}
}

/**
 * Batch processing callback; Generate aliases for nodes.
 */
function routes_pathauto_bulk_update_batch_process(&$context) {
	if (!isset($context['sandbox']['current'])) {
		$context['sandbox']['count'] = 0;
		$context['sandbox']['current'] = 0;
	}

	$query = db_select('routes', 'r');
	$query->leftJoin('url_alias', 'ua', "CONCAT('routes/', r.rid) = ua.source");
	$query->addField('r', 'rid');
	$query->isNull('ua.source');
	$query->condition('r.rid', $context['sandbox']['current'], '>');
	$query->orderBy('r.rid');
	$query->addTag('pathauto_bulk_update');
	$query->addMetaData('entity', 'routes');

	// Get the total amount of items to process.
	if (!isset($context['sandbox']['total'])) {
		$context['sandbox']['total'] = $query->countQuery()->execute()->fetchField();

		// If there are no nodes to update, the stop immediately.
		if (!$context['sandbox']['total']) {
			$context['finished'] = 1;
			return;
		}
	}

	$query->range(0, 25);
	$rids = $query->execute()->fetchCol();

	routes_update_alias_multiple($rids, 'bulkupdate');
	$context['sandbox']['count'] += count($rids);
	$context['sandbox']['current'] = max($rids);
	$context['message'] = t('Updated alias for premise @rid.', array('@rid' => end($rids)));

	if ($context['sandbox']['count'] != $context['sandbox']['total']) {
		$context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
	}

}

/**
 * Update the URL aliases for an individual routes.
 *
 * @param $routes
 *   A routes object.
 * @param $op
 *   Operation being performed on the routes ('insert', 'update' or 'bulkupdate').
 * @param $options
 *   An optional array of additional options.
 */
function routes_update_alias(Routes $routes, $op, array $options = array()) {
	// Skip processing if the user has disabled pathauto for the geopoint.
	if (isset($routes->path['pathauto']) && empty($routes->path['pathauto'])) {
		return;
	}

	// Skip processing if the geopoint has no pattern.
	if (!pathauto_pattern_load_by_entity('routes', $routes->type)) {
		return;
	}

	module_load_include('inc', 'pathauto');
	$uri = entity_uri('routes', $routes);
	pathauto_create_alias('routes', $op, $uri['path'], array('routes' => $routes), $routes->type);
}

/**
 * Update the URL aliases for multiple premises.
 *
 * @param $pids
 *   An array of premise IDs.
 * @param $op
 *   Operation being performed on the nodes ('insert', 'update' or
 *   'bulkupdate').
 * @param $options
 *   An optional array of additional options.
 */
function routes_update_alias_multiple(array $rids, $op, array $options = array()) {
	$options += array('message' => FALSE);

	$premises = routes_load_multiple($rids);
	foreach ($premises as $routes) {
		routes_update_alias($routes, $op, $options);
	}

	if (!empty($options['message'])) {
		drupal_set_message(format_plural(count($rids), 'Updated URL alias for 1 premise.', 'Updated URL aliases for @count premises.'));
	}
}