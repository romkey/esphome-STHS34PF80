# STHS34PF80 Examples

This directory contains example configurations demonstrating different use cases for the STHS34PF80 sensor.

## Available Examples

### 1. basic.yaml
**Use Case:** Simple presence and temperature monitoring

**Features:**
- Minimal configuration
- Ambient temperature sensor
- Presence and motion binary sensors
- 1 second update interval
- Perfect for getting started

**When to use:**
- Testing the sensor for the first time
- Simple presence detection
- Quick prototyping
- Learning the component basics

---

### 2. advanced.yaml
**Use Case:** Full-featured configuration with all sensors and options

**Features:**
- All 6 numeric sensors configured
- All 3 binary sensors configured
- Custom low-pass filter settings
- Custom averaging settings
- Interrupt configuration
- Template sensors for derived values
- Demonstrates all configuration options

**When to use:**
- Need access to all sensor data
- Fine-tuning sensor performance
- Understanding all available options
- Production deployments with specific requirements

---

### 3. home_automation.yaml
**Use Case:** Automated lighting and HVAC control

**Features:**
- Presence-based automation
- Light control based on occupancy
- Room state tracking
- Template binary sensors with hysteresis
- Event notifications to Home Assistant
- Scripting for complex behaviors

**When to use:**
- Smart lighting systems
- HVAC automation
- Room occupancy tracking
- Energy saving applications
- Integration with Home Assistant automations

---

### 4. deep_sleep.yaml
**Use Case:** Battery-powered applications with interrupt wake-up

**Features:**
- Deep sleep configuration
- Interrupt-based wake-up on motion/presence
- Battery-saving optimizations
- Fast WiFi connect
- Boot counting and wake reason tracking
- Status LED feedback
- Manual sleep trigger
- Sleep prevention switch for debugging

**When to use:**
- Battery-powered devices
- Low-power applications
- Security sensors
- Remote monitoring
- Solar-powered installations
- Anywhere power consumption is critical

**Key Configuration:**
- Sensor configured with interrupts enabled
- INT pin connected to wakeup-capable GPIO
- Deep sleep with run duration and wakeup pin
- Fallback timer for periodic wake-up

---

### 5. multiple_devices.yaml
**Use Case:** Multiple STHS34PF80 sensors in different rooms

**Features:**
- Two physical sensors at different I2C addresses
- Different update rates per device
- Different configurations per room (living room vs. hallway)
- Aggregate temperature and occupancy sensors
- Per-room automation
- House-wide status tracking

**When to use:**
- Multi-room monitoring
- Different sensitivity needs per location
- Whole-house occupancy tracking
- Multiple security zones
- Temperature monitoring in different areas

**Key Configuration:**
- Each sensor at different I2C address (0x5A, 0x5B, etc.)
- Independent update intervals per room
- Room-specific automation triggers
- Template sensors for aggregate data

---

## Comparison Table

| Feature | basic.yaml | advanced.yaml | home_automation.yaml | deep_sleep.yaml | multiple_devices.yaml |
|---------|-----------|---------------|---------------------|-----------------|----------------------|
| **Complexity** | ⭐ Simple | ⭐⭐⭐ Complex | ⭐⭐ Moderate | ⭐⭐ Moderate | ⭐⭐⭐ Complex |
| **Power Usage** | High | High | High | ⭐ Very Low | High |
| **Update Rate** | 1s | 500ms | 500ms | 1s (when awake) | Mixed (500ms-5s) |
| **Sensors** | 1 + 2 flags | All 6 + 3 flags | 1 + 2 flags | 3 + 2 flags | Multiple devices |
| **Devices** | 1 | 1 | 1 | 1 | ✅ 2+ |
| **Interrupts** | No | Yes | No | ✅ Yes (required) | Optional |
| **Automation** | No | Template only | ✅ Full | ✅ Sleep management | ✅ Multi-room |
| **Battery Powered** | No | No | No | ✅ Yes | No |
| **Home Assistant** | Basic | Advanced | ✅ Events | ✅ Events | ✅ Events |

## Getting Started

1. **New to the sensor?** → Start with `basic.yaml`
2. **Need all features?** → Use `advanced.yaml`
3. **Building automation?** → Check `home_automation.yaml`
4. **Battery powered?** → Use `deep_sleep.yaml`
5. **Multiple sensors?** → See `multiple_devices.yaml`

