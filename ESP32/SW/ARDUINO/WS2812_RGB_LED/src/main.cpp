#include "board.h"
#include "Arduino.h"
#include <adafruit_NeoPixel.h>
#define SERIAL_NUM 2
#define RED   0xFF0000
#define GREEN 0x00FF00
#define BLUE  0x0000FF
#define WHITE  0xFFFFFF
#define BLACK  0x000000

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(SERIAL_NUM,RGB_LED_PIN, NEO_GRB + NEO_KHZ800);
uint32_t color_table[] {
  RED,
  GREEN,
  BLUE,
  WHITE,
  BLACK,
};
#define COLOR_TABLE_SIZE  (sizeof(color_table)/sizeof(color_table[0]))
void setup() {
    pixels.begin();
}
void loop() {
  for(int color=0; color < COLOR_TABLE_SIZE; color++ ){
      pixels.setPixelColor(0, pixels.Color(color_table[color]>>16 , (uint8_t)(color_table[color]>>8) , (uint8_t)color_table[color] )); 
      pixels.setPixelColor(1, pixels.Color(color_table[COLOR_TABLE_SIZE-1-color]>>16 , (uint8_t)(color_table[COLOR_TABLE_SIZE-1-color]>>8)\
       , (uint8_t)color_table[COLOR_TABLE_SIZE-1-color] )); 
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(1000);
  }
}
