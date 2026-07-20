import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

CODEOWNERS = ["@romkey"]
DEPENDENCIES = ["i2c"]
MULTI_CONF = True

sths34pf80_ns = cg.esphome_ns.namespace("sths34pf80")
STHS34PF80Component = sths34pf80_ns.class_(
    "STHS34PF80Component", cg.PollingComponent, i2c.I2CDevice
)

# Configuration constants
CONF_STHS34PF80_ID = "sths34pf80_id"
CONF_OUTPUT_DATA_RATE = "output_data_rate"
CONF_MOTION_LPF = "motion_lpf"
CONF_MOTION_PRESENCE_LPF = "motion_presence_lpf"
CONF_PRESENCE_LPF = "presence_lpf"
CONF_TEMPERATURE_LPF = "temperature_lpf"
CONF_AMB_TEMP_AVG = "amb_temp_avg"
CONF_OBJ_TEMP_AVG = "obj_temp_avg"
CONF_WIDE_GAIN_MODE = "wide_gain_mode"
CONF_BLOCK_DATA_UPDATE = "block_data_update"
CONF_ENABLE_PRESENCE_INTERRUPT = "enable_presence_interrupt"
CONF_ENABLE_MOTION_INTERRUPT = "enable_motion_interrupt"
CONF_INT_ACTIVE_LOW = "int_active_low"
CONF_INT_OPEN_DRAIN = "int_open_drain"
CONF_INT_LATCHED = "int_latched"
CONF_FORCE_UPDATE = "force_update"

# Enums
OutputDataRate = sths34pf80_ns.enum("OutputDataRate")
OUTPUT_DATA_RATES = {
    "POWER_DOWN": OutputDataRate.ODR_POWER_DOWN,
    "0.25HZ": OutputDataRate.ODR_0_25_HZ,
    "0.5HZ": OutputDataRate.ODR_0_5_HZ,
    "1HZ": OutputDataRate.ODR_1_HZ,
    "2HZ": OutputDataRate.ODR_2_HZ,
    "4HZ": OutputDataRate.ODR_4_HZ,
    "8HZ": OutputDataRate.ODR_8_HZ,
    "15HZ": OutputDataRate.ODR_15_HZ,
    "30HZ": OutputDataRate.ODR_30_HZ,
}

LPFConfig = sths34pf80_ns.enum("LPFConfig")
LPF_CONFIGS = {
    "ODR_DIV_9": LPFConfig.LPF_ODR_DIV_9,
    "ODR_DIV_20": LPFConfig.LPF_ODR_DIV_20,
    "ODR_DIV_50": LPFConfig.LPF_ODR_DIV_50,
    "ODR_DIV_100": LPFConfig.LPF_ODR_DIV_100,
    "ODR_DIV_200": LPFConfig.LPF_ODR_DIV_200,
    "ODR_DIV_400": LPFConfig.LPF_ODR_DIV_400,
    "ODR_DIV_800": LPFConfig.LPF_ODR_DIV_800,
}

AmbTempAvg = sths34pf80_ns.enum("AmbTempAvg")
AMB_TEMP_AVGS = {
    "8": AmbTempAvg.AVG_T_8,
    "4": AmbTempAvg.AVG_T_4,
    "2": AmbTempAvg.AVG_T_2,
    "1": AmbTempAvg.AVG_T_1,
}

ObjTempAvg = sths34pf80_ns.enum("ObjTempAvg")
OBJ_TEMP_AVGS = {
    "2": ObjTempAvg.AVG_TMOS_2,
    "8": ObjTempAvg.AVG_TMOS_8,
    "32": ObjTempAvg.AVG_TMOS_32,
    "128": ObjTempAvg.AVG_TMOS_128,
    "256": ObjTempAvg.AVG_TMOS_256,
    "512": ObjTempAvg.AVG_TMOS_512,
    "1024": ObjTempAvg.AVG_TMOS_1024,
    "2048": ObjTempAvg.AVG_TMOS_2048,
}

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(STHS34PF80Component),
            cv.Optional(CONF_OUTPUT_DATA_RATE, default="1HZ"): cv.enum(
                OUTPUT_DATA_RATES, upper=True
            ),
            cv.Optional(CONF_MOTION_LPF, default="ODR_DIV_9"): cv.enum(
                LPF_CONFIGS, upper=True
            ),
            cv.Optional(CONF_MOTION_PRESENCE_LPF, default="ODR_DIV_20"): cv.enum(
                LPF_CONFIGS, upper=True
            ),
            cv.Optional(CONF_PRESENCE_LPF, default="ODR_DIV_50"): cv.enum(
                LPF_CONFIGS, upper=True
            ),
            cv.Optional(CONF_TEMPERATURE_LPF, default="ODR_DIV_100"): cv.enum(
                LPF_CONFIGS, upper=True
            ),
            cv.Optional(CONF_AMB_TEMP_AVG, default="8"): cv.enum(AMB_TEMP_AVGS),
            cv.Optional(CONF_OBJ_TEMP_AVG, default="32"): cv.enum(OBJ_TEMP_AVGS),
            cv.Optional(CONF_WIDE_GAIN_MODE, default=False): cv.boolean,
            cv.Optional(CONF_BLOCK_DATA_UPDATE, default=True): cv.boolean,
            cv.Optional(CONF_ENABLE_PRESENCE_INTERRUPT, default=False): cv.boolean,
            cv.Optional(CONF_ENABLE_MOTION_INTERRUPT, default=False): cv.boolean,
            cv.Optional(CONF_INT_ACTIVE_LOW, default=False): cv.boolean,
            cv.Optional(CONF_INT_OPEN_DRAIN, default=False): cv.boolean,
            cv.Optional(CONF_INT_LATCHED, default=False): cv.boolean,
            cv.Optional(CONF_FORCE_UPDATE, default=False): cv.boolean,
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x5A))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    # Configure settings
    cg.add(var.set_output_data_rate(config[CONF_OUTPUT_DATA_RATE]))
    cg.add(var.set_motion_lpf(config[CONF_MOTION_LPF]))
    cg.add(var.set_motion_presence_lpf(config[CONF_MOTION_PRESENCE_LPF]))
    cg.add(var.set_presence_lpf(config[CONF_PRESENCE_LPF]))
    cg.add(var.set_temperature_lpf(config[CONF_TEMPERATURE_LPF]))
    cg.add(var.set_amb_temp_avg(config[CONF_AMB_TEMP_AVG]))
    cg.add(var.set_obj_temp_avg(config[CONF_OBJ_TEMP_AVG]))
    cg.add(var.set_wide_gain_mode(config[CONF_WIDE_GAIN_MODE]))
    cg.add(var.set_block_data_update(config[CONF_BLOCK_DATA_UPDATE]))
    cg.add(var.set_enable_presence_interrupt(config[CONF_ENABLE_PRESENCE_INTERRUPT]))
    cg.add(var.set_enable_motion_interrupt(config[CONF_ENABLE_MOTION_INTERRUPT]))
    cg.add(var.set_int_active_low(config[CONF_INT_ACTIVE_LOW]))
    cg.add(var.set_int_open_drain(config[CONF_INT_OPEN_DRAIN]))
    cg.add(var.set_int_latched(config[CONF_INT_LATCHED]))
    cg.add(var.set_force_update(config[CONF_FORCE_UPDATE]))


