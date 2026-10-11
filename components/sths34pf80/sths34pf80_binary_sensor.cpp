#include "sths34pf80_binary_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace sths34pf80 {

static const char *const TAG = "sths34pf80.binary_sensor";

void STHS34PF80BinarySensor::update() {
  bool detected = false;

  switch (this->sensor_type_) {
    case BINARY_SENSOR_PRESENCE:
      if (this->parent_->read_presence_flag(detected)) {
        // Always publish state for low latency - binary sensors don't throttle by default
        this->publish_state(detected);
      } else {
        ESP_LOGW(TAG, "Failed to read presence flag");
      }
      break;
    case BINARY_SENSOR_MOTION:
      if (this->parent_->read_motion_flag(detected)) {
        // Always publish state for low latency - binary sensors don't throttle by default
        this->publish_state(detected);
      } else {
        ESP_LOGW(TAG, "Failed to read motion flag");
      }
      break;
    case BINARY_SENSOR_TEMPERATURE_SHOCK:
      if (this->parent_->read_temperature_shock_flag(detected)) {
        // Always publish state for low latency - binary sensors don't throttle by default
        this->publish_state(detected);
      } else {
        ESP_LOGW(TAG, "Failed to read temperature shock flag");
      }
      break;
  }
}

}  // namespace sths34pf80
}  // namespace esphome
