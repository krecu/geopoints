<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying links to entities
 * as fields.
 */

class geopoint_handler_index_field extends views_handler_field {
	function construct() {
		parent::construct();
		$this->additional_fields['pid'] = 'pid';
	}

	function option_definition() {
		$options = parent::option_definition();

		$options['text'] = array('default' => '', 'translatable' => TRUE);

		return $options;
	}

	function options_form(&$form, &$form_state) {
		parent::options_form($form, $form_state);

		$form['text'] = array(
			'#type' => 'textfield',
			'#title' => t('Text to display'),
			'#default_value' => $this->options['text'],
		);
	}


	function query() {
		$this->ensure_my_table();
		$this->add_additional_fields();

		$field = str_replace('_geopoint', '', $this->real_field);
		// join 'users' to this table via vid
		$join = new views_join();
		$join->construct('geopoint_index', $this->table_alias, 'pid', 'pid');
		$join->extra = array(array('field' => 'type', 'operator' => '=', 'value' => $field));

		$this->geopoint_index = $this->query->ensure_table('geopoint_index', $this->relationship, $join);
		$this->index_name = $this->query->add_field($this->geopoint_index, 'name');


	}

	function render($values) {
//		$text = !empty($this->options['text']) ? $this->options['text'] : t('view');
//		$pid = $values->{$this->aliases['index_name']};

//		dsm($this->index_name);

		return $values->{$this->index_name};
	}
}
