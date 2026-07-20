# Quick Start Guide

Get up and running with the STHS34PF80 sensor in 5 minutes!

## 1. Hardware Setup

Connect your STHS34PF80 to your ESP32:

```
STHS34PF80    ESP32
---------     -----
VDD      →    3.3V
GND      →    GND
SDA      →    GPIO21
SCL      →    GPIO22
```

**Important:** Make sure to use 3.3V power, NOT 5V!

## 2. ESPHome Configuration

Create a new YAML file or add to your existing ESPHome configuration:

```yaml
# Enable I2C
i2c:
  sda: GPIO21
  scl: GPIO22
  scan: true

# Add the external component
external_components:
  - source: github://romkey/esphome-STHS34PF80
    components: [ sths34pf80 ]

# Configure the sensor component
sths34pf80:
  - id: ir_sensor
    update_interval: 1s

# Temperature reading
sensor:
  - platform: sths34pf80
    sths34pf80_id: ir_sensor
    ambient_temperature:
      name: "Room Temperature"

# Presence and motion detection
binary_sensor:
  - platform: sths34pf80
    sths34pf80_id: ir_sensor
    presence_flag:
      name: "Presence Detected"
    motion_flag:
      name: "Motion Detected"
```

## 3. Flash Your Device

```bash
esphome run your-config.yaml
```

## 4. Verify in Home Assistant

After the device boots up and connects to Home Assistant, you should see:
- **Room Temperature** - Current ambient temperature
- **Presence Detected** - Binary sensor for presence
- **Motion Detected** - Binary sensor for motion

## 5. Wait for Calibration

⏱️ **Important:** After power-on, wait 1-2 minutes for the sensor to stabilize before relying on presence/motion detection.

## Troubleshooting

### Sensor Not Found?
1. Check your wiring
2. Look in the logs for "STHS34PF80 setup complete"
3. Verify the I2C scan shows device at 0x5A

### No Presence/Motion Detection?
1. Wait 1-2 minutes after power-on for calibration
2. Make sure nothing is blocking the sensor
3. Try adjusting sensitivity in advanced configuration

### Still Having Issues?
Check the [full README](README.md) for detailed troubleshooting and configuration options.

## Next Steps

- See [examples/basic.yaml](examples/basic.yaml) for a complete working example
- See [examples/advanced.yaml](examples/advanced.yaml) for all configuration options
- See [examples/home_automation.yaml](examples/home_automation.yaml) for automation ideas
- See [examples/deep_sleep.yaml](examples/deep_sleep.yaml) for battery-powered deep sleep usage
- See [examples/multiple_devices.yaml](examples/multiple_devices.yaml) for using multiple sensors
- Read the [README](README.md) for complete documentation

## Need Help?

- Check the [README](README.md) troubleshooting section
- Review the example configurations in the `examples/` folder
- Open an issue on GitHub

