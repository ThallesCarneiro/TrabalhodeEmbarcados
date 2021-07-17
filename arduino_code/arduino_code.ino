#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include <SoftwareSerial.h>

SoftwareSerial hc05(10, 11); // RX | TX

#define PIN 6

#define BUTTON1_PIN   7
#define BUTTON2_PIN   8

#define PIXEL_PIN    3

#define PIXEL_COUNT 30  // Number of NeoPixels

long previousMillis = 0;        // will store last time LED was updated

//unsigned long currentMillis = millis();

void setup()
{
  Serial.begin(9600);
  hc05.begin(9600);  //Default Baud for comm, it may be different for your Module. 
  Serial.println("The bluetooth gates are open.\n Connect to HC-05 from any other bluetooth device with 1234 as pairing key!.");
}

void loop()
{
  if (hc05.available()){
    Serial.println("temperatura");
    Serial.println(hc05.read());
  }
  delay(20);  
}
