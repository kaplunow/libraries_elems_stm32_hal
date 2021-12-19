/*******************************************************************************
 *  This is source file of library for DHT11 humidity and temperature sensor.
 *  Communication with sensor is provided by "bit panding".
 *  Software is designed for STM32 HAL library.
 *******************************************************************************
 * @file dht11.c
 * @author Kaplunow
 * @date Dec 2021
 * @brief File containing implementations for DHT11.
 * @ver 0.1
 *******************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include "dht11.h"

/* Private defines -----------------------------------------------------------*/
#define BASEPRI_REG_SHIFT 		4
#define LO_PREMPTION_PRIORITY   3						/* Lowest less significant premption priority interrupt (change if another) */
#define DHT11_DATA_FRAME_SIZE	5

#define DHT_GPIO_Port		    GPIOC				 	/* Define DHT11 GPIO (change if another port/pin) */
#define DHT_Pin 			    GPIO_PIN_10
/* Global variables ----------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;							/* TIM whose base is 1us (rename if another) */

/* Private prototypes ---------------------------------------------------------*/
static uint8_t DHT11_ReadBit(void);
static uint8_t DHT11_ReadByte(void);
static HAL_StatusTypeDef DHT11_Start();
static void delay_us(uint32_t us);


/**
  * @brief Delay in microseconds unit.
  *
  * @param us Microseconds.
  */
void delay_us(uint32_t us)
{
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while (__HAL_TIM_GET_COUNTER(&htim1) < us);
}
/**
  * @brief Starts communication process with DHT11.
  *
  * @return HAL_OK/HALL_ERROR If there is communication with sensor.
  */
HAL_StatusTypeDef DHT11_Start()
{
	int ticks = 0;

	HAL_GPIO_WritePin(DHT_GPIO_Port, DHT_Pin, GPIO_PIN_RESET);					  /* MCU send start signal */
	HAL_Delay(18);
	HAL_GPIO_WritePin(DHT_GPIO_Port, DHT_Pin, GPIO_PIN_SET);

	while (HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin) == GPIO_PIN_SET) {			  /* Waiting for DHT11 response */
		ticks++;
		if (ticks > 50)
			return HAL_ERROR;
		delay_us(1);
	}

	delay_us(80);															      /* DHT11 response */
	delay_us(80);
	delay_us(25);

	return HAL_OK;
}
/**
 * @brief Reading humidity and temperature from DHT11.
 *
 * @param data Pointer to array to store results.
 * @return HAL_OK/HALL_ERROR If the reading is ok.
 */
HAL_StatusTypeDef DHT11_Read(uint8_t* data)
{
	__set_BASEPRI(LO_PREMPTION_PRIORITY << BASEPRI_REG_SHIFT);					/* Disable lowest priority interrupts (protection from communication) */
	if (DHT11_Start() == HAL_ERROR) {
		__set_BASEPRI(0);														/* Enable all interrupts */
		return HAL_ERROR;
	}

	else {
		uint8_t DHT11_data [DHT11_DATA_FRAME_SIZE];
		for (int i = 0; i < DHT11_DATA_FRAME_SIZE; i++) {
			DHT11_data[i] = DHT11_ReadByte();
		}
		__set_BASEPRI(0);													    /* Enable all interrupts */
		if (DHT11_data[0] + DHT11_data[1] + DHT11_data[2] + DHT11_data[3] == DHT11_data[4] && DHT11_data[4] != 0) {			/* Validating check sum */
			data[0] = DHT11_data[0];		/* Humidity */
			data[1] = DHT11_data[2];		/* Temperature */
			return HAL_OK;
		}
		else
			return HAL_ERROR;
	}
}
/**
  * @brief Read bit of DHT11 data.
  *
  * @return Bit.
  */
uint8_t DHT11_ReadBit()
{
	delay_us(77);

	if (HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin) == GPIO_PIN_RESET)
		return 0;
	else {
		delay_us(43);
		return 1;
	}
}
/**
  * @brief Read byte of DHT11 data.
  *
  * @return Byte.
  */
uint8_t DHT11_ReadByte()
{
	uint8_t byte = 0;
	uint8_t bit = 0;
	for (int i = 0; i < 8; i++) {
		bit = DHT11_ReadBit();
		byte |= (bit << (7 - i));
	}
	return byte;
}
/**
  * @brief DHT11 initialization.
  */
void DHT11_Init()
{
	HAL_Delay(1000);
}
