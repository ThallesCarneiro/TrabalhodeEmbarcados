#include <SoftwareSerial.h>
#include <TimerOne.h>
#include <FastLED.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "definicoes_sistema.h"
#include "LED_matrix.h"
#include "LED_strip.h"

SoftwareSerial hc05(10, 11); // RX | TX

long previousMillis = 0;        

String inputString = "";
String tempString = "";

boolean stringComplete = false; 

volatile unsigned int button_count;
unsigned int color_mode;
volatile bool temperature_mode = true;

void serialEvent() {
  while (hc05.available()) {
    char inChar = (char)hc05.read();
    if (inChar == '|') {
      stringComplete = true;
    }
    else {
      tempString += inChar;
    }
  }
  
  if (stringComplete){
    inputString = tempString;
  }
  tempString = "";
  stringComplete = false;
}

void callback()
{
  serialEvent();
  Serial.println(inputString);
}

void button2()
{
  button_count++;
}

void display_stripLED() {
  if(stripLED++ < 30) { 
    switch(color_mode){
      case 0:
        leds[stripLED] = CRGB::Red;
        break;
      case 1:
        leds[stripLED] = CRGB::Green;
        break;
      case 2:
        leds[stripLED] = CRGB::Blue;
        break;
    }
  }
  else {
    stripLED = 0;
  }
  FastLED.show();
}

void setup()
{
  Timer1.initialize(2000000);         // initialize timer1, and set a 2 seconds period
  Timer1.attachInterrupt(callback);   // attaches callback() as a timer overflow interrupt
  pinMode(button1, INPUT_PULLUP);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), button2, CHANGE);
  Serial.begin(9600);
  hc05.begin(9600); 
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}

void loop()
{
  color_mode = button_count % 3;
  if(digitalRead(button1)){
    temperature_mode = true;
  }
  else {
    temperature_mode = false;
  }

  switch(temperature_mode){
    case true:
      display_temperature(color_mode, inputString);
      break;
    case false:
      display_lines(color_mode);
      break;
  }  
  matrix.setBrightness(map(analogRead(A0),0,1023,0,100));
  FastLED.setBrightness(map(analogRead(A0),0,1023,0,100));  
  display_stripLED(color_mode);
  delay(150);  
}
