print("### CC1101_CUSTOM PYTHON MODULE LOADED ###")

import esphome.codegen as cg
import esphome.config_validation as cv
import esphome.automation as automation
import esphome.pins as pins
from esphome.const import CONF_ID

# Namespace + classes
cc1101_ns = cg.esphome_ns.namespace("cc1101_custom")
CC1101Custom = cc1101_ns.class_("CC1101Custom", cg.Component)
BeginRxAction = cc1101_ns.class_("BeginRxAction", automation.Action)

# ---------------------------
# CONFIG SCHEMA
# ---------------------------

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(CC1101Custom),

    cv.Required("spi_id"): cv.use_id(cg.spi_bus),
    cv.Required("cs_pin"): pins.gpio_output_pin_schema,
    cv.Required("gdo0_pin"): pins.gpio_input_pin_schema,

    cv.Required("frequency"): cv.int_range(min=100000, max=1000000000),
    cv.Required("modulation_type"): cv.one_of("ASK", "OOK"),
}).extend(cv.COMPONENT_SCHEMA)

# ---------------------------
# to_code()
# ---------------------------

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    cg.register_component(var, config)

    cg.add(var.set_spi(config["spi_id"]))
    cg.add(var.set_cs_pin(config["cs_pin"]))
    cg.add(var.set_gdo0_pin(config["gdo0_pin"]))
    cg.add(var.set_frequency(config["frequency"]))
    cg.add(var.set_modulation_type(config["modulation_type"]))

# ---------------------------
# ACTION REGISTRATION
# ---------------------------

@automation.register_action(
    "cc1101_custom.begin_rx",
    BeginRxAction,
    cv.Schema({
        cv.GenerateID(): cv.use_id(CC1101Custom),
    }),
)
def begin_rx_to_code(config, action_id, template_arg):
    var = cg.new_Pvariable(action_id, template_arg)
    cc1101 = cg.get_variable(config[CONF_ID])
    cg.add(var.set_parent(cc1101))
    return var
