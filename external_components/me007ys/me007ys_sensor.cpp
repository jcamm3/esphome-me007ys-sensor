#include "me007ys_sensor.h"
#include "esphome/core/log.h"
#include <cmath>

namespace esphome {
namespace me007ys {

static const char *const TAG = "me007ys";

void ME007YSSensor::update() {
  enum ParseState : uint8_t { WAIT_HEADER = 0, READ_HIGH = 1, READ_LOW = 2, READ_SUM = 3 };
  static ParseState state = WAIT_HEADER;
  static uint8_t high = 0, low = 0;

  uint32_t valid_frames = 0;
  bool any_bytes = false;

  while (this->available() > 0) {
    int rb = this->read();
    if (rb < 0) break;
    uint8_t b = static_cast<uint8_t>(rb);
    any_bytes = true;

    if (this->debug_raw_) {
      ESP_LOGVV(TAG, "RX byte: 0x%02X (state=%u)", b, static_cast<unsigned>(state));
    }

    switch (state) {
      case WAIT_HEADER:
        if (b == 0xFF) {
          state = READ_HIGH;
        }
        break;

      case READ_HIGH:
        high = b;
        state = READ_LOW;
        break;

      case READ_LOW:
        low = b;
        state = READ_SUM;
        break;

      case READ_SUM: {
        uint8_t sum = static_cast<uint8_t>((0xFF + high + low) & 0xFF);
        if (sum != b) {
          ESP_LOGW(TAG, "Checksum mismatch: got 0x%02X, expected 0x%02X", b, sum);
          publish_status_("checksum_error");
          state = (b == 0xFF) ? READ_HIGH : WAIT_HEADER;
          break;
        }

        if (this->debug_raw_) {
          ESP_LOGVV(TAG, "FRAME: ff %02x %02x %02x (ok)", high, low, b);
        }

        const uint16_t distance_mm = (static_cast<uint16_t>(high) << 8) | low;

        // Lower bound
        if (distance_mm <= this->min_valid_mm_) {
          switch (this->behavior_) {
            case TooCloseBehavior::PUBLISH_MIN: {
              float cm = this->min_valid_mm_ / 10.0f;
              ESP_LOGW(TAG, "Too close (%u mm). Publishing min %.1f cm", distance_mm, cm);
              this->publish_state(cm);
              this->last_valid_cm_ = cm;
              publish_status_("too_close");
              break;
            }
            case TooCloseBehavior::HOLD_LAST: {
              ESP_LOGW(TAG, "Too close (%u mm). Holding last valid %.1f cm",
                       distance_mm, this->last_valid_cm_);
              if (!std::isnan(this->last_valid_cm_))
                this->publish_state(this->last_valid_cm_);
              else
                this->publish_state(NAN);
              publish_status_("too_close");
              break;
            }
            case TooCloseBehavior::NAN_OUT:
            default:
              ESP_LOGW(TAG, "Distance %u mm <= min_valid_mm (%u), publishing NaN",
                       distance_mm, this->min_valid_mm_);
              this->publish_state(NAN);
              publish_status_("too_close");
              break;
          }
        }
        // Upper bound
        else if (distance_mm >= this->max_valid_mm_) {
          ESP_LOGW(TAG, "Too far (%u mm >= %u mm), publishing NaN", distance_mm, this->max_valid_mm_);
          this->publish_state(NAN);
          publish_status_("too_far");
        }
        // Valid range
        else {
          const float distance_cm = distance_mm / 10.0f;
          ESP_LOGD(TAG, "Distance: %.1f cm", distance_cm);
          this->publish_state(distance_cm);
          this->last_valid_cm_ = distance_cm;
          publish_status_("ok");
        }

        valid_frames++;
        state = WAIT_HEADER;
        break;
      }
    }
  }

  if (!any_bytes) {
    publish_status_("idle");
  }

  if (this->frame_rate_sensor_ != nullptr) {
    float dt_s = this->get_update_interval() / 1000.0f;
    if (dt_s <= 0.0f) dt_s = 0.5f;
    float rate_hz = valid_frames / dt_s;
    this->frame_rate_sensor_->publish_state(rate_hz);
  }
}

}  // namespace me007ys
}  // namespace esphome
