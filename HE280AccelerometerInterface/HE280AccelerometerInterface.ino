#include <Wire.h>
#include "Trash80_IIS2DH.h"

#define Z_PROBE_SENSITIVITY 20 // 0-126 7 bit value

const uint8_t click_pin = 2; // "INT" interrupt pin from HE280 - Blue on my model
const uint8_t mod_pin = 3; // Used to arm the probe. Z Probe Mod pin.
const uint8_t trigger_pin = 13; // Sends a HIGH signal when probe touches the table. Connected to Duet's Z probe in. Duet set to Type: 5.

Trash80_IIS2DH accel = Trash80_IIS2DH();

uint8_t lastState = false;

unsigned long activationWindow;

void setup() {
  pinMode(click_pin, INPUT);
  pinMode(mod_pin, INPUT);
  pinMode(trigger_pin, OUTPUT);

  digitalWriteFast(trigger_pin,HIGH); // Trigger by default in case of probe comm failure

  accel.init(true); // true = fast 400khz mode
  while(!accel.connected()) ; // do nothing if not connected

  accel.setDataRate(9);       // 0:Power-down mode, 1:1Hz, 2:10Hz, 3:25Hz, 4:50Hz, 5:100Hz, 6:200Hz, 7:400Hz, 8:1.620kHz, 9:1.344kHz Normal / 5.376 kHz Low power)
  accel.setDataResolution(0); // 0:Low-power mode (8bit resolution), 1:Normal mode (10bit), 2:High-resolution mode (12bit)
  accel.setScale(0);          // 0: ±2g, 1: ±4g, 2: ±8g, 3: ±16g
  accel.setReference(0);      // Reset reference to zero
  /*
  * setFilter info.
  * mode: 0:Normal mode (reset by reading REFERENCE/DATACAPTURE (26h) register), 1:Reference signal for filtering, 2: Normal mode, 3:Autoreset on interrupt event
  * cutoff  0ODR         1ODR        2ODR        3ODR       4ODR     5ODR      6ODR      7ODR       8ODR
  *    0:   0.02hz       0.2hz       0.5hz       1hz        2hz      4hz       8hz       32hz       100hz
  *    1:   0.008hz      0.08hz      0.2hz       0.5hz      1hz      2hz       4hz       16hz       50hz
  *    2:   0.004hz      0.04hz      0.1hz       0.2hz      0.5hz    1hz       2hz       8hz        25hz
  *    3:   0.002hz      0.02hz      0.05hz      0.1hz      0.2hz    0.5hz     1hz       4hz        12hz
  */
  //              mode, cutoff filterDataOutput enableClick enableAndOr1 enableAndOr2
  accel.setFilter(0,    1,     true,            false,      true,       false);

  // enableAxis    X     Y     Z
  accel.enableAxis(false,false,true);

  accel.setInt1Mode(0);       // 0: OR combination of int events, 1: 6-direction movement recognition, 2: AND combination of interrupt events, 3: 6D position recognition
 
  // INT1 Trig on Axis direction    xDown  xUp    yDown  yUp    zDown  zUp
  accel.setInt1AxisTrigger(         false, false, false, false, false, true);

  // INT1 Trigger threshold
  accel.setInt1Threshold(127); // 0 to 127 - Set automatically be probe activation

  // INT1 Latching mode
  accel.setInt1Latch(true);

  //    Int1Options:   Click  AOI1  AOI2   DRDY1 DRDY2  FIFOWM FIFOOVERRUN
  accel.setInt1Options(false, true, false, false, false, false, false);

  // FIFO mode
  accel.setFifoMode(3); // 0: Bypass mode, 1:FIFO mode, 2:Stream mode, 3:Stream-to-FIFO mode
  accel.setFifoTrigger(0); // 0:Trigger INT1, 1:Trigger INT2
  
  // Block Data updating before a read
  accel.blockDataUpdate(true);

  accel.enableFifo(true);

  accel.enableTemperature(true);

  digitalWriteFast(trigger_pin,LOW);
}

void loop() {
  
  uint8_t state = digitalReadFast(mod_pin);
  
  if(state != lastState) {
    if(state) {
      activationWindow = millis() + 100;
    } else {
      activationWindow = 0;
      accel.setInt1Threshold(127);
    }
    lastState = state;
  }

  if(activationWindow && activationWindow < millis()) {
    activationWindow = 0;
    accel.setInt1Threshold(Z_PROBE_SENSITIVITY);
  }
  
  if(!digitalReadFast(click_pin)) {
    if(!state) {
      // probe not armed clear int and return
      accel.int1Status(); // Reset INT1
      accel.fifoReset(); // reset FIFO
      accel.setFifoMode(3);
      return;
    }
    
    digitalWriteFast(trigger_pin,HIGH);
    accel.setInt1Threshold(127);

    delay(50);

    accel.int1Status(); // Reset INT1
    accel.fifoReset(); // reset FIFO
    accel.setFifoMode(3);
    digitalWrite(trigger_pin,LOW);
  }
}
