#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "config.h"
#include "board.h"
#include "drv_sht21.h"
#include "driver/i2c.h"
#include "drv_touch.h"


//#define LED_GPIO 2

#define STATE

char str_buff[30];
bool print_f=false;


void ws2812_control(void){
    ets_delay_us(1);

}

static void test_taxk(void *ar){
    touch_init();
    gpio_pad_select_gpio(WS2812_PIN);
    gpio_set_direction(WS2812_PIN, GPIO_MODE_OUTPUT);

    while(1){
//        tp_read_handler();
        gpio_set_level(LED_GPIO, 1);
        if(print_f) {
            print_f=false;
           printf(str_buff);
        }
    }

}

static void measuring_task(void *arg){
    sht21_init();

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