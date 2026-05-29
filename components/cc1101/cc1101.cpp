#include "cc1101.h"
#include "esphome/core/log.h"

namespace esphome {
namespace cc1101 {

static const char *const TAG = "cc1101";

void CC1101Component::setup() {
  ESP_LOGI(TAG, "CC1101 setup()");
  if (cs_pin_) cs_pin_->setup();
  if (gdo0_pin_) gdo0_pin_->setup();
}

void CC1101Component::loop() {
  // later vullen
}

}  // namespace cc1101
}  // namespace esphome
