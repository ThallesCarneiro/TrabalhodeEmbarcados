#ifndef LED_STRIP_H_INCLUDED
#define LED_STRIP_H_INCLUDED
#include <FastLED.h>
#include "definicoes_sistema.h"

CRGB leds[NUM_LEDS];

int stripLED = 0;

void display_stripLED(int color_mode);


void display_stripLED(int color_mode) {
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

#endif
