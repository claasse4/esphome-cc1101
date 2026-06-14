#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"
#include "esphome/core/gpio.h"
#include <cstdint>
#include <driver/spi_master.h>

namespace esphome {
namespace cc1101_custom {

class CC1101Custom : public Component {
 public:
  void set_cs_pin(GPIOPin *pin) { cs_pin_ = pin; }
  void set_gdo0_pin(GPIOPin *pin) { gdo0_pin_ = pin; }

  void setup() override;
  void dump_config() override;
  void loop() override;

 protected:
  void strobe(uint8_t cmd);
  void write_reg(uint8_t reg, uint8_t value);
  uint8_t read_reg(uint8_t reg);

  GPIOPin *cs_pin_{nullptr};
  GPIOPin *gdo0_pin_{nullptr};
  spi_device_handle_t spi_{nullptr};
};

}  // namespace cc1101_custom
}  // namespace esphome
