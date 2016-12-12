#include <Wire.h>

uint8_t click_pin = 2; // "INT" interrupt pin from HE280 - Blue on my model
uint8_t reset_pin = 3; // Used to re-send the initialization code to the HE280's accelerometer. I used the Z-Access homing switch, input side, not ground.
uint8_t trigger_pin = 13; // Sends a HIGH signal when probe touches the table. Connected to Duet's E0 Stop input. Duet set to Type: 4.

boolean resetSwitch = false;

void setup() {
  pinMode(reset_pin, INPUT_PULLUP);
  pinMode(click_pin, INPUT);
  pinMode(trigger_pin, OUTPUT);
  accelerometer_init();
}

void loop() {
  if(!digitalRead(click_pin)) {
    digitalWrite(trigger_pin,HIGH);
    delay(50);
    digitalWrite(trigger_pin,LOW);
    uint8_t data = accelerometer_read(0x31);
  }
  if(!digitalRead(reset_pin)) {
    if(!resetSwitch) {
      resetSwitch = true;
      accelerometer_init();
      delay(1500);
    }
  } else {
    resetSwitch = false;
  }
}
