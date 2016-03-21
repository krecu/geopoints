<?php

/**
 * Implements hook_token_info().
 *
 * @ingroup token_example
 */
function geopoint_token_info() {

  $info['types']['geopoint'] = array(
    'name' => t('GeoPoint'),
    'description' => t('Geopoint'),
    'needs-data' => 'name',
  );

  foreach (geopoint_get_types() as $geopoint_type => $geopoint_obj) {
    $info['tokens']['geopoint'][$geopoint_type] = array(
      'name' => t($geopoint_type),
      'description' => t("Geopoint " . $geopoint_type),
    );
  }
  $info['tokens']['geopoint']['full-path'] = array(
    'name' => t('Full geo'),
    'description' => t("Geopoint full geo address"),
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function geopoint_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);

  // Text format tokens.
  if ($type == 'geopoint' && !empty($data['geopoint'])) {
    $geopoint = $data['geopoint'];

    foreach ($tokens as $name => $original) {

      switch ($name) {

        case 'full-path' :
          $shema = array();
          foreach ($geopoint->shema as $pid) {
            $point = geopoint_load($pid);
            $short = strtolower($point->short);
            $short = transliteration_get($short);

            $shema[] = $short;
          }
          $replacements[$original] = implode("/", $shema);

          break;

        default :
          $val = db_select('geopoint_index', 'gi')
            ->condition('gi.pid', $geopoint->pid)
            ->condition('gi.type', $name)
            ->fields('gi', array('pid_original'))
            ->execute()
            ->fetchField();
          if ($val) {
            $pid_original = geopoint_load($val);
            $replacements[$original] = $sanitize ? filter_xss($pid_original->short) : $pid_original->short;
          }
      }
    }
  }

  return $replacements;
}