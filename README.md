# Google Maps Embed

Display locations using the [Google Map Embed API](https://developers.google.com/maps/documentation/embed).
This API has unlimited free usage limit but has fewer capabilities than the
[JavaScript API](https://developers.google.com/maps/documentation/javascript/).

## Supported Field Types

Provides a field formatter for the following fields types:

- [`addressfield`](https://drupal.org/project/google_map_field)
- [`google_map_field`](https://drupal.org/project/google_map_field)
- [`location` (CCK field from 'Location CCK' submodule)](https://drupal.org/project/location)

## Views Integration

Provides the following views plugins:

- [Location](https://drupal.org/project/location) field plugin from the address

## Initial Setup

One must set the API key variable `google_map_embed_api_key` with the value of
an API key that has the Embed API enabled.
