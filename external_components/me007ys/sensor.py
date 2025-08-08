import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor, text_sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_DISTANCE,
    STATE_CLASS_MEASUREMENT,
    ICON_ARROW_EXPAND_VERTICAL,
    UNIT_CENTIMETER,
)

# New cm-based options
CONF_MIN_VALID_CM = "min_valid_cm"
CONF_MAX_VALID_CM = "max_valid_cm"

# Deprecated mm aliases (we'll silently convert for back-compat)
CONF_MIN_VALID_MM = "min_valid_mm"
CONF_MAX_VALID_MM = "max_valid_mm"

CONF_TOO_CLOSE_BEHAVIOR = "too_close_behavior"
CONF_DEBUG_RAW = "debug_raw"
CONF_FRAME_RATE_HZ = "frame_rate_hz"
CONF_STATUS = "status"

me007ys_ns = cg.esphome_ns.namespace("me007ys")
ME007YSSensor = me007ys_ns.class_(
    "ME007YSSensor", sensor.Sensor, cg.PollingComponent, uart.UARTDevice
)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        unit_of_measurement=UNIT_CENTIMETER,
        icon=ICON_ARROW_EXPAND_VERTICAL,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_DISTANCE,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(
        {
            cv.GenerateID(): cv.declare_id(ME007YSSensor),

            # New cm-based config (DFRobot spec: 28–450 cm)
            cv.Optional(CONF_MIN_VALID_CM, default=28.0): cv.float_range(min=0.0, max=10000.0),
            cv.Optional(CONF_MAX_VALID_CM, default=450.0): cv.float_range(min=0.0, max=10000.0),

            # Back-compat: allow mm keys and convert to cm if used
            cv.Optional(CONF_MIN_VALID_MM): cv.int_range(min=0, max=65535),
            cv.Optional(CONF_MAX_VALID_MM): cv.int_range(min=0, max=65535),

            cv.Optional(CONF_TOO_CLOSE_BEHAVIOR, default="nan"): cv.one_of(
                "nan", "min", "last", lower=True
            ),
            cv.Optional(CONF_DEBUG_RAW, default=False): cv.boolean,

            # Optional diagnostics
            cv.Optional(CONF_FRAME_RATE_HZ): sensor.sensor_schema(
                unit_of_measurement="Hz",
                accuracy_decimals=2,
                icon="mdi:sine-wave",
            ),
            cv.Optional(CONF_STATUS): text_sensor.text_sensor_schema(
                icon="mdi:information-outline"
            ),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.polling_component_schema("500ms"))
)

async def to_code(config):
    # Resolve cm values, honoring deprecated mm keys if present
    min_cm = config.get(CONF_MIN_VALID_CM, 28.0)
    max_cm = config.get(CONF_MAX_VALID_CM, 450.0)

    if CONF_MIN_VALID_MM in config and CONF_MIN_VALID_CM not in config:
        min_cm = float(config[CONF_MIN_VALID_MM]) / 10.0
    if CONF_MAX_VALID_MM in config and CONF_MAX_VALID_CM not in config:
        max_cm = float(config[CONF_MAX_VALID_MM]) / 10.0

    var = cg.new_Pvariable(config[CONF_ID])
    await uart.register_uart_device(var, config)
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)

    cg.add(var.set_min_valid_cm(min_cm))
    cg.add(var.set_max_valid_cm(max_cm))
    cg.add(var.set_too_close_behavior(config[CONF_TOO_CLOSE_BEHAVIOR]))
    cg.add(var.set_debug_raw(config[CONF_DEBUG_RAW]))

    if CONF_FRAME_RATE_HZ in config:
        fr = await sensor.new_sensor(config[CONF_FRAME_RATE_HZ])
        cg.add(var.set_frame_rate_sensor(fr))

    if CONF_STATUS in config:
        st = await text_sensor.new_text_sensor(config[CONF_STATUS])
        cg.add(var.set_status_sensor(st))
