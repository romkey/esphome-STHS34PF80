# esphome-STHS34PF80 overview

**What:** ESPHome external component and package for the ST STHS34PF80 IR
presence/motion/temperature sensor.
**Kind:** library (component + package; no firmware/).
**Hardware:** STHS34PF80 on I2C, fixed address 0x5A, WHO_AM_I 0xD3.
Datasheet: `datasheets/sths34pf80.pdf` (DS13916).

## Files

- `components/sths34pf80/`: `__init__.py` hub (`MULTI_CONF`, polling, I2C),
  `sensor.py` and `binary_sensor.py` platforms; C++ hub in `sths34pf80.*`,
  per-entity classes in `sths34pf80_sensor.*` / `sths34pf80_binary_sensor.*`.
  The hub's `update()` calls `update()` on every registered entity.
- `packages/sths34pf80.yaml`: loads the component, creates ambient
  temperature, presence and motion entities and raw diagnostics.
- `examples/esp32*.yaml`: package on ESP32, C3, S3; all board/pin choices.
- `examples/advanced.yaml`: every component option (CI coverage).
- `examples/{home_automation,low_latency,multiple_devices,deep_sleep}.yaml`:
  use cases.
- `examples/common/base.yaml`: shared project, network, recovery and
  esphome-lib config.

## Required by the package

- An `i2c` bus with `id: i2c0` (or `sths34pf80_i2c_id`).
- Examples set `sths34pf80_component_source: ../components` so CI builds
  the local component.

## Conventions

- Substitutions prefixed `sths34pf80_`; all have defaults.
- Every component option must appear in an example (advanced.yaml).
- Register addresses and bit positions come from the datasheet and the
  Adafruit STHS34PF80 library; cite the source when changing them.
- Rules: https://github.com/romkey/esphome-ai-coding-rules/blob/main/RULES.md
