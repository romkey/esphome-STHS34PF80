# ESPHome STHS34PF80 Component

ESPHome custom component for the STHS34PF80 infrared temperature sensor with presence and motion detection capabilities.

## Features

The STHS34PF80 is an infrared temperature sensor that can detect:
- Ambient temperature
- Object temperature (raw and compensated)
- Human presence detection
- Motion detection
- Temperature shock events

This component provides:
- ✅ WHO_AM_I device verification (0xD3)
- ✅ Full sensor data access (temperature, presence, motion, shock)
- ✅ Binary sensors for presence/motion/shock detection flags
- ✅ Configurable output data rate (0.25HZ to 30Hz)
- ✅ Configurable low-pass filters
- ✅ Configurable averaging for temperature measurements
- ✅ Interrupt support for presence and motion detection
- ✅ Deep sleep wake-up support for battery-powered applications
- ✅ I2C interface (default address: 0x5A)

## Installation

### Option 1: External Components (Recommended)

Add this to your ESPHome YAML configuration:

```yaml
external_components:
  - source: github://romkey/esphome-STHS34PF80
    components: [ sths34pf80 ]
```

### Option 2: Local Custom Component

1. Create a `components` directory in your ESPHome configuration directory
2. Copy the `sths34pf80` folder into the `components` directory
3. Reference it in your YAML:

```yaml
external_components:
  - source: components
    components: [ sths34pf80 ]
```

## Basic Configuration

### Minimal Configuration

```yaml
i2c:
  sda: GPIO21
  scl: GPIO22
  scan: true

sths34pf80:
  - id: ir_sensor
    update_interval: 1s

sensor:
  - platform: sths34pf80
    sths34pf80_id: ir_sensor
    ambient_temperature:
      name: "Ambient Temperature"

binary_sensor:
  - platform: sths34pf80
    sths34pf80_id: ir_sensor
    presence_flag:
      name: "Presence Detected"
    motion_flag:
      name: "Motion Detected"
```

### Full Configuration Example

```yaml
i2c:
  sda: GPIO21
  scl: GPIO22
  scan: true

# STHS34PF80 Component (top-level configuration)
sths34pf80:
  - id: ir_sensor
    address: 0x5A  # I2C address (default: 0x5A)
    update_interval: 1s  # How often component updates
    
    # Configuration options
    output_data_rate: 1HZ  # Options: POWER_DOWN, 0.25HZ, 0.5HZ, 1HZ, 2HZ, 4HZ, 8HZ, 15HZ, 30HZ
    
    # Low-pass filter settings
    motion_lpf: ODR_DIV_9           # Motion detection filter
    motion_presence_lpf: ODR_DIV_20  # Motion+Presence detection filter
    presence_lpf: ODR_DIV_50         # Presence detection filter
    temperature_lpf: ODR_DIV_100     # Temperature shock filter
    
    # Averaging settings
    amb_temp_avg: 8    # Ambient temp averaging: 1, 2, 4, 8
    obj_temp_avg: 32   # Object temp averaging: 2, 8, 32, 128, 256, 512, 1024, 2048
    
    # Gain and update settings
    wide_gain_mode: false      # Wide gain mode (default: false)
    block_data_update: true    # Block data update (default: true)
    
    # Interrupt configuration (optional)
    enable_presence_interrupt: true   # Enable interrupt on presence detection
    enable_motion_interrupt: true     # Enable interrupt on motion detection
    int_active_low: false             # Interrupt polarity (false = active high)
    int_open_drain: false             # Interrupt output type (false = push-pull)
    int_latched: false                # Interrupt mode (false = pulsed, true = latched)

# Sensors (numeric values)
sensor:
  - platform: sths34pf80
    sths34pf80_id: ir_sensor
    ambient_temperature:
      name: "Ambient Temperature"
      id: amb_temp
    object_temperature:
      name: "Object Temperature Raw"
      id: obj_temp
    compensated_object_temperature:
      name: "Object Temperature Compensated"
      id: obj_temp_comp
    presence:
      name: "Presence Value"
      id: presence_val
    motion:
      name: "Motion Value"
      id: motion_val
    temperature_shock:
      name: "Temperature Shock Value"
      id: temp_shock_val

# Binary sensors (detection flags)
binary_sensor:
  - platform: sths34pf80
    sths34pf80_id: ir_sensor
    presence_flag:
      name: "Presence Detected"
      id: presence_flag
    motion_flag:
      name: "Motion Detected"
      id: motion_flag
    temperature_shock_flag:
      name: "Temperature Shock Detected"
      id: temp_shock_flag
```

