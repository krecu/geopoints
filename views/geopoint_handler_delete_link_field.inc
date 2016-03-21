<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying deletes links
 * as fields
 */


class geopoint_handler_delete_link_field extends geopoint_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};
    
    //Creating a dummy model to check access against
    $dummy_geopoint = (object) array('type' => $type);
    if (!geopoint_access('edit', $dummy_geopoint)) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $pid = $values->{$this->aliases['pid']};
    
    return l($text, 'admin/content/geopoints/geopoint/' . $pid . '/delete');
  }
}
