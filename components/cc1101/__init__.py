import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins

cc1101_ns = cg.esphome_ns.namespace("cc1101")
CC1101Component = cc1101_ns.class_("CC1101Component", cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(CC1101Component),
    cv.Required("cs_pin"): pins.gpio_output_pin_schema,
    cv.Required("gdo0_pin"): pins.gpio_input_pin_schema,
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[cv.GenerateID()])
    await cg.register_component(var, config)

    cs = await cg.gpio_pin_expression(config["cs_pin"])
    cg.add(var.set_cs_pin(cs))

    gdo0 = await cg.gpio_pin_expression(config["gdo0_pin"])
    cg.add(var.set_gdo0_pin(gdo0))
