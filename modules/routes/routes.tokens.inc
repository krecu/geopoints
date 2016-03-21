<?php

/**
 * Implements hook_token_info().
 *
 * @ingroup token_example
 */
function routes_token_info() {

	$info['types']['routes'] = array(
		'name' => t('Routes'),
		'description' => t('Routes'),
		'needs-data' => 'name',
	);

	$info['tokens']['routes']['full-pid1-path'] = array(
		'name' => t('Full first point'),
		'description' => '',
	);
	$info['tokens']['routes']['full-pid2-path'] = array(
		'name' => t('Full second point'),
		'description' => '',
	);
	return $info;
}

/**
 * Implements hook_tokens().
 */
function routes_tokens($type, $tokens, array $data = array(), array $options = array()) {
	$replacements = array();
	$sanitize = !empty($options['sanitize']);

	// Text format tokens.
	if ($type == 'routes' && !empty($data['routes'])) {
		$routes = $data['routes'];

		foreach ($tokens as $name => $original) {

			switch ($name) {

				case 'full-pid1-path' :
					$pid1_url = drupal_get_path_alias('geopoint/'.$routes->pid1);
					$pid1_url = str_replace('geopoint/', '', $pid1_url);
					$replacements[$original] = $pid1_url;
					break;
				case 'full-pid2-path' :
					$pid2_url = drupal_get_path_alias('geopoint/'.$routes->pid2);
					$pid2_url = str_replace('geopoint/', '', $pid2_url);
					$replacements[$original] = $pid2_url;
					break;
			}
		}
	}

	return $replacements;
}
