<?php

/**
 * @file
 * Field handler to display a location address as an embedded Google map.
 */

// @codingStandardsIgnoreStart
class google_map_embed_location_address_handler extends location_handler_field_location_address {

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();

    // Map settings.
    $options['map'] = array(
      'contains' => array(
        'classes' => array('default' => ''),
        'width'   => array('default' => ''),
        'height'  => array('default' => ''),
      ),
    );

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['map'] = array(
      '#type' => 'fieldset',
      '#title' => t('Embedded map settings'),
      'classes' => array(
        '#type' => 'textfield',
        '#title' => t('Classes'),
        '#description' => t('Classes to apply to the embed <code>&lt;iframe></code> element.'),
        '#default_value' => $this->options['map']['classes'],
      ),
      'width' => array(
        '#type' => 'textfield',
        '#title' => t('Width'),
        '#description' => t('<code>&lt;iframe></code> <code>width</code> attribute value.'),
        '#default_value' => $this->options['map']['width'],
      ),
      'height' => array(
        '#type' => 'textfield',
        '#title' => t('Height'),
        '#description' => t('<code>&lt;iframe></code> <code>height</code> attribute value.'),
        '#default_value' => $this->options['map']['height'],
      ),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function render($values) {
    if (!$values->{$this->field_alias}) {
      return;
    }

    $location = location_load_location($values->{$this->field_alias});

    if ($location['lid']) {
      module_load_include('inc', 'google_map_embed', 'google_map_embed.field');

      // Hiding certain fields in the config removes them from the geocoding
      // place query that is built later.
      if (is_array($this->options['hide'])) {
        foreach (array_keys(array_filter($this->options['hide'])) as $key) {
          unset($location[$key]);
          // Special case for coords.
          if ($key == 'coords') {
            unset($location['latitude']);
            unset($location['longitude']);
          }
        }
      }

      if ($place = _google_map_embed_get_field_place($location, 'location')) {
        $attributes = array();
        $settings = $this->options['map'];

        if ($settings['classes']) {
          $attributes['class'] = explode(' ', $settings['classes']);
        }
        if ($settings['width']) {
          $attributes['width'] = $settings['width'];
        }
        if ($settings['height']) {
          $attributes['height'] = $settings['height'];
        }

        return theme(
          'google_map_embed',
          array(
            'place' => $place,
            'attributes_array' => $attributes,
          )
        );
      }
    }
  }

}
// @codingStandardsIgnoreEnd
