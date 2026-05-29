#pragma once
#include "esphome/core/component.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace cc1101 {

class CC1101Component : public Component {
 public:
  void set_cs_pin(GPIOPin *pin) { cs_pin_ = pin; }
  void set_gdo0_pin(GPIOPin *pin) { gdo0_pin_ = pin; }

  void setup() override;
  void loop() override;

 protected:
  GPIOPin *cs_pin_{nullptr};
  GPIOPin *gdo0_pin_{nullptr};
};

}  // namespace cc1101
}  // namespace esphome
