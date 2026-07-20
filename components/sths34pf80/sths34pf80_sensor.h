#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "sths34pf80.h"

namespace esphome {
namespace sths34pf80 {

enum SensorType {
  SENSOR_AMBIENT_TEMP,
  SENSOR_OBJECT_TEMP,
  SENSOR_COMPENSATED_OBJECT_TEMP,
  SENSOR_PRESENCE,
  SENSOR_MOTION,
  SENSOR_TEMPERATURE_SHOCK,
};

class STHS34PF80Sensor : public sensor::Sensor {
 public:
  void set_parent(STHS34PF80Component *parent) { parent_ = parent; }
  void set_sensor_type(SensorType type) { sensor_type_ = type; }
  void update();

 protected:
  STHS34PF80Component *parent_;
  SensorType sensor_type_;
};

}  // namespace sths34pf80
}  // namespace esphome

