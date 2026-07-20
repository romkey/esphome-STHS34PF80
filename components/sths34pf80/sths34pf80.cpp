#include "sths34pf80.h"
#include "sths34pf80_sensor.h"
#include "sths34pf80_binary_sensor.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace sths34pf80 {

static const char *const TAG = "sths34pf80";

void STHS34PF80Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up STHS34PF80...");

  // Check WHO_AM_I register
  uint8_t who_am_i;
  if (!this->read_byte(STHS34PF80_REG_WHO_AM_I, &who_am_i)) {
    ESP_LOGE(TAG, "Failed to read WHO_AM_I register");
    this->mark_failed();
    return;
  }

  if (who_am_i != STHS34PF80_DEVICE_ID) {
    ESP_LOGE(TAG, "Invalid device ID. Expected 0x%02X, got 0x%02X", STHS34PF80_DEVICE_ID, who_am_i);
    this->mark_failed();
    return;
  }

  ESP_LOGD(TAG, "Device ID verified: 0x%02X", who_am_i);

  // Reset the sensor
  if (!this->reset_sensor_()) {
    ESP_LOGE(TAG, "Failed to reset sensor");
    this->mark_failed();
    return;
  }

  delay(10);  // Wait for reset to complete

  // Configure object temperature averaging
  if (!this->write_register_bits_(STHS34PF80_REG_AVG_TRIM, 0, 3, static_cast<uint8_t>(this->obj_temp_avg_))) {
    ESP_LOGE(TAG, "Failed to set object temperature averaging");
    this->mark_failed();
    return;
  }

  // Configure ambient temperature averaging
  if (!this->write_register_bits_(STHS34PF80_REG_AVG_TRIM, 4, 2, static_cast<uint8_t>(this->amb_temp_avg_))) {
    ESP_LOGE(TAG, "Failed to set ambient temperature averaging");
    this->mark_failed();
    return;
  }

  // Configure motion LPF
  if (!this->write_register_bits_(STHS34PF80_REG_LPF1, 0, 3, static_cast<uint8_t>(this->motion_lpf_))) {
    ESP_LOGE(TAG, "Failed to set motion LPF");
    this->mark_failed();
    return;
  }

  // Configure motion+presence LPF
  if (!this->write_register_bits_(STHS34PF80_REG_LPF1, 3, 3, static_cast<uint8_t>(this->motion_presence_lpf_))) {
    ESP_LOGE(TAG, "Failed to set motion+presence LPF");
    this->mark_failed();
    return;
  }

  // Configure presence LPF
  if (!this->write_register_bits_(STHS34PF80_REG_LPF2, 3, 3, static_cast<uint8_t>(this->presence_lpf_))) {
    ESP_LOGE(TAG, "Failed to set presence LPF");
    this->mark_failed();
    return;
  }

  // Configure temperature LPF
  if (!this->write_register_bits_(STHS34PF80_REG_LPF2, 0, 3, static_cast<uint8_t>(this->temperature_lpf_))) {
    ESP_LOGE(TAG, "Failed to set temperature LPF");
    this->mark_failed();
    return;
  }

  // Configure wide gain mode
  uint8_t gain_value = this->wide_gain_mode_ ? 0x00 : 0x07;
  if (!this->write_register_bits_(STHS34PF80_REG_CTRL0, 4, 3, gain_value)) {
    ESP_LOGE(TAG, "Failed to set wide gain mode");
    this->mark_failed();
    return;
  }

  // Configure block data update
  if (!this->write_register_bits_(STHS34PF80_REG_CTRL1, 4, 1, this->block_data_update_ ? 1 : 0)) {
    ESP_LOGE(TAG, "Failed to set block data update");
    this->mark_failed();
    return;
  }

  // Configure interrupt settings if enabled
  if (this->enable_presence_interrupt_ || this->enable_motion_interrupt_) {
    // Set interrupt polarity
    if (!this->write_register_bits_(STHS34PF80_REG_CTRL3, 7, 1, this->int_active_low_ ? 1 : 0)) {
      ESP_LOGE(TAG, "Failed to set interrupt polarity");
      this->mark_failed();
      return;
    }

    // Set interrupt output type
    if (!this->write_register_bits_(STHS34PF80_REG_CTRL3, 6, 1, this->int_open_drain_ ? 1 : 0)) {
      ESP_LOGE(TAG, "Failed to set interrupt output type");
      this->mark_failed();
      return;
    }

    // Set interrupt latched mode
    if (!this->write_register_bits_(STHS34PF80_REG_CTRL3, 2, 1, this->int_latched_ ? 1 : 0)) {
      ESP_LOGE(TAG, "Failed to set interrupt latched mode");
      this->mark_failed();
      return;
    }

    // Set interrupt mask
    uint8_t int_mask = 0;
    if (this->enable_presence_interrupt_)
      int_mask |= 0x04;  // PRES_FLAG
    if (this->enable_motion_interrupt_)
      int_mask |= 0x02;  // MOT_FLAG

    if (!this->write_register_bits_(STHS34PF80_REG_CTRL3, 3, 3, int_mask)) {
      ESP_LOGE(TAG, "Failed to set interrupt mask");
      this->mark_failed();
      return;
    }

    // Set interrupt signal type to INT_OR (function flags)
    if (!this->write_register_bits_(STHS34PF80_REG_CTRL3, 0, 2, 0x02)) {
      ESP_LOGE(TAG, "Failed to set interrupt signal type");
      this->mark_failed();
      return;
    }
  }

  // Set output data rate (must be last after all other configurations)
  if (!this->safe_set_output_data_rate_(ODR_POWER_DOWN, this->odr_)) {
    ESP_LOGE(TAG, "Failed to set output data rate");
    this->mark_failed();
    return;
  }

  ESP_LOGCONFIG(TAG, "STHS34PF80 setup complete");
}

void STHS34PF80Component::update() {
  // Update all registered sensors
  for (auto *sensor : sensors_) {
    sensor->update();
  }
  
  // Update all registered binary sensors
  for (auto *binary_sensor : binary_sensors_) {
    binary_sensor->update();
  }
}

void STHS34PF80Component::register_sensor(STHS34PF80Sensor *sensor) {
  sensors_.push_back(sensor);
}

void STHS34PF80Component::register_binary_sensor(STHS34PF80BinarySensor *binary_sensor) {
  binary_sensors_.push_back(binary_sensor);
}

void STHS34PF80Component::force_update_binary_sensors() {
  // Force immediate update of all binary sensors for low latency
  for (auto *binary_sensor : binary_sensors_) {
    binary_sensor->force_update();
  }
}

bool STHS34PF80Component::read_data_ready(bool &ready) {
  return this->read_register_bit_(STHS34PF80_REG_STATUS, 2, ready);
}

bool STHS34PF80Component::read_ambient_temperature(float &temp) {
  uint8_t data[2];
  if (this->read_bytes(STHS34PF80_REG_TAMBIENT_L, data, 2)) {
    int16_t raw_temp = (int16_t) (data[0] | (data[1] << 8));
    temp = raw_temp / 100.0f;
    return true;
  }
  return false;
}

bool STHS34PF80Component::read_object_temperature(int16_t &temp) {
  uint8_t data[2];
  if (this->read_bytes(STHS34PF80_REG_TOBJECT_L, data, 2)) {
    temp = (int16_t) (data[0] | (data[1] << 8));
    return true;
  }
  return false;
}

bool STHS34PF80Component::read_compensated_object_temperature(int16_t &temp) {
  uint8_t data[2];
  if (this->read_bytes(STHS34PF80_REG_TOBJ_COMP_L, data, 2)) {
    temp = (int16_t) (data[0] | (data[1] << 8));
    return true;
  }
  return false;
}

bool STHS34PF80Component::read_presence(int16_t &presence) {
  uint8_t data[2];
  if (this->read_bytes(STHS34PF80_REG_TPRESENCE_L, data, 2)) {
    presence = (int16_t) (data[0] | (data[1] << 8));
    return true;
  }
  return false;
}

bool STHS34PF80Component::read_motion(int16_t &motion) {
  uint8_t data[2];
  if (this->read_bytes(STHS34PF80_REG_TMOTION_L, data, 2)) {
    motion = (int16_t) (data[0] | (data[1] << 8));
    return true;
  }
  return false;
}

bool STHS34PF80Component::read_temperature_shock(int16_t &shock) {
  uint8_t data[2];
  if (this->read_bytes(STHS34PF80_REG_TAMB_SHOCK_L, data, 2)) {
    shock = (int16_t) (data[0] | (data[1] << 8));
    return true;
  }
  return false;
}

