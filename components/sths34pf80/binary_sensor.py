import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
    DEVICE_CLASS_MOTION,
    DEVICE_CLASS_PRESENCE,
)

from . import CONF_STHS34PF80_ID, STHS34PF80Component, sths34pf80_ns

DEPENDENCIES = ["sths34pf80"]

CONF_PRESENCE_FLAG = "presence_flag"
CONF_MOTION_FLAG = "motion_flag"
CONF_TEMPERATURE_SHOCK_FLAG = "temperature_shock_flag"

STHS34PF80BinarySensor = sths34pf80_ns.class_(
    "STHS34PF80BinarySensor", binary_sensor.BinarySensor
)
BinarySensorType = sths34pf80_ns.enum("BinarySensorType")
BINARY_SENSOR_TYPES = {
    CONF_PRESENCE_FLAG: BinarySensorType.BINARY_SENSOR_PRESENCE,
    CONF_MOTION_FLAG: BinarySensorType.BINARY_SENSOR_MOTION,
    CONF_TEMPERATURE_SHOCK_FLAG: BinarySensorType.BINARY_SENSOR_TEMPERATURE_SHOCK,
}

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_STHS34PF80_ID): cv.use_id(STHS34PF80Component),
        cv.Optional(CONF_PRESENCE_FLAG): binary_sensor.binary_sensor_schema(
            STHS34PF80BinarySensor,
            device_class=DEVICE_CLASS_PRESENCE,
        ),
        cv.Optional(CONF_MOTION_FLAG): binary_sensor.binary_sensor_schema(
            STHS34PF80BinarySensor,
            device_class=DEVICE_CLASS_MOTION,
        ),
        cv.Optional(CONF_TEMPERATURE_SHOCK_FLAG): binary_sensor.binary_sensor_schema(
            STHS34PF80BinarySensor,
        ),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_STHS34PF80_ID])

    # Create binary sensor instances
    for key, sensor_type in BINARY_SENSOR_TYPES.items():
        if key in config:
            sens = await binary_sensor.new_binary_sensor(config[key])
            cg.add(sens.set_parent(parent))
            cg.add(sens.set_sensor_type(sensor_type))
            cg.add(parent.register_binary_sensor(sens))
