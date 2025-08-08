# ME007YS Ultrasonic Sensor for ESPHome

An external ESPHome component for the **DFRobot SEN0312 / DYP-ME007YS** waterproof ultrasonic sensor (UART), with a robust frame parser, configurable “too close” behavior, optional raw byte debug, and built-in diagnostics.

> ✅ Works on ESP32 (incl. ESP32-C3). Sensor must be powered at **5V**; UART logic is 3.3V-safe.

---

## Features

- UART frame sync on `0xFF`, checksum validation
- Publishes **distance (cm)** with 0.1 cm precision
- `min_valid_mm` (default **280**) to define sensor’s minimum valid distance
- `too_close_behavior`: 
  - `nan` → publish NaN
  - `min` → publish min distance (e.g., 28.0 cm)
  - `last` → hold last valid reading
- `debug_raw`: optional byte/frame tracing for troubleshooting
- Diagnostics:
  - `frame_rate_hz` (valid frames per second)
  - `status` text sensor: `ok`, `too_close`, `checksum_error`, `idle`
  - Both can be marked as **diagnostic** in Home Assistant UI

---

## Wiring

| ME007YS | ESP32 example (ESP32-C3-DevKitM-1) |
|---------|------------------------------------|
| VCC     | 5V                                 |
| GND     | GND                                |
| TX      | GPIO3 (ESP RX)                     |
| RX      | GPIO1 (ESP TX)                     |

- **Power:** 5V to sensor; common ground with ESP.
- **UART:** RX/TX **crossed** between ESP and sensor.
- **Baud:** 9600 8N1 (default).

---

## Quick Start (use from GitHub)

```yaml
external_components:
  - source: github://YOUR_GITHUB_USERNAME/esphome-me007ys-sensor
    components: [me007ys]

uart:
  id: uart_me007ys
  tx_pin: GPIO1      # to ME007YS RX
  rx_pin: GPIO3      # from ME007YS TX
  baud_rate: 9600
  rx_buffer_size: 256

sensor:
  - platform: me007ys
    uart_id: uart_me007ys
    name: "Water Level Distance (cm)"
    id: water_level_cm
    update_interval: 500ms
    min_valid_mm: 280
    too_close_behavior: min     # nan | min | last
    debug_raw: false
    frame_rate_hz:
      name: "ME007YS Frame Rate"
      entity_category: diagnostic   # Shows under Diagnostics in HA
    status:
      name: "ME007YS Status"
      entity_category: diagnostic   # Shows under Diagnostics in HA

  # Optional: inches conversion
  - platform: template
    name: "Water Level Distance (in)"
    unit_of_measurement: "in"
    accuracy_decimals: 2
    lambda: |-
      if (isnan(id(water_level_cm).state)) return NAN;
      return id(water_level_cm).state * 0.393701;
