#pragma once
#include "esphome.h"
#include "driver/gpio.h"

class CC1101Custom : public Component {
 public:
  GPIOPin *cs_pin_;
  GPIOPin *gdo2_pin_;

  void set_cs_pin(GPIOPin *pin) { cs_pin_ = pin; }
  void set_gdo2_pin(GPIOPin *pin) { gdo2_pin_ = pin; }

  void setup() override;
  void loop() override;
  void dump_config() override;

 protected:
  volatile uint32_t last_edge_us_ = 0;
  static void IRAM_ATTR gdo2_isr_handler(void *arg);
};
