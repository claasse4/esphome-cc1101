#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/core/automation.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace cc1101_custom {

class CC1101Custom : public Component {
 public:
  // ESPHome lifecycle
  void setup() override;
  void loop() override;

  // RX start action
  void begin_rx();

  // Low-level helpers (you already have these in your .cpp)
  void write_register(uint8_t reg, uint8_t value);
  uint8_t read_register(uint8_t reg);
  void strobe_command(uint8_t command);

  // Optional: add your GDO0 interrupt handler here
  // void handle_gdo0_interrupt();
};

// Action class for cc1101_custom.begin_rx
class BeginRxAction : public Action {
 public:
  void set_parent(CC1101Custom *parent) { parent_ = parent; }

  void play() override {
    if (parent_ != nullptr) {
      parent_->begin_rx();
    } else {
      ESP_LOGE("cc1101_custom", "BeginRxAction: parent is null");
    }
  }

 protected:
  CC1101Custom *parent_{nullptr};
};

}  // namespace cc1101_custom
}  // namespace esphome
