#include "sths34pf80_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace sths34pf80 {

static const char *const TAG = "sths34pf80.sensor";

void STHS34PF80Sensor::update() {
  bool data_ready = false;
  if (!this->parent_->read_data_ready(data_ready)) {
    ESP_LOGW(TAG, "Failed to check data ready status");
    return;
  }

  if (!data_ready) {
    return;
  }

  switch (this->sensor_type_) {
    case SENSOR_AMBIENT_TEMP: {
      float temp;
      if (this->parent_->read_ambient_temperature(temp)) {
        this->publish_state(temp);
      } else {
        ESP_LOGW(TAG, "Failed to read ambient temperature");
      }
      break;
    }
    case SENSOR_OBJECT_TEMP: {
      int16_t temp;
      if (this->parent_->read_object_temperature(temp)) {
        this->publish_state(temp);
      } else {
        ESP_LOGW(TAG, "Failed to read object temperature");
      }
      break;
    }
    case SENSOR_COMPENSATED_OBJECT_TEMP: {
      int16_t temp;
      if (this->parent_->read_compensated_object_temperature(temp)) {
        this->publish_state(temp);
      } else {
        ESP_LOGW(TAG, "Failed to read compensated object temperature");
      }
      break;
    }
    case SENSOR_PRESENCE: {
      int16_t presence;
      if (this->parent_->read_presence(presence)) {
        this->publish_state(presence);
      } else {
        ESP_LOGW(TAG, "Failed to read presence value");
      }
      break;
    }
    case SENSOR_MOTION: {
      int16_t motion;
      if (this->parent_->read_motion(motion)) {
        this->publish_state(motion);
      } else {
        ESP_LOGW(TAG, "Failed to read motion value");
      }
      break;
    }
    case SENSOR_TEMPERATURE_SHOCK: {
      int16_t shock;
      if (this->parent_->read_temperature_shock(shock)) {
        this->publish_state(shock);
      } else {
        ESP_LOGW(TAG, "Failed to read temperature shock value");
      }
      break;
    }
  }
}

}  // namespace sths34pf80
}  // namespace esphome

