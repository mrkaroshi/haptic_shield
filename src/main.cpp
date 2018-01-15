#include "main.hpp"

int LEAVE_STANDBY = 6;
int PWM_MODE = 1;

int clear = 0;
int set = 1;
int success = 0;

int MOT1_EN = 24;
int MOT2_EN = 28;
int MOT3_EN = 32;
int MOT4_EN = 36;
int MOT5_EN = 40;
int MOT6_EN = 44;
int MOT7_EN = 46;
int MOT8_EN = 48;

int MOT1_PWM = 2;


int mux_address_mot1 = 0x70;
int drv_address_mot1 = 0x5A; // 0xB4 8-bit for writing

Adafruit_DRV2605 drv1;

void tcaselect(uint8_t i) {
  if (i > 7) return;

  Wire.beginTransmission(mux_address_mot1);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void disabledrv(){
  pinMode(MOT1_EN, OUTPUT);
  pinMode(MOT2_EN, OUTPUT);
  pinMode(MOT3_EN, OUTPUT);
  pinMode(MOT4_EN, OUTPUT);
  pinMode(MOT5_EN, OUTPUT);
  pinMode(MOT6_EN, OUTPUT);
  pinMode(MOT7_EN, OUTPUT);
  pinMode(MOT8_EN, OUTPUT);
  digitalWrite(MOT1_EN, LOW);
  digitalWrite(MOT2_EN, LOW);
  digitalWrite(MOT3_EN, LOW);
  digitalWrite(MOT4_EN, LOW);
  digitalWrite(MOT5_EN, LOW);
  digitalWrite(MOT6_EN, LOW);
  digitalWrite(MOT7_EN, LOW);
  digitalWrite(MOT8_EN, LOW);
}

uint8_t readDRV(uint8_t mux_address, uint8_t drv_address, uint8_t register_add, uint8_t bytes){

  Wire.beginTransmission(mux_address); // transmits a 0x70
  Wire.write(1 << 0); // transmits a 0x01
  Wire.endTransmission();

  Wire.beginTransmission(drv_address); // transmits a 0x5A
  Wire.write((byte)register_add); // transmits a 0x00
  Wire.endTransmission();

  Wire.requestFrom(drv_address, bytes);

  uint8_t read_byte;

  if (bytes <= Wire.available()){
    read_byte = Wire.read(); // read the current config
  }

  return read_byte;

}

uint8_t writeDRV(uint8_t mux_address, uint8_t drv_address, uint8_t register_add, uint8_t register_data){

  Wire.beginTransmission(mux_address);
  Wire.write(1 << 0);
  Wire.endTransmission();

  Wire.beginTransmission(drv_address);
  Wire.write((byte)register_add);
  Wire.write((byte)register_data);
  Wire.endTransmission();

  return 1;

}

void setup() {

    Serial.begin(9600);
    // setup the enable pins
    pinMode(OUTPUT, MOT1_EN); // MOT1 enable
    // enable selected driver
    digitalWrite(MOT1_EN, 1);

    Wire.begin();

}

void loop() {

  uint8_t config = 0x00;

  while(config != 0xE0){
    config = readDRV(mux_address_mot1, drv_address_mot1, DRV2605_REG_STATUS, 1);
  }


    while(1){

      delay(1000);

      Serial.println(config, HEX);

      if(success == 1){
        Serial.println("Success!");
      }

    }
}
