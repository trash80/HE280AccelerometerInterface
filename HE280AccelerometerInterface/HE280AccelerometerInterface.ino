#include <Wire.h>

#define Z_PROBE_SENSITIVITY  12 // Seemecnc's default is 20, 15 is a good middle ground, 0-126 7 bit value
#define USE_RESET_PIN 1 // Comment out this line to not use the reset pin for reinitializing the probe
#define DEBUG 1 // Comment out this line to not echo results to serial

const uint8_t click_pin = 2; // "INT" interrupt pin from HE280 - Blue on my model
const uint8_t reset_pin = 3; // Used to re-send the initialization code to the HE280's accelerometer. Z Probe Mod pin.
const uint8_t trigger_pin = 13; // Sends a HIGH signal when probe touches the table. Connected to Duet's Z probe in. Duet set to Type: 5.

boolean resetSwitch = false;

#include "accelerometer.h"

void setup() {
  pinMode(click_pin, INPUT);
  pinMode(trigger_pin, OUTPUT);
  
  #ifdef USE_RESET_PIN
  pinMode(reset_pin, INPUT_PULLUP);
  #endif
  
  while(!accelerometer_init()) {
    debug_println("Failed Accel Init");
    accelerometer_status();
    digitalWrite(trigger_pin,HIGH); // Keep HIGH so that travel stops
    delay(50);
  }
}

void loop() {
  if(!digitalRead(click_pin)) {
    digitalWrite(trigger_pin,HIGH);
    delay(100);
    digitalWrite(trigger_pin,LOW);
    debug_print("Accel hit: ");
    debug_println(accelerometer_read(0x31));
  }
  #ifdef USE_RESET_PIN
  if(!digitalRead(reset_pin)) {
      resetSwitch = true;
  } else {
    if(resetSwitch == true) {
      debug_println("Reset Accel");
      accelerometer_read(0x31);
      
      while(!accelerometer_init()) {
        debug_println("Failed Accel Init");
        accelerometer_status();
        digitalWrite(trigger_pin,HIGH); // Keep HIGH so that travel stops
        delay(50);
      }
      
      delay(1500);
    }
    resetSwitch = false;
  }
  #endif
}
