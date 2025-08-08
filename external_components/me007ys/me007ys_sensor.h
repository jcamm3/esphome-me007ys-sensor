#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace me007ys {

enum class OutOfRangeBehavior : uint8_t { NAN_OUT, PUBLISH_LIMIT, HOLD_LAST };

class ME007YSSensor : public PollingComponent, public uart::UARTDevice, public sensor::Sensor {
 public:
  ME007YSSensor() = default;

  void set_min_valid_cm(float cm) { min_valid_mm_ = static_cast<uint16_t>(cm * 10.0f + 0.5f); }
  void set_max_valid_cm(float cm) { max_valid_mm_ = static_cast<uint16_t>(cm * 10.0f + 0.5f); }

  void set_too_close_behavior(const std::string &s) {
    if (s == "min")
      too_close_behavior_ = OutOfRangeBehavior::PUBLISH_LIMIT;
    else if (s == "last")
      too_close_behavior_ = OutOfRangeBehavior::HOLD_LAST;
    else
      too_close_behavior_ = OutOfRangeBehavior::NAN_OUT;
  }
  void set_too_far_behavior(const std::string &s) {
    if (s == "max")
      too_far_behavior_ = OutOfRangeBehavior::PUBLISH_LIMIT;
    else if (s == "last")
      too_far_behavior_ = OutOfRangeBehavior::HOLD_LAST;
    else
      too_far_behavior_ = OutOfRangeBehavior::NAN_OUT;
  }
  void set_debug_raw(bool v) { debug_raw_ = v; }

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

  uint16_t min_valid_mm_{280};   // 28.0 cm default
  uint16_t max_valid_mm_{4500};  // 450.0 cm default

  OutOfRangeBehavior too_close_behavior_{OutOfRangeBehavior::NAN_OUT};
  OutOfRangeBehavior too_far_behavior_{OutOfRangeBehavior::NAN_OUT};

  float last_valid_cm_{NAN};
  bool debug_raw_{false};

  sensor::Sensor *frame_rate_sensor_{nullptr};
  text_sensor::TextSensor *status_sensor_{nullptr};
  const char *last_status_{"idle"};
};

}  // namespace me007ys
}  // namespace esphome
