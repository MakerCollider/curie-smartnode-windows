#include "BLESerial.h"

void setup() {
  Serial.begin(115200);    // initialize serial communication

  BLESerial.setName("Bluno101");
  BLESerial.begin();
  //Serial.println("Bluetooth device active, waiting for connections...");
  while(!BLESerial);
}

void loop() {
	bool result = BLESerial.operator bool();
  while (result) {
    while(Serial.available()){
        BLESerial.write(Serial.read());
    }
    while(BLESerial.available()){
        Serial.write(BLESerial.read());
    }
  }
}
