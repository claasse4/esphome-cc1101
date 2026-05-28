#include "cc1101.h"

namespace esphome {
namespace cc1101 {

static const uint8_t CC1101_SRES = 0x30;
static const uint8_t CC1101_SRX  = 0x34;

void CC1101Component::setup() {
  ESP_LOGI("cc1101", "Starting CC1101 component...");

  this->spi_ = new SPIClass(VSPI);
  this->spi_->begin(18, 19, 23);  // SCK, MISO, MOSI

  this->cs_pin_->pin_mode(gpio::FLAG_OUTPUT);
  this->cs_pin_->digital_write(true);

  this->init_radio();
}

void CC1101Component::init_radio() {
  ESP_LOGI("cc1101", "Resetting CC1101...");

  this->cs_pin_->digital_write(false);
  delay(1);
  this->cs_pin_->digital_write(true);
  delay(1);

  this->cs_pin_->digital_write(false);
  this->spi_->transfer(CC1101_SRES);
  this->cs_pin_->digital_write(true);
  delay(5);

  auto write = [&](uint8_t reg, uint8_t val) {
    this->write_reg(reg, val);
  };

  // Frequency 433.92 MHz
  write(0x0B, 0x06);
  write(0x0C, 0x00);
  write(0x0D, 0x10);
  write(0x0E, 0xA7);
  write(0x0F, 0x62);

  // OOK mode
  write(0x12, 0xF8);
  write(0x13, 0x93);
  write(0x14, 0x03);
  write(0x15, 0x22);
  write(0x16, 0xF8);

  // Enter RX
  this->cs_pin_->digital_write(false);
  this->spi_->transfer(CC1101_SRX);
  this->cs_pin_->digital_write(true);

  this->ready_ = true;
  ESP_LOGI("cc1101", "CC1101 READY (RX MODE)");
}

void CC1101Component::write_reg(uint8_t addr, uint8_t value) {
  this->cs_pin_->digital_write(false);
  this->spi_->transfer(addr);
  this->spi_->transfer(value);
  this->cs_pin_->digital_write(true);
}

void CC1101Component::strobe(uint8_t cmd) {
  this->cs_pin_->digital_write(false);
  this->spi_->transfer(cmd);
  this->cs_pin_->digital_write(true);
}

void CC1101Component::loop() {
  if (!this->ready_)
    return;

  if (this->gdo0_pin_->digital_read()) {
    ESP_LOGD("cc1101", "RF pulse detected");
    HomeassistantServiceResponse resp;
    resp.event = "esphome.rfbridge_pulse";
    resp.data["timestamp"] = millis();
    this->publish_homeassistant_event(resp);
  }
}

}  // namespace cc1101
}  // namespace esphome