bool STHS34PF80Component::read_presence_flag(bool &detected) {
  uint8_t func_status;
  if (this->read_byte(STHS34PF80_REG_FUNC_STATUS, &func_status)) {
    detected = (func_status & 0x04) != 0;
    return true;
  }
  return false;
}

bool STHS34PF80Component::read_motion_flag(bool &detected) {
  uint8_t func_status;
  if (this->read_byte(STHS34PF80_REG_FUNC_STATUS, &func_status)) {
    detected = (func_status & 0x02) != 0;
    return true;
  }
  return false;
}

bool STHS34PF80Component::read_temperature_shock_flag(bool &detected) {
  uint8_t func_status;
  if (this->read_byte(STHS34PF80_REG_FUNC_STATUS, &func_status)) {
    detected = (func_status & 0x01) != 0;
    return true;
  }
  return false;
}

void STHS34PF80Component::dump_config() {
  ESP_LOGCONFIG(TAG, "STHS34PF80:");
  LOG_I2C_DEVICE(this);

  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with STHS34PF80 failed!");
    return;
  }

  ESP_LOGCONFIG(TAG, "  Output Data Rate: %d", static_cast<int>(this->odr_));
  ESP_LOGCONFIG(TAG, "  Object Temp Averaging: %d", static_cast<int>(this->obj_temp_avg_));
  ESP_LOGCONFIG(TAG, "  Ambient Temp Averaging: %d", static_cast<int>(this->amb_temp_avg_));
  ESP_LOGCONFIG(TAG, "  Wide Gain Mode: %s", YESNO(this->wide_gain_mode_));
  ESP_LOGCONFIG(TAG, "  Block Data Update: %s", YESNO(this->block_data_update_));

  if (this->enable_presence_interrupt_ || this->enable_motion_interrupt_) {
    ESP_LOGCONFIG(TAG, "  Interrupts Enabled:");
    ESP_LOGCONFIG(TAG, "    Presence: %s", YESNO(this->enable_presence_interrupt_));
    ESP_LOGCONFIG(TAG, "    Motion: %s", YESNO(this->enable_motion_interrupt_));
    ESP_LOGCONFIG(TAG, "    Active Low: %s", YESNO(this->int_active_low_));
    ESP_LOGCONFIG(TAG, "    Open Drain: %s", YESNO(this->int_open_drain_));
    ESP_LOGCONFIG(TAG, "    Latched: %s", YESNO(this->int_latched_));
  }
}

bool STHS34PF80Component::reset_sensor_() {
  // Reboot OTP memory
  if (!this->write_register_bits_(STHS34PF80_REG_CTRL2, 7, 1, 1)) {
    return false;
  }

  delay(5);

  // Reset the internal algorithm
  return this->algorithm_reset_();
}

bool STHS34PF80Component::algorithm_reset_() {
  uint8_t reset_value = 1;
  return this->write_embedded_function_(STHS34PF80_EMBEDDED_RESET_ALGO, &reset_value, 1);
}

