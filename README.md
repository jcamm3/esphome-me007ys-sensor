# ME007YS Ultrasonic Distance Sensor (ESPHome External Component)

An ESPHome external component for the **DFRobot SEN0312 / DYP-ME007YS** waterproof ultrasonic distance sensor.  
Reads the sensor over UART and publishes distance in **centimeters**, with built-in range validation, guard band filtering, step limiting, and optional diagnostics.

## Features
- UART parsing: `0xFF, HIGH, LOW, CHECKSUM`
- Distance in **centimeters** (0.1 cm)
- Range enforcement: `min_valid_cm` (default 28.0), `max_valid_cm` (default 450.0)
- Lower bound behavior: `too_close_behavior` = `nan|min|last`
- Upper bound: always **NaN** + status `too_far`
- Robustness:
  - `guard_cm` — ignore a band above `min_valid_cm` (default 5.0)
  - `max_step_cm` — reject single-frame jumps (0=off)
  - `require_consecutive` — publish only after N similar frames
- Diagnostics:
  - `frame_rate_hz` (Hz), `status` text (`ok`, `too_close`, `too_far`, `checksum_error`, `idle`)
  - `raw_mm` (mm) with `raw_mm_policy: all|valid_only`
- Optional `debug_raw` for byte-level logging

## Quick Start
```yaml
external_components:
  - source: github://YOUR_GITHUB_USERNAME/esphome-me007ys-sensor
    components: [me007ys]

uart:
  id: uart_me007ys
  tx_pin: GPIO1
  rx_pin: GPIO3
  baud_rate: 9600
  rx_buffer_size: 256

sensor:
  - platform: me007ys
    uart_id: uart_me007ys
    name: "Water Level Distance (cm)"
    min_valid_cm: 28.0
    max_valid_cm: 450.0
    too_close_behavior: nan
    guard_cm: 12.0
    max_step_cm: 120.0
    require_consecutive: 3
    raw_mm:
      name: "ME007YS Raw Distance (mm)"
      entity_category: diagnostic
    raw_mm_policy: valid_only
    frame_rate_hz:
      name: "ME007YS Frame Rate"
      entity_category: diagnostic
    status:
      name: "ME007YS Status"
      entity_category: diagnostic
