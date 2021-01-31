#include <Arduino.h>

#include "driver/ledc.h"
#include "pitches.h"

const int buzzerpin = 2;

int freq = 2000;
int channel = 0;
int resolution = 8;



void setup() {
  Serial.begin(115200);
  ledcSetup(channel, freq, resolution);
  


}

void loop() {
  // put your main code here, to run repeatedly:

  ledcWrite(channel, 125);
  ledcWriteTone(channel, NOTE_C4);
  delay(1000);
  ledcDetachPin(buzzerpin);
  delay(1000);
  ledcAttachPin(buzzerpin, channel);
  ledcWriteTone(channel, NOTE_D4);
  delay(1000);
  ledcWrite(channel, 0);
  delay(1000);
  ledcWriteTone(channel, NOTE_E4);
  delay(1000);
  ledcWriteTone(channel, NOTE_F4);
  delay(1000);
  ledcWriteTone(channel, NOTE_G4);
  delay(1000);
  ledcWriteTone(channel, NOTE_A5);
  delay(1000);
  ledcWriteTone(channel, NOTE_B5);
  delay(1000);
  ledcWriteTone(channel, NOTE_C5);
  delay(1000);

}