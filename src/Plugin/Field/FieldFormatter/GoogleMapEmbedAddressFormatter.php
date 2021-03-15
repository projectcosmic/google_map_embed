<?php

namespace Drupal\google_map_embed\Plugin\Field\FieldFormatter;

@trigger_error(__NAMESPACE__ . '\GoogleMapEmbedAddressFormatter is deprecated in google_map_embed:2.1.0 and will be removed before google_map_embed:3.0.0. Instead, use ' . __NAMESPACE__ . '\GoogleMapEmbedFormatter.');

/**
 * Plugin implementation of the 'google_map_embed_address' formatter.
 *
 * @FieldFormatter(
 *   id = "google_map_embed_address",
 *   label = @Translation("Google Map (Embed API)"),
 *   field_types = {
 *     "address"
 *   },
 *   no_ui = TRUE,
 * )
 */
class GoogleMapEmbedAddressFormatter extends GoogleMapEmbedFormatter {}
