#include <Wire.h>
#include "Adafruit_DRV2605.h"

Adafruit_DRV2605 drv;

#include "Wire.h"
extern "C" {
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}

// mux
#define TCAADDR 0x70

// drv
int MOT1_EN = 24;
int MOT2_EN = 28;
int MOT3_EN = 32;
int MOT4_EN = 36;
int MOT5_EN = 40;
int MOT6_EN = 44;
int MOT7_EN = 46;
int MOT8_EN = 48;

// serial
#define INPUT_SIZE 20
int new_data = 0;

// functions
void tcaselect(uint8_t i) {

  if (i > 255) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(i);
  Wire.endTransmission(true);

}

void tcaclear() {

  Wire.beginTransmission(TCAADDR);
  Wire.write(0x00);
  Wire.endTransmission(true);

}

void tcaenableall() {

  uint8_t transmission = 8; // BE CAREFUL!
  Wire.beginTransmission(TCAADDR);
  Wire.write(transmission);
  Wire.endTransmission(true);
}
void setOutputs(){
  pinMode(MOT1_EN, OUTPUT); // MOT1 enable
  pinMode(MOT2_EN, OUTPUT); // MOT2 enable
  pinMode(MOT3_EN, OUTPUT); // MOT3 enable
  pinMode(MOT4_EN, OUTPUT); // MOT4 enable
  pinMode(MOT5_EN, OUTPUT); // MOT5 enable
  pinMode(MOT6_EN, OUTPUT); // MOT6 enable
  pinMode(MOT7_EN, OUTPUT); // MOT7 enable
  pinMode(MOT8_EN, OUTPUT); // MOT8 enable
}

void enableDRV(uint8_t bit_pattern){

  digitalWrite(MOT1_EN, (bit_pattern & B00000001));
  digitalWrite(MOT2_EN, (bit_pattern & B00000010) >> 1);
  digitalWrite(MOT3_EN, (bit_pattern & B00000100) >> 2); // driver 3
  digitalWrite(MOT4_EN, (bit_pattern & B00001000) >> 3); // driver 4
  digitalWrite(MOT5_EN, (bit_pattern & B00010000) >> 4);
  digitalWrite(MOT6_EN, (bit_pattern & B00100000) >> 5);
  digitalWrite(MOT7_EN, (bit_pattern & B01000000) >> 6);
  digitalWrite(MOT8_EN, (bit_pattern & B10000000) >> 7);
  //tcaselect(bit_pattern);
}

void initDRV(){

  setOutputs();
  enableDRV(0xFF);
  Wire.begin();
  delay(250);
  tcaclear();
  tcaenableall();
  delay(250);
  drv.begin();
  drv.selectLibrary(6);
  drv.setMode(DRV2605_MODE_INTTRIG);
  drv.useERM();

}

// MENU

// RTP
void enableRTP(uint8_t vibe_val){

  drv.setMode(DRV2605_MODE_REALTIME);
  drv.setRealtimeValue(vibe_val);

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

  Serial.begin(115200);
  Serial.println("Haptic Shield v0.1b");

  //initDRV();
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
  digitalWrite(MOT3_EN, HIGH); // driver 3
  digitalWrite(MOT4_EN, HIGH); // driver 4
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
  drv.selectLibrary(6);
  drv.setMode(DRV2605_MODE_INTTRIG);
  drv.useERM();
  drv.setWaveform(0, 17);  // play effect
  drv.setWaveform(1, 0);       // end waveform
}

int i = 0;

void loop() {

  // choose between RTP or INTTRIG

  // wait for incoming command
  char input[INPUT_SIZE + 1]; // create array for incoming serial data of set size
  byte size = Serial.readBytes(input, INPUT_SIZE); // read in input
  input[size] = 0; // clear array after read
  char* command = strtok(input, " "); // returns pointer to beginning of token

  while (command != 0){

    // split command into a, b and c
    char* RTP_mode = strchr(command, 'r'); // returns pointer to location of instance
    char* INTTRIG_mode = strchr(command, 'i'); // returns pointer to location of instance
    char* ENABLE_selection = strchr(command, 'e'); // returns pointer to location of instance
    char* TRIGGER_selection = strchr(command, 't'); // returns pointer to location of instance

    // pick mode
    if (ENABLE_selection != 0){
      *ENABLE_selection = 0;
      ++ENABLE_selection;
      int enableSelection = atoi(ENABLE_selection);
      tcaselect(enableSelection);
      Serial.print("DRV selected: "); Serial.println(enableSelection, DEC);
      new_data = 1;
    }

    if (TRIGGER_selection != 0){
      *TRIGGER_selection = 0;
      ++TRIGGER_selection;
      int TRIGGER_DRV = atoi(TRIGGER_selection);
      tcaselect(TRIGGER_DRV);
      drv.setMode(DRV2605_MODE_INTTRIG);
      drv.go();
      Serial.print("DRV triggered: "); Serial.println(TRIGGER_DRV, DEC);
      new_data = 1;
    }

    if (RTP_mode != 0){
      *RTP_mode = 0;
      ++RTP_mode;
      int RTP_val = atoi(RTP_mode);
      enableRTP(RTP_val);
      Serial.print("DRV RTP: "); Serial.println(RTP_val, DEC);
      new_data = 1;
    }

    if (INTTRIG_mode != 0){
      *INTTRIG_mode = 0;
      ++INTTRIG_mode;
      int INTTRIG_val = atoi(INTTRIG_mode);
      enableRTP(INTTRIG_val);
      Serial.print("DRV "); Serial.println(INTTRIG_val, DEC); Serial.println(" set to INTTRIG.");
      new_data = 1;
    }

    command = strtok(0, " "); // split command based on the space
  }

  if(new_data){
    new_data = 0;
  }

}
