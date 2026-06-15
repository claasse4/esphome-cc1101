#pragma once
#include "esphome/core/component.h"

namespace esphome {
namespace cc1101custom {

class CC1101Custom : public Component {
 public:
  int cs_pin;
  int gdo2_pin;

  void setup() override;
  void loop() override {}
};

}  // namespace cc1101custom
}  // namespace esphome
