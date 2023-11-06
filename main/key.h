/**
 * 按键程序头文件
 */
#ifndef __KEY_H
#define __KEY_H

#include <stdio.h>
#include "driver/gpio.h"

#define LED_R_IO        GPIO_NUM_13
#define KEY_IO          GPIO_NUM_12
#define LONG_PRESS_TIME pdMS_TO_TICKS(2000)

void key_init(void);
void key_read(void);

extern bool power_flag;


#endif /* __XTP_BTN_H */
