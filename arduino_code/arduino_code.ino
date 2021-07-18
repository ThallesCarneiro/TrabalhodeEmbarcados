#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include<FastLED.h>
#ifndef PSTR
 #define PSTR 
#endif
#ifdef __AVR__
 #include <avr/power.h> 
#endif
#include <SoftwareSerial.h>
#include <TimerOne.h>

SoftwareSerial hc05(10, 11); // RX | TX

#define MATRIX_PIN 6

#define DATA_PIN 5

#define NUM_LEDS 30

#define mw 8

#define mh 8
CRGB leds[NUM_LEDS];

byte  r, g, b;

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(mw, mh, MATRIX_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

#define LED_BLACK         0

#define LED_RED_HIGH      (31 << 11)

#define LED_GREEN_HIGH    (63 << 5)  

#define LED_BLUE_HIGH     31


const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

long previousMillis = 0;        // will store last time LED was updated

String inputString = "";
String tempString = "";

boolean stringComplete = false; 

int x = matrix.width();
int pass = 0;

const byte button1 = 2;
const byte interruptPin = 3;

volatile unsigned int button_count;
unsigned int color_mode;
volatile bool temperature_mode = true;
int stripLED = 0;

//unsigned long currentMillis = millis();

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

void display_lines() {
    matrix.clear();

    matrix.drawLine(0,mh/2+1, mw-1,mh/2+1, LED_RED_HIGH);

    matrix.drawLine(mw/2+1, 0, mw/2+1, mh-1, LED_GREEN_HIGH);

    // Diagonal blue line.
    matrix.drawLine(0,0, mw-1,mh-1, LED_BLUE_HIGH);
    matrix.show();
}

void display_temperature() {
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(inputString);
  if(--x < -36) {
    x = matrix.width();
    matrix.setTextColor(colors[color_mode]);
  }
  matrix.show();
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
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
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
      display_temperature();
      break;
    case false:
      display_lines();
      break;
  }  
  matrix.setBrightness(map(analogRead(A0),0,1023,0,100));
  FastLED.setBrightness(map(analogRead(A0),0,1023,0,100));  
  display_stripLED();
  Serial.println(color_mode);
  delay(200);  
}
