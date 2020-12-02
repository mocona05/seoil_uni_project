//https://github.com/espressif/arduino-esp32/blob/master/libraries/BluetoothSerial/examples/SerialToSerialBT/SerialToSerialBT.ino

#include <Arduino.h>
#include "board.h"
#include "freertos/FreeRTOS.h"
#include "drv_gpio.h"
#include "BluetoothSerial.h"

  
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
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
    RED_LED_TOGGLE;
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
    BLUE_LED_TOGGLE;
  }
  delay(20);
  GREEN_LED_TOGGLE;
}

