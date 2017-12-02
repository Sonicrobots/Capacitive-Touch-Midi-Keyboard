#include "MIDIUSB.h"
#include <Wire.h> // include I2C library
#include <i2c_touch_sensor.h>

// include our Grove I2C touch sensor library
// initialize the Grove I2C touch sensor
// IMPORTANT: in this case, INT pin was connected to pin7 of the Arduino
// (this is the interrupt pin)

i2ctouchsensor touchsensor; // keep track of 4 pads' states

const uint8_t numbPads = 4;
uint8_t oldTouchedState;

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void setup() {
  Serial.begin(115200);
  Serial.print("begin to init");
  Wire.begin(); // needed by the GroveMultiTouch lib
  touchsensor.initialize(); // initialize the feelers
}

void loop() {

  // read which sensors are being touched
  touchsensor.getTouchState();

  // check if any touch state changed to previous run
  if (touchsensor.touched != oldTouchedState) {

    // go through all of them..
    for (uint8_t i=0; i<numbPads; i++) {

      // ..and check which one changed
      if ((touchsensor.touched & (1<<i)) && !(oldTouchedState & (1<<i))) {

        Serial.print("Touched "); Serial.println(i);

        // do something different for each feeler
        switch (i) {
          case 0:
          noteOn(0, 48, 64);   // Channel 0, middle C, normal velocity
          MidiUSB.flush();
          break;

          case 1:
          noteOn(0, 52, 64);   // Channel 0, middle C, normal velocity
          MidiUSB.flush();
          break;

          case 2:
          noteOn(0, 54, 64);   // Channel 0, middle C, normal velocity
          MidiUSB.flush();
          break;

          case 3:

          noteOn(0, 56, 64);   // Channel 0, middle C, normal velocity
          MidiUSB.flush();
          break;
        }
      }
    }

    // store current state as new one for comparision on next run
    oldTouchedState = touchsensor.touched;
  }
}
