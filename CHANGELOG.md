
---

### Updated `CHANGELOG.md`
```markdown
# Changelog

## v1.0.2 — 2025-08-08
- Defaulted `frame_rate_hz` and `status` sensors to `entity_category: diagnostic` in example YAML.
- Updated README with badge and documentation on diagnostic entities.

## v1.0.1 — 2025-08-08
- Added documentation for using `entity_category: diagnostic` with `frame_rate_hz` and `status` sensors.
- Clarified options to hide or move diagnostic entities in Home Assistant.

## v1.0.0 — 2025-08-08
- Initial public release of ME007YS external component for ESPHome.
- Features:
  - Robust UART frame parser with header sync (0xFF) and checksum validation.
  - Configurable `min_valid_mm`.
  - `too_close_behavior`: `nan` | `min` | `last`.
  - Optional `debug_raw` byte/frame logging.
  - Diagnostics: `frame_rate_hz` numeric sensor, `status` text sensor.
  - Example YAML for ESP32-C3 (GPIO1/3).
