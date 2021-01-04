<?php

/**
 * @file
 * Block API hook implementations.
 */

/**
 * Implements hook_block_info().
 */
function google_map_embed_block_info() {
  $blocks['map'] = array(
    'info' => t('Google Map (Embed API)'),
  );

  return $blocks;
}

/**
 * Implements hook_block_configure().
 */
function google_map_embed_block_configure($delta = '') {
  $form = array();

  $settings = _google_map_embed_get_block_settings();

  $form['place'] = array(
    '#type' => 'textfield',
    '#title' => t('Place Query'),
    '#description' => t('The place query that specifies the location of the pin.'),
    '#default_value' => $settings['place'],
    '#maxlength' => 256,
  );

  $form['classes'] = array(
    '#type' => 'textfield',
    '#title' => t('Classes'),
    '#description' => t('Classes to apply to the embed <code>&lt;iframe></code> element.'),
    '#default_value' => $settings['classes'],
  );

  $form['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('<code>&lt;iframe></code> <code>width</code> attribute value.'),
    '#default_value' => $settings['width'],
  );

  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('<code>&lt;iframe></code> <code>height</code> attribute value.'),
    '#default_value' => $settings['height'],
  );

  return $form;
}

/**
 * Implements hook_block_save().
 */
function google_map_embed_block_save($delta = '', $edit = array()) {
  // Only save white-listed keys.
  $to_save = array_intersect_key($edit, _google_map_embed_get_block_settings());
  variable_set('google_map_embed_block_map_settings', $to_save);
}

/**
 * Implements hook_block_view().
 */
function google_map_embed_block_view($delta = '') {
  $settings = _google_map_embed_get_block_settings();

  if (!$settings['place']) {
    return array();
  }

  $attributes = array();
  if ($settings['classes']) {
    $attributes['class'] = explode(' ', $settings['classes']);
  }
  if ($settings['width']) {
    $attributes['width'] = $settings['width'];
  }
  if ($settings['height']) {
    $attributes['height'] = $settings['height'];
  }

  return array(
    'content' => array(
      '#theme' => 'google_map_embed',
      '#place' => $settings['place'],
      '#attributes_array' => $attributes,
    ),
  );
}

/**
 * Gets configuration settings for a block with defaults.
 *
 * @return array
 *   The configuration settings.
 */
function _google_map_embed_get_block_settings() {
  return variable_get('google_map_embed_block_map_settings', array()) + array(
    'classes' => '',
    'width'   => '',
    'height'  => '',
    'place'   => '',
  );
}
