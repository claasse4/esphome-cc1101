#include "cc1101_custom.h"

namespace esphome {
namespace cc1101_custom {

static const char *const TAG = "cc1101_custom";

void CC1101Custom::setup() {
  ESP_LOGI(TAG, "Initializing CC1101...");

  cs_pin_->setup();
  gdo0_pin_->setup();

  spi_bus_config_t buscfg = {};
  buscfg.mosi_io_num = 23;
  buscfg.miso_io_num = 19;
  buscfg.sclk_io_num = 18;
  buscfg.quadwp_io_num = -1;
  buscfg.quadhd_io_num = -1;

  ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

  spi_device_interface_config_t devcfg = {};
  devcfg.clock_speed_hz = 1 * 1000 * 1000;
  devcfg.mode = 0;
  devcfg.spics_io_num = -1;
  devcfg.queue_size = 1;

  ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &spi_));

  strobe(0x30);  // CC1101_SRES
}

void CC1101Custom::dump_config() {
  ESP_LOGCONFIG(TAG, "CC1101 Custom Component");
}

void CC1101Custom::loop() {
}

void CC1101Custom::strobe(uint8_t cmd) {
}

void CC1101Custom::write_reg(uint8_t reg, uint8_t value) {
}

uint8_t CC1101Custom::read_reg(uint8_t reg) {
  return 0;
}

}  // namespace cc1101_custom
}  // namespace esphome
