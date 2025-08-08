# Changelog

## v1.0.0 — 2025-08-08
- Initial public release of ME007YS external component for ESPHome
- Features:
  - Robust UART frame parser with header sync (0xFF) and checksum validation
  - Configurable `min_valid_mm`
  - `too_close_behavior`: `nan` | `min` | `last`
  - Optional `debug_raw` byte/frame logging
  - Diagnostics: `frame_rate_hz` numeric sensor, `status` text sensor
  - Example YAML for ESP32‑C3 (GPIO1/3)
