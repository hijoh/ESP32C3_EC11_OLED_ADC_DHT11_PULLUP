#include <string.h>

#include "key.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "font.h"


bool power_flag = false;
TickType_t button_press_time = 0;
uint8_t right_turn_count = 0;

static const char *TAG = "led";  // 定义标签

void key_init(void)
{
    // Configure LED pin as output
    gpio_pad_select_gpio(LED_R_IO);
    gpio_set_direction(LED_R_IO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_R_IO, 0); // Turn off initially

    // Configure button pin as input
    gpio_pad_select_gpio(KEY_IO);
    gpio_set_direction(KEY_IO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(KEY_IO, GPIO_PULLUP_ONLY); // Enable internal pull-up resistor 
    
}

void key_read(void)
{  
    TickType_t current_time = xTaskGetTickCount();
    TickType_t button_press_duration = current_time - button_press_time;

    if (gpio_get_level(KEY_IO) == 0) // 按键按下
    {
        if (button_press_time == 0) // 按键之前未被按下
        {
            button_press_time = current_time;
        }
        else if (button_press_duration >= LONG_PRESS_TIME) // 按键按下持续2秒
        {
            // 切换开关状态标志
            power_flag = !power_flag;
            
            if (power_flag)
            {
                ESP_LOGI(TAG, "开关已打开");
            //    xTaskCreate(dht11_task, "DHT11 Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &dht11_task_handle);
                
                OLED_ShowString(12, 6, "2023-11-3", 16);
                gpio_set_level(LED_R_IO, 1);
            }
            else
            {
                ESP_LOGI(TAG, "开关已关闭");
                 OLED_Clear();
                // OLED_ShowCHinese(0 * 18, 0, 0);
                // OLED_ShowCHinese(1 * 18, 0, 1);
                OLED_ShowString(30, 5, "GOODBYE!", 16);
                
                gpio_set_level(LED_R_IO, 0);
                vTaskDelay(1000 / portTICK_PERIOD_MS);//延时1秒
                OLED_Clear();
            }
            button_press_time = 0; // 重置按键按下时间
            
        }
    }
    else // 按键释放
    {
        button_press_time = 0; // 重置按键按下时间
    }
}
