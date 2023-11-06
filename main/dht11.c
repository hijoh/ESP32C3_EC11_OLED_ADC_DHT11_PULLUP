#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "dht11.h"
#include "key.h"
#include "font.h"

// static TaskHandle_t dht11_task_handle = NULL; // 声明任务句柄
uint8_t DHT11Data[4] = {0};
uint8_t Temp, Humi;
// us延时函数，误差不能太大
static void DelayUs(  uint32_t nCount)  
{
    ets_delay_us(nCount);
}  

void DHT11_Start(void)
{ 
  DHT11_OUT;      //设置端口方向
  DHT11_CLR;      //拉低端口  
  gpio_pad_select_gpio(DHT11_PIN);
  DelayUs(19*1000);   
//   vTaskDelay(19 * portTICK_RATE_MS); //持续最低18ms;

  DHT11_SET;      //释放总线
  DelayUs(30);    //总线由上拉电阻拉高，主机延时30uS;
  DHT11_IN;       //设置端口方向

  while(!gpio_get_level(DHT11_PIN));   //DHT11等待80us低电平响应信号结束
  while(gpio_get_level(DHT11_PIN));//DHT11   将总线拉高80us
}

uint8_t DHT11_ReadValue(void)
{ 
  uint8_t i,sbuf=0;
  for(i=8;i>0;i--)
  {
    sbuf<<=1; 
    while(!gpio_get_level(DHT11_PIN));
    DelayUs(30);                        // 延时 30us 后检测数据线是否还是高电平 
    if(gpio_get_level(DHT11_PIN))
    {
      sbuf|=1;  
    }
    else
    {
      sbuf|=0;
    }
    while(gpio_get_level(DHT11_PIN));
  }
  return sbuf;   
}

uint8_t DHT11_ReadTemHum(uint8_t *buf)
{
  uint8_t check;

  buf[0]=DHT11_ReadValue();
  buf[1]=DHT11_ReadValue();
  buf[2]=DHT11_ReadValue();
  buf[3]=DHT11_ReadValue();
    
  check =DHT11_ReadValue();

  if(check == buf[0]+buf[1]+buf[2]+buf[3])
    return 1;
  else
    return 0;
} 


// FreeRTOS任务函数，用于读取DHT11数据
void dht11_task(void *pvParameters)
{
    while (1)
    {
        if (power_flag)
        {
            // Read temperature and humidity from DHT11 sensor
            DHT11_Start();
            if (DHT11_ReadTemHum(DHT11Data))
            {
                Temp = DHT11Data[2];
                Humi = DHT11Data[0];

                // Display temperature and humidity on OLED screen
                char tempStr[20];
                char humiStr[20];
                snprintf(tempStr, sizeof(tempStr), "Temp: %d C", Temp);
                snprintf(humiStr, sizeof(humiStr), "Humi: %d%%", Humi);

                OLED_ShowString(0, 0, tempStr, 16);
                OLED_ShowString(0, 2, humiStr, 16);
            }
            else
            {
                printf("DHT11 Error!\r\n");
            }
        }

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}