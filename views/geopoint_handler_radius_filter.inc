<?php

class geopoint_handler_radius_filter extends views_handler_filter {

	// exposed filter options
	var $always_multiple = TRUE;

	function option_definition() {
		$options = parent::option_definition();

		$options['expose']['contains']['required'] = array('default' => FALSE);

		return $options;
	}

	/**
	 * This kind of construct makes it relatively easy for a child class
	 * to add or remove functionality by overriding this function and
	 * adding/removing items from this array.
	 */
	function operators() {
		$operators = array(
			'in' => array(
				'title' => t('In radius'),
				'short' => t('In'),
				'method' => 'op_in',
				'values' => 1,
			),
			'not' => array(
				'title' => t('Not in radius'),
				'short' => t('Not in'),
				'method' => 'op_not',
				'values' => 1,
			),
		);
		return $operators;
	}

	/**
	 * Build strings from the operators() for 'select' options
	 */
	function operator_options($which = 'title') {
		$options = array();
		foreach ($this->operators() as $id => $info) {
			$options[$id] = $info[$which];
		}

		return $options;
	}

	function admin_summary() {
		if (!empty($this->options['exposed'])) {
			return t('exposed');
		}

		$options = $this->operator_options('short');
		$output = '';
		if(!empty($options[$this->operator])) {
			$output = $options[$this->operator];
		}
		if (in_array($this->operator, $this->operator_values(1))) {
			$output .= ' ' . $this->value;
		}
		return $output;
	}

	function operator_values($values = 1) {
		$options = array();
		foreach ($this->operators() as $id => $info) {
			if (isset($info['values']) && $info['values'] == $values) {
				$options[] = $id;
			}
		}

		return $options;
	}


	/**
	 * Provide a simple textfield for equality
	 */
	function value_form(&$form, &$form_state) {
		$form['value']['#tree'] = TRUE;


		$form['value']['radius'] = array(
			'#type' => 'textfield',
			'#title' => t('Radius'),
			'#size' => 30,
			'#default_value' => $this->value['radius'],
		);
		$form['value']['latitude'] = array(
			'#type' => 'textfield',
			'#title' => t('Latitude'),
			'#size' => 30,
			'#default_value' => $this->value['latitude'],
		);
		$form['value']['longitude'] = array(
			'#type' => 'textfield',
			'#title' => t('Longitude'),
			'#size' => 30,
			'#default_value' => $this->value['longitude'],
		);

		if (!isset($form['value'])) {
			$form['value'] = array(
				'#type' => 'value',
				'#value' => NULL
			);
		}
	}


	function query() {
		$this->ensure_my_table();

		$X1 = $this->value['latitude'];
		$Y1 = $this->value['longitude'];
		$R = $this->value['radius'];

		if ( ($X1!='0') & ($Y1!='0') & ($R!='0')) {
			$distance = "sqrt( (({$X1} - {$this->table_alias}.latitude)*({$X1} - {$this->table_alias}.latitude)) + (({$Y1} - {$this->table_alias}.longitude)*({$Y1} - {$this->table_alias}.longitude)) ) * 100";
			$this->query->add_where_expression($this->options['group'], "{$distance} <= {$R}");
		 }
	}


	function op_in($field) {
	}

	function op_not($field) {
	}

}