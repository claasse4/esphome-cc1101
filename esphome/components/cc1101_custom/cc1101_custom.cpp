#include "cc1101_custom.h"
#include "esphome/core/log.h"

namespace esphome {
namespace cc1101_custom {

static const char *const TAG = "cc1101_custom";

// CC1101 register addresses (add more if needed)
static const uint8_t CC1101_IOCFG2 = 0x00;
static const uint8_t CC1101_IOCFG0 = 0x02;
static const uint8_t CC1101_SRX    = 0x34;

// ------------------------------------------------------
// Setup
// ------------------------------------------------------
void CC1101Custom::setup() {
  ESP_LOGI(TAG, "CC1101Custom setup() start");

  // Example: configure GDO pins (optional)
  this->write_register(CC1101_IOCFG2, 0x06);  // GDO2 asserts on sync word
  this->write_register(CC1101_IOCFG0, 0x06);  // GDO0 asserts on packet

  ESP_LOGI(TAG, "CC1101Custom setup() complete");
}

// ------------------------------------------------------
// Loop
// ------------------------------------------------------
void CC1101Custom::loop() {
  // Optional: handle RX FIFO, GDO interrupts, etc.
  // For now, nothing required.
}

// ------------------------------------------------------
// Begin RX mode (action)
// ------------------------------------------------------
void CC1101Custom::begin_rx() {
  ESP_LOGI(TAG, "Starting RX mode");

  // Configure GDO pins for RX
  this->write_register(CC1101_IOCFG2, 0x06);
  this->write_register(CC1101_IOCFG0, 0x06);

  // Enter RX mode
  this->strobe_command(CC1101_SRX);

  ESP_LOGI(TAG, "RX mode active");
}

// ------------------------------------------------------
// Low-level helpers
// ------------------------------------------------------
void CC1101Custom::write_register(uint8_t reg, uint8_t value) {
  // TODO: replace with your actual SPI write implementation
  ESP_LOGD(TAG, "write_register: reg=0x%02X value=0x%02X", reg, value);
}

uint8_t CC1101Custom::read_register(uint8_t reg) {
  // TODO: replace with your actual SPI read implementation
  ESP_LOGD(TAG, "read_register: reg=0x%02X", reg);
  return 0x00;
}

void CC1101Custom::strobe_command(uint8_t command) {
  // TODO: replace with your actual SPI strobe implementation
  ESP_LOGD(TAG, "strobe_command: cmd=0x%02X", command);
}

}  // namespace cc1101_custom
}  // namespace esphome
