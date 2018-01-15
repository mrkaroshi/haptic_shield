#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_DRV2605.h>

extern "C" {
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}

uint8_t readDRV(uint8_t mux_address, uint8_t drv_address, uint8_t register_add, uint8_t bytes);
uint8_t writeDRV(uint8_t mux_address, uint8_t drv_address, uint8_t register_add, uint8_t register_data);
