/*
 * led.c
 *
 *  Created on: Jan 31, 2018
 *      Author: manuel
 */

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "led.h"

/*
 * funcion que prende verde
*/
void led_green()
{
	/*Escribimos el led segun el valor de state*/
	GPIO_WritePinOutput (GPIOB, 21, 1);

	/*Escribimos el led segun el valor de state*/
	GPIO_WritePinOutput (GPIOB, 22, 1);

	/*Escribimos el led segun el valor de state*/
	GPIO_WritePinOutput (GPIOE, 26, 0);
}

/*
 * funcion que prende rojo
*/
void led_red()
{
	/*Escribimos el led segun el valor de state*/
	GPIO_WritePinOutput (GPIOB, 21, 1);

	/*Escribimos el led segun el valor de state*/
	GPIO_WritePinOutput (GPIOB, 22, 0);

	/*Escribimos el led segun el valor de state*/
	GPIO_WritePinOutput (GPIOE, 26, 1);
}

/*
 * funcion que prende azul
*/
void led_blue()
{
	/*Escribimos el led segun el valor de state*/
	GPIO_WritePinOutput (GPIOB, 21, 0);

	/*Escribimos el led segun el valor de state*/
	GPIO_WritePinOutput (GPIOB, 22, 1);

	/*Escribimos el led segun el valor de state*/
	GPIO_WritePinOutput (GPIOE, 26, 1);
}

void led_off()
{
	/*Escribimos el led segun el valor de state*/
	GPIO_WritePinOutput (GPIOB, 21, 1);

	/*Escribimos el led segun el valor de state*/
	GPIO_WritePinOutput (GPIOB, 22, 1);

	/*Escribimos el led segun el valor de state*/
	GPIO_WritePinOutput (GPIOE, 26, 1);
}
