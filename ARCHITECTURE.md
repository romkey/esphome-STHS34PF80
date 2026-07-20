# STHS34PF80 Component Architecture

## Platform-Based Design (No Top-Level Component)

This component follows a **pure platform-based architecture** where:
1. The `sensor:` platform is **PRIMARY** - contains all hardware configuration
2. The `binary_sensor:` platform **REFERENCES** the sensor platform via `sensor_id`
3. No redundant configuration - binary_sensor inherits hardware config from sensor
4. There is NO top-level `sths34pf80:` component

## Why This Architecture?

### ✅ No Configuration Redundancy
```yaml
sensor:
  - platform: sths34pf80
    id: my_sensor
    address: 0x5A
    output_data_rate: 2Hz
    enable_presence_interrupt: true
    # ... all hardware config here

binary_sensor:
  - platform: sths34pf80
    sensor_id: my_sensor  # ← References sensor, inherits ALL config!
    presence_flag:
      name: "Presence"
```

### ✅ Supports Multiple Physical Devices
```yaml
sensor:
  # Device 1 at address 0x5A
  - platform: sths34pf80
    id: room1_sensor
    address: 0x5A
    ambient_temperature:
      name: "Room 1 Temp"
  
  # Device 2 at address 0x5B
  - platform: sths34pf80
    id: room2_sensor
    address: 0x5B
    ambient_temperature:
      name: "Room 2 Temp"

binary_sensor:
  - platform: sths34pf80
    sensor_id: room1_sensor  # Uses room 1 config
    presence_flag:
      name: "Room 1 Presence"
  
  - platform: sths34pf80
    sensor_id: room2_sensor  # Uses room 2 config
    presence_flag:
      name: "Room 2 Presence"
```

### ✅ Independent Update Rates
```yaml
sensor:
  - platform: sths34pf80
    id: my_sensor
    update_interval: 5s  # Slow for temperature
    ambient_temperature:
      name: "Temperature"

binary_sensor:
  - platform: sths34pf80
    sensor_id: my_sensor
    update_interval: 500ms  # Fast for presence!
    presence_flag:
      name: "Presence"
```

### ✅ Clean and Simple
Sensor platform has all the config, binary_sensor just references it.

## Configuration Structure

### Sensor Platform

```yaml
sensor:
  - platform: sths34pf80
    # I2C Configuration
    address: 0x5A  # Default: 0x5A
    
    # Hardware Configuration
    output_data_rate: 1Hz  # Sensor sampling rate
    motion_lpf: ODR_DIV_9
    motion_presence_lpf: ODR_DIV_20
    presence_lpf: ODR_DIV_50
    temperature_lpf: ODR_DIV_100
    amb_temp_avg: 8
    obj_temp_avg: 32
    wide_gain_mode: false
    block_data_update: true
    
    # Interrupt Configuration
    enable_presence_interrupt: false
    enable_motion_interrupt: false
    int_active_low: false
    int_open_drain: false
    int_latched: false
    
    # Polling Configuration
    update_interval: 60s
    
    # Sensors (pick what you need)
    ambient_temperature:
      name: "Ambient Temperature"
    object_temperature:
      name: "Object Temperature"
    compensated_object_temperature:
      name: "Compensated Object Temperature"
    presence:
      name: "Presence Value"
    motion:
      name: "Motion Value"
    temperature_shock:
      name: "Temperature Shock Value"
```

### Binary Sensor Platform

```yaml
binary_sensor:
  - platform: sths34pf80
    sensor_id: my_sensor  # References sensor platform
    
    # Polling Configuration (optional, can differ from sensor)
    update_interval: 60s
    
    # Binary Sensors (pick what you need)
    presence_flag:
      name: "Presence Detected"
    motion_flag:
      name: "Motion Detected"
    temperature_shock_flag:
      name: "Temperature Shock Detected"
```

## Component Sharing

Binary sensor platform **references** the sensor platform via `sensor_id`:

### ✅ Correct - Clean and Simple:
```yaml
sensor:
  - platform: sths34pf80
    id: my_sensor  # ← Give it an ID
    address: 0x5A
    output_data_rate: 2Hz
    enable_presence_interrupt: true
    update_interval: 1s
    ambient_temperature:
      name: "Temperature"

binary_sensor:
  - platform: sths34pf80
    sensor_id: my_sensor  # ← Reference the sensor
    update_interval: 500ms  # Different update rate OK!
    presence_flag:
      name: "Presence"
```

### Key Benefits:
- ✅ **No configuration duplication** - hardware config only in sensor platform
- ✅ **Single source of truth** - binary_sensor inherits from sensor
- ✅ **Independent update rates** - each can poll at different speeds
- ✅ **Cleaner YAML** - much less repetition

## Internal Architecture

### Component Hierarchy

```
sensor/binary_sensor platform
        ↓
STHS34PF80Sensor / STHS34PF80BinarySensor (PollingComponent)
        ↓
STHS34PF80Component (hardware I/O)
        ↓
I2C Device
```

### Data Flow

1. **Setup:** Platform creates/shares `STHS34PF80Component`
2. **Component setup:** Hardware initialization (WHO_AM_I, reset, configure)
3. **Update:** Each sensor/binary_sensor polls on its own schedule
4. **Read:** Calls public methods on `STHS34PF80Component`
5. **Publish:** Updates state in Home Assistant

