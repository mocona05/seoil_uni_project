#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "config.h"
#include "drv_sht21.h"
#include "driver/i2c.h"
//#include "webpage.h"


//#define LED_GPIO 2

char str_buff[30];
bool print_f=false;

static void test_taxk(void *ar){
    gpio_pad_select_gpio(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    while(1){
        delay(30);
        gpio_set_level(LED_GPIO, 1);
        delay(30);
        gpio_set_level(LED_GPIO, 0);
        if(print_f) {
            print_f=false;
           printf(str_buff);
        }
    }

}

static void measuring_task(void *arg){
    sht21_init();
    float temp, humi;

    while(1){
        if(temp_humi_Measure_Handler(&temp, &humi)){
            sprintf(str_buff,"temp= %5.2f, hum= %5.2f\r\n",temp, humi);
            print_f =true;

        }
        delay(100);

    }

}


void app_main() {

    printf("system boot!\r\n");
    xTaskCreate(&measuring_task, "measuring_task", 2048, (void* ) 0, 10, NULL);
    xTaskCreate(&test_taxk, "test_taxk", 2048, (void* ) 0, 1, NULL);
}