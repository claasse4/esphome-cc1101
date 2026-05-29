# CC1101 RF Bridge for ESPHome

This repository provides a custom ESPHome component for the CC1101 433MHz transceiver.

Features:
- Hardware SPI (VSPI)
- OOK mode
- 433.92 MHz default
- GDO0 pulse detection
- Home Assistant event publishing

Usage in ESPHome:

```yaml
external_components:
  - source: github://claasse4/esphome-cc1101

cc1101:
  cs_pin: 5
  gdo0_pin: 4
