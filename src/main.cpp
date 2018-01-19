#include <Wire.h>
#include "Adafruit_DRV2605.h"

Adafruit_DRV2605 drv;

#include "Wire.h"
extern "C" {
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}

#define TCAADDR 0x70

int MOT1_EN = 24;
int MOT2_EN = 28;
int MOT3_EN = 32;
int MOT4_EN = 36;
int MOT5_EN = 40;
int MOT6_EN = 44;
int MOT7_EN = 46;
int MOT8_EN = 48;

void tcaselect(uint8_t i) {
  if (i > 7) return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission(true);
}

void tcaclear() {

  Wire.beginTransmission(TCAADDR);
  Wire.write(0x00);
  Wire.endTransmission(true);

}

void tcaenableall() {

  uint8_t transmission = 0xFF; // 4th motor needs replacing
  Wire.beginTransmission(TCAADDR);
  Wire.write(transmission);
  Wire.endTransmission(true);
}

void enableDRV(uint8_t bit_pattern){

  digitalWrite(MOT1_EN, (bit_pattern & 00000001));
  digitalWrite(MOT2_EN, (bit_pattern & 00000010) >> 1);
  digitalWrite(MOT3_EN, (bit_pattern & 00000100) >> 2); // driver 3
  digitalWrite(MOT4_EN, (bit_pattern & 00001000) >> 3); // driver 4
  digitalWrite(MOT5_EN, (bit_pattern & 00010000) >> 4);
  digitalWrite(MOT6_EN, (bit_pattern & 00100000) >> 5);
  digitalWrite(MOT7_EN, (bit_pattern & 01000000) >> 6);
  digitalWrite(MOT8_EN, (bit_pattern & 10000000) >> 7);

}

// MENU

// RTP
void enableRTP(){

  // read in current register value of MODE 0x01

  // flip the mode bits in MODE[2:0] to 5

  // overwrite the register

}

// PWM
void enablePWM(){

  // read in current register value of MODE 0x01 and N_PWM_ANALOG 0x1D

  // flip MODE[2:0] to 3 and N_PWM_ANALOG[1] to 1

  // overwrite the registers

}

// calibrate the closed loop operation
void runCalibration(){

  // read in the current register value of MODE 0x01

  // flip the mode bits in the MODE[2:0] to 7

  // set the required parameters

  // send a GO bit

}

void setClosedloopERM(){



}

void runDiagnostics(){

  // read in the current register value of MODE 0x01

  // flip the mode bits in the MODE[2:0] to 6

  // set the required parameters

  // send a GO bit

}

// Audio-to-vibe

void setup() {
  Serial.begin(9600);
  Serial.println("DRV test");

  pinMode(MOT1_EN, OUTPUT); // MOT1 enable
  pinMode(MOT2_EN, OUTPUT); // MOT1 enable
  pinMode(MOT3_EN, OUTPUT); // MOT1 enable
  pinMode(MOT4_EN, OUTPUT); // MOT1 enable
  pinMode(MOT5_EN, OUTPUT); // MOT1 enable
  pinMode(MOT6_EN, OUTPUT); // MOT1 enable
  pinMode(MOT7_EN, OUTPUT); // MOT1 enable
  pinMode(MOT8_EN, OUTPUT); // MOT1 enable
  digitalWrite(MOT1_EN, HIGH); // driver 1
  digitalWrite(MOT2_EN, LOW);
  digitalWrite(MOT3_EN, LOW); // driver 3
  digitalWrite(MOT4_EN, LOW); // driver 4
  digitalWrite(MOT5_EN, LOW);
  digitalWrite(MOT6_EN, LOW);
  digitalWrite(MOT7_EN, LOW);
  digitalWrite(MOT8_EN, LOW);

  Wire.begin();

  delay(250);
  tcaclear();
  tcaenableall();

  delay(250);

  drv.begin();

  drv.selectLibrary(1);

  // I2C trigger by sending 'go' command
  // default, internal trigger when sending GO command
  drv.setMode(DRV2605_MODE_INTTRIG);

  drv.useERM();
}

uint8_t effect = 1;

void loop() {

  // play the effect!
  drv.go();

  // wait a bit
  delay(500);

  effect++;
  if (effect > 117) effect = 1;
}
