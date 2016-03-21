<?php

class routes_handler_edit_link_field extends routes_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};
    
    //Creating a dummy model to check access against
    $routes = (object) array('type' => $type);
    if (!routes_access('edit', $routes)) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $rid = $values->{$this->aliases['$rid']};
    
    return l($text, 'admin/content/routes/' . $rid . '/edit');
  }
}
