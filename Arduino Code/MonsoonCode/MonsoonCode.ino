#include <stdint.h>
#include "TouchScreen.h"
#include "MIDIcontroller.h"

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A4  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

#define potPin0 A0
#define potPin1 A1
#define potPin2 A2


TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

byte MIDIchannel = 1;

MIDIpot myPot0(potPin0, 20);
MIDIpot myPot1(potPin1, 21);
MIDIpot myPot2(potPin2, 22);

int led = 13;

void setup(void) {
  Serial.begin(9600);
  pinMode(led, OUTPUT);

}

void loop() {
  // a point object holds x y and z coordinates
  TSPoint p = ts.getPoint();

  myPot0.send();
  myPot1.send();
  myPot2.send();

  
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > ts.pressureThreshhold) {
     Serial.print("X = "); Serial.print(p.x);
     Serial.print("\tY = "); Serial.print(p.y);
     Serial.print("\tPressure = "); Serial.println(p.z);

     delay(100);

     digitalWrite(led, HIGH);
     
     int xScaled = map(p.x, 100, 950, 0, 127);
     int yScaled = map(p.y, 100, 900, 0, 127);

     usbMIDI.sendControlChange(26, xScaled, 1);
     usbMIDI.sendControlChange(27, yScaled, 1); 
  }
  else {
    digitalWrite(led, LOW); 
  }

  while (usbMIDI.read()) {
    // ignore incoming messages
  }

}
