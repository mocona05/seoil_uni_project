/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Libraries for SD card
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "Wire.h"            // SD Card ESP32
#include "config.h"            // SD Card ESP32

#include <SPI.h>

#include "drv_sht21.h"
#include "sd_memory.h"
#include "webpage.h"

// Libraries to get time from NTP Server
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define CAMERA_MODEL_AI_THINKER // Has PSRAM




// Define deep sleep options
uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
// Sleep for 10 minutes = 600 seconds
uint64_t TIME_TO_SLEEP = 10;



// Save reading number on RTC memory



    
void setup() {
  // Start serial communication for debugging purposes
  Serial.begin(115200);

  sht21_init(I2C_SCL, I2C_SDA);

  pinMode(BUTTON_GPIO, INPUT_PULLUP);

  pinMode(LED_GPIO, OUTPUT);
  
  init_webServer();
  init_NTPClient();
}

typedef enum {
  SYSTEM_INIT,
  SYSTE_IDLE,
  LOGGING,
}e_system_status;

#define SENSOR_READ_CYCLE   2
void loop() {
  static uint32_t restart_time=0;  
  uint32_t timeNow;
  static bool over_flow_f =false;
  static e_system_status sys_status = SYSTEM_INIT;

  timeNow =millis();

  if(timeNow >= restart_time && !(over_flow_f  && timeNow > (0xFFFFFFFF - SENSOR_READ_CYCLE)) ) {
   over_flow_f= time_end_calculation(timeNow, SENSOR_READ_CYCLE, &restart_time);
   if(temp_humi_Measure_Handler(&temperature, &humi)) {
      webServer_handler();
   }
  }
  button_read_handler();
  webServer_handler();

  switch((uint8_t)sys_status) {
    case SYSTEM_INIT:
      btn_stat =  BTN_READ;
      LED_OFF;
      sys_status =  SYSTE_IDLE;
       Serial.println("System Init!");
      break;
    case SYSTE_IDLE:
      if(btn_stat == PUSH_SHORT) {
        btn_stat =  BTN_READ;
        logging_data_handler(true);
        sys_status = LOGGING;
        Serial.println("Data Logging Start!");
      }
      break;
    case LOGGING:
      logging_data_handler(false);
      if(btn_stat == PUSH_SHORT) {
        btn_stat =  BTN_READ;
        logging_data_handler(true);
        LED_OFF;
        Serial.println("Data Logging Stop!");
        sys_status = SYSTE_IDLE;
      }
      break;
    default:
      break;
  }
}
