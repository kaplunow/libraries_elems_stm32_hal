/*******************************************************************************
 *  This is header file for LED RGB helper functions. LED is controlled by PWM.
 *  Software is designed for STM32 HAL library.
 *******************************************************************************
 * @file rgb.h
 * @author Kaplunow
 * @date Dec 2021
 * @brief File containing declaraionts for RGB LED
 * @ver 0.1
 *******************************************************************************/

#ifndef INC_RGB_H_
#define INC_RGB_H_


/* Typedef -----------------------------------------------------------*/
typedef struct { double red; double green; double blue; } rgb;
typedef struct { double hue; double saturation; double value; } hsv;

/* Function prototypes -----------------------------------------------*/
void set_rgb(hsv color_hsv);


#endif /* INC_RGB_H_ */