## Configuration Variables

### Main Component (sths34pf80:)

The top-level component configures the hardware. Supports multiple instances via MULTI_CONF.

- **id** (*Required*, ID): Unique ID for this sensor instance.
- **address** (*Optional*, int): I2C address of the sensor. Defaults to `0x5A`.
- **update_interval** (*Optional*, Time): How often the component updates. Defaults to `60s`.

**Hardware Configuration:**
- **output_data_rate** (*Optional*, enum): Sensor sampling rate. Defaults to `1HZ`.
- **motion_lpf** (*Optional*, enum): Motion detection low-pass filter. Defaults to `ODR_DIV_9`.
- **motion_presence_lpf** (*Optional*, enum): Motion+Presence low-pass filter. Defaults to `ODR_DIV_20`.
- **presence_lpf** (*Optional*, enum): Presence detection low-pass filter. Defaults to `ODR_DIV_50`.
- **temperature_lpf** (*Optional*, enum): Temperature shock low-pass filter. Defaults to `ODR_DIV_100`.
- **amb_temp_avg** (*Optional*, int): Ambient temperature averaging samples. Defaults to `8`.
- **obj_temp_avg** (*Optional*, int): Object temperature averaging samples. Defaults to `32`.
- **wide_gain_mode** (*Optional*, boolean): Enable wide gain mode. Defaults to `false`.
- **block_data_update** (*Optional*, boolean): Enable block data update. Defaults to `true`.
- **enable_presence_interrupt** (*Optional*, boolean): Enable interrupt on presence detection. Defaults to `false`.
- **enable_motion_interrupt** (*Optional*, boolean): Enable interrupt on motion detection. Defaults to `false`.
- **int_active_low** (*Optional*, boolean): Set interrupt as active low. Defaults to `false`.
- **int_open_drain** (*Optional*, boolean): Set interrupt output as open-drain. Defaults to `false`.
- **int_latched** (*Optional*, boolean): Set interrupt as latched mode. Defaults to `false`.

### Sensor Platform

The sensor platform provides access to numeric sensor values.

- **sths34pf80_id** (*Required*, ID): ID of the STHS34PF80 component to use.

**Available Sensors:**
- **ambient_temperature** (*Optional*): Ambient temperature in °C (float).
- **object_temperature** (*Optional*): Raw object temperature value (int16).
- **compensated_object_temperature** (*Optional*): Compensated object temperature value (int16).
- **presence** (*Optional*): Raw presence detection value (int16).
- **motion** (*Optional*): Raw motion detection value (int16).
- **temperature_shock** (*Optional*): Temperature shock detection value (int16).

### Binary Sensor Platform

The binary sensor platform provides detection flags.

- **sths34pf80_id** (*Required*, ID): ID of the STHS34PF80 component to use.

**Available Binary Sensors:**
- **presence_flag** (*Optional*): Boolean flag indicating presence detection.
- **motion_flag** (*Optional*): Boolean flag indicating motion detection.
- **temperature_shock_flag** (*Optional*): Boolean flag indicating temperature shock.

### Configuration Options

#### Output Data Rate
- **output_data_rate** (*Optional*, enum): Sensor sampling rate. Defaults to `1HZ`.
  - `POWER_DOWN` - Power-down mode
  - `0.25HZ` - 0.25 Hz
  - `0.5HZ` - 0.5 Hz
  - `1HZ` - 1 Hz (default)
  - `2HZ` - 2 Hz
  - `4HZ` - 4 Hz
  - `8Hz` - 8 Hz
  - `15Hz` - 15 Hz
  - `30Hz` - 30 Hz

#### Low-Pass Filters
- **motion_lpf** (*Optional*, enum): Motion detection low-pass filter. Defaults to `ODR_DIV_9`.
- **motion_presence_lpf** (*Optional*, enum): Motion+Presence low-pass filter. Defaults to `ODR_DIV_20`.
- **presence_lpf** (*Optional*, enum): Presence detection low-pass filter. Defaults to `ODR_DIV_50`.
- **temperature_lpf** (*Optional*, enum): Temperature shock low-pass filter. Defaults to `ODR_DIV_100`.

Filter options: `ODR_DIV_9`, `ODR_DIV_20`, `ODR_DIV_50`, `ODR_DIV_100`, `ODR_DIV_200`, `ODR_DIV_400`, `ODR_DIV_800`

#### Averaging
- **amb_temp_avg** (*Optional*, int): Ambient temperature averaging samples. Defaults to `8`.
  - Options: `1`, `2`, `4`, `8`
- **obj_temp_avg** (*Optional*, int): Object temperature averaging samples. Defaults to `32`.
  - Options: `2`, `8`, `32`, `128`, `256`, `512`, `1024`, `2048`

#### Other Settings
- **wide_gain_mode** (*Optional*, boolean): Enable wide gain mode. Defaults to `false`.
- **block_data_update** (*Optional*, boolean): Enable block data update. Defaults to `true`.

#### Interrupt Configuration
- **enable_presence_interrupt** (*Optional*, boolean): Enable interrupt on presence detection. Defaults to `false`.
- **enable_motion_interrupt** (*Optional*, boolean): Enable interrupt on motion detection. Defaults to `false`.
- **int_active_low** (*Optional*, boolean): Set interrupt as active low. Defaults to `false` (active high).
- **int_open_drain** (*Optional*, boolean): Set interrupt output as open-drain. Defaults to `false` (push-pull).
- **int_latched** (*Optional*, boolean): Set interrupt as latched mode. Defaults to `false` (pulsed).

## Use Cases

### Home Automation - Presence Detection

```yaml
sths34pf80:
  - id: room_sensor
    update_interval: 500ms
    output_data_rate: 2HZ

binary_sensor:
  - platform: sths34pf80
    sths34pf80_id: room_sensor
    presence_flag:
      name: "Room Occupancy"
      on_press:
        - light.turn_on: room_light
      on_release:
        - light.turn_off: room_light
    motion_flag:
      name: "Room Motion"
```

### Security - Motion Detection

```yaml
sths34pf80:
  - id: security_sensor
    update_interval: 250ms
    output_data_rate: 4HZ

binary_sensor:
  - platform: sths34pf80
    sths34pf80_id: security_sensor
    motion_flag:
      name: "Security Motion"
      on_press:
        - homeassistant.event:
            event: esphome.motion_detected
            data:
              location: front_door
```

### Temperature Monitoring

```yaml
sths34pf80:
  - id: temp_monitor
    update_interval: 5s

sensor:
  - platform: sths34pf80
    sths34pf80_id: temp_monitor
    ambient_temperature:
      name: "Room Temperature"
      filters:
        - sliding_window_moving_average:
            window_size: 10
            send_every: 10
```

### Deep Sleep with Interrupt Wake-up (Battery Powered)

```yaml
# Deep sleep configuration
deep_sleep:
  id: deep_sleep_control
  run_duration: 10s
  sleep_duration: 60min
  wakeup_pin: GPIO4  # Connected to sensor INT pin
  wakeup_pin_mode: IGNORE

# Configure sensor with interrupts for wake-up
sths34pf80:
  - id: ir_sensor
    update_interval: 1s
    output_data_rate: 2HZ
    enable_presence_interrupt: true
    enable_motion_interrupt: true
    int_active_low: false
    int_open_drain: false
    int_latched: false

binary_sensor:
  - platform: sths34pf80
    sths34pf80_id: ir_sensor
    presence_flag:
      name: "Presence"
      on_press:
        then:
          - logger.log: "Presence detected - woke from deep sleep!"
    motion_flag:
      name: "Motion"
      on_press:
        then:
          - logger.log: "Motion detected - woke from deep sleep!"
```

