#include "cc1101custom.h"
#include "esphome/core/log.h"

namespace esphome {
namespace cc1101custom {

static const char *const TAG = "cc1101custom";

void CC1101Custom::setup() {
  ESP_LOGI(TAG, "CC1101 custom component initialized");
  ESP_LOGI(TAG, "CS pin: %d", this->cs_pin_);
  ESP_LOGI(TAG, "GDO2 pin: %d", this->gdo2_pin_);
}

}  // namespace cc1101custom
}  // namespace esphome
