#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"

extern "C" {
  #include "driver/spi_master.h"
}

#include "registers.h"

namespace esphome {
namespace cc1101_custom {

class CC1101CustomComponent : public Component {
 public:
  void set_cs_pin(GPIOPin *pin) { cs_pin_ = pin; }
  void set_gdo0_pin(GPIOPin *pin) { gdo0_pin_ = pin; }

  void setup() override;
  void dump_config() override;

 protected:
  void strobe(uint8_t cmd);
  void write_reg(uint8_t reg, uint8_t value);
  uint8_t read_reg(uint8_t reg);

  GPIOPin *cs_pin_{nullptr};
  GPIOPin *gdo0_pin_{nullptr};

  spi_device_handle_t spi_{nullptr};
};

static volatile bool gdo0_flag;

}  // namespace cc1101_custom
}  // namespace esphome
