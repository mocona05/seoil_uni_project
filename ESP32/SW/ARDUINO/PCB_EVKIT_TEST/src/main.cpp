
#include "Arduino.h"
#include "sd_test.h"
#include "gpio.h"
#include "rgb_led.h"
#include "board.h"
#include "drv_sht21.h"
#include "dac.h"

void setup() {
  Serial.begin(115200);
  sd_test();
  gpio_init();
  rgb_led_init();

  sht21_init(SCL_PIN, SDA_PIN);

}

void loop(){
  float humi, temp;
  char buffer[50];

  if(temp_humi_Measure_Handler(&temp, &humi)) {
    sprintf(buffer, "Temp=%.2f, humi=%.2f", temp, humi);
    Serial.println(buffer);
  }
  rgb_led_handler();
  gpio_handler();
  dac_handler();

}