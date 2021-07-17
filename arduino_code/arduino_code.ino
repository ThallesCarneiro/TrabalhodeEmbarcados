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
#include <TimerOne.h>

SoftwareSerial hc05(10, 11); // RX | TX

#define MATRIX_PIN 6

#define LED_PIN    3

#define LED_COUNT 30  // Number of NeoPixels

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, MATRIX_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

long previousMillis = 0;        // will store last time LED was updated

String inputString = "";
String tempString = "";

boolean stringComplete = false; 

int x = matrix.width();
int pass = 0;

const byte interruptPin1 = 7;
const byte interruptPin2 = 8;

volatile int color_mode = 0;
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

void button1()
{
  
  delayMicroseconds(1000);
}

void button2()
{
  
  delayMicroseconds(1000);
}

void colorWipe(uint32_t color, int wait) {
  if(wait<(millis()-previousMillis); stripLED++) { // For each pixel in strip...
    strip.setPixelColor(stripLED, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    previousMillis = millis();
  }
  if(stripLED > 30){
    stripLED = 0;
  }
}

void setup()
{
  Timer1.initialize(2000000);         // initialize timer1, and set a 2 seconds period
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
  pinMode(interruptPin1, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), button1, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), button2, RISING);
  Serial.begin(9600);
  hc05.begin(9600); 
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);
  
  strip.begin();           
  strip.show();            
  strip.setBrightness(40);
}

void loop()
{
  matrix.setBrightness(map(analogRead(A0),0,1023,0,255));
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(inputString);
  if(--x < -36) {
    x = matrix.width();
    if(++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();

  colorWipe(strip.Color(255,   0,   0), 50);    // Red
  strip.setBrightness(map(analogRead(A0),0,1023,0,255));
  
  
  delay(200);  
}
