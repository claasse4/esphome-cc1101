#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace cc1101custom {

class CC1101Custom : public Component {
 public:
  CC1101Custom(int cs_pin, int gdo2_pin)
      : cs_pin_(cs_pin), gdo2_pin_(gdo2_pin) {}

  void setup() override;
  void loop() override {}

 protected:
  int cs_pin_;
  int gdo2_pin_;
};

}  // namespace cc1101custom
}  // namespace esphome
