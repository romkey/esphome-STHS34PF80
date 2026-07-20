#pragma once

#include <vector>
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace sths34pf80 {

// Forward declarations
class STHS34PF80Sensor;
class STHS34PF80BinarySensor;

// Register addresses
static const uint8_t STHS34PF80_REG_LPF1 = 0x0C;
static const uint8_t STHS34PF80_REG_LPF2 = 0x0D;
static const uint8_t STHS34PF80_REG_WHO_AM_I = 0x0F;
static const uint8_t STHS34PF80_REG_AVG_TRIM = 0x10;
static const uint8_t STHS34PF80_REG_CTRL0 = 0x17;
static const uint8_t STHS34PF80_REG_SENS_DATA = 0x1D;
static const uint8_t STHS34PF80_REG_CTRL1 = 0x20;
static const uint8_t STHS34PF80_REG_CTRL2 = 0x21;
static const uint8_t STHS34PF80_REG_CTRL3 = 0x22;
static const uint8_t STHS34PF80_REG_STATUS = 0x23;
static const uint8_t STHS34PF80_REG_FUNC_STATUS = 0x25;
static const uint8_t STHS34PF80_REG_TOBJECT_L = 0x26;
static const uint8_t STHS34PF80_REG_TOBJECT_H = 0x27;
static const uint8_t STHS34PF80_REG_TAMBIENT_L = 0x28;
static const uint8_t STHS34PF80_REG_TAMBIENT_H = 0x29;
static const uint8_t STHS34PF80_REG_TOBJ_COMP_L = 0x38;
static const uint8_t STHS34PF80_REG_TOBJ_COMP_H = 0x39;
static const uint8_t STHS34PF80_REG_TPRESENCE_L = 0x3A;
static const uint8_t STHS34PF80_REG_TPRESENCE_H = 0x3B;
static const uint8_t STHS34PF80_REG_TMOTION_L = 0x3C;
static const uint8_t STHS34PF80_REG_TMOTION_H = 0x3D;
static const uint8_t STHS34PF80_REG_TAMB_SHOCK_L = 0x3E;
static const uint8_t STHS34PF80_REG_TAMB_SHOCK_H = 0x3F;
static const uint8_t STHS34PF80_REG_FUNC_CFG_ADDR = 0x08;
static const uint8_t STHS34PF80_REG_FUNC_CFG_DATA = 0x09;
static const uint8_t STHS34PF80_REG_PAGE_RW = 0x11;

static const uint8_t STHS34PF80_EMBEDDED_RESET_ALGO = 0x2A;

static const uint8_t STHS34PF80_DEVICE_ID = 0xD3;
static const uint8_t STHS34PF80_DEFAULT_ADDR = 0x5A;

// Low-pass filter configuration options
enum LPFConfig : uint8_t {
  LPF_ODR_DIV_9 = 0x00,
  LPF_ODR_DIV_20 = 0x01,
  LPF_ODR_DIV_50 = 0x02,
  LPF_ODR_DIV_100 = 0x03,
  LPF_ODR_DIV_200 = 0x04,
  LPF_ODR_DIV_400 = 0x05,
  LPF_ODR_DIV_800 = 0x06,
};

// Ambient temperature averaging options
enum AmbTempAvg : uint8_t {
  AVG_T_8 = 0x00,
  AVG_T_4 = 0x01,
  AVG_T_2 = 0x02,
  AVG_T_1 = 0x03,
};

// Object temperature averaging options
enum ObjTempAvg : uint8_t {
  AVG_TMOS_2 = 0x00,
  AVG_TMOS_8 = 0x01,
  AVG_TMOS_32 = 0x02,
  AVG_TMOS_128 = 0x03,
  AVG_TMOS_256 = 0x04,
  AVG_TMOS_512 = 0x05,
  AVG_TMOS_1024 = 0x06,
  AVG_TMOS_2048 = 0x07,
};

// Output data rate options
enum OutputDataRate : uint8_t {
  ODR_POWER_DOWN = 0x00,
  ODR_0_25_HZ = 0x01,
  ODR_0_5_HZ = 0x02,
  ODR_1_HZ = 0x03,
  ODR_2_HZ = 0x04,
  ODR_4_HZ = 0x05,
  ODR_8_HZ = 0x06,
  ODR_15_HZ = 0x07,
  ODR_30_HZ = 0x08,
};

class STHS34PF80Component : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  
  // Public methods for reading sensor data (called by sensor/binary_sensor platforms)
  bool read_data_ready(bool &ready);
  bool read_ambient_temperature(float &temp);
  bool read_object_temperature(int16_t &temp);
  bool read_compensated_object_temperature(int16_t &temp);
  bool read_presence(int16_t &presence);
  bool read_motion(int16_t &motion);
  bool read_temperature_shock(int16_t &shock);
  bool read_presence_flag(bool &detected);
  bool read_motion_flag(bool &detected);
  bool read_temperature_shock_flag(bool &detected);

  // Sensor registration
  void register_sensor(STHS34PF80Sensor *sensor);
  void register_binary_sensor(STHS34PF80BinarySensor *binary_sensor);
  
  // Force immediate updates for low latency
  void force_update_binary_sensors();

  // Configuration setters
  void set_output_data_rate(OutputDataRate odr) { odr_ = odr; }
  void set_motion_lpf(LPFConfig lpf) { motion_lpf_ = lpf; }
  void set_motion_presence_lpf(LPFConfig lpf) { motion_presence_lpf_ = lpf; }
  void set_presence_lpf(LPFConfig lpf) { presence_lpf_ = lpf; }
  void set_temperature_lpf(LPFConfig lpf) { temperature_lpf_ = lpf; }
  void set_amb_temp_avg(AmbTempAvg avg) { amb_temp_avg_ = avg; }
  void set_obj_temp_avg(ObjTempAvg avg) { obj_temp_avg_ = avg; }
  void set_wide_gain_mode(bool wide_mode) { wide_gain_mode_ = wide_mode; }
  void set_block_data_update(bool enable) { block_data_update_ = enable; }

  // Interrupt configuration
  void set_enable_presence_interrupt(bool enable) { enable_presence_interrupt_ = enable; }
  void set_enable_motion_interrupt(bool enable) { enable_motion_interrupt_ = enable; }
  void set_int_active_low(bool active_low) { int_active_low_ = active_low; }
  void set_int_open_drain(bool open_drain) { int_open_drain_ = open_drain; }
  void set_int_latched(bool latched) { int_latched_ = latched; }
  void set_force_update(bool force_update) { force_update_ = force_update; }

 protected:
  bool reset_sensor_();
  bool algorithm_reset_();
  bool write_embedded_function_(uint8_t addr, uint8_t *data, uint8_t len);
  bool safe_set_output_data_rate_(OutputDataRate current_odr, OutputDataRate new_odr);
  bool read_register_bit_(uint8_t reg, uint8_t bit_pos, bool &value);
  bool write_register_bits_(uint8_t reg, uint8_t bit_pos, uint8_t num_bits, uint8_t value);
  bool read_register_bits_(uint8_t reg, uint8_t bit_pos, uint8_t num_bits, uint8_t &value);

  // Configuration
  OutputDataRate odr_{ODR_1_HZ};
  LPFConfig motion_lpf_{LPF_ODR_DIV_9};
  LPFConfig motion_presence_lpf_{LPF_ODR_DIV_20};
  LPFConfig presence_lpf_{LPF_ODR_DIV_50};
  LPFConfig temperature_lpf_{LPF_ODR_DIV_100};
  AmbTempAvg amb_temp_avg_{AVG_T_8};
  ObjTempAvg obj_temp_avg_{AVG_TMOS_32};
  bool wide_gain_mode_{false};
  bool block_data_update_{true};
  bool enable_presence_interrupt_{false};
  bool enable_motion_interrupt_{false};
  bool int_active_low_{false};
  bool int_open_drain_{false};
  bool int_latched_{false};
  bool force_update_{false};

  // Sensor references
  std::vector<STHS34PF80Sensor *> sensors_;
  std::vector<STHS34PF80BinarySensor *> binary_sensors_;
};

}  // namespace sths34pf80
}  // namespace esphome

