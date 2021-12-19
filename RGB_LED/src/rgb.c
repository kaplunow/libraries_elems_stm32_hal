/*******************************************************************************
 *  This is source file for LED RGB helper functions. LED is controlled by PWM.
 *  Software is designed for STM32 HAL library.
 *******************************************************************************
 * @file rgb.c
 * @author Kaplunow
 * @date Dec 2021
 * @brief File containing definitions for RGB LED
 * @ver 0.1
 *******************************************************************************/


/* Private defines -----------------------------------------------------------*/
#define RED_CHANNEL		TIM_CHANNEL_1			/* Change defines for your TIM configuration */
#define GREEN_CHANNEL	TIM_CHANNEL_2
#define BLUE_CHANNEL	TIM_CHANNEL_3
#define TIM 			htim1
#define TIM_PERIOD		999

/* Private includes ----------------------------------------------------------*/
#include "rgb.h"
#include "stm32f1xx_hal.h"
#include <math.h>

/* Global variables ----------------------------------------------------------*/
extern TIM_HandleTypeDef TIM;

/* Private prototypes ---------------------------------------------------------*/
static uint16_t calc_led_brightness(float x);
static rgb hsv2rgb(hsv in);


/**
 * @brief LED brightness correction for human eye sensitivity. It's designed for x = 0-255.
 *
 * @param x Argument of function.
 * @return Corrected value.
 */
uint16_t calc_led_brightness(float x)
{
	if (x < 1e-3)
		return 0;

	const float k = 0.039f;
	const float x0 = 180.0f;
	const float L = TIM_PERIOD;

	return (uint16_t)(L / (1.0f + exp(-k * (x - x0))));		/* Logistic function */
}
/**
 * @brief Transform HSV color structure into RGB color structure
 *
 * @param in HSV color structure.
 * @return RGB color structure.
 */
rgb hsv2rgb(hsv in)
{
	rgb out;

	if ( (in.hue<0 || in.hue>360) || (in.saturation<0 || in.saturation>1) || (in.value<0 || in.value>1) ) {
		in.hue = 0;
		in.saturation = 0;
		in.value = 0;
	}
	else {
		double C = in.value * in.saturation;
		double X = C * (1 - fabs(fmod(in.hue/60, 2) - 1));
		double m = in.value - C;
		switch ((int)(in.hue/60)) {
		case 0:
			out.red = C; out.green = X; out.blue = 0; break;
		case 1:
			out.red = X; out.green = C; out.blue = 0; break;
		case 2:
			out.red = 0; out.green = C; out.blue = X; break;
		case 3:
			out.red = 0; out.green = X; out.blue = C; break;
		case 4:
			out.red = X; out.green = 0; out.blue = C; break;
		case 5:
			out.red = C; out.green = 0; out.blue = X; break;
		}
		out.red = (out.red+m)*255, out.green = (out.green+m)*255, out.blue = (out.blue+m)*255;
	}

	return out;
}
/**
 * @brief Set RGB LED.
 *
 * @param color_hsv HSV color structure.
 */
void set_rgb(hsv color_hsv)
{
	rgb color_rgb = hsv2rgb(color_hsv);
    __HAL_TIM_SET_COMPARE(&TIM, RED_CHANNEL, calc_led_brightness(color_rgb.red));
    __HAL_TIM_SET_COMPARE(&TIM, GREEN_CHANNEL, calc_led_brightness(color_rgb.green));
    __HAL_TIM_SET_COMPARE(&TIM, BLUE_CHANNEL, calc_led_brightness(color_rgb.blue));
}
