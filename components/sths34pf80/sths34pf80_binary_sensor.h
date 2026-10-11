#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "sths34pf80.h"

namespace esphome {
namespace sths34pf80 {

enum BinarySensorType {
  BINARY_SENSOR_PRESENCE,
  BINARY_SENSOR_MOTION,
  BINARY_SENSOR_TEMPERATURE_SHOCK,
};

class STHS34PF80BinarySensor : public binary_sensor::BinarySensor {
 public:
  void set_parent(STHS34PF80Component *parent) { parent_ = parent; }
  void set_sensor_type(BinarySensorType type) { sensor_type_ = type; }
  void update();
  void force_update() { this->update(); }  // Force immediate update

 protected:
  STHS34PF80Component *parent_;
  BinarySensorType sensor_type_;
};

}  // namespace sths34pf80
}  // namespace esphome
