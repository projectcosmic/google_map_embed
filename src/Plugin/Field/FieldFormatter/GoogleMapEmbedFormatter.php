<?php

namespace Drupal\google_map_embed\Plugin\Field\FieldFormatter;

use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Field\FormatterBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Plugin implementation of the 'google_map_embed' formatter.
 *
 * @FieldFormatter(
 *   id = "google_map_embed",
 *   label = @Translation("Google Map (Embed API)"),
 *   field_types = {
 *     "address",
 *     "string",
 *   }
 * )
 */
class GoogleMapEmbedFormatter extends FormatterBase {

  /**
   * {@inheritdoc}
   */
  public static function defaultSettings() {
    return [
      'classes' => '',
      'width' => '',
      'height' => '',
    ] + parent::defaultSettings();
  }

  /**
   * {@inheritdoc}
   */
  public function settingsForm(array $form, FormStateInterface $form_state) {
    $elements = parent::settingsForm($form, $form_state);

    $elements['classes'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Classes'),
      '#description' => $this->t('Classes to apply to the embed <code>&lt;iframe></code> element.'),
      '#default_value' => $this->getSetting('classes'),
    ];

    $elements['width'] = [
      '#title' => $this->t('Width'),
      '#type' => 'textfield',
      '#description' => $this->t('<code>&lt;iframe></code> <code>width</code> attribute value.'),
      '#default_value' => $this->getSetting('width'),
    ];

    $elements['height'] = [
      '#title' => $this->t('Height'),
      '#type' => 'textfield',
      '#description' => $this->t('<code>&lt;iframe></code> <code>height</code> attribute value.'),
      '#default_value' => $this->getSetting('height'),
    ];

    return $elements;
  }

  /**
   * {@inheritdoc}
   */
  public function settingsSummary() {
    $settings = $this->getSettings();
    $summary = parent::settingsSummary();

    if ($settings['classes']) {
      $summary[] = $this->t('Classes: @classes', ['@classes' => $settings['classes']]);
    }

    if ($settings['width']) {
      $summary[] = $this->t('Width: @width', ['@width' => $settings['width']]);
    }

    if ($settings['height']) {
      $summary[] = $this->t('Height: @height', ['@height' => $settings['height']]);
    }

    return $summary;
  }

  /**
   * {@inheritdoc}
   */
  public function viewElements(FieldItemListInterface $items, $langcode) {
    $elements = [];
    $settings = $this->getSettings();

    foreach ($items as $delta => $item) {
      $place = '';

      switch ($items->getFieldDefinition()->getType()) {
        case 'address':
          $address_components = [];
          $component_order = [
            'given_name',
            'additional_name',
            'family_name',
            'organization',
            'address_line1',
            'address_line2',
            'dependent_locality',
            'locality',
            'administrative_area',
            'sorting_code',
            'postal_code',
            'country_code',
          ];
          foreach ($component_order as $field) {
            if (!empty($item->$field)) {
              $address_components[] = $item->$field;
            }
          }
          $place = implode(',', $address_components);
          break;

        case 'string':
          $place = $item->value;
          break;
      }

      $map = [
        '#theme' => 'google_map_embed',
        '#place' => rawurlencode($place),
      ];

      if ($settings['classes']) {
        $map['#attributes']['class'] = explode(' ', $settings['classes']);
      }
      if ($settings['width']) {
        $map['#attributes']['width'] = $settings['width'];
      }
      if ($settings['height']) {
        $map['#attributes']['height'] = $settings['height'];
      }

      $elements[$delta] = $map;
    }

    return $elements;
  }

}
