#include <Arduino.h>

#define DAC2  26
#define LED_BUILTIN 2
#define TOUCH_CH1   T9

void setup()
{
  Serial.begin(115200);
  Serial.println("ESP32 Touch Test");
}

void loop()
{
  Serial.println(touchRead(TOUCH_CH1));  // get value using T0
  delay(500);
}
