<?php

/**
 * Implements hook_path_alias_types().
 */
function geopoint_path_alias_types() {
	return array('geopoint/' => t('GeoPoint'));
}

/**
 * Implements hook_pathauto().
 */
function geopoint_pathauto($op) {
	switch ($op) {
		case 'settings':
			$settings = array();
			$settings['module'] = 'geopoint';
			$settings['token_type'] = 'geopoint';
			$settings['groupheader'] = t('GeoPoint paths');
			$settings['patterndescr'] = t('Default path pattern');
			$settings['patterndefault'] = 'geopoint/[geopoint:name]';
			$settings['batch_update_callback'] = 'geopoint_pathauto_bulk_update_batch_process';
			$settings['batch_file'] = drupal_get_path('module', 'geopoint') . '/geopoint.pathauto.inc';

			foreach (geopoint_get_types() as $geopoint_type => $geopoint_name) {
				$settings['patternitems'][$geopoint_type] = t('Pattern for all @type paths', array('@type' => $geopoint_type));
			}

			return (object) $settings;
		default:
			break;
	}
}

/**
 * Batch processing callback; Generate aliases for nodes.
 */
function geopoint_pathauto_bulk_update_batch_process(&$context) {
	if (!isset($context['sandbox']['current'])) {
		$context['sandbox']['count'] = 0;
		$context['sandbox']['current'] = 0;
	}

	$query = db_select('geopoint', 'p');
	$query->leftJoin('url_alias', 'ua', "CONCAT('geopoint/', p.pid) = ua.source");
	$query->addField('p', 'pid');
	$query->isNull('ua.source');
	$query->condition('p.pid', $context['sandbox']['current'], '>');
	$query->orderBy('p.pid');
	$query->addTag('pathauto_bulk_update');
	$query->addMetaData('entity', 'geopoint');

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
	$pids = $query->execute()->fetchCol();

	geopoint_update_alias_multiple($pids, 'bulkupdate');
	$context['sandbox']['count'] += count($pids);
	$context['sandbox']['current'] = max($pids);
	$context['message'] = t('Updated alias for premise @pid.', array('@pid' => end($pids)));

	if ($context['sandbox']['count'] != $context['sandbox']['total']) {
		$context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
	}
}

/**
 * Update the URL aliases for an individual geopoint.
 *
 * @param $geopoint
 *   A geopoint object.
 * @param $op
 *   Operation being performed on the geopoint ('insert', 'update' or 'bulkupdate').
 * @param $options
 *   An optional array of additional options.
 */
function geopoint_update_alias(GeoPoint $geopoint, $op, array $options = array()) {
	// Skip processing if the user has disabled pathauto for the geopoint.
	if (isset($geopoint->path['pathauto']) && empty($geopoint->path['pathauto'])) {
		return;
	}

	// Skip processing if the geopoint has no pattern.
	if (!pathauto_pattern_load_by_entity('geopoint', $geopoint->type)) {
		return;
	}

	module_load_include('inc', 'pathauto');
	$uri = entity_uri('geopoint', $geopoint);
	pathauto_create_alias('geopoint', $op, $uri['path'], array('geopoint' => $geopoint), $geopoint->type);
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
function geopoint_update_alias_multiple(array $pids, $op, array $options = array()) {
	$options += array('message' => FALSE);

	$premises = geopoint_load_multiple($pids);
	foreach ($premises as $geopoint) {
		geopoint_update_alias($geopoint, $op, $options);
	}

	if (!empty($options['message'])) {
		drupal_set_message(format_plural(count($pids), 'Updated URL alias for 1 premise.', 'Updated URL aliases for @count premises.'));
	}
}