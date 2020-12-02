//https://github.com/espressif/arduino-esp32/blob/master/libraries/BluetoothSerial/examples/SerialToSerialBT/SerialToSerialBT.ino

#include <Arduino.h>
#include "board.h"
#include "freertos/FreeRTOS.h"
#include "drv_gpio.h"
#include "BluetoothSerial.h"
#include "drv_sht21.h"

  
//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  init_gpio();
  sht21_init(SCL_PIN,SDA_PIN);
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  static float temp, humi;
  static char buffer[100];

  
    if(temp_humi_Measure_Handler(&temp, &humi)){
        GREEN_LED_TOGGLE;
          sprintf(buffer, "temp=%4.2f, humi=%2.2fr\r\n",temp, humi);
          SerialBT.print(buffer);
          Serial.print(buffer);
          delay(200);
    }

    delay(20);
  }


