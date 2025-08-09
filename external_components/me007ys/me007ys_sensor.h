#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace me007ys {

enum class TooCloseBehavior : uint8_t { NAN_OUT, PUBLISH_MIN, HOLD_LAST };

class ME007YSSensor : public PollingComponent, public uart::UARTDevice, public sensor::Sensor {
 public:
  ME007YSSensor() = default;

  void set_min_valid_mm(uint16_t v) { min_valid_mm_ = v; }
  void set_too_close_behavior(const std::string &s) {
    if (s == "min")
      behavior_ = TooCloseBehavior::PUBLISH_MIN;
    else if (s == "last")
      behavior_ = TooCloseBehavior::HOLD_LAST;
    else
      behavior_ = TooCloseBehavior::NAN_OUT;
  }
  void set_debug_raw(bool v) { debug_raw_ = v; }

  // Optional diagnostics
  void set_frame_rate_sensor(sensor::Sensor *s) { frame_rate_sensor_ = s; }
  void set_status_sensor(text_sensor::TextSensor *s) { status_sensor_ = s; }

  void update() override;

 protected:
  void publish_status_(const char *s) {
    if (status_sensor_ == nullptr) return;
    if (last_status_ != s) {
      last_status_ = s;
      status_sensor_->publish_state(s);
    }
  }

  uint16_t min_valid_mm_{280};
  TooCloseBehavior behavior_{TooCloseBehavior::NAN_OUT};
  float last_valid_cm_{NAN};
  bool debug_raw_{false};

  // Diagnostics
  sensor::Sensor *frame_rate_sensor_{nullptr};
  text_sensor::TextSensor *status_sensor_{nullptr};
  const char *last_status_{"idle"};
};

}  // namespace me007ys
}  // namespace esphome
