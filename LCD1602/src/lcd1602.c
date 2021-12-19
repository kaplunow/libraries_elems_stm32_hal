/********************************************************************************
 *  This is source file of library for LCD 2x16 connected to 
 *  PCF85474 I/O expander (LCM1602 I2C converter).
 *  Software is designed for STM32 HAL library.
 *******************************************************************************
 * @file lcd1602.h
 * @author Kaplunow
 * @date Dec 2021
 * @brief File containing implementations for LCD 2x16
 * @ver 0.1
 *******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "lcd1602.h"

/* Private defines ----------------------------------------------------------*/
#define PCF85474A_ADDRESS 		0x3F
#define PCF85474_ADDRESS 		0x27

#define LCD_RS				 	(1 << 0)
#define LCD_RW				 	(1 << 1)
#define LCD_E				 	(1 << 2)
#define LCD_BL				 	(1 << 3)

#define LCD_INIT_DELAY			 20						/* milliseconds */
#define LCD_DELAY				 5						/* milliseconds */

#define LCD_CLEAR_DISPLAY      	 0x01
#define LCD_SET_DDRAM_ADDRESS	 0x80
#define LCD_FULL_LINE 			 0x40

/* Global variables ----------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;								/* Rename if need another */

/* Private variables ----------------------------------------------------------*/
static uint8_t lcd_backlight = 1;
static uint8_t PCF85474_ADD = PCF85474_ADDRESS << 1;		/* Choose PCF85474_ADDRESS or PCF85474_ADDRESS */

/* Private prototypes ----------------------------------------------------------*/
static void PCF85474_Write(uint8_t data, uint8_t rs);

/**
  * @brief LCD1602 initialization.
  */
void LCD1602_Init(void)
{
	HAL_Delay(LCD_INIT_DELAY);

	PCF85474_Write(0x33, 0);
	PCF85474_Write(0x32, 0);
	PCF85474_Write(0x2C, 0);		 	/* Setting interface 4-bit, 2-line display, 5x10 dots font */
	PCF85474_Write(0x08, 0);			/* Display off */
	PCF85474_Write(0x01, 0);			/* Display clear */
	PCF85474_Write(0x04, 0);			/* Cursor moving direction left, shift entire display off */

	PCF85474_Write(0x0C, 0);			/* Display on, cursor off, blink off */
}
/**
  * @brief Writing byte to LCD in 4-bit interface mode.
  *
  * @param data Byte to send.
  * @param rs Register select.
  */
void PCF85474_Write(uint8_t data, uint8_t rs)
{
	while ( !(HAL_I2C_IsDeviceReady(&hi2c1, PCF85474_ADD, 1, HAL_MAX_DELAY) == HAL_OK) );

	uint8_t up = data & 0xF0;
	uint8_t lo = (data << 4) & 0xF0;

	uint8_t data_arr[4];
	data_arr[0] = up | (lcd_backlight?LCD_BL:0x00) | (rs?LCD_RS:0x00) | LCD_E;
	data_arr[1] = up | (lcd_backlight?LCD_BL:0x00) | (rs?LCD_RS:0x00);
	data_arr[2] = lo | (lcd_backlight?LCD_BL:0x00) | (rs?LCD_RS:0x00) | LCD_E;
	data_arr[3] = lo | (lcd_backlight?LCD_BL:0x00) | (rs?LCD_RS:0x00);

	HAL_I2C_Master_Transmit(&hi2c1, PCF85474_ADD, data_arr, sizeof(data_arr), HAL_MAX_DELAY);
	HAL_Delay(LCD_DELAY);
}
/**
  * @brief Print on display.
  *
  * @param str String to display.
  */
void LCD1602_Print(char* str)
{
	while (*str) {
		PCF85474_Write((uint8_t)(*str), 1);
		str++;
	}
}
/**
  * @brief Clear display
  */
void LCD1602_Clear()
{
	PCF85474_Write(LCD_CLEAR_DISPLAY, 0);
}
/**
  * @brief Set cursor on display.
  *
  * @param col Column.
  * @param row Row.
  */
void LCD1602_SetCursor(uint8_t col, uint8_t row)
{
	PCF85474_Write(LCD_SET_DDRAM_ADDRESS+col+(LCD_FULL_LINE*row), 0);
}
/**
  * @brief Backlight turn on/off
  *
  * @param state State of the backlight (0/1).
  */
void LCD1602_Backlight(uint8_t state)
{
	lcd_backlight = state;
	PCF85474_Write(0x00, 0);
}
/**
  * @brief Blinking of cursor on.
  */
void LCD1602_Blink_On(void)
{
	PCF85474_Write(0x0D, 0);						/* Cursor off, blink on */
}
/**
  * @brief Blinking of cursor off.
  */
void LCD1602_Blink_Off(void)
{
	PCF85474_Write(0x0C, 0);						/* Cursor off, blink off */
}
