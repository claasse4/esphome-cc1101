#include "cc1101_custom.h"

namespace esphome {
namespace cc1101_custom {

void CC1101CustomComponent::setup() {
  ESP_LOGI("cc1101", "Initializing CC1101...");

  // Configure CS pin
  cs_pin_->setup();
  cs_pin_->digital_write(true);

  // Configure GDO0 pin (no interrupt yet)
  gdo0_pin_->setup();

  // SPI bus config
  spi_bus_config_t buscfg = {};
  buscfg.mosi_io_num = 23;
  buscfg.miso_io_num = 19;
  buscfg.sclk_io_num = 18;
  buscfg.quadwp_io_num = -1;
  buscfg.quadhd_io_num = -1;

  ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

  // Device config
  spi_device_interface_config_t devcfg = {};
  devcfg.clock_speed_hz = 1 * 1000 * 1000;  // 1 MHz
  devcfg.mode = 0;
  devcfg.spics_io_num = -1;  // we toggle CS manually
  devcfg.queue_size = 1;

  ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &spi_));

  // Reset CC1101
  strobe(CC1101_SRES);

  ESP_LOGI("cc1101", "CC1101 init done");
}

void CC1101CustomComponent::dump_config() {
  ESP_LOGCONFIG("cc1101", "CC1101 Custom Component");
}

void CC1101CustomComponent::strobe(uint8_t cmd) {
  spi_transaction_t t = {};
  t.flags = SPI_TRANS_USE_TXDATA;
  t.length = 8;
  t.tx_data[0] = cmd;

  cs_pin_->digital_write(false);
  spi_device_polling_transmit(spi_, &t);
  cs_pin_->digital_write(true);
}

void CC1101CustomComponent::write_reg(uint8_t reg, uint8_t value) {
  spi_transaction_t t = {};
  t.flags = SPI_TRANS_USE_TXDATA;
  t.length = 16;
  t.tx_data[0] = reg | CC1101_WRITE_SINGLE;
  t.tx_data[1] = value;

  cs_pin_->digital_write(false);
  spi_device_polling_transmit(spi_, &t);
  cs_pin_->digital_write(true);
}

uint8_t CC1101CustomComponent::read_reg(uint8_t reg) {
  spi_transaction_t t = {};
  t.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA;
  t.length = 16;
  t.tx_data[0] = reg | CC1101_READ_SINGLE;

  cs_pin_->digital_write(false);
  spi_device_polling_transmit(spi_, &t);
  cs_pin_->digital_write(true);

  return t.rx_data[1];
}

volatile bool CC1101CustomComponent::gdo0_flag = false;

}  // namespace cc1101_custom
}  // namespace esphome
