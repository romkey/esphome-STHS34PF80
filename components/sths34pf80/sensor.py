import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    ICON_THERMOMETER,
)
from . import sths34pf80_ns, STHS34PF80Component, CONF_STHS34PF80_ID

DEPENDENCIES = ["sths34pf80"]

CONF_AMBIENT_TEMPERATURE = "ambient_temperature"
CONF_OBJECT_TEMPERATURE = "object_temperature"
CONF_COMPENSATED_OBJECT_TEMPERATURE = "compensated_object_temperature"
CONF_PRESENCE = "presence"
CONF_MOTION = "motion"
CONF_TEMPERATURE_SHOCK = "temperature_shock"

ICON_MOTION_SENSOR = "mdi:motion-sensor"
ICON_PULSE = "mdi:pulse"

STHS34PF80Sensor = sths34pf80_ns.class_("STHS34PF80Sensor", sensor.Sensor)
SensorType = sths34pf80_ns.enum("SensorType")
SENSOR_TYPES = {
    CONF_AMBIENT_TEMPERATURE: SensorType.SENSOR_AMBIENT_TEMP,
    CONF_OBJECT_TEMPERATURE: SensorType.SENSOR_OBJECT_TEMP,
    CONF_COMPENSATED_OBJECT_TEMPERATURE: SensorType.SENSOR_COMPENSATED_OBJECT_TEMP,
    CONF_PRESENCE: SensorType.SENSOR_PRESENCE,
    CONF_MOTION: SensorType.SENSOR_MOTION,
    CONF_TEMPERATURE_SHOCK: SensorType.SENSOR_TEMPERATURE_SHOCK,
}

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_STHS34PF80_ID): cv.use_id(STHS34PF80Component),
        cv.Optional(CONF_AMBIENT_TEMPERATURE): sensor.sensor_schema(
            STHS34PF80Sensor,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            icon=ICON_THERMOMETER,
        ),
        cv.Optional(CONF_OBJECT_TEMPERATURE): sensor.sensor_schema(
            STHS34PF80Sensor,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            icon=ICON_THERMOMETER,
        ),
        cv.Optional(CONF_COMPENSATED_OBJECT_TEMPERATURE): sensor.sensor_schema(
            STHS34PF80Sensor,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            icon=ICON_THERMOMETER,
        ),
        cv.Optional(CONF_PRESENCE): sensor.sensor_schema(
            STHS34PF80Sensor,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            icon=ICON_PULSE,
        ),
        cv.Optional(CONF_MOTION): sensor.sensor_schema(
            STHS34PF80Sensor,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            icon=ICON_MOTION_SENSOR,
        ),
        cv.Optional(CONF_TEMPERATURE_SHOCK): sensor.sensor_schema(
            STHS34PF80Sensor,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            icon=ICON_THERMOMETER,
        ),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_STHS34PF80_ID])

    # Create sensor instances
    for key, sensor_type in SENSOR_TYPES.items():
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(sens.set_parent(parent))
            cg.add(sens.set_sensor_type(sensor_type))
            cg.add(parent.register_sensor(sens))

