# ESPHome STHS34PF80

[![CI](https://github.com/romkey/esphome-STHS34PF80/actions/workflows/ci.yaml/badge.svg)](https://github.com/romkey/esphome-STHS34PF80/actions/workflows/ci.yaml)
[![Lint](https://github.com/romkey/esphome-STHS34PF80/actions/workflows/lint.yaml/badge.svg)](https://github.com/romkey/esphome-STHS34PF80/actions/workflows/lint.yaml)
[![ESPHome](https://img.shields.io/badge/ESPHome-external%20component-18BCF2?logo=esphome&logoColor=white)](https://esphome.io/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

An ESPHome external component for ST's STHS34PF80, a low-power infrared
sensor that detects human presence and motion and measures ambient
temperature, without a camera and through most enclosures that pass IR.
It exposes ambient and object temperatures, raw presence, motion and
temperature-shock values, and the sensor's own presence, motion and shock
flags as binary sensors, and can drive the sensor's INT pin to wake a
sleeping ESP32. A ready-made package sets up the common entities in one line.

> **AI disclosure:** This project was written by AI under human oversight,
> following [these rules](https://github.com/romkey/esphome-ai-coding-rules/blob/main/RULES.md).

## What's here

| Path | What it is |
|------|------------|
| `components/sths34pf80/` | The external component: `sths34pf80:` hub plus `sensor` and `binary_sensor` platforms |
| `packages/sths34pf80.yaml` | Loads the component and creates ambient temperature, presence and motion entities |
| `examples/esp32.yaml`, `esp32-c3.yaml`, `esp32-s3.yaml` | The package on each board |
| `examples/advanced.yaml` | Every component option and entity |
| `examples/home_automation.yaml` | Occupancy-switched light and Home Assistant events |
| `examples/low_latency.yaml` | Fastest polling, filters and averaging |
| `examples/multiple_devices.yaml` | Two sensors on two I2C buses, package included twice |
| `examples/deep_sleep.yaml` | Battery device woken by the sensor's INT pin |
| `docs/configuration.md` | Full configuration reference |
| `datasheets/sths34pf80.pdf` | ST datasheet DS13916 |

## Technical details

- **Interface:** I2C, standard, fast (400 kHz) and fast-mode plus. The
  component does not support the sensor's SPI mode.
- **Address:** fixed at `0x5A`, with no address pin (datasheet section 5.1).
  To use two sensors, put each on its own I2C bus.
- **Supply:** 1.7 V to 3.6 V; power it from the ESP32's 3.3 V.
- **WHO_AM_I:** `0xD3`; setup fails with a log error if it doesn't match.
- **Warm-up:** presence and motion detection need a minute or two to settle
  after power-on.
- **Datasheet:** [datasheets/sths34pf80.pdf](datasheets/sths34pf80.pdf)
  (ST DS13916 rev 2). Register handling follows the Adafruit STHS34PF80 Arduino library.

Wiring used by the examples:

| STHS34PF80 | ESP32 DevKit | ESP32-C3 DevKitM-1 | ESP32-S3 DevKitC-1 |
|------------|--------------|--------------------|--------------------|
| VDD | 3.3V | 3.3V | 3.3V |
| GND | GND | GND | GND |
| SDA | GPIO21 | GPIO4 | GPIO8 |
| SCL | GPIO22 | GPIO5 | GPIO9 |
| INT (optional) | GPIO4 | GPIO6 | GPIO4 |

None of these are strapping pins. Breakout boards usually include SDA/SCL
pull-ups; add 4.7 kΩ resistors if yours doesn't. For deep sleep wake-up, INT
must go to an RTC-capable GPIO and the sensor must stay powered.

### Troubleshooting

- **Setup failed / WHO_AM_I mismatch:** check wiring and that the I2C scan
  (the "I2C Bus Scan Results" entity in the examples) shows `0x5A`.
- **No readings:** `output_data_rate` must not be `POWER_DOWN`.
- **Presence never triggers:** wait for warm-up, then try shorter filters
  (`presence_lpf`) or less averaging (`obj_temp_avg`). The raw presence and
  motion values help tuning.
- **Deep sleep never wakes:** interrupts must be enabled, `int_active_low`
  must match the wake level, and `wakeup_pin` must be the INT GPIO.

## Using it in your own config

The package is the quickest way. It needs an I2C bus with `id: i2c0`:

```yaml
i2c:
  id: i2c0
  sda: GPIO21
  scl: GPIO22

packages:
  sths34pf80: github://romkey/esphome-STHS34PF80/packages/sths34pf80.yaml@v0.1.0
```

Or use the component directly:

```yaml
external_components:
  - source: github://romkey/esphome-STHS34PF80@v0.1.0
    components: [sths34pf80]

sths34pf80:
  - id: ir_sensor

sensor:
  - platform: sths34pf80
    sths34pf80_id: ir_sensor
    ambient_temperature:
      name: "Ambient Temperature"

binary_sensor:
  - platform: sths34pf80
    sths34pf80_id: ir_sensor
    presence_flag:
      name: "Presence"
    motion_flag:
      name: "Motion"
```

See [docs/configuration.md](docs/configuration.md) for every option.

## Building

Requires Docker.

```bash
cp examples/secrets.yaml.example examples/secrets.yaml   # then edit it
docker compose run --rm validate
docker compose run --rm build
docker compose run --rm lint
```

The first build downloads the toolchains into a Docker volume; later builds
reuse them and only recompile what changed. Docker Desktop on macOS can't
reach USB devices, so flash the first time with
[ESPHome Web](https://web.esphome.io) and use OTA after that:

```bash
docker compose run --rm esphome run examples/esp32.yaml
```

## License

[MIT](LICENSE)