bool STHS34PF80Component::write_embedded_function_(uint8_t addr, uint8_t *data, uint8_t len) {
  // Save current ODR
  uint8_t current_odr_val;
  if (!this->read_register_bits_(STHS34PF80_REG_CTRL1, 0, 4, current_odr_val)) {
    return false;
  }
  OutputDataRate current_odr = static_cast<OutputDataRate>(current_odr_val);

  // Enter power-down mode
  if (!this->safe_set_output_data_rate_(current_odr, ODR_POWER_DOWN)) {
    return false;
  }

  // Enable access to embedded functions register
  if (!this->write_register_bits_(STHS34PF80_REG_CTRL2, 4, 1, 1)) {
    this->safe_set_output_data_rate_(ODR_POWER_DOWN, current_odr);
    return false;
  }

  // Enable write mode
  if (!this->write_register_bits_(STHS34PF80_REG_PAGE_RW, 6, 1, 1)) {
    this->write_register_bits_(STHS34PF80_REG_CTRL2, 4, 1, 0);
    this->safe_set_output_data_rate_(ODR_POWER_DOWN, current_odr);
    return false;
  }

  // Select register address
  if (!this->write_byte(STHS34PF80_REG_FUNC_CFG_ADDR, addr)) {
    this->write_register_bits_(STHS34PF80_REG_PAGE_RW, 6, 1, 0);
    this->write_register_bits_(STHS34PF80_REG_CTRL2, 4, 1, 0);
    this->safe_set_output_data_rate_(ODR_POWER_DOWN, current_odr);
    return false;
  }

  // Write data
  for (uint8_t i = 0; i < len; i++) {
    if (!this->write_byte(STHS34PF80_REG_FUNC_CFG_DATA, data[i])) {
      this->write_register_bits_(STHS34PF80_REG_PAGE_RW, 6, 1, 0);
      this->write_register_bits_(STHS34PF80_REG_CTRL2, 4, 1, 0);
      this->safe_set_output_data_rate_(ODR_POWER_DOWN, current_odr);
      return false;
    }
  }

  // Disable write mode
  if (!this->write_register_bits_(STHS34PF80_REG_PAGE_RW, 6, 1, 0)) {
    this->write_register_bits_(STHS34PF80_REG_CTRL2, 4, 1, 0);
    this->safe_set_output_data_rate_(ODR_POWER_DOWN, current_odr);
    return false;
  }

  // Disable access to embedded functions register
  if (!this->write_register_bits_(STHS34PF80_REG_CTRL2, 4, 1, 0)) {
    this->safe_set_output_data_rate_(ODR_POWER_DOWN, current_odr);
    return false;
  }

  // Restore ODR
  return this->safe_set_output_data_rate_(ODR_POWER_DOWN, current_odr);
}

bool STHS34PF80Component::safe_set_output_data_rate_(OutputDataRate current_odr, OutputDataRate new_odr) {
  if (new_odr > ODR_POWER_DOWN) {
    // Set to power-down first
    if (!this->write_register_bits_(STHS34PF80_REG_CTRL1, 0, 4, ODR_POWER_DOWN)) {
      return false;
    }

    // Reset algorithm
    if (!this->algorithm_reset_()) {
      return false;
    }
  } else {
    // Going to power-down from operative state
    if (current_odr > ODR_POWER_DOWN) {
      // Read func_status to clear DRDY
      uint8_t dummy;
      this->read_byte(STHS34PF80_REG_FUNC_STATUS, &dummy);

      // Wait for DRDY to go high (with timeout)
      uint32_t start_time = millis();
      bool drdy = false;
      while (millis() - start_time < 1000) {
        if (this->read_register_bit_(STHS34PF80_REG_STATUS, 2, drdy) && drdy) {
          break;
        }
        delay(1);
      }

      // Set to power-down
      if (!this->write_register_bits_(STHS34PF80_REG_CTRL1, 0, 4, ODR_POWER_DOWN)) {
        return false;
      }

      // Clear DRDY again
      this->read_byte(STHS34PF80_REG_FUNC_STATUS, &dummy);
    }
  }

  // Set final ODR
  return this->write_register_bits_(STHS34PF80_REG_CTRL1, 0, 4, static_cast<uint8_t>(new_odr));
}

bool STHS34PF80Component::read_register_bit_(uint8_t reg, uint8_t bit_pos, bool &value) {
  uint8_t reg_value;
  if (!this->read_byte(reg, &reg_value)) {
    return false;
  }
  value = (reg_value & (1 << bit_pos)) != 0;
  return true;
}

bool STHS34PF80Component::write_register_bits_(uint8_t reg, uint8_t bit_pos, uint8_t num_bits, uint8_t value) {
  uint8_t reg_value;
  if (!this->read_byte(reg, &reg_value)) {
    return false;
  }

  uint8_t mask = ((1 << num_bits) - 1) << bit_pos;
  reg_value = (reg_value & ~mask) | ((value << bit_pos) & mask);

  return this->write_byte(reg, reg_value);
}

bool STHS34PF80Component::read_register_bits_(uint8_t reg, uint8_t bit_pos, uint8_t num_bits, uint8_t &value) {
  uint8_t reg_value;
  if (!this->read_byte(reg, &reg_value)) {
    return false;
  }

  uint8_t mask = (1 << num_bits) - 1;
  value = (reg_value >> bit_pos) & mask;
  return true;
}

}  // namespace sths34pf80
}  // namespace esphome

