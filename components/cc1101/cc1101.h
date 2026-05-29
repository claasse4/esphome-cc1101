#pragma once
#include "esphome/core/component.h"

namespace esphome {
namespace cc1101 {

class CC1101Component : public Component {
 public:
  void setup() override;
  void loop() override;
};

}  // namespace cc1101
}  // namespace esphome
