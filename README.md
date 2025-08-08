# ME007YS Ultrasonic Sensor for ESPHome

[![ESPHome](https://img.shields.io/badge/ESPHome-Compatible-blue.svg)](https://esphome.io/)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![HA Diagnostics](https://img.shields.io/badge/HA-Diagnostic%20Entities-orange.svg)](#diagnostics-in-home-assistant)

An external ESPHome component for the **DFRobot SEN0312 / DYP-ME007YS** waterproof ultrasonic sensor (UART), with a robust frame parser, configurable “too close”/“too far” behavior, optional raw byte debug, and built-in diagnostics.

> ✅ Works on ESP32 (incl. ESP32-C3, ESP32-S3). Sensor must be powered at **5 V**; UART logic is 3.3 V-safe.

---

## Features

- UART frame sync on `0xFF`, checksum validation
- Publishes **distance (cm)** with 0.1 cm precision
- Configurable valid range (`min_valid_cm`, `max_valid_cm`)
  - Defaults to **28.0–450.0 cm** (DFRobot spec for flat objects)
- `too_close_behavior`:  
  - `nan` → publish NaN  
  - `min` → publish min distance  
  - `last` → hold last valid reading
- Reports `too_far` when exceeding `max_valid_cm`
- `debug_raw`: optional byte/frame tracing for troubleshooting
- Diagnostics:
  - `frame_rate_hz` (valid frames per second)
  - `status` text sensor: `ok`, `too_close`, `too_far`, `checksum_error`, `idle`
  - Both can be hidden from the main entity list via `entity_category: diagnostic`

---

## Wiring

| ME007YS | ESP32 example (ESP32-C3-DevKitM-1) |
|---------|-------------------------------------|
| VCC     | 5 V                                 |
| GND     | GND                                 |
| TX      | GPIO3 (ESP RX)                      |
| RX      | GPIO1 (ESP TX)                      |

- **Power:** 5 V to sensor; common ground with ESP
- **UART:** RX/TX crossed between ESP and sensor
- **Baud:** 9600 8N1 (default)

---

## Quick Start (use from GitHub)

```yaml
external_components:
  - source: github://YOUR_GITHUB_USERNAME/esphome-me007ys-sensor
    components: [me007ys]

uart:
  id: uart_me007ys
  tx_pin: GPIO1      # ESP TX → sensor RX
  rx_pin: GPIO3      # ESP RX ← sensor TX
  baud_rate: 9600
  rx_buffer_size: 256

sensor:
  - platform: me007ys
    uart_id: uart_me007ys
    name: "Water Level Distance (cm)"
    id: water_level_cm
    update_interval: 500ms
    min_valid_cm: 28.0     # default 28.0 cm
    max_valid_cm: 450.0    # default 450.0 cm
    too_close_behavior: min  # nan | min | last
    debug_raw: false
    frame_rate_hz:
      name: "ME007YS Frame Rate"
      entity_category: diagnostic
    status:
      name: "ME007YS Status"
      entity_category: diagnostic

  # Optional: inches conversion
  - platform: template
    name: "Water Level Distance (in)"
    unit_of_measurement: "in"
    accuracy_decimals: 2
    lambda: |-
      if (isnan(id(water_level_cm).state)) return NAN;
      return id(water_level_cm).state * 0.393701;
