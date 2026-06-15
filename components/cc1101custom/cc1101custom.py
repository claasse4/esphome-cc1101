import esphome.codegen as cg
import esphome.config_validation as cv

cc1101_ns = cg.esphome_ns.namespace("cc1101custom")
CC1101Custom = cc1101_ns.class_("CC1101Custom", cg.Component)

CONF_CS_PIN = "cs_pin"
CONF_GDO2_PIN = "gdo2_pin"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(CC1101Custom),
        cv.Required(CONF_CS_PIN): cv.int_,
        cv.Required(CONF_GDO2_PIN): cv.int_,
    }
)

async def to_code(config):
    var = cg.new_Pvariable(
        config[cv.CONF_ID],
        config[CONF_CS_PIN],
        config[CONF_GDO2_PIN],
    )
    await cg.register_component(var, config)

# Register the component name so ESPHome knows this is a YAML component
cv.register_component("cc1101custom", CONFIG_SCHEMA)
