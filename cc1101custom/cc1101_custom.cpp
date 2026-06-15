#include "cc1101_custom.h"

static const char *TAG = "cc1101_custom";

void CC1101Custom::setup() {
  ESP_LOGI(TAG, "Initializing CC1101 custom OOK receiver...");

  // Configure GDO2 pin
  gdo2_pin_->setup();
  gdo2_pin_->pin_mode(gpio::FLAG_INPUT);

  // Attach interrupt
  gpio_set_intr_type((gpio_num_t) gdo2_pin_->get_pin(), GPIO_INTR_ANYEDGE);
  gpio_install_isr_service(0);
  gpio_isr_handler_add((gpio_num_t) gdo2_pin_->get_pin(), gdo2_isr_handler, this);

  ESP_LOGI(TAG, "GDO2 interrupt attached on GPIO%d", gdo2_pin_->get_pin());

  // Set IOCFG2 = 0x0D (Carrier Detect)
  // Register address 0x00 + IOCFG2 offset
  uint8_t addr = 0x00;
  uint8_t value = 0x0D;

  cs_pin_->setup();
  cs_pin_->pin_mode(gpio::FLAG_OUTPUT);

  cs_pin_->digital_write(false);
  delayMicroseconds(5);
  SPI.transfer(addr);
  SPI.transfer(value);
  cs_pin_->digital_write(true);

  ESP_LOGI(TAG, "IOCFG2 set to 0x0D (Carrier Detect)");
}

void IRAM_ATTR CC1101Custom::gdo2_isr_handler(void *arg) {
  CC1101Custom *self = (CC1101Custom *) arg;
  uint32_t now = micros();
  uint32_t delta = now - self->last_edge_us_;
  self->last_edge_us_ = now;

  // Log pulse width
  ESP_LOGD("cc1101_pulse", "Pulse: %u us", delta);
}

void CC1101Custom::loop() {
  // nothing needed here
}

void CC1101Custom::dump_config() {
  ESP_LOGCONFIG(TAG, "CC1101 Custom OOK Receiver:");
  ESP_LOGCONFIG(TAG, "  CS Pin: GPIO%d", cs_pin_->get_pin());
  ESP_LOGCONFIG(TAG, "  GDO2 Pin: GPIO%d", gdo2_pin_->get_pin());
}
