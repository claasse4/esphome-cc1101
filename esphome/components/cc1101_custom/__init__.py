print("### CC1101_CUSTOM PYTHON MODULE LOADED ###")

import esphome.codegen as cg
import esphome.config_validation as cv
import esphome.automation as automation
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

    cv.Required("spi_id"): cv.use_id(cg.global_ns.class_("SPIComponent")),
    cv.Required("cs_pin"): cv.Any(),
    cv.Required("gdo0_pin"): cv.Any(),

    cv.Optional("frequency", default="433.92MHz"): cv.string,
    cv.Optional("modulation_type", default="ASK/OOK"): cv.string,
})

# ---------------------------
# to_code()
# ---------------------------

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

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
