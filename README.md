# Google Maps Embed

Provides a formatter to display field values using the
[Google Map Embed API](https://developers.google.com/maps/documentation/embed).
This API has unlimited free usage limit but has fewer capabilities than the
[JavaScript API](https://developers.google.com/maps/documentation/javascript/).

## Supported Field Types

- [`address`](https://drupal.org/project/address)
- `string` (as raw place query string)

## Initial Setup

One must set the site setting `google_map_embed_api_key` with the value of
an API key that has the Embed API enabled:

```php
$settings['google_map_embed_api_key'] = 'api_key_here';
```
