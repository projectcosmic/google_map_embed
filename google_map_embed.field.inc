<?php

/**
 * @file
 * Field API hook implementations.
 */

/**
 * Implements hook_field_formatter_info().
 */
function google_map_embed_field_formatter_info() {
  $formats = array(
    'google_map_embed_embed' => array(
      'label' => t('Map (Embed API)'),
      'description' => t('Google Map using the Embed API.'),
      'field types' => array('google_map_field', 'addressfield'),
      'settings' => array(
        'classes' => '',
        'width'   => '',
        'height'  => '',
      ),
    ),
  );
  return $formats;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function google_map_embed_field_formatter_settings_form($field, $instance, $view_mode) {
  $settings = $instance['display'][$view_mode]['settings'];

  $elements['classes'] = array(
    '#type' => 'textfield',
    '#title' => t('Classes'),
    '#description' => t('Classes to apply to the embed <code>&lt;iframe></code> element.'),
    '#default_value' => $settings['classes'],
  );

  $elements['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('<code>&lt;iframe></code> <code>width</code> attribute value.'),
    '#default_value' => $settings['width'],
  );

  $elements['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('<code>&lt;iframe></code> <code>height</code> attribute value.'),
    '#default_value' => $settings['height'],
  );

  return $elements;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function google_map_embed_field_formatter_settings_summary($field, $instance, $view_mode) {
  $settings = $instance['display'][$view_mode]['settings'];
  $summary = [];

  if ($settings['classes']) {
    $summary[] = t('Classes: @classes', array('@classes' => $settings['classes']));
  }
  if ($settings['width']) {
    $summary[] = t('Width: @width', array('@width' => $settings['width']));
  }
  if ($settings['height']) {
    $summary[] = t('Height: @height', array('@height' => $settings['height']));
  }

  return implode('<br/>', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function google_map_embed_field_formatter_view($object_type, $object, $field, $instance, $langcode, $items, $display) {
  $settings = $display['settings'];
  $attributes = array(
    'frameborder' => '0',
    'style' => 'border:0',
    'allowfullscreen' => TRUE,
  );

  if ($settings['classes']) {
    $attributes['class'] = explode(' ', $settings['classes']);
  }
  if ($settings['width']) {
    $attributes['width'] = $settings['width'];
  }
  if ($settings['height']) {
    $attributes['height'] = $settings['height'];
  }

  $element = array();
  foreach ($items as $delta => $item) {
    $element[$delta] = array(
      '#theme' => 'html_tag',
      '#tag' => 'iframe',
      '#attributes' => array(
        'src' => _google_map_embed_build_url_from_field($item, $field['type']),
      ) + $attributes,
      '#value' => '',
    );
  }
  return $element;
}

/**
 * Builds a Google Embed API iframe src URL for a field value.
 *
 * @param array $item
 *   A field value.
 * @param string $field_type
 *   The field type that $item was from.
 *
 * @return string
 *   The Google Embed API iframe src URL.
 */
function _google_map_embed_build_url_from_field(array $item, $field_type) {
  static $api_key;
  if (!isset($api_key)) {
    $api_key = variable_get('google_map_embed_api_key', '');
  }

  $parameters = '';
  switch ($field_type) {
    case 'google_map_field':
      $parameters = "$item[lat],$item[lon]&zoom=$item[zoom]";
      break;

    case 'addressfield':
      $components = array();
      $component_order = array(
        'name_line',
        'first_name',
        'last_name',
        'organisation_name',
        'premise',
        'sub_premise',
        'thoroughfare',
        'administrative_area',
        'sub_administrative_area',
        'locality',
        'dependent_locality',
        'postal_code',
      );
      foreach ($component_order as $key) {
        if ($item[$key]) {
          $components[] = urlencode($item[$key]);
        }
      }
      $parameters = implode(',', $components);
  }

  return "https://www.google.com/maps/embed/v1/place?q=$parameters&key=$api_key";
}