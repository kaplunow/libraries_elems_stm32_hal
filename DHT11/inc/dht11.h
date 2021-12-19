/*******************************************************************************
 *  This is header file of library for DHT11 humidity and temperature sensor.
 *  Communication with sensor is provided by "bit panding".
 *  Software is designed for STM32 HAL library.
 *******************************************************************************
 * @file dht11.h
 * @author Kaplunow
 * @date Dec 2021
 * @brief File containing declaraionts for DHT11.
 * @ver 0.1
 *******************************************************************************/

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

/* Includes ----------------------------------------------------------*/
#include "stm32f1xx_hal.h"


void DHT11_Init(void);
HAL_StatusTypeDef DHT11_Read(uint8_t* data);

#endif /* INC_DHT11_H_ */
