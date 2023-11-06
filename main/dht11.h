#ifndef DHT11_h
#define DHT11_h

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"


#define DHT11_PIN     (7)   //可通过宏定义，修改引脚
#define DHT11_CLR     gpio_set_level(DHT11_PIN, 0) 
#define DHT11_SET     gpio_set_level(DHT11_PIN, 1) 
#define DHT11_IN      gpio_set_direction(DHT11_PIN, GPIO_MODE_INPUT)
#define DHT11_OUT     gpio_set_direction(DHT11_PIN, GPIO_MODE_OUTPUT)

void DHT11_Start(void);
uint8_t DHT11_ReadValue(void);
uint8_t DHT11_ReadTemHum(uint8_t *buf);
void dht11_task(void *pvParameters);



#endif