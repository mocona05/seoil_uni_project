#pragma once

#define DEFAULT_FIME_NAME   "data"

#define CELL_DIVISION   ","
//#include <NTPClient.h>

typedef enum  {
  SD_MOMORY_CHECK,
  FILE_CREATING,
  DATA_LOGGING,
  RESTART_WAIT,
    
}e_logging_stat;




bool logging_data_handler(bool reset);
void getTimeStamp(void);
void init_NTPClient(void);

 
