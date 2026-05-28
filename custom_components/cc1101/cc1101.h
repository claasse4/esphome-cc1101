#pragma once
#include "esphome.h"
#include <SPI.h>

namespace esphome {
namespace cc1101 {

class CC1101Component : public Component {
 public:
  CC1101Component(GPIOPin *cs_pin, GPIOPin *gdo0_pin)
      : cs_pin_(cs_pin), gdo0_pin_(gdo0_pin) {}

  void setup() override;
  void loop() override;

 protected:
  void write_reg(uint8_t addr, uint8_t value);
  void strobe(uint8_t cmd);
  void init_radio();

  GPIOPin *cs_pin_;
  GPIOPin *gdo0_pin_;
  SPIClass *spi_ = nullptr;
  bool ready_ = false;
};

}  // namespace cc1101
}  // namespace esphome
