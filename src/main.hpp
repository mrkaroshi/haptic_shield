#include <Arduino.h>

uint8_t readDRV(uint8_t mux_address, uint8_t drv_address, uint8_t register_add, uint8_t bytes);
uint8_t writeDRV(uint8_t mux_address, uint8_t drv_address, uint8_t register_add, uint8_t register_data);
void initDRV(uint8_t mux_address, uint8_t drv_address);
void selectLib(uint8_t mux_address, uint8_t drv_address, uint8_t lib);
void setMode(uint8_t mux_address, uint8_t drv_address, uint8_t mode);
void setWaveform(uint8_t mux_address, uint8_t drv_address, uint8_t slot, uint8_t w);
