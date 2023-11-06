#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/queue.h"
#include "encoder.h"
#include "key.h"
#include "font.h"

static u_int8_t count = 0;
static xQueueHandle gpioEventQueue = NULL;
static uint8_t counter = 0; // 声明计数变量


static void time_out_task(void *arg)
{
    vTaskDelay(200 / portTICK_PERIOD_MS);
    count = 0;
    vTaskDelete(NULL);
}

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpioEventQueue, &gpio_num, NULL);
}

static void gpio_task_example(void* arg)
{
    uint32_t io_num;
    char count_str[10]; // 用于存储右转次数的字符串
    for(;;) {
        if(xQueueReceive(gpioEventQueue, &io_num, portMAX_DELAY)) {
            // printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
            if (power_flag) // 开关打开
            {
            if(io_num == EC11_GPIO_SCL)
            {                
                if(count == 1)
                {
                    count = 3;
                    if (counter < 9) 
                    {counter++;}
                     else {
                            printf("Counter: MAX\n");
                            OLED_ShowString(0, 4, "MAX", 16); // 在 OLED 上显示 "MAX"
                            // vTaskDelay(pdMS_TO_TICKS(500)); // 延时1秒
                            // OLED_ClearLine(4,5,0,32);
                            // OLED_ShowString(0, 4, "9", 16); 
                            continue; // 跳过打印计数和更新 OLED 的步骤
                        } // 累加数递增
                    printf("Counter: %d\n", counter); // 打印累加数
                    snprintf(count_str, sizeof(count_str), "%d", counter);
                    OLED_ShowNum(0, 4, atoi(count_str), 1, 16); // atoi()函数将字符串转换为整数
                }
                else if(count == 0)
                {
                    count = 2;
                    xTaskCreate(time_out_task, "time_out_task", 2048, NULL, 2, NULL);
                }                                  
            } 
            else if(io_num == EC11_GPIO_DAT)
            {

                if(count == 2)
                {
                    count = 3;
                    OLED_ClearLine(4,5,0,32);
                    if(counter > 0) {
                        // OLED_ClearLine(4,5);
                            counter--; // 累加数递减
                        }
                    printf("Counter: %d\n", counter);
                    snprintf(count_str, sizeof(count_str), "%d", counter);
                    OLED_ShowNum(0, 4, atoi(count_str), 1, 16); // atoi()函数将字符串转换为整数
                }
                else if(count==0){
                    count = 1;
                    xTaskCreate(time_out_task, "time_out_task", 2048, NULL, 2, NULL);
                }
            }           
        }
        }
    }
}

void encoder_init(void)
{
    printf("Hello EC11!\n");
    gpio_config_t gpio_10 = {
        .pin_bit_mask = 1ULL << EC11_GPIO_SCL,
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_NEGEDGE,
        .pull_up_en = 1,
    };
    gpio_config_t gpio_11 = {
        .pin_bit_mask = 1ULL << EC11_GPIO_DAT,
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_NEGEDGE,
        .pull_up_en = 1,
    };
    gpio_config(&gpio_10);
    gpio_config(&gpio_11);
    gpioEventQueue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 1, NULL);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(EC11_GPIO_SCL, gpio_isr_handler, (void *)EC11_GPIO_SCL);
    gpio_isr_handler_add(EC11_GPIO_DAT, gpio_isr_handler, (void *)EC11_GPIO_DAT);
}
