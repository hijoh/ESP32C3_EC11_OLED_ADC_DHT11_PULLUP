#ifndef FONT_H
#define FONT_H

#include <stdint.h>
#include "driver/i2c.h"

void OLED_Init(void);

extern unsigned char F6x8[][6];
extern unsigned char F8X16[];
extern unsigned char Hzk[][32];

extern void OLED_Set_Pos(uint8_t x, uint8_t y);
extern void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);
extern void OLED_Clear(void);
extern void OLED_ClearRegion_1(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
extern void OLED_ClearLine(uint8_t startLine, uint8_t endLine,uint8_t startColumn, uint8_t endColumn); // Clear line 2 on the OLED display
extern void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size2);
extern void OLED_ShowString(uint8_t x, uint8_t y, char *chr, uint8_t Char_Size);
extern void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t no);

esp_err_t i2c_master_init(void);
esp_err_t OLED_WR_Byte(uint8_t data, uint8_t cmd_);

uint32_t oled_pow(uint8_t m, uint8_t n);

#endif