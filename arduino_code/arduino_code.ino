#include <SoftwareSerial.h>
#include <TimerOne.h>
#include <FastLED.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "definicoes_sistema.h"
#include "LED_matrix.h"
#include "LED_strip.h"

// Configura a comunicação serial com o módulo blutooth nos pinos 10 (RX) e 11 (TX)     
SoftwareSerial hc06(10, 11);

String temperatures = ""; // String que contém a mensagem com as temperaturas da CPU e GPU
String bt_string = "";    // String que acumula os caracteres recebidos pela comunicação serial

volatile bool stringComplete = false; // Flag que indica quando a string de interesse terminou de ser recebida

volatile unsigned int button_count; // Contador de bordas no sinal do botão 
unsigned int color_mode;            // Modo de iluminação selecionado: 0 = RED ; 1 = GREEN ; 2 = BLUE

bool temperature_mode = true;       // Modo de exibição da matriz de LEDs: true = monitoramento de temperatura ; false = efeito estético

// Função de recebimento de temperaturas por Bluetooth
void serialEvent() {
  while (hc06.available()) {
    char inChar = (char)hc06.read(); // Armazena caractere lido na comunicação serial
    if (inChar == '|') {             // Verifica caractere de fim de mensagem
      stringComplete = true;         // Registra fim de mensagem
    }
    else {
      bt_string += inChar;           // Acumula caracteres para formar a mensagem
    }
  }
  if (stringComplete){
    temperatures = bt_string;        // Armazena mensagem completa na string a ser exibida
  }
  bt_string = "";                    // Esvazia string auxiliar
  stringComplete = false;            // Reverte o estado da flag para nova mensagem
}

// Rotina de interrupção por tempo - comunicação Bluetooth
void callback()
{
  serialEvent();
  Serial.println(temperatures);
}

// Rotina de interrupção externa - botão
void button2()
{
  button_count++; // conta a mudança de estados no botão
}

void setup()
{
  Timer1.initialize(2000000);           // Inicializa timer 1, intervalo de 2 segundos
  Timer1.attachInterrupt(callback);     // Vincula o interrupt pelo timer 1 à função callback()
  pinMode(button1, INPUT_PULLUP);       // Configura o botão1 como input
  pinMode(interruptPin, INPUT_PULLUP);  // Configura o interruptPin (botão2) como input
  attachInterrupt(digitalPinToInterrupt(interruptPin), button2, CHANGE); // Vincula o interrupt externo em qualquer borda do interruptPin à função button2() 
  Serial.begin(9600);                   // Inicializa comunicação serial padrão por USB, para debugging e verificação
  hc06.begin(9600);                     // Inicializa comunicação serial com o módulo Bluetooth
  matrix.begin();                       // Inicializa matriz de LEDs
  matrix.setTextWrap(false);            
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); // Inicializa fita de LEDs
}

void loop()
{
  color_mode = button_count % 3;  // Determina a cor selecionada a partir do contador do botão 
  if(digitalRead(button1)){       // Verifica o estado do botão1
    temperature_mode = true;      // Caso ele esteja acionado, ativa-se o modo de monitoramento
  }
  else {
    temperature_mode = false;     // Caso contrário, ativa-se o modo sem monitoramento
  }

  switch(temperature_mode){       // Estados do sistema
    case true:  // Modo com monitoramento
      display_temperature(color_mode, temperatures); 
      break;
    case false: // Modo sem monitoramento
      display_lines(color_mode);                      
      break;
  }  
  matrix.setBrightness(map(analogRead(A0),0,1023,0,100));  // Definir brilho da matriz a partir da posição do potenciômetro
  FastLED.setBrightness(map(analogRead(A0),0,1023,0,100)); // Definir brilho da fita a partir da posição do potenciômetro  
  display_stripLED(color_mode);    
  delay(150);  
}
