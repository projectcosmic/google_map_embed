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
      'field types' => array(
        'addressfield',
        'google_map_field',
        'location',
      ),
      'settings' => array(
        'classes' => '',
        'width'   => '',
        'height'  => '',
        'zoom'    => 18,
        'use_coordinates' => FALSE,
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

  if ($field['type'] != 'google_map_field') {
    $elements['zoom'] = array(
      '#type' => 'textfield',
      '#title' => t('Zoom Level'),
      '#description' => t('The initial zoom level of the map, between 0 (the whole world) and 21 (individual buildings).'),
      '#default_value' => $settings['zoom'],
      '#required' => TRUE,
      '#size' => 2,
      '#element_validate' => array('_google_map_field_validate_zoom_level'),
    );
  }

  if ($field['type'] == 'location') {
    $elements['use_coordinates'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use coordinates'),
      '#description' => t('Embed will search using coordinates instead of address text fields.'),
      '#default_value' => $settings['use_coordinates'],
    );
  }

  return $elements;
}

/**
 * Validates a form element as a Google Map Embed zoom parameter.
 *
 * @param array $element
 *   The element to validate.
 * @param array $form_state
 *   The current form state.
 */
function _google_map_field_validate_zoom_level(array &$element, array &$form_state) {
  $value = $element['#value'];
  $name = $element['#title'];

  if (empty($value)) {
    return;
  }

  if (!is_numeric($value)) {
    return form_set_error($element, t('%name must be a number.', array('%name' => $name)));
  }

  if (!intval($value)) {
    return form_set_error($element, t('%name must be an integer.', array('%name' => $name)));
  }

  if ($value < 0 || $value > 21) {
    return form_set_error($element, t('%name must be between 0 and 21.', array('%name' => $name)));
  }
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
  if ($field['type'] != 'google_map_field') {
    $summary[] = t('Zoom: @zoom', array('@zoom' => $settings['zoom']));
  }
  if ($field['type'] == 'location') {
    $summary[] = t('Use coordinates: @value', array('@value' => $settings['use_coordinates'] ? t('Yes') : t('No')));
  }

  if ($summary) {
    return implode('<br/>', $summary);
  }

  return t('No settings set');
}

/**
 * Implements hook_field_formatter_view().
 */
function google_map_embed_field_formatter_view($object_type, $object, $field, $instance, $langcode, $items, $display) {
  $settings = $display['settings'];
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

  $options = array(
    'use_coordinates' => isset($settings['use_coordinates']) ? $settings['use_coordinates'] : FALSE,
  );

  $element = array();
  foreach ($items as $delta => $item) {
    $place = _google_map_embed_get_field_place($item, $field['type'], $options);
    if ($place) {
      $element[$delta] = array(
        '#theme' => 'google_map_embed',
        '#place' => $place,
        '#attributes_array' => $attributes,
        '#zoom' => $field['type'] == 'google_map_field' ? $item['zoom'] : $settings['zoom'],
      );
    }
  }
  return $element;
}

/**
 * Builds query string for Google Map Embed API place query.
 *
 * @param array $item
 *   A field value.
 * @param string $field_type
 *   The field type that $item was from.
 * @param array $options
 *   An associative array of options containing:
 *   - use_coordinates: Whether to use coordinates instead of address text field
 *     components in the place query. Only applies to location field types.
 *     Default FALSE.
 *
 * @return string
 *   The place query.
 */
function _google_map_embed_get_field_place(array $item, $field_type, array $options = array()) {
  $options += array('use_coordinates' => FALSE);

  switch ($field_type) {
    case 'google_map_field':
      return "$item[lat],$item[lon]";

    case 'addressfield':
      $components = array();
      $component_order = array(
        'name_line',
        'first_name',
        'last_name',
        'organisation_name',
        'thoroughfare',
        'premise',
        'sub_premise',
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
      return implode(',', $components);

    case 'location':
      if ($options['use_coordinates']) {
        return "$item[latitude],$item[longitude]";
      }

      $components = array();
      $component_order = array(
        'name',
        'street',
        'additional',
        'city',
        'province_name',
        'postal_code',
        'country_name',
      );
      foreach ($component_order as $key) {
        if ($item[$key]) {
          $components[] = urlencode($item[$key]);
        }
      }
      return implode(',', $components);
  }

  return '';
}
