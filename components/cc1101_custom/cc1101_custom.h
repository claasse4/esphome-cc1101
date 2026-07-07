#pragma once

#include "esphome/core/component.h"
#include "esphome/components/spi/spi.h"

namespace esphome {
namespace cc1101_custom {

class CC1101Custom : public Component, public spi::SPIDevice {
 public:
  void setup() override;
  void loop() override;

  void set_cs_pin(GPIOPin *pin) { cs_pin_ = pin; }
  void set_gdo0_pin(GPIOPin *pin) { gdo0_pin_ = pin; }
  void set_frequency(const std::string &freq) { frequency_ = freq; }
  void set_modulation_type(const std::string &mod) { modulation_type_ = mod; }

  void begin_rx();   // ACTION

 protected:
  GPIOPin *cs_pin_{nullptr};
  GPIOPin *gdo0_pin_{nullptr};
  std::string frequency_;
  std::string modulation_type_;
};

}  // namespace cc1101_custom
}  // namespace esphome
