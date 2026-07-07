import esphome.codegen as cg
import esphome.automation as automation
import esphome.config_validation as cv
from esphome.const import CONF_ID

cc1101_ns = cg.esphome_ns.namespace("cc1101_custom")
CC1101Custom = cc1101_ns.class_("CC1101Custom", cg.Component)
BeginRxAction = cc1101_ns.class_("BeginRxAction", automation.Action)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(CC1101Custom),
})

@automation.register_action(
    "cc1101_custom.begin_rx",
    BeginRxAction,
    cv.Schema({
        cv.GenerateID(): cv.use_id(CC1101Custom),
    })
)
async def begin_rx_to_code(config, action_id):
    var = cg.new_Pvariable(action_id)
    cc = cg.get_variable(config[CONF_ID])
    cg.add(var.set_parent(cc))
    return var