## Multiple Devices

This component supports multiple STHS34PF80 sensors using MULTI_CONF:

```yaml
# Two sensors on the same I2C bus with different addresses
i2c:
  sda: GPIO21
  scl: GPIO22

# Multiple component instances (MULTI_CONF)
sths34pf80:
  # First sensor at default address (0x5A)
  - id: room1_sensor
    address: 0x5A
    update_interval: 1s
  
  # Second sensor at alternate address (requires hardware address pin change)
  - id: room2_sensor
    address: 0x5B
    update_interval: 1s

sensor:
  # Sensors for first device
  - platform: sths34pf80
    sths34pf80_id: room1_sensor
    ambient_temperature:
      name: "Room 1 Temperature"
  
  # Sensors for second device
  - platform: sths34pf80
    sths34pf80_id: room2_sensor
    ambient_temperature:
      name: "Room 2 Temperature"

binary_sensor:
  # Presence detection for first sensor
  - platform: sths34pf80
    sths34pf80_id: room1_sensor
    presence_flag:
      name: "Room 1 Presence"
  
  # Presence detection for second sensor
  - platform: sths34pf80
    sths34pf80_id: room2_sensor
    presence_flag:
      name: "Room 2 Presence"
```

**Key Point:** Use top-level `sths34pf80:` with list syntax (`- id:`) for multiple devices!

## Hardware Requirements

### All Examples
- ESP32 (recommended) or ESP8266
- STHS34PF80 sensor
- I2C connections (SDA, SCL, VDD, GND)

### Deep Sleep Example (additional)
- INT pin connection to wakeup-capable GPIO (e.g., GPIO4)
- Battery or low-power supply
- Optional: Status LED

## Configuration Tips

### Update Intervals
- **Fast (250-500ms):** Security, rapid response needed
- **Normal (1-2s):** General monitoring, home automation
- **Slow (5-60s):** Temperature logging, energy saving
- **Deep sleep:** Wake on interrupt only

### Interrupt Configuration
```yaml
# Configure interrupts in the component
sths34pf80:
  - id: my_sensor
    enable_presence_interrupt: true
    enable_motion_interrupt: true
    int_active_low: false        # Active high for most ESP32 pins
    int_open_drain: false        # Push-pull recommended
    int_latched: false           # Pulsed for immediate response

binary_sensor:
  - platform: sths34pf80
    sths34pf80_id: my_sensor
    presence_flag:
      name: "Presence"
```

### Low-Pass Filters
- **Lower values (ODR_DIV_9):** Faster response, more sensitive
- **Higher values (ODR_DIV_800):** Slower response, less noise

Tune based on your environment:
- **Quiet spaces:** Higher filter values
- **Busy spaces:** Lower filter values

### Averaging
- **Less averaging (2-32 samples):** Faster response
- **More averaging (128-2048 samples):** More stable, less noise

## Common Modifications

### Change Update Rate
```yaml
sensor:
  - platform: sths34pf80
    update_interval: 2s  # Change this
```

### Enable Only Specific Sensors
```yaml
sensor:
  - platform: sths34pf80
    # Only include the sensors you need
    ambient_temperature:
      name: "Temperature"
    # Omit others to save resources
```

### Adjust Sensitivity
```yaml
sths34pf80:
  - id: my_sensor
    # Lower filter = more sensitive
    motion_lpf: ODR_DIV_9
    presence_lpf: ODR_DIV_20
    
    # Less averaging = faster response
    obj_temp_avg: 32

sensor:
  - platform: sths34pf80
    sths34pf80_id: my_sensor
    ambient_temperature:
      name: "Temperature"

binary_sensor:
  - platform: sths34pf80
    sths34pf80_id: my_sensor
    presence_flag:
      name: "Presence"
```

## Troubleshooting

If an example doesn't work:
1. Check I2C connections
2. Verify I2C address (0x5A)
3. Enable I2C scanning: `i2c: scan: true`
4. Check ESP logs for errors
5. Ensure sensor has 1-2 minutes to stabilize
6. Review the main [README](../README.md) troubleshooting section

## Further Reading

- [Main README](../README.md) - Complete documentation
- [QUICKSTART Guide](../QUICKSTART.md) - 5-minute setup
- [Component Summary](../COMPONENT_SUMMARY.md) - Technical details

