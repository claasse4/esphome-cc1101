import esphome.codegen as cg
import esphome.config_validation as cv
import esphome.pins as pins
from esphome.components import spi
from esphome.const import CONF_ID

cc1101_ns = cg.esphome_ns.namespace("cc1101_custom")
CC1101Custom = cc1101_ns.class_("CC1101Custom", cg.Component, spi.SPIDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(CC1101Custom),
    cv.Required("spi_id"): cv.use_id(spi.SPIComponent),
    cv.Required("cs_pin"): pins.gpio_pin_schema,
    cv.Required("gdo0_pin"): pins.gpio_pin_schema,
    cv.Required("frequency"): cv.string,
    cv.Required("modulation_type"): cv.string,
})

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield spi.register_spi_device(var, config["spi_id"])
    cg.add(var.set_cs_pin(config["cs_pin"]))
    cg.add(var.set_gdo0_pin(config["gdo0_pin"]))
    cg.add(var.set_frequency(config["frequency"]))
    cg.add(var.set_modulation_type(config["modulation_type"]))
