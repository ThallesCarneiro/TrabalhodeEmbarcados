#ifndef LED_STRIP_H_INCLUDED
#define LED_STRIP_H_INCLUDED
#include <FastLED.h>
#include "definicoes_sistema.h"

CRGB leds[NUM_LEDS];

int stripLED = 0;

void display_stripLED(int color_mode);

// Função de acionamento sequencial da fita de LEDs com a cor selecionada
void display_stripLED(int color_mode) {
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
  if(stripLED < 30) {
    stripLED++;
  }
  else {
    stripLED = 0;
  }
  FastLED.show();
}

#endif
