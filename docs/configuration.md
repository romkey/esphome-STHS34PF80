# Configuration

## `packages/sths34pf80.yaml`

Loads the component and creates ambient temperature (smoothed), presence and
motion entities, plus the raw presence and motion values as diagnostics.

**Requires:** an `i2c` bus with `id: i2c0`, or set `sths34pf80_i2c_id`.

| Substitution | Default | Meaning |
|--------------|---------|---------|
| `sths34pf80_component_source` | `github://romkey/esphome-STHS34PF80@v0.1.0` | Where to load the component from; `../components` in this repo's examples |
| `sths34pf80_id` | `sths34pf80_hub` | Component id |
| `sths34pf80_i2c_id` | `i2c0` | I2C bus id |
| `sths34pf80_update_interval` | `1s` | Polling interval |
| `sths34pf80_output_data_rate` | `1HZ` | See `output_data_rate` below |
| `sths34pf80_motion_lpf` | `ODR_DIV_9` | See filters below |
| `sths34pf80_motion_presence_lpf` | `ODR_DIV_20` | |
| `sths34pf80_presence_lpf` | `ODR_DIV_50` | |
| `sths34pf80_temperature_lpf` | `ODR_DIV_100` | |
| `sths34pf80_amb_temp_avg` | `8` | See averaging below |
| `sths34pf80_obj_temp_avg` | `32` | |
| `sths34pf80_temperature_name` | `Ambient Temperature` | Entity name |
| `sths34pf80_temperature_id` | `sths34pf80_temperature` | Sensor id |
| `sths34pf80_temperature_window` | `5` | Moving-average window (samples); publishes once per window |
| `sths34pf80_presence_name` | `Presence` | Entity name |
| `sths34pf80_presence_id` | `sths34pf80_presence` | Binary sensor id |
| `sths34pf80_motion_name` | `Motion` | Entity name |
| `sths34pf80_motion_id` | `sths34pf80_motion` | Binary sensor id |
| `sths34pf80_presence_value_name` | `Presence Value` | Entity name |
| `sths34pf80_presence_value_id` | `sths34pf80_presence_value` | Sensor id |
| `sths34pf80_motion_value_name` | `Motion Value` | Entity name |
| `sths34pf80_motion_value_id` | `sths34pf80_motion_value` | Sensor id |
| `sths34pf80_entity_category` | `diagnostic` | Category of the raw values; `""` for the main card |

Change every `*_id` when including the package twice
(see `examples/multiple_devices.yaml`).

```yaml
i2c:
  id: i2c0
  sda: GPIO21
  scl: GPIO22

packages:
  sths34pf80: github://romkey/esphome-STHS34PF80/packages/sths34pf80.yaml@v0.1.0
```

## Component: `sths34pf80:`

```yaml
external_components:
  - source: github://romkey/esphome-STHS34PF80@v0.1.0
    components: [sths34pf80]
```

The hub talks to the sensor. It's a list, so several sensors (each on its own
I2C bus; the address is fixed) can be configured.

| Option | Default | Meaning |
|--------|---------|---------|
| `id` | required | Id the platforms refer to |
| `i2c_id` | the only bus | I2C bus |
| `address` | `0x5A` | I2C address; the sensor has no other |
| `update_interval` | `60s` | How often sensors and flags are read |
| `output_data_rate` | `1HZ` | `POWER_DOWN`, `0.25HZ`, `0.5HZ`, `1HZ`, `2HZ`, `4HZ`, `8HZ`, `15HZ`, `30HZ` |
| `motion_lpf` | `ODR_DIV_9` | Motion low-pass filter |
| `motion_presence_lpf` | `ODR_DIV_20` | Motion and presence low-pass filter |
| `presence_lpf` | `ODR_DIV_50` | Presence low-pass filter |
| `temperature_lpf` | `ODR_DIV_100` | Ambient temperature shock low-pass filter |
| `amb_temp_avg` | `8` | Ambient temperature averaging: `1`, `2`, `4`, `8` samples |
| `obj_temp_avg` | `32` | Object temperature averaging: `2`, `8`, `32`, `128`, `256`, `512`, `1024`, `2048` samples |
| `wide_gain_mode` | `false` | Wide gain mode |
| `block_data_update` | `true` | Output registers update only after both bytes are read |
| `enable_presence_interrupt` | `false` | Drive INT on presence |
| `enable_motion_interrupt` | `false` | Drive INT on motion |
| `int_active_low` | `false` | INT polarity |
| `int_open_drain` | `false` | INT open-drain instead of push-pull |
| `int_latched` | `false` | INT latched instead of pulsed |

Filters are `ODR_DIV_9`, `ODR_DIV_20`, `ODR_DIV_50`, `ODR_DIV_100`,
`ODR_DIV_200`, `ODR_DIV_400` or `ODR_DIV_800`: the cut-off is the output data
rate divided by that number. Lower divisors and less averaging respond
faster; higher ones reject more noise.

## Platform: `sensor`

| Key | Unit | Meaning |
|-----|------|---------|
| `sths34pf80_id` | | Hub id (required if more than one) |
| `ambient_temperature` | °C | Ambient temperature |
| `object_temperature` | raw | Object temperature (int16) |
| `compensated_object_temperature` | raw | Ambient-compensated object temperature (int16) |
| `presence` | raw | Presence algorithm output (int16) |
| `motion` | raw | Motion algorithm output (int16) |
| `temperature_shock` | raw | Ambient temperature shock output (int16) |

Each is an optional standard ESPHome sensor (`name`, `id`, `filters`...).

## Platform: `binary_sensor`

| Key | Meaning |
|-----|---------|
| `sths34pf80_id` | Hub id (required if more than one) |
| `presence_flag` | Presence detected (`device_class: presence`) |
| `motion_flag` | Motion detected (`device_class: motion`) |
| `temperature_shock_flag` | Ambient temperature shock detected |

## Deep sleep wake-up

Enable `enable_presence_interrupt` and/or `enable_motion_interrupt`, wire INT
to an RTC-capable GPIO, and set that GPIO as `deep_sleep: wakeup_pin`. See
`examples/deep_sleep.yaml`.
