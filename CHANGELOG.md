
---

# CHANGELOG.md (updated)

```markdown
# Changelog

## v1.2.0 — 2025-08-08
- Added **`too_far_behavior`** option with choices `nan` | `max` | `last`, symmetric with `too_close_behavior`.
- Updated README with `too_far_behavior` examples and behavior notes.

## v1.1.0 — 2025-08-08
- Switched configuration to **centimeters** for min/max valid distances.
- Added `max_valid_cm` option (default 450.0 cm) per DFRobot spec.
- Now reports `too_far` in `status` when exceeding `max_valid_cm`.
- Retained backwards compatibility for `min_valid_mm`/`max_valid_mm` (auto-converted to cm).
- Updated README with new options and default range (28.0–450.0 cm).

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
  - Configurable min valid distance.
  - `too_close_behavior`: `nan` | `min` | `last`.
  - Optional `debug_raw` byte/frame logging.
  - Diagnostics: `frame_rate_hz` numeric sensor, `status` text sensor.
  - Example YAML for ESP32‑C3 (GPIO1/3).
