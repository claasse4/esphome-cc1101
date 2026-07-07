#include "cc1101_custom.h"
#include "esphome/core/log.h"

namespace esphome {
namespace cc1101_custom {

static const char *const TAG = "cc1101_custom";

void CC1101Custom::setup() {
  ESP_LOGI(TAG, "CC1101 setup starting");

  // Zet GDO0 als input
  if (this->gdo0_pin_) {
    this->gdo0_pin_->setup();
    this->gdo0_pin_->pin_mode(gpio::FLAG_INPUT);
  }

  // Init SPI device
  this->spi_setup();
}

void CC1101Custom::loop() {
  // Eventueel RX polling hier
}

void CC1101Custom::begin_rx() {
  ESP_LOGI(TAG, "Switching CC1101 into RX mode");

  // GDO0 naar input
  if (this->gdo0_pin_) {
    this->gdo0_pin_->pin_mode(gpio::FLAG_INPUT);
  }

  // Hier komt jouw echte CC1101 RX‑init code
  // Bijvoorbeeld:
  // this->write_register(CC1101_IOCFG0, 0x06);
  // this->strobe(CC1101_SRX);

  ESP_LOGI(TAG, "RX mode enabled");
}

}  // namespace cc1101_custom
}  // namespace esphome
