<?php

/**
 * This field handler aggregates operations that can be done on a model
 * under a single field providing a more flexible way to present them in a view
 */
class routes_handler_routes_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['rid'] = 'rid';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
		//&todo check callback
		$links = array();
//    $links = menu_contextual_links('routes', 'routes', array($this->get_value($values, 'rid')));
//    dsm($links);


    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
