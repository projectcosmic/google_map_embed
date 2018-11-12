<?php

namespace Drupal\google_map_embed\Plugin\Field\FieldFormatter;

use CommerceGuys\Addressing\AddressFormat\AddressFormatRepositoryInterface;
use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Field\FormatterBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\geolocation\GoogleMapsDisplayTrait;
use Drupal\Core\Field\FieldStorageDefinitionInterface;
use Drupal\geolocation\GeolocationItemTokenTrait;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\address\Plugin\Field\FieldFormatter\AddressDefaultFormatter;

/**
 * Plugin implementation of the 'google_map_embed_address' formatter.
 *
 * @FieldFormatter(
 *   id = "google_map_embed_address",
 *   label = @Translation("Google Map (Embed API)"),
 *   field_types = {
 *     "address"
 *   }
 * )
 */
class GoogleMapEmbedAddressFormatter extends AddressDefaultFormatter {

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
      '#default_value' => $this->getSetting('width'),
    ];

    $elements['height'] = [
      '#title' => $this->t('Height'),
      '#type' => 'textfield',
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
      $country_code = $item->getCountryCode();
      $address_format = $this->addressFormatRepository->get($country_code);
      $values = $this->getValues($item, $address_format);

      $address_components = [];
      $component_order = array_intersect([
        'givenName',
        'additionalName',
        'familyName',
        'organization',
        'addressLine1',
        'addressLine2',
        'dependentLocality',
        'locality',
        'administrativeArea',
        'country',
        'sortingCode',
        'postalCode',
      ], $address_format->getUsedFields());
      
      foreach ($component_order as $field) {
        if ($values[$field]) {
          $address_components[] = urlencode($values[$field]);
        }
      }

      $map = [
        '#theme' => 'google_map_embed',
        '#place' => implode(',', $address_components),
      ];

      if ($settings['classes']) {
        $map['#attributes']['class'] = implode(' ', $settings['classes']);
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
