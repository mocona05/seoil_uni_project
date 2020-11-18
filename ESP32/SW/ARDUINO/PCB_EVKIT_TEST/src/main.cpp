
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

void loop() {
  float temp, humi;
  char buff[50];

  if(temp_humi_Measure_Handler(&temp, &humi)) {
    sprintf(buff,"temp=%.2f, humi=%.2f\r\n", temp, humi);
    Serial.print(buff);
  }
    delay(1);   //esp32의 arduino frame work 는 Free rtos가 포팅되어 있어 delay가 없을 경우 os 리소스 반환이 없어 문제가 되어 삽입

}
