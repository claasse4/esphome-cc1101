#pragma once
#include "esphome.h"
#include "registers.h"

namespace esphome {
namespace cc1101_custom {

class CC1101CustomComponent : public Component {
 public:
  void set_cs_pin(GPIOPin *pin) { cs_pin_ = pin; }
  void set_gdo0_pin(GPIOPin *pin) { gdo0_pin_ = pin; }

  void setup() override;
  void loop() override;

 protected:
  GPIOPin *cs_pin_{nullptr};
  GPIOPin *gdo0_pin_{nullptr};
  spi_device_handle_t spi_{nullptr};
  volatile bool pulse_flag_{false};

  static void IRAM_ATTR gdo0_isr(void *arg);

  void write_reg(uint8_t reg, uint8_t value);
  uint8_t read_reg(uint8_t reg);
  void strobe(uint8_t cmd);
};

}  // namespace cc1101_custom
}  // namespace esphome
