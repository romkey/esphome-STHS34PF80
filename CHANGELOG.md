# Changelog

All notable changes to this project are documented here. The format follows
[Keep a Changelog](https://keepachangelog.com/en/1.1.0/) and the project uses
[Semantic Versioning](https://semver.org/).

## [Unreleased]

### Added

- `sths34pf80:` component (multiple instances) with WHO_AM_I check,
  configurable output data rate, low-pass filters, averaging, wide gain,
  block data update and INT pin configuration for presence and motion.
- `sensor` platform: ambient, object and compensated object temperature,
  presence, motion and temperature shock values.
- `binary_sensor` platform: presence, motion and temperature shock flags.
- `packages/sths34pf80.yaml` with the common entities, configurable through
  substitutions.
- Examples for ESP32, ESP32-C3 and ESP32-S3, plus advanced, home
  automation, low latency, two-sensor and deep sleep examples, all built in
  CI.
- Docker Compose, pre-commit, GitHub CI/lint/release workflows and Renovate,
  following esphome-ai-coding-rules.
- docs/configuration.md and docs/overview.md.

### Fixed

- `amb_temp_avg` and `obj_temp_avg` now accept plain numbers (`8`), as
  documented; they previously only accepted quoted strings.
- The component loads `sensor` and `binary_sensor` itself, so it builds with
  only one of the two platforms configured.
- The two-sensor example used a second I2C address (0x5B) the sensor doesn't
  have; it now uses two I2C buses.

### Removed

- The `force_update` option, which had no effect.
