# STHS34PF80 - Final Architecture (MULTI_CONF)

## ✅ Top-Level Component with MULTI_CONF Support

This component uses a **top-level component architecture** with `MULTI_CONF = True` to support multiple sensor instances.

## Architecture Overview

```
sths34pf80: (top-level)
    ↓
STHS34PF80Component (PollingComponent + I2CDevice)
    ↓
sensor/binary_sensor platforms reference component via sths34pf80_id
```

## Configuration Structure

### Single Device

```yaml
# Top-level component (all hardware configuration)
sths34pf80:
  - id: my_sensor
    address: 0x5A
    update_interval: 1s
    output_data_rate: 2Hz
    enable_presence_interrupt: true
    # ... all hardware config

# Sensors reference the component
sensor:
  - platform: sths34pf80
    sths34pf80_id: my_sensor
    ambient_temperature:
      name: "Temperature"

# Binary sensors reference the component
binary_sensor:
  - platform: sths34pf80
    sths34pf80_id: my_sensor
    presence_flag:
      name: "Presence"
```

### Multiple Devices (MULTI_CONF)

```yaml
# Multiple component instances
sths34pf80:
  - id: living_room
    address: 0x5A
    update_interval: 1s
    output_data_rate: 1Hz
  
  - id: hallway
    address: 0x5B
    update_interval: 500ms
    output_data_rate: 4Hz
    motion_lpf: ODR_DIV_9  # More sensitive

# Sensors for each device
sensor:
  - platform: sths34pf80
    sths34pf80_id: living_room
    ambient_temperature:
      name: "Living Room Temperature"
  
  - platform: sths34pf80
    sths34pf80_id: hallway
    ambient_temperature:
      name: "Hallway Temperature"

# Binary sensors for each device
binary_sensor:
  - platform: sths34pf80
    sths34pf80_id: living_room
    presence_flag:
      name: "Living Room Presence"
  
  - platform: sths34pf80
    sths34pf80_id: hallway
    motion_flag:
      name: "Hallway Motion"
```

## Component Responsibilities

### Top-Level Component (`sths34pf80:`)
- ✅ I2C hardware configuration (`address`)
- ✅ Polling interval (`update_interval`)
- ✅ Hardware settings (ODR, filters, averaging, interrupts)
- ✅ Device initialization and setup
- ✅ Provides data reading methods for platforms

### Sensor Platform (`sensor:`)
- ✅ References component via `sths34pf80_id`
- ✅ Polls component for sensor data
- ✅ Can have own update interval (if needed)
- ✅ Publishes numeric sensor values

### Binary Sensor Platform (`binary_sensor:`)
- ✅ References component via `sths34pf80_id`
- ✅ Polls component for detection flags
- ✅ Can have own update interval (if needed)
- ✅ Publishes boolean detection states

## Key Features

### ✅ MULTI_CONF Support
Declare `MULTI_CONF = True` in `__init__.py` allows multiple instances:

```yaml
sths34pf80:
  - id: sensor1
    address: 0x5A
  - id: sensor2
    address: 0x5B
  - id: sensor3
    address: 0x5C
```

### ✅ Independent Configuration
Each component instance has completely independent configuration:

```yaml
sths34pf80:
  - id: slow_temp
    update_interval: 60s
    output_data_rate: 0.25Hz
  
  - id: fast_motion
    update_interval: 250ms
    output_data_rate: 8Hz
    motion_lpf: ODR_DIV_9
```

### ✅ Shared by Sensors and Binary Sensors
Same component can be used by both:

```yaml
sths34pf80:
  - id: shared
    address: 0x5A

sensor:
  - platform: sths34pf80
    sths34pf80_id: shared
    ambient_temperature:
      name: "Temp"

binary_sensor:
  - platform: sths34pf80
    sths34pf80_id: shared
    presence_flag:
      name: "Presence"
```

## Python Implementation