**See `examples/deep_sleep.yaml` for a complete battery-powered example.**

## Hardware Connection

### I2C Wiring

| STHS34PF80 Pin | ESP32 Pin | Description |
|----------------|-----------|-------------|
| VDD | 3.3V | Power supply |
| GND | GND | Ground |
| SDA | GPIO21 | I2C Data |
| SCL | GPIO22 | I2C Clock |
| INT (optional) | GPIO4 | Interrupt output |

### Notes
- The sensor operates at 3.3V
- Pull-up resistors (4.7kΩ) are typically required on SDA and SCL lines
- The INT pin is optional and only needed if you're using interrupts
- For deep sleep applications, connect INT to a wakeup-capable GPIO pin

### Interrupt Pin Configuration

When using interrupts (especially for deep sleep wake-up):
1. Connect sensor INT pin to a GPIO that supports external wake-up (e.g., GPIO4)
2. Configure the sensor interrupt settings:
   - `enable_presence_interrupt: true` and/or `enable_motion_interrupt: true`
   - `int_active_low: false` (active high) or `true` (active low) based on your needs
   - `int_open_drain: false` (push-pull) recommended for ESP32
   - `int_latched: false` (pulsed) for immediate response
3. In your deep_sleep configuration, set `wakeup_pin` to the connected GPIO

## Troubleshooting

### Sensor Not Detected
- Check I2C wiring (SDA, SCL, VDD, GND)
- Verify I2C address (default: 0x5A)
- Enable I2C scanning: `i2c: scan: true`
- Check power supply (3.3V)

### Invalid WHO_AM_I Error
- Device ID should be 0xD3
- If you get a different value, verify you have the correct sensor
- Check for I2C bus conflicts

### No Data Updates
- Verify `output_data_rate` is not set to `POWER_DOWN`
- Check `update_interval` is appropriate
- Ensure sensor has completed initialization

### Presence/Motion Not Detecting
- The sensor needs time to stabilize after power-on (recommend 1-2 minutes)
- Adjust low-pass filter settings for your environment
- Try adjusting `obj_temp_avg` for better sensitivity
- Ensure there's no obstruction in front of the sensor

### Deep Sleep Not Waking on Interrupt
- Verify INT pin is connected to a wakeup-capable GPIO
- Check interrupt configuration: `enable_presence_interrupt` or `enable_motion_interrupt` must be `true`
- Ensure `int_active_low` matches your wiring (usually `false` for active high)
- Verify the sensor is powered during deep sleep (VDD should remain connected)
- Check that `wakeup_pin` in `deep_sleep` configuration matches the connected GPIO
- Use `wakeup_pin_mode: IGNORE` to wake on any edge
- Test with a manual GPIO interrupt first to verify wiring
- Add a pull-down resistor (10kΩ) to the INT pin if you experience false wake-ups

## Technical Details

### Register Access
The component implements all major STHS34PF80 registers including:
- WHO_AM_I verification (0xD3)
- Control registers (CTRL0-CTRL3)
- Temperature data registers
- Presence/Motion detection registers
- Embedded function access for algorithm reset

### Sensor Specifications
- Interface: I2C (up to 400 kHz)
- Supply voltage: 1.7V to 3.6V (typically 3.3V)
- Operating temperature: -40°C to +85°C
- Detection range: Several meters (depending on configuration)

## Credits

- Component developed for ESPHome
- Based on Adafruit STHS34PF80 Arduino library
- Sensor datasheet: ST STHS34PF80

## License

This component is provided as-is under the MIT License.

## Contributing

Contributions are welcome! Please submit issues and pull requests on GitHub.

