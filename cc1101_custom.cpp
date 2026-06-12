#include "cc1101_custom.h"
#include "esphome/core/log.h"

namespace esphome {
namespace cc1101_custom {

static const char *TAG = "cc1101_custom";

void CC1101CustomComponent::setup() {
  ESP_LOGI(TAG, "Initialising CC1101 (custom)...");

  cs_pin_->setup();
  cs_pin_->digital_write(true);

  gdo0_pin_->setup();
  gdo0_pin_->attach_interrupt(gdo0_isr, this, gpio::INTERRUPT_FALLING_EDGE);

  spi_bus_config_t buscfg = {};
  buscfg.miso_io_num = 19;
  buscfg.mosi_io_num = 23;
  buscfg.sclk_io_num = 18;

  spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);

  spi_device_interface_config_t devcfg = {};
  devcfg.clock_speed_hz = 1000000;
  devcfg.mode = 0;
  devcfg.spics_io_num = -1;
  devcfg.queue_size = 3;

  spi_bus_add_device(SPI2_HOST, &devcfg, &spi_);

  strobe(CC1101_SRES);
  delay(10);

  write_reg(CC1101_FREQ2, 0x10);
  write_reg(CC1101_FREQ1, 0xB1);
  write_reg(CC1101_FREQ0, 0x3B);

  write_reg(CC1101_MDMCFG4, 0xC6);
  write_reg(CC1101_MDMCFG3, 0x4A);
  write_reg(CC1101_MDMCFG2, 0x02);

  write_reg(CC1101_IOCFG0, 0x0D);

  strobe(CC1101_SRX);

  ESP_LOGI(TAG, "CC1101 custom driver ready.");
}

void CC1101CustomComponent::loop() {
  if (pulse_flag_) {
    pulse_flag_ = false;
    ESP_LOGD(TAG, "Pulse detected on GDO0");
  }
}

void IRAM_ATTR CC1101CustomComponent::gdo0_isr(void *arg) {
  auto *self = static_cast<CC1101CustomComponent *>(arg);
  self->pulse_flag_ = true;
}

void CC1101CustomComponent::write_reg(uint8_t reg, uint8_t value) {
  spi_transaction_t t = {};
  t.flags = SPI_TRANS_USE_TXDATA;
  t.length = 16;
  t.tx_data[0] = reg;
  t.tx_data[1] = value;

  cs_pin_->digital_write(false);
  spi_device_polling_transmit(spi_, &t);
  cs_pin_->digital_write(true);
}

uint8_t CC1101CustomComponent::read_reg(uint8_t reg) {
  spi_transaction_t t = {};
  t.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA;
  t.length = 16;
  t.tx_data[0] = reg | 0x80;

  cs_pin_->digital_write(false);
  spi_device_polling_transmit(spi_, &t);
  cs_pin_->digital_write(true);

  return t.rx_data[1];
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

}  // namespace cc1101_custom
}  // namespace esphome
