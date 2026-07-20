# STHS34PF80 ESPHome Component - Implementation Summary

## Overview

This document provides a technical summary of the STHS34PF80 ESPHome component implementation.

## Component Structure

## Component Architecture

This component uses a **platform-based architecture** with no top-level component:

1. All configuration is done in `sensor:` and `binary_sensor:` platforms
2. Each platform instance creates or shares a `STHS34PF80Component` for hardware access
3. Multiple physical devices supported via different I2C addresses
4. Same physical device can be accessed by multiple platform instances
5. Each platform instance has its own `update_interval`

### Benefits:
- ✅ Supports multiple physical STHS34PF80 devices
- ✅ Each sensor/binary_sensor can have independent update rates
- ✅ No redundant top-level configuration
- ✅ Cleaner YAML configuration
- ✅ Follows ESPHome platform patterns

### Core Files

#### `components/sths34pf80/sths34pf80.h`
The header file defines:
- All register addresses from the STHS34PF80 datasheet
- Enumeration types for configuration options:
  - `OutputDataRate` (ODR_POWER_DOWN to ODR_30_HZ)
  - `LPFConfig` (LPF_ODR_DIV_9 to LPF_ODR_DIV_800)
  - `AmbTempAvg` (AVG_T_1 to AVG_T_8)
  - `ObjTempAvg` (AVG_TMOS_2 to AVG_TMOS_2048)
- Main component class `STHS34PF80Component` inheriting from:
  - `PollingComponent` for periodic updates
  - `i2c::I2CDevice` for I2C communication
- Sensor and configuration setters
- Protected helper methods for register access

#### `components/sths34pf80/sths34pf80.cpp`
The implementation file contains:
- **setup()**: Initialization sequence
  - WHO_AM_I verification (expects 0xD3)
  - Sensor reset (OTP reboot + algorithm reset)
  - Configuration of all settings (filters, averaging, ODR, interrupts)
- **update()**: Polling method
  - Data ready check
  - Reading all sensor values
  - Publishing to Home Assistant
- **dump_config()**: Configuration logging
- **Helper methods**:
  - `reset_sensor_()`: Complete sensor reset
  - `algorithm_reset_()`: Internal algorithm reset
  - `write_embedded_function_()`: Access embedded function registers
  - `safe_set_output_data_rate_()`: ODR change with proper power-down procedures
  - Register bit manipulation helpers

#### `components/sths34pf80/__init__.py`
The Python schema file defines:
- Namespace and enum exports for sensor/binary_sensor platforms
- No top-level component schema (all configuration done in platforms)
- Shared constants and enums for hardware configuration

#### `components/sths34pf80/sensor.py`
The sensor platform schema file defines:
- Sensor platform configuration including hardware settings
- Creates STHS34PF80Component automatically
- Registers all sensor instances with polling

#### `components/sths34pf80/binary_sensor.py`
The binary sensor platform schema file defines:
- Binary sensor platform configuration including hardware settings
- Shares STHS34PF80Component with sensor platform (same address)
- Registers all binary sensor instances with polling

## Key Features Implemented

### 1. WHO_AM_I Verification ✅
- Reads register 0x0F
- Expects device ID 0xD3
- Fails setup if ID doesn't match

### 2. Sensor Readings ✅

#### Numeric Sensors:
- **Ambient Temperature**: 16-bit signed, scaled by /100 to °C
- **Object Temperature**: 16-bit signed raw value
- **Compensated Object Temperature**: 16-bit signed compensated value
- **Presence**: 16-bit signed detection value
- **Motion**: 16-bit signed detection value
- **Temperature Shock**: 16-bit signed shock value

#### Binary Sensors:
- **Presence Flag**: Bit 2 of FUNC_STATUS (0x25)
- **Motion Flag**: Bit 1 of FUNC_STATUS (0x25)
- **Temperature Shock Flag**: Bit 0 of FUNC_STATUS (0x25)

### 3. Configuration Options ✅

#### Output Data Rate (ODR):
- Power-down mode
- 0.25 Hz to 30 Hz in 9 steps
- Safe ODR changing with algorithm reset

#### Low-Pass Filters:
- Motion detection LPF
- Motion+Presence LPF
- Presence detection LPF
- Temperature shock LPF
- 7 filter options (ODR/9 to ODR/800)

#### Averaging:
- Ambient temperature: 1, 2, 4, 8 samples
- Object temperature: 2, 8, 32, 128, 256, 512, 1024, 2048 samples

#### Other Settings:
- Wide gain mode (boolean)
- Block data update (boolean)

### 4. Interrupt Support ✅

Configurable interrupts for:
- Presence detection
- Motion detection

