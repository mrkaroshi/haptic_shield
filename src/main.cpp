#include <Wire.h>
#include "Adafruit_DRV2605.h"
#include <Chrono.h>

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

// vibe
int previous_waveform = 16;

// serial
#define INPUT_SIZE 4
int new_data = 0;
bool debug_mode = false;

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

  uint8_t transmission = 255; // BE CAREFUL!
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
  enableDRV(255);
  Wire.begin();
  delay(250);
  tcaclear();
  tcaenableall();
  delay(250);
  drv.begin();
  drv.selectLibrary(6);
  drv.setMode(DRV2605_MODE_INTTRIG);
  drv.useERM();
  //enableDRV(0);

}

// MENU

// RTP
void enableRTP(){

  drv.setMode(DRV2605_MODE_REALTIME);

}

void sendRTP(uint8_t vibe_val){

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

// software timers
Chrono chronoi2c(Chrono::MICROS);

void setup() {

  Serial.begin(9600, SERIAL_8E1);
  Serial.setTimeout(2);
  Serial.println("Haptic Shield v0.1b");

  initDRV();
  Wire.begin();
  delay(250);
  tcaclear();
  tcaenableall();
  delay(250);
  drv.begin();
  drv.selectLibrary(2);
  drv.setMode(DRV2605_MODE_INTTRIG);
  drv.useERM();
  drv.setWaveform(0, previous_waveform);  // play effect
  drv.setWaveform(1, 0);       // end waveform
}

int i = 0;
int trig = 0;
int RTP_val = 0;
int enableSelection = 0;
int TRIGGER_DRV = 0;
int INTTRIG_val = 0;
//char input[INPUT_SIZE + 1]; // create array for incoming serial data of set size

void loop() {

  // choose between RTP or INTTRIG

  // wait for incoming command
  if(Serial.available() > 0){

    char input[INPUT_SIZE + 1]; // create array for incoming serial data of set size

    chronoi2c.restart();

    byte size = Serial.readBytes(input, INPUT_SIZE); // THIS NEEDS REPLACING WITH SERIAL.READ()

    chronoi2c.stop();
    long currentTime = chronoi2c.elapsed();
    Serial.print("Time used: "); Serial.println(currentTime);

    input[size] = 0; // clear array after read
    char* command = strtok(input, " "); // returns pointer to beginning of token
    if(debug_mode){
      Serial.print("Raw serial: "); Serial.println(command);
    }

    while (command != 0){
      // split command into a, b and c
      char* RTP_transmit = strchr(command, 'r'); // returns pointer to location of instance
      char* INTTRIG_mode = strchr(command, 'i'); // returns pointer to location of instance
      char* RTP_mode = strchr(command, 'p'); // returns pointer to location of instance
      char* DEBUG_mode = strchr(command, 'd'); // returns pointer to location of instance
      char* ENABLE_selection = strchr(command, 'e'); // returns pointer to location of instance
      char* TRIGGER_selection = strchr(command, 't'); // returns pointer to location of instance

      // pick mode
      if (TRIGGER_selection != 0){
        *TRIGGER_selection = 0;
        ++TRIGGER_selection;
        TRIGGER_DRV = atoi(TRIGGER_selection);
        drv.go();
        if(debug_mode){
          Serial.println("DRV triggered!"); // Serial.println(TRIGGER_DRV, DEC);
        }
        Serial.flush();
        //trig = 1; // set trigger flag ready for end of loop
        //new_data = 1;
      }

      if (DEBUG_mode != 0){
        *DEBUG_mode = 0;
        ++DEBUG_mode;
        debug_mode = !debug_mode;
        if(debug_mode){
          Serial.println("Debug mode!");
        }
      }

      if (ENABLE_selection != 0){
        *ENABLE_selection = 0;
        ++ENABLE_selection;
        enableSelection = atoi(ENABLE_selection);
        tcaselect(enableSelection);
        if(debug_mode){
          Serial.print("DRV selected: "); Serial.println(enableSelection, DEC);
        }
        new_data = 1;
      }

      if (RTP_mode != 0){
        *RTP_mode = 0;
        ++RTP_mode;
        enableRTP();
        if(debug_mode){
          Serial.println("RTP mode enabled.");
        }
        new_data = 1;
      }

      if (RTP_transmit != 0){
        *RTP_transmit = 0;
        ++RTP_transmit;
        RTP_val = atoi(RTP_transmit);
        if(debug_mode){
          Serial.print("DRV RTP: "); Serial.println(RTP_val, DEC);
        }
        new_data = 1;
      }

      if (INTTRIG_mode != 0){
        *INTTRIG_mode = 0;
        ++INTTRIG_mode;
        INTTRIG_val = atoi(INTTRIG_mode); // issue is here!
        if(debug_mode){
          Serial.print("Waveform trigger "); Serial.print(INTTRIG_val, DEC); Serial.println(" set on current DRV.");
        }
        new_data = 1;
      }

      command = strtok(0, " "); // split command based on the space
    }
  }

  if(new_data){

    if(RTP_val >= 0){
      sendRTP(RTP_val);
      RTP_val = 0;
    }

    if(INTTRIG_val > 0){
      drv.setMode(DRV2605_MODE_INTTRIG);
      drv.setWaveform(0, INTTRIG_val);  // play effect
      drv.setWaveform(1, 0);       // end waveform
      INTTRIG_val = 0;
    }
    // if(trig){
    //   drv.go();
    //   trig = 0;
    // }

    new_data = 0;
  }

}
