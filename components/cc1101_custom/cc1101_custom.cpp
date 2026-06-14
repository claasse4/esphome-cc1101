#include "cc1101_custom.h"

namespace esphome {
namespace cc1101_custom {

static const char *const TAG = "cc1101";

void CC1101Custom::setup() {
  ESP_LOGI(TAG, "Initializing CC1101 in OOK RX mode...");

  cs_pin_->setup();
  gdo0_pin_->setup();

  // --- SPI bus init ---
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

  // --- CC1101 register init for 433.92 MHz OOK ---
  write_reg(0x00, 0x0D);  // IOCFG2
  write_reg(0x02, 0x06);  // IOCFG0: async serial data on GDO0
  write_reg(0x0B, 0x06);  // FSCTRL1
  write_reg(0x0C, 0x00);  // FSCTRL0
  write_reg(0x0D, 0x10);  // FREQ2
  write_reg(0x0E, 0xA7);  // FREQ1
  write_reg(0x0F, 0x62);  // FREQ0  => 433.92 MHz

  write_reg(0x12, 0x00);  // MDMCFG2: ASK/OOK
  write_reg(0x13, 0x22);  // MDMCFG1
  write_reg(0x14, 0xF8);  // MDMCFG0

  write_reg(0x15, 0x34);  // DEVIATN
  write_reg(0x18, 0x18);  // MCSM0
  write_reg(0x19, 0x16);  // FOCCFG
  write_reg(0x1A, 0x6C);  // BSCFG
  write_reg(0x1B, 0x03);  // AGCCTRL2
  write_reg(0x1C, 0x40);  // AGCCTRL1
  write_reg(0x1D, 0x91);  // AGCCTRL0

  write_reg(0x21, 0x56);  // FREND1
  write_reg(0x22, 0x10);  // FREND0

  write_reg(0x23, 0xE9);  // FSCAL3
  write_reg(0x24, 0x2A);  // FSCAL2
  write_reg(0x25, 0x00);  // FSCAL1
  write_reg(0x26, 0x1F);  // FSCAL0

  strobe(0x34); // SRX (enter receive mode)

  ESP_LOGI(TAG, "CC1101 ready, listening for OOK pulses...");
}

void CC1101Custom::loop() {
  static uint32_t last_change = 0;
  static bool last_state = false;

  // buffer voor pulses
  static uint32_t pulses[200];
  static int pulse_count = 0;

  bool state = gdo0_pin_->digital_read();
  uint32_t now = micros();

  if (state != last_state) {
    uint32_t pulse = now - last_change;
    last_change = now;
    last_state = state;

    // filter: negeer alles > 20 ms (overflow / idle)
    if (pulse > 20000) {
      pulse_count = 0;
      return;
    }

    // opslaan
    if (pulse_count < 200) {
      pulses[pulse_count++] = pulse;
    }

    // debug (optioneel, kan je uitzetten als het teveel wordt)
    ESP_LOGD(TAG, "Pulse: %u us, level=%d", pulse, state);

    // sync detectie: APA3/NEXA heeft ~10 ms pauze
    if (pulse > 8000) {
      if (pulse_count > 40) {
        int bitpos = 0;
        uint32_t code = 0;

        for (int i = 0; i < pulse_count - 2; i += 2) {
          uint32_t p1 = pulses[i];
          uint32_t p2 = pulses[i + 1];

          bool short1 = p1 < 600;
          bool short2 = p2 < 600;

          if (short1 && !short2) {
            // 0 = kort + lang
            code = (code << 1) | 0;
            bitpos++;
          } else if (!short1 && short2) {
            // 1 = lang + kort
            code = (code << 1) | 1;
            bitpos++;
          } else {
            // floating / invalid -> negeren
          }
        }

        if (bitpos >= 32) {
          uint32_t address = (code >> 6) & 0x3FFFFFF;
          uint8_t unit = (code >> 2) & 0x0F;
          bool on = (code >> 1) & 0x01;

          ESP_LOGI(TAG,
                   "APA3/NEXA: addr=%u unit=%u state=%s (bits=%d)",
                   address, unit, on ? "ON" : "OFF", bitpos);
        }
      }

      // buffer reset na sync
      pulse_count = 0;
    }
  }
}

void CC1101Custom::dump_config() {
  ESP_LOGCONFIG(TAG, "CC1101 Custom Component (OOK RX, APA3/NEXA decode)");
}

void CC1101Custom::strobe(uint8_t cmd) {
  spi_transaction_t t = {};
  t.length = 8;
  t.tx_buffer = &cmd;
  spi_device_transmit(spi_, &t);
}

void CC1101Custom::write_reg(uint8_t reg, uint8_t value) {
  uint8_t buf[2] = {reg, value};
  spi_transaction_t t = {};
  t.length = 16;
  t.tx_buffer = buf;
  spi_device_transmit(spi_, &t);
}

uint8_t CC1101Custom::read_reg(uint8_t reg) {
  uint8_t tx[2] = {static_cast<uint8_t>(reg | 0x80), 0x00};
  uint8_t rx[2] = {};
  spi_transaction_t t = {};
  t.length = 16;
  t.tx_buffer = tx;
  t.rx_buffer = rx;
  spi_device_transmit(spi_, &t);
  return rx[1];
}

}  // namespace cc1101_custom
}  // namespace esphome