Interrupt settings:
- Polarity (active high/low)
- Output type (push-pull/open-drain)
- Mode (pulsed/latched)
- Mask configuration

### 5. Special Procedures ✅

#### Algorithm Reset:
- Implemented via embedded function register write
- Address 0x2A, write value 0x01
- Requires power-down mode

#### Safe ODR Changing:
- When increasing ODR: Power-down → Algorithm reset → Set new ODR
- When decreasing ODR: Wait for DRDY → Power-down → Clear DRDY
- Prevents invalid sensor states

#### Embedded Function Access:
- Enable function page access
- Enable write mode
- Write to function registers
- Restore previous state

## Register Map Summary

Key registers implemented:

| Register | Address | Purpose |
|----------|---------|---------|
| WHO_AM_I | 0x0F | Device ID (0xD3) |
| LPF1 | 0x0C | Motion and Motion+Presence LPF |
| LPF2 | 0x0D | Presence and Temperature LPF |
| AVG_TRIM | 0x10 | Temperature averaging config |
| CTRL0 | 0x17 | Gain mode |
| CTRL1 | 0x20 | ODR and BDU |
| CTRL2 | 0x21 | Boot, function access, one-shot |
| CTRL3 | 0x22 | Interrupt configuration |
| STATUS | 0x23 | Data ready status |
| FUNC_STATUS | 0x25 | Detection flags |
| TAMBIENT_L/H | 0x28/0x29 | Ambient temperature |
| TOBJECT_L/H | 0x26/0x27 | Object temperature |
| TOBJ_COMP_L/H | 0x38/0x39 | Compensated object temp |
| TPRESENCE_L/H | 0x3A/0x3B | Presence value |
| TMOTION_L/H | 0x3C/0x3D | Motion value |
| TAMB_SHOCK_L/H | 0x3E/0x3F | Temperature shock value |

## Testing

### Test Files Provided:

1. **test.yaml**: Minimal configuration for compilation testing
2. **examples/basic.yaml**: Simple working example
3. **examples/advanced.yaml**: All features enabled
4. **examples/home_automation.yaml**: Real-world automation example

### Verification Steps:

1. ✅ Component compiles without errors
2. ✅ No linter errors in C++ code
3. ✅ No linter errors in Python code
4. ✅ Configuration schema validates correctly
5. ✅ All register addresses match datasheet
6. ✅ All procedures match reference implementation

## Code Quality

### Safety Features:
- Proper error checking on all I2C operations
- Safe ODR changing to prevent sensor lock-up
- Timeout on DRDY wait (1 second)
- Graceful degradation on communication errors

### ESPHome Integration:
- Follows ESPHome component patterns
- Uses standard ESPHome logging
- Implements PollingComponent correctly
- Proper sensor registration and publishing

### Documentation:
- Comprehensive README with examples
- Quick start guide
- Multiple example configurations
- Inline code comments
- Configuration option documentation

## Reference Implementation

Based on:
- **Adafruit STHS34PF80 Arduino Library**
  - File: `Adafruit_STHS34PF80.cpp`
  - File: `Adafruit_STHS34PF80.h`
- **ST STHS34PF80 Datasheet**
  - File: `datasheets/sths34pf80.pdf`

Key algorithms ported:
1. `sths34pf80_tmos_odr_set()` → `safe_set_output_data_rate_()`
2. `sths34pf80_algo_reset()` → `algorithm_reset_()`
3. `sths34pf80_func_cfg_write()` → `write_embedded_function_()`

## Future Enhancements (Optional)

Potential future features not implemented in v1.0:
- One-shot mode trigger capability
- Sensitivity value adjustment
- Threshold configuration for interrupts
- Advanced embedded function access
- Temperature shock threshold configuration

## File Manifest

```
esphome-STHS34PF80/
├── components/sths34pf80/
│   ├── __init__.py           # ESPHome Python schema
│   ├── sths34pf80.h          # C++ header file
│   └── sths34pf80.cpp        # C++ implementation
├── examples/
│   ├── basic.yaml            # Basic example
│   ├── advanced.yaml         # Advanced example
│   └── home_automation.yaml  # Home automation example
├── reference/                # Original Arduino library
├── datasheets/              # Sensor datasheet
├── README.md                # Main documentation
├── QUICKSTART.md           # Quick start guide
├── CHANGELOG.md            # Version history
├── LICENSE                 # MIT License
├── .gitignore             # Git ignore rules
└── test.yaml              # Test configuration
```

## License

MIT License - See LICENSE file for details.

## Credits

- Component implementation: John Romkey
- Reference library: Adafruit Industries
- Sensor manufacturer: STMicroelectronics