### `__init__.py`
```python
MULTI_CONF = True  # ← Enable multiple instances

STHS34PF80Component = sths34pf80_ns.class_(
    "STHS34PF80Component", cg.PollingComponent, i2c.I2CDevice
)

CONFIG_SCHEMA = (
    cv.Schema({
        cv.GenerateID(): cv.declare_id(STHS34PF80Component),
        cv.Optional(CONF_OUTPUT_DATA_RATE, default="1Hz"): ...,
        # ... all hardware config
    })
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x5A))
)
```

### `sensor.py`
```python
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_STHS34PF80_ID): cv.use_id(STHS34PF80Component),
    # ... sensor definitions
})

async def to_code(config):
    parent = await cg.get_variable(config[CONF_STHS34PF80_ID])
    # ... create sensors
```

### `binary_sensor.py`
```python
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_STHS34PF80_ID): cv.use_id(STHS34PF80Component),
    # ... binary sensor definitions
})

async def to_code(config):
    parent = await cg.get_variable(config[CONF_STHS34PF80_ID])
    # ... create binary sensors
```

## Benefits of This Design

1. ✅ **Clear separation**: Hardware config in component, sensor readings in platforms
2. ✅ **Multiple devices**: Full MULTI_CONF support for multiple physical sensors
3. ✅ **Independent polling**: Component and individual sensor platforms can poll independently
4. ✅ **Reusable**: Same component serves both sensor and binary_sensor platforms
5. ✅ **Standard ESPHome pattern**: Matches other multi-sensor components
6. ✅ **Single source of truth**: All hardware config in one place per device

## Example: Deep Sleep with Interrupt

```yaml
# Component with interrupt configuration
sths34pf80:
  - id: ir_sensor
    update_interval: 1s
    output_data_rate: 2Hz
    enable_presence_interrupt: true
    enable_motion_interrupt: true

# Deep sleep setup
deep_sleep:
  wakeup_pin: GPIO4
  run_duration: 10s

# Binary sensors (wake-up triggers)
binary_sensor:
  - platform: sths34pf80
    sths34pf80_id: ir_sensor
    presence_flag:
      name: "Presence"
    motion_flag:
      name: "Motion"
```

## Example: Multiple Devices

```yaml
# Two devices with different configurations
sths34pf80:
  - id: living_room
    address: 0x5A
    update_interval: 5s
    output_data_rate: 1Hz
  
  - id: hallway
    address: 0x5B
    update_interval: 250ms
    output_data_rate: 4Hz
    motion_lpf: ODR_DIV_9

# Sensors from both devices
sensor:
  - platform: sths34pf80
    sths34pf80_id: living_room
    ambient_temperature:
      name: "Living Room Temp"
  
  - platform: sths34pf80
    sths34pf80_id: hallway
    ambient_temperature:
      name: "Hallway Temp"

# Binary sensors from both devices
binary_sensor:
  - platform: sths34pf80
    sths34pf80_id: living_room
    presence_flag:
      name: "Living Room Presence"
  
  - platform: sths34pf80
    sths34pf80_id: hallway
    motion_flag:
      name: "Hallway Motion"
```

## File Structure

```
components/sths34pf80/
├── __init__.py                      # Top-level component with MULTI_CONF=True
├── sensor.py                        # Sensor platform
├── binary_sensor.py                 # Binary sensor platform
├── sths34pf80.h                     # Component C++ header
├── sths34pf80.cpp                   # Component C++ implementation
├── sths34pf80_sensor.h              # Sensor polling logic header
├── sths34pf80_sensor.cpp            # Sensor polling logic
├── sths34pf80_binary_sensor.h       # Binary sensor polling header
└── sths34pf80_binary_sensor.cpp     # Binary sensor polling logic
```

## Summary

**This is the FINAL and CORRECT architecture:**

- ✅ Top-level `sths34pf80:` component with hardware configuration
- ✅ `MULTI_CONF = True` for multiple device instances
- ✅ Sensor and binary_sensor platforms reference component via `sths34pf80_id`
- ✅ Clean, standard ESPHome pattern
- ✅ Supports all requested features (WHO_AM_I, interrupts, deep sleep, multiple devices)

All examples and documentation have been updated to reflect this architecture!

