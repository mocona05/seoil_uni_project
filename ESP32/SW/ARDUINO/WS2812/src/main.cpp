#include "board.h"
#include "Arduino.h"
#include <adafruit_NeoPixel.h>
#include "ESPRandom.h"

#define SERIAL_NUM 2


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(SERIAL_NUM,RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    pixels.begin();
    esp_random();
}

void loop() {
  uint8_t i,j,k;
    
    pixels.setPixelColor(0, pixels.Color(random(2)*127 , random(2)*127 , random(2)*127 )); 
    pixels.setPixelColor(1, pixels.Color(random(5)*51 , random(5)*51 , random(5)*51 )); 
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(10);

}