#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "key.h"
#include "encoder.h"
#include "font.h"
#include "dht11.h"
// #include "lwip/err.h"
// #include "lwip/sys.h"

#define TOUCH_PIN GPIO_NUM_6

static const char *TAG = "app_main";          // 定义标签
static TaskHandle_t dht11_task_handle = NULL; // 声明任务句柄

void adc_task(void *pvParameters)
{
    // 配置ADC通道
    adc1_config_width(ADC_WIDTH_BIT_12);                        // 设置ADC位宽为12位
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11); // 配置ADC通道和增益

    while (1)
    {
        if (power_flag)
        {
            // 读取ADC值
            int adc_value = adc1_get_raw(ADC1_CHANNEL_3);

            char adc_str[16];
            snprintf(adc_str, sizeof(adc_str), "ADC: %d", adc_value);

            // 在OLED上显示ADC值
            OLED_ShowString(40, 4, adc_str, 16);

            // 打印ADC值
            printf("ADC value: %d\n", adc_value);
            
        }vTaskDelay(pdMS_TO_TICKS(1000)); // 延时1秒
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Running");
    i2c_master_init();
    encoder_init();
    key_init();
    OLED_Init();
    // 初始化ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);

    gpio_pad_select_gpio(TOUCH_PIN);
    gpio_set_direction(TOUCH_PIN, GPIO_MODE_INPUT);
    int touch_value = gpio_get_level(TOUCH_PIN);
    ESP_LOGI(TAG, "Pin value: %d", touch_value);
    // 创建ADC任务
    xTaskCreate(adc_task, "adc_task", 2048, NULL, 4, NULL);
    // 创建DHT11C任务
    xTaskCreate(dht11_task, "dht11_task", configMINIMAL_STACK_SIZE * 4, NULL, 5, &dht11_task_handle);
    while (1)
    {
        key_read();
        vTaskDelay(pdMS_TO_TICKS(1000)); // 等待一段时间，以允许其他任务运行
    }
}