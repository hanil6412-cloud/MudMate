



#include "lcd.h"

extern SPI_HandleTypeDef hspi2;

#if USE_DMA
static volatile uint8_t tx_busy = 0;
#endif

// === INTERNAL GPIO CONTROL ===
static void lcd_Select(void)   { HAL_GPIO_WritePin(lcd_CS_PORT, lcd_CS_PIN, GPIO_PIN_RESET); }
static void lcd_Unselect(void){ HAL_GPIO_WritePin(lcd_CS_PORT, lcd_CS_PIN, GPIO_PIN_SET);   }
static void lcd_DC_Command(void){ HAL_GPIO_WritePin(lcd_DC_PORT, lcd_DC_PIN, GPIO_PIN_RESET); }
static void lcd_DC_Data(void)   { HAL_GPIO_WritePin(lcd_DC_PORT, lcd_DC_PIN, GPIO_PIN_SET);   }

// === RESET ===
void lcd_Reset(void)
{
	HAL_GPIO_WritePin(lcd_RST_PORT, lcd_RST_PIN, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(lcd_RST_PORT, lcd_RST_PIN, GPIO_PIN_SET);
	HAL_Delay(120);
}

// === COMMAND/DATA ===
void lcd_WriteCommand(uint8_t cmd)
{
	lcd_Select();
	lcd_DC_Command();
	HAL_SPI_Transmit(&lcd_SPI, &cmd, 1, lcd_SPI_TIMEOUT);
	lcd_Unselect();
}

void lcd_WriteData(uint8_t data)
{
	lcd_Select();
	lcd_DC_Data();
	HAL_SPI_Transmit(&lcd_SPI, &data, 1, lcd_SPI_TIMEOUT);
	lcd_Unselect();
}

void lcd_WriteDataBuffer(uint8_t *data, uint32_t size)
{
	lcd_Select();
	lcd_DC_Data();
#if USE_DMA
	tx_busy = 1;
	HAL_SPI_Transmit_DMA(&lcd_SPI, data, size);
#else
	HAL_SPI_Transmit(&lcd_SPI, data, size, lcd_SPI_TIMEOUT);
	lcd_Unselect();
#endif
}

// === SET ADDRESS WINDOW ===
void lcd_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	lcd_WriteCommand(0x2A);  // Column
	uint8_t col_data[] = {x0 >> 8, x0 & 0xFF, x1 >> 8, x1 & 0xFF};
	lcd_WriteDataBuffer(col_data, 4);

	lcd_WriteCommand(0x2B);  // Row
	uint8_t row_data[] = {y0 >> 8, y0 & 0xFF, y1 >> 8, y1 & 0xFF};
	lcd_WriteDataBuffer(row_data, 4);

	lcd_WriteCommand(0x2C);  // Write
}

// === DRAW PIXEL ===
void lcd_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	lcd_SetAddressWindow(x, y, x, y);
	uint8_t data[] = {color >> 8, color & 0xFF};
	lcd_WriteDataBuffer(data, 2);
#if USE_DMA
	while (tx_busy);
#endif
}

// === FILL RECTANGLE ===
void lcd_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	uint32_t size = w * h;
	lcd_SetAddressWindow(x, y, x + w - 1, y + h - 1);

	uint8_t buf[512];
	for (int i = 0; i < sizeof(buf); i += 2) {
		buf[i] = color >> 8;
		buf[i + 1] = color & 0xFF;
	}

	while (size > 0) {
		uint16_t chunk = (size > 256) ? 256 : size;
		lcd_WriteDataBuffer(buf, chunk * 2);
#if USE_DMA
while (tx_busy);
#endif
size -= chunk;
	}
}

// === FLUSH for LVGL ===
void lcd_Flush(const void *color_map, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
	uint32_t size = (x2 - x1 + 1) * (y2 - y1 + 1) * 2;
	lcd_SetAddressWindow(x1, y1, x2, y2);
	lcd_Select();
	lcd_DC_Data();
#if USE_DMA
	tx_busy = 1;
	HAL_SPI_Transmit_DMA(&lcd_SPI, (uint8_t *)color_map, size);
#else
	HAL_SPI_Transmit(&lcd_SPI, (uint8_t *)color_map, size, lcd_SPI_TIMEOUT);
	lcd_Unselect();
	lcd_FlushReady();
#endif
}

