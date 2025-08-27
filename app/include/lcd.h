#ifndef INC_GC9A01_H_
#define INC_GC9A01_H_

#include "def.h"

// ==== USER CONFIGURATIONS ====
#define lcd_SPI              hspi2
#define lcd_SPI_TIMEOUT      100
#define USE_DMA              0  // Enable [1] /disable[0] DMA

#define lcd_DC_PIN  GPIO_PIN_11
#define lcd_DC_PORT GPIOD

#define lcd_CS_PIN  GPIO_PIN_12
#define lcd_CS_PORT GPIOD

#define lcd_RST_PIN GPIO_PIN_13
#define lcd_RST_PORT GPIOD

// Optional user callback for flush complete
void lcd_FlushReady(void);

// ==== API ====
void lcd_Init(void);
void lcd_Reset(void);
void lcd_WriteCommand(uint8_t cmd);
void lcd_WriteData(uint8_t data);
void lcd_WriteDataBuffer(uint8_t *data, uint32_t size);
void lcd_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void lcd_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void lcd_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void lcd_Flush(const void *color_map, int32_t x1, int32_t y1, int32_t x2, int32_t y2);

#endif