### File Responsibilities

| File | Purpose |
|------|---------|
| `__init__.py` | Exports namespace, enums, constants |
| `sensor.py` | Sensor platform schema, creates component |
| `binary_sensor.py` | Binary sensor platform schema, shares component |
| `sths34pf80.h/.cpp` | Hardware component (I2C, registers, init) |
| `sths34pf80_sensor.h/.cpp` | Sensor polling logic |
| `sths34pf80_binary_sensor.h/.cpp` | Binary sensor polling logic |

## Advantages of This Design

### 1. Multi-Device Support
```yaml
# Two sensors, two different configurations
sensor:
  - platform: sths34pf80
    address: 0x5A
    output_data_rate: 1Hz
    ambient_temperature:
      name: "Indoor Temp"
  
  - platform: sths34pf80
    address: 0x5B
    output_data_rate: 4Hz
    ambient_temperature:
      name: "Outdoor Temp"
```

### 2. Independent Update Rates
```yaml
# Same device, different update speeds
sensor:
  - platform: sths34pf80
    update_interval: 5s  # Slow
    ambient_temperature:
      name: "Temp"

binary_sensor:
  - platform: sths34pf80
    update_interval: 500ms  # Fast
    motion_flag:
      name: "Motion"
```

### 3. Selective Sensors
```yaml
# Only use what you need
sensor:
  - platform: sths34pf80
    # Only temperature, no motion/presence
    ambient_temperature:
      name: "Temperature"
```

### 4. Deep Sleep Integration
```yaml
binary_sensor:
  - platform: sths34pf80
    enable_presence_interrupt: true
    enable_motion_interrupt: true
    presence_flag:
      name: "Presence"
    motion_flag:
      name: "Motion"
# No separate component config needed!
```

## Migration from Top-Level Component

If you had an old configuration with a top-level component:

### Before (Old):
```yaml
sths34pf80:
  id: my_sensor
  address: 0x5A
  output_data_rate: 2Hz

sensor:
  - platform: sths34pf80
    sths34pf80_id: my_sensor
    update_interval: 1s
    ambient_temperature:
      name: "Temperature"
```

### After (Current):
```yaml
sensor:
  - platform: sths34pf80
    address: 0x5A
    output_data_rate: 2Hz
    update_interval: 1s
    ambient_temperature:
      name: "Temperature"
```

**Changes:**
1. ❌ Remove `sths34pf80:` top-level block
2. ✅ Move `address` to platform
3. ✅ Move all hardware config to platform
4. ❌ Remove `sths34pf80_id` reference
5. ✅ Add `update_interval` to platform

## Best Practices

### 1. Sensor Platform is Primary
Always configure hardware in the sensor platform, binary_sensor just references it:

```yaml
sensor:
  - platform: sths34pf80
    id: my_sensor  # ← Give it an ID
    address: 0x5A
    output_data_rate: 2Hz
    enable_presence_interrupt: true
    enable_motion_interrupt: true
    update_interval: 1s
    ambient_temperature:
      name: "Temperature"

binary_sensor:
  - platform: sths34pf80
    sensor_id: my_sensor  # ← Just reference it
    update_interval: 500ms
    presence_flag:
      name: "Presence"
```

### 2. Only Configure What You Need
All hardware config options have sensible defaults. Only specify what you need to change:

```yaml
sensor:
  - platform: sths34pf80
    id: my_sensor  # ID is needed if using binary_sensor too
    # Minimal config - uses all defaults
    ambient_temperature:
      name: "Temperature"

binary_sensor:
  - platform: sths34pf80
    sensor_id: my_sensor
    presence_flag:
      name: "Presence"
```

### 3. Document Multiple Devices
When using multiple devices, use descriptive IDs and add comments:

```yaml
sensor:
  # Living room sensor (0x5A)
  - platform: sths34pf80
    id: living_room_sensor
    address: 0x5A
    ambient_temperature:
      name: "Living Room Temp"
  
  # Bedroom sensor (0x5B)
  - platform: sths34pf80
    id: bedroom_sensor
    address: 0x5B
    ambient_temperature:
      name: "Bedroom Temp"

binary_sensor:
  - platform: sths34pf80
    sensor_id: living_room_sensor
    presence_flag:
      name: "Living Room Presence"
  
  - platform: sths34pf80
    sensor_id: bedroom_sensor
    presence_flag:
      name: "Bedroom Presence"
```

## Summary

This architecture provides:
- ✅ **No configuration redundancy** - binary_sensor references sensor
- ✅ **Maximum flexibility** - each sensor independent
- ✅ **Multi-device support** - different addresses, different configs
- ✅ **Independent update rates** - sensor and binary_sensor can poll differently
- ✅ **Clean YAML** - minimal duplication
- ✅ **Follows ESPHome conventions** - platform-based design
- ✅ **Single source of truth** - hardware config only in sensor platform

**Key Principle:** Sensor platform is PRIMARY, binary_sensor is SECONDARY (references sensor via `sensor_id`).

All examples have been updated to reflect this architecture.

