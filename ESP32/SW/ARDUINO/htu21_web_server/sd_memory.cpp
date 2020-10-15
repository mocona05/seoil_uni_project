#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "Wire.h"            // SD Card ESP32
#include "config.h"            // SD Card ESP32
#include "sd_memory.h"

#include <SPI.h>

#include "drv_sht21.h"

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

static void logSDCard(char * pfile_name);
static void writeFile(fs::FS &fs, const char * path, const char * message);
static void appendFile(fs::FS &fs, const char * path, const char * message);

// Define NTP Client to get time
WiFiUDP ntpUDP;

RTC_DATA_ATTR uint32_t readingID = 0;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

String dataMessage;

char file_name_str[50] ={NULL,};


void init_NTPClient(void) {
  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(32400);
}



bool logging_data_handler(bool reset) {
  static uint32_t restart_time=0;  
  uint32_t timeNow;
  static bool over_flow_f =false;
  static uint16_t file_number=0;
  static e_logging_stat logging_stat = SD_MOMORY_CHECK;
  File file;
  uint8_t cardType;
  
  timeNow=millis();

  if(reset) {
    /*
     over_flow_f= time_end_calculation(timeNow, 1000, &restart_time);  
     while(!temp_humi_Measure_Handler(&temperature, &humi)){   //처음 값을 가져올때까지 대기
      if(timeNow >= restart_time && !(over_flow_f  && timeNow > (0xFFFFFFFF - 1000)) ) {        
        over_flow_f= time_end_calculation(timeNow, LOGGING_CYCLE_TIME, &restart_time);
        Serial.println("Sensor Not Found!");
      }  
      timeNow=millis();
     }
     */
     readingID =0;
     logging_stat = SD_MOMORY_CHECK;
     Serial.println("Sensor initialize OK");
  }

  switch((uint8_t)logging_stat) {
    case SD_MOMORY_CHECK:
      if(SD_MMC.begin()) {
        LED_ON;
        Serial.println("Card Mount OK!");
        cardType = SD_MMC.cardType();
        //        SD_MMC.dateTimeCallback
        //        SD_MMC::dateTimeCallback(dateTime);
//        sprintf(file_name_str, "/%s%03d.csv", DEFAULT_FIME_NAME, file_number);
        logging_stat = FILE_CREATING;
        file.close();
      }
      break;

    case FILE_CREATING:
        do{
         sprintf(file_name_str, "/%s%03d.csv", DEFAULT_FIME_NAME, file_number);
         file_number++;
//         Serial.println("%s",file);
         Serial.println(file_name_str);
         file = SD_MMC.open(file_name_str);
        }while(file);
        Serial.println("Creating file...");
        writeFile(SD_MMC, file_name_str, "Reading ID,Date,Hour,Temperature,Humidity \r\n");
        file.close();
        logging_stat = DATA_LOGGING;
      break;

    case DATA_LOGGING:
      LED_ON;
        if(timeNow >= restart_time && !(over_flow_f  && timeNow > (0xFFFFFFFF - LOGGING_CYCLE_TIME)) ) {
//          if(temp_humi_Measure_Handler(&temperature, &humi)){
            over_flow_f= time_end_calculation(timeNow, LOGGING_CYCLE_TIME, &restart_time);  
//            Serial.printf("Temp:%f, Humid:%f\n", temperature, humi );
  //          esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);          
            getTimeStamp();
            logSDCard(file_name_str);          
            readingID++;
        }
      break;

    case RESTART_WAIT:
      LED_OFF;
      return false;
      break;
    default :
      break;  
  }
  return false;
}


// Function to get date and time from NTPClient
void getTimeStamp(void) {
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
//  Serial.println(formattedDate);

  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
//  Serial.println(dayStamp);
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
//  Serial.println(timeStamp);
}

// Write the sensor readings on the SD card
static void logSDCard(char * pfile_name) {
  dataMessage = String(readingID) + CELL_DIVISION + String(dayStamp) + CELL_DIVISION + String(timeStamp) + CELL_DIVISION + 
                String(temperature)+CELL_DIVISION+String(humi) + "\r\n";
//  Serial.print("Save data: ");
//  Serial.println(dataMessage);
  appendFile(SD_MMC, pfile_name, dataMessage.c_str());
}

// Write to the SD card (DON'T MODIFY THIS FUNCTION)
static void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
//  file.time();
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to the SD card (DON'T MODIFY THIS FUNCTION)
static void appendFile(fs::FS &fs, const char * path, const char * message) {
//  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
//    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
//    Serial.println("Message appended");
  }
//else {
//    Serial.println("Append failed");
//  }
  file.close();
}
