import esphome.codegen as cg
import esphome.config_validation as cv

cc1101_ns = cg.esphome_ns.namespace("cc1101custom")
CC1101Custom = cc1101_ns.class_("CC1101Custom", cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(CC1101Custom),
    cv.Required("cs_pin"): cv.int_,
    cv.Required("gdo2_pin"): cv.int_,
})

async def to_code(config):
    var = cg.new_Pvariable(config["id"], config["cs_pin"], config["gdo2_pin"])
    await cg.register_component(var, config)
