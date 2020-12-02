#include <Arduino.h>  
#include "config.h"           
#include "drv_sht21.h"
#include "webpage.h"

#define SENSOR_DATA_PRINTER_CYCLE_TIME 1000
void sensor_status_print_handler(void){
  static uint32_t restart_time=0;  
  uint32_t timeNow;
  static bool over_flow_f =false;  
  char value_str[80];
  timeNow =millis();
  if(timeNow >= restart_time && !(over_flow_f  && timeNow > (0xFFFFFFFF - SENSOR_DATA_PRINTER_CYCLE_TIME)) ) {
   over_flow_f= time_end_calculation(timeNow, SENSOR_DATA_PRINTER_CYCLE_TIME, &restart_time);
    sprintf(value_str, "Temp= %6.2f℃, Humi= %5.2f%RH%%", temperature, humi);
    Serial.println(value_str);
  }
}
    
void setup() {
  // Start serial communication for debugging purposes
  Serial.begin(115200);
  sht21_init(I2C_SCL, I2C_SDA);
  pinMode(BUTTON_GPIO, INPUT_PULLUP);
  pinMode(LED_GPIO, OUTPUT);
  init_webServer();
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
}
