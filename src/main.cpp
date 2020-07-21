#include <Arduino.h>
#include <SPI.h>

extern "C" {
  #include "XSPI.h"
  #include "Flasher.h"
}

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }

  Serial.println("butt");

  XSPIInit();
  Serial.println("hello?");

  XSPIEnterFlashMode();

  BYTE FlashConfigBuffer[4];
  XSPIRead(0, FlashConfigBuffer);
	XSPIRead(0, FlashConfigBuffer);

  Serial.print("0x");
  Serial.println(FlashConfigBuffer[0], HEX);

  Serial.print("0x");
  Serial.println(FlashConfigBuffer[1], HEX);

  Serial.print("0x");
  Serial.println(FlashConfigBuffer[2], HEX);

  Serial.print("0x");
  Serial.println(FlashConfigBuffer[3], HEX);
}

void loop() {

}