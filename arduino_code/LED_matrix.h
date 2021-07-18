#ifndef LED_MATRIX_H_INCLUDED
#define LED_MATRIX_H_INCLUDED
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR 
#endif
#include "definicoes_sistema.h"

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(mw, mh, MATRIX_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

int colum1 = 0;
int row1 = 0;
int colum2 = 4;
int row2 = 4;

int x = matrix.width();

void display_temperature(int color_mode, String temperatures);

void display_lines(int color_mode);

void display_lines(int color_mode) {
  matrix.clear();
  if(row2 < 8){
    row2 ++;
  }
  else {
    row2 = 0;
    colum2 = millis() % 8;
  }
  if(row1 < 8){
    row1 ++;
  }
  else {
    row1 = 0;
    colum1 = millis() % 8;
  }
  matrix.drawLine(colum1, row1-3, colum1, row1, colors[color_mode]);
  matrix.drawLine(colum2, row2-3, colum2, row2, colors[color_mode]);
  matrix.show();
}

void display_temperature(int color_mode, String temperatures) {  
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(temperatures);
  if(--x < -36) {
    x = matrix.width();
    matrix.setTextColor(colors[color_mode]);
  }
  matrix.show();
}

#endif
