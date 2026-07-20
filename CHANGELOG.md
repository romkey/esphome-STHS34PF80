# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-01-XX

### Added
- Initial release of STHS34PF80 ESPHome component
- Platform-based architecture with independent sensor update intervals
- Deep sleep example with interrupt wake-up support
- WHO_AM_I device verification (0xD3)
- Support for all sensor readings:
  - Ambient temperature (°C)
  - Object temperature (raw)
  - Compensated object temperature
  - Presence detection value
  - Motion detection value
  - Temperature shock value
- Binary sensors for detection flags:
  - Presence detection flag
  - Motion detection flag
  - Temperature shock detection flag
- Configurable output data rate (0.25Hz to 30Hz)
- Configurable low-pass filters for:
  - Motion detection
  - Motion + Presence detection
  - Presence detection
  - Temperature shock detection
- Configurable averaging for temperature measurements:
  - Ambient temperature averaging (1, 2, 4, 8 samples)
  - Object temperature averaging (2, 8, 32, 128, 256, 512, 1024, 2048 samples)
- Interrupt support:
  - Presence detection interrupt
  - Motion detection interrupt
  - Configurable interrupt polarity (active high/low)
  - Configurable interrupt output type (push-pull/open-drain)
  - Configurable interrupt mode (pulsed/latched)
- Wide gain mode configuration
- Block data update configuration
- Proper algorithm reset on initialization
- Safe ODR changing with power-down procedures
- Embedded function register access
- Comprehensive examples:
  - Basic configuration
  - Advanced configuration with all features
  - Home automation use case
  - Deep sleep configuration for battery-powered applications
- Detailed README with:
  - Installation instructions
  - Configuration options
  - Hardware wiring guide
  - Troubleshooting tips
  - Use case examples

### Notes
- Based on Adafruit STHS34PF80 Arduino library
- Tested with ESP32 and I2C at 400kHz
- Requires ESPHome 2023.12.0 or later (recommended)

