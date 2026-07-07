import esphome.codegen as cg
import esphome.automation as automation
import esphome.config_validation as cv
from esphome.const import CONF_ID

cc1101_ns = cg.esphome_ns.namespace("cc1101_custom")
CC1101Custom = cc1101_ns.class_("CC1101Custom", cg.Component)
BeginRxAction = cc1101_ns.class_("BeginRxAction", automation.Action)

# -----------------------------
# CONFIG SCHEMA
# -----------------------------
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(CC1101Custom),

    cv.Required("spi_id"): cv.use_id(cg.SPIComponent),

    cv.Required("cs_pin"): cv.pin,

    cv.Required("gdo0_pin"): cv.Schema({
        cv.Required("number"): cv.pin,
        cv.Optional("allow_other_uses", default=False): cv.boolean,
    }),

    cv.Required("frequency"): cv.string,
    cv.Required("modulation_type"): cv.string,
})

# -----------------------------
# to_code() — MUST be sync
# -----------------------------
def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    cg.register_component(var, config)

    spi = cg.get_variable(config["spi_id"])
    cg.add(var.set_spi(spi))

    cg.add(var.set_cs_pin(config["cs_pin"]))

    gdo0 = config["gdo0_pin"]
    cg.add(var.set_gdo0_pin(gdo0["number"]))

    cg.add(var.set_frequency(config["frequency"]))
    cg.add(var.set_modulation(config["modulation_type"]))

# -----------------------------
# ACTION: begin_rx — MUST be sync
# -----------------------------
@automation.register_action(
    "cc1101_custom.begin_rx",
    BeginRxAction,
    cv.Schema({
        cv.GenerateID(): cv.use_id(CC1101Custom),
    })
)
def begin_rx_to_code(config, action_id):
    var = cg.new_Pvariable(action_id)
    cc = cg.get_variable(config[CONF_ID])
    cg.add(var.set_parent(cc))
    return var
