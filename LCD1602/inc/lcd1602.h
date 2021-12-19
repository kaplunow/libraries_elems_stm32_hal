/*******************************************************************************
 *  This is header file of library for LCD 2x16 connected to 
 *  PCF85474 I/O expander (LCM1602 I2C converter).
 *  Software is designed for STM32 HAL library.
 *******************************************************************************
 * @file lcd1602.h
 * @author Kaplunow
 * @date Dec 2021
 * @brief File containing declarations for LCD 2x16
 * @ver 0.1
 *  ******************************************************************************
 */

#ifndef INC_LCD1602_H_
#define INC_LCD1602_H_

/* Includes ----------------------------------------------------------*/
#include "stm32f1xx_hal.h"


void LCD1602_Init(void);
void LCD1602_Print(char *str);
void LCD1602_Clear(void);
void LCD1602_SetCursor(uint8_t col, uint8_t row);
void LCD1602_Backlight(uint8_t state);
void LCD1602_Blink_On(void);
void LCD1602_Blink_Off(void);


#endif /* INC_LCD1602_H_ */