// === DMA CALLBACK ===
#if USE_DMA
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi == &lcd_SPI) {
		lcd_Unselect();
		tx_busy = 0;
		lcd_FlushReady();
	}
}
#endif

// === OPTIONAL WEAK CALLBACK ===
__weak void lcd_FlushReady(void)
{
	// Implement in user code:
}

// === DISPLAY INIT ===
void lcd_Init(void)
{
	lcd_Reset();

	lcd_WriteCommand(0xEF);
	lcd_WriteCommand(0xEB);
	lcd_WriteData(0x14);

	lcd_WriteCommand(0xFE);
	lcd_WriteCommand(0xEF);

	lcd_WriteCommand(0xEB);
	lcd_WriteData(0x14);

	lcd_WriteCommand(0x84);
	lcd_WriteData(0x40);

	lcd_WriteCommand(0x85);
	lcd_WriteData(0xFF);

	lcd_WriteCommand(0x86);
	lcd_WriteData(0xFF);

	lcd_WriteCommand(0x87);
	lcd_WriteData(0xFF);

	lcd_WriteCommand(0x88);
	lcd_WriteData(0x0A);

	lcd_WriteCommand(0x89);
	lcd_WriteData(0x21);

	lcd_WriteCommand(0x8A);
	lcd_WriteData(0x00);

	lcd_WriteCommand(0x8B);
	lcd_WriteData(0x80);

	lcd_WriteCommand(0x8C);
	lcd_WriteData(0x01);

	lcd_WriteCommand(0x8D);
	lcd_WriteData(0x01);

	lcd_WriteCommand(0x8E);
	lcd_WriteData(0xFF);

	lcd_WriteCommand(0x8F);
	lcd_WriteData(0xFF);


	lcd_WriteCommand(0xB6);
	lcd_WriteData(0x00);
	lcd_WriteData(0x20);

	lcd_WriteCommand(0x36);
	lcd_WriteData(0x08);//Set as vertical screen

	lcd_WriteCommand(0x3A);
	lcd_WriteData(0x05);


	lcd_WriteCommand(0x90);
	lcd_WriteData(0x08);
	lcd_WriteData(0x08);
	lcd_WriteData(0x08);
	lcd_WriteData(0x08);

	lcd_WriteCommand(0xBD);
	lcd_WriteData(0x06);

	lcd_WriteCommand(0xBC);
	lcd_WriteData(0x00);

	lcd_WriteCommand(0xFF);
	lcd_WriteData(0x60);
	lcd_WriteData(0x01);
	lcd_WriteData(0x04);

	lcd_WriteCommand(0xC3);
	lcd_WriteData(0x13);
	lcd_WriteCommand(0xC4);
	lcd_WriteData(0x13);

	lcd_WriteCommand(0xC9);
	lcd_WriteData(0x22);

	lcd_WriteCommand(0xBE);
	lcd_WriteData(0x11);

	lcd_WriteCommand(0xE1);
	lcd_WriteData(0x10);
	lcd_WriteData(0x0E);

	lcd_WriteCommand(0xDF);
	lcd_WriteData(0x21);
	lcd_WriteData(0x0c);
	lcd_WriteData(0x02);

	lcd_WriteCommand(0xF0);
	lcd_WriteData(0x45);
	lcd_WriteData(0x09);
	lcd_WriteData(0x08);
	lcd_WriteData(0x08);
	lcd_WriteData(0x26);
	lcd_WriteData(0x2A);

	lcd_WriteCommand(0xF1);
	lcd_WriteData(0x43);
	lcd_WriteData(0x70);
	lcd_WriteData(0x72);
	lcd_WriteData(0x36);
	lcd_WriteData(0x37);
	lcd_WriteData(0x6F);


	lcd_WriteCommand(0xF2);
	lcd_WriteData(0x45);
	lcd_WriteData(0x09);
	lcd_WriteData(0x08);
	lcd_WriteData(0x08);
	lcd_WriteData(0x26);
	lcd_WriteData(0x2A);

	lcd_WriteCommand(0xF3);
	lcd_WriteData(0x43);
	lcd_WriteData(0x70);
	lcd_WriteData(0x72);
	lcd_WriteData(0x36);
	lcd_WriteData(0x37);
	lcd_WriteData(0x6F);

	lcd_WriteCommand(0xED);
	lcd_WriteData(0x1B);
	lcd_WriteData(0x0B);

	lcd_WriteCommand(0xAE);
	lcd_WriteData(0x77);

	lcd_WriteCommand(0xCD);
	lcd_WriteData(0x63);


	lcd_WriteCommand(0x70);
	lcd_WriteData(0x07);
	lcd_WriteData(0x07);
	lcd_WriteData(0x04);
	lcd_WriteData(0x0E);
	lcd_WriteData(0x0F);
	lcd_WriteData(0x09);
	lcd_WriteData(0x07);
	lcd_WriteData(0x08);
	lcd_WriteData(0x03);

	lcd_WriteCommand(0xE8);
	lcd_WriteData(0x34);

	lcd_WriteCommand(0x62);
	lcd_WriteData(0x18);
	lcd_WriteData(0x0D);
	lcd_WriteData(0x71);
	lcd_WriteData(0xED);
	lcd_WriteData(0x70);
	lcd_WriteData(0x70);
	lcd_WriteData(0x18);
	lcd_WriteData(0x0F);
	lcd_WriteData(0x71);
	lcd_WriteData(0xEF);
	lcd_WriteData(0x70);
	lcd_WriteData(0x70);

	lcd_WriteCommand(0x63);
	lcd_WriteData(0x18);
	lcd_WriteData(0x11);
	lcd_WriteData(0x71);
	lcd_WriteData(0xF1);
	lcd_WriteData(0x70);
	lcd_WriteData(0x70);
	lcd_WriteData(0x18);
	lcd_WriteData(0x13);
	lcd_WriteData(0x71);
	lcd_WriteData(0xF3);
	lcd_WriteData(0x70);
	lcd_WriteData(0x70);

	lcd_WriteCommand(0x64);
	lcd_WriteData(0x28);
	lcd_WriteData(0x29);
	lcd_WriteData(0xF1);
	lcd_WriteData(0x01);
	lcd_WriteData(0xF1);
	lcd_WriteData(0x00);
	lcd_WriteData(0x07);

	lcd_WriteCommand(0x66);
	lcd_WriteData(0x3C);
	lcd_WriteData(0x00);
	lcd_WriteData(0xCD);
	lcd_WriteData(0x67);
	lcd_WriteData(0x45);
	lcd_WriteData(0x45);
	lcd_WriteData(0x10);
	lcd_WriteData(0x00);
	lcd_WriteData(0x00);
	lcd_WriteData(0x00);

	lcd_WriteCommand(0x67);
	lcd_WriteData(0x00);
	lcd_WriteData(0x3C);
	lcd_WriteData(0x00);
	lcd_WriteData(0x00);
	lcd_WriteData(0x00);
	lcd_WriteData(0x01);
	lcd_WriteData(0x54);
	lcd_WriteData(0x10);
	lcd_WriteData(0x32);
	lcd_WriteData(0x98);

	lcd_WriteCommand(0x74);
	lcd_WriteData(0x10);
	lcd_WriteData(0x85);
	lcd_WriteData(0x80);
	lcd_WriteData(0x00);
	lcd_WriteData(0x00);
	lcd_WriteData(0x4E);
	lcd_WriteData(0x00);

	lcd_WriteCommand(0x98);
	lcd_WriteData(0x3e);
	lcd_WriteData(0x07);

	lcd_WriteCommand(0x35);
	lcd_WriteCommand(0x21);

	lcd_WriteCommand(0x11);
	HAL_Delay(120);
	lcd_WriteCommand(0x29);
	HAL_Delay(20);
}
