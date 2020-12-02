#include <Arduino.h>
#include "board.h"
#include "freertos/FreeRTOS.h"
#include "drv_gpio.h"
#include "BluetoothSerial.h"
#include "freertos/task.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


void led_task(void *Parameter);
void main_task(void *Parameter);
void green_led_on(void);
void blue_led_on(void);
void red_led_on(void);
void all_led_off(void);

typedef enum {
  IDLE,
  BLUE,
  GREEN,
  RED,
}staus_e;

typedef struct {
  char cmd;
  void (*funtion)(void);
  const char * msg_str;
  staus_e state;
}logic_t;

BluetoothSerial SerialBT;

logic_t cmd_list[]={
  {'G',green_led_on, "Green led on", GREEN},
  {'R',red_led_on, "Red led on", RED},
  {'B',blue_led_on, "Blue led on", BLUE},
  {'O',all_led_off, "All led off",IDLE},
};
#define CMD_LIST_SIZE sizeof(cmd_list)/sizeof(cmd_list[0])

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Remote");
  init_gpio();
  xTaskCreate(led_task, "led_task", 1024, NULL, 1, NULL);
  xTaskCreate(main_task, "main_task", 4096, NULL, 10, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}

staus_e led_state= IDLE;

void led_task(void *Parameter){
  while(1){
    switch(led_state){
      case IDLE:
        break;
      case RED:
        GREEN_LED_TOGGLE;
        break;
      case BLUE:
        RED_LED_TOGGLE;
        break;
      case GREEN:
        BLUE_LED_TOGGLE;
        break;

      default:
        break;
    }
    delay(100);
    BUILTIN_LED_TOGGLE;
  }
}

void main_task(void *Parameter){
  void (*funptr)(void);
  while(1){
    if(SerialBT.available()){
      char cmd = SerialBT.read();
      for(int i=0; i<CMD_LIST_SIZE;i++){
        if(cmd == cmd_list[i].cmd){
          funptr = cmd_list[i].funtion;
          led_state = cmd_list[i].state;
          funptr();
          Serial.println(cmd_list[i].msg_str);
          SerialBT.println(cmd_list[i].msg_str);
          break;
        }
      }
    }
    delay(10);
  }
}
void green_led_on(void){
    GREEN_LED_ON;
    RED_LED_OFF;
    BLUE_LED_OFF;
    BUILTIN_LED_OFF;
}
void red_led_on(void){
    GREEN_LED_OFF;
    RED_LED_ON;
    BLUE_LED_OFF;
    BUILTIN_LED_OFF;
}
void blue_led_on(void){
    GREEN_LED_OFF;
    RED_LED_OFF;
    BLUE_LED_ON;
    BUILTIN_LED_OFF;
}

void all_led_off(void){
    GREEN_LED_OFF;
    RED_LED_OFF;
    BLUE_LED_OFF;
    BUILTIN_LED_OFF;
}