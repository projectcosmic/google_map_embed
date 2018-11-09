<?php

/**
 * @file
 * Hooks related to views.
 */

/**
 * Implements hook_views_data_alter().
 */
function google_map_embed_views_data_alter(&$data) {
  if (module_exists('location')) {
    $data['location']['google_map_embed_embed'] = array(
      'title' => t('Google Map from Address (Embed API)'),
      'help' => t('The address as an embedded Google map.'),
      'field' => array(
        'field' => 'lid',
        'handler' => 'google_map_embed_location_address_handler',
        'element type' => 'div',
      ),
    );
  }
}
