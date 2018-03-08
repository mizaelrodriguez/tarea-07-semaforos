/*
 * Copyright (c) 2017, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    MK64FN1M0xxx12_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "led.h"
#include "fsl_pit.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

SemaphoreHandle_t g_led_green;
SemaphoreHandle_t g_led_blue;

void PORTC_IRQHandler()
{
	PORT_ClearPinsInterruptFlags(PORTC, 1 << 6);

}

void PORTA_IRQHandler()
{
	BaseType_t xHigherPriorityTaskWoken;
	PORT_ClearPinsInterruptFlags(PORTA, 1 << 4);
	xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR( g_led_green, &xHigherPriorityTaskWoken );
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );


}

void led_green_task(void * pvParameters)
{
	for(;;)
	{
		led_green();
		xSemaphoreTake(g_led_green,portMAX_DELAY);
		led_off();
	}
}

void led_blue_task(void * pvParameters)
{
	for(;;)
	{
		xSemaphoreGive(g_led_green);
		led_blue();
		led_off();

	}
}
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

	/*Habilitar el reloj SCG*/
	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortE);
	CLOCK_EnableClock(kCLOCK_PortC);

	/*Configurar el puerto para encender un LED*/
	/* Input pin PORT configuration */
	port_pin_config_t config_led_blue = { kPORT_PullDisable, /*Resistencias deshabilitadas*/
	kPORT_SlowSlewRate, /*SlewRate menor velocidad*/
	kPORT_PassiveFilterEnable, /*Filtro habilitado*/
	kPORT_OpenDrainDisable, /**/
	kPORT_LowDriveStrength, /**/
	kPORT_MuxAsGpio, /*Modo GPIO*/
	kPORT_UnlockRegister }; /**/

	/*Configurar el puerto para encender un LED*/
	/* Input pin PORT configuration */
	port_pin_config_t config_led_red = { kPORT_PullDisable, /*Resistencias deshabilitadas*/
	kPORT_SlowSlewRate, /*SlewRate menor velocidad*/
	kPORT_PassiveFilterEnable, /*Filtro habilitado*/
	kPORT_OpenDrainDisable, /**/
	kPORT_LowDriveStrength, /**/
	kPORT_MuxAsGpio, /*Modo GPIO*/
	kPORT_UnlockRegister }; /**/

	/*Configurar el puerto para encender un LED*/
	/* Input pin PORT configuration */
	port_pin_config_t config_led_green = { kPORT_PullDisable, /*Resistencias deshabilitadas*/
	kPORT_SlowSlewRate, /*SlewRate menor velocidad*/
	kPORT_PassiveFilterEnable, /*Filtro habilitado*/
	kPORT_OpenDrainDisable, /**/
	kPORT_LowDriveStrength, /**/
	kPORT_MuxAsGpio, /*Modo GPIO*/
	kPORT_UnlockRegister }; /**/

	/* Input pin PORT configuration */
	port_pin_config_t config_switch_sw3 = { kPORT_PullDisable,
			kPORT_SlowSlewRate, kPORT_PassiveFilterEnable,
			kPORT_OpenDrainDisable, kPORT_LowDriveStrength, kPORT_MuxAsGpio,
			kPORT_UnlockRegister };

	/* Input pin PORT configuration */
	port_pin_config_t config_switch_sw2 = { kPORT_PullDisable,
			kPORT_SlowSlewRate, kPORT_PassiveFilterEnable,
			kPORT_OpenDrainDisable, kPORT_LowDriveStrength, kPORT_MuxAsGpio,
			kPORT_UnlockRegister };

	PORT_SetPinInterruptConfig(PORTA, 4, kPORT_InterruptFallingEdge);
	PORT_SetPinInterruptConfig(PORTC, 6, kPORT_InterruptFallingEdge);
	/* Sets the configuration */
	PORT_SetPinConfig(PORTB, 21, &config_led_blue);
	PORT_SetPinConfig(PORTB, 22, &config_led_red);
	PORT_SetPinConfig(PORTE, 26, &config_led_green);
	PORT_SetPinConfig(PORTA, 4, &config_switch_sw3);
	PORT_SetPinConfig(PORTC, 6, &config_switch_sw2);

	NVIC_EnableIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTC_IRQn);

	/* Output pin configuration */
	gpio_pin_config_t led_config_blue = { kGPIO_DigitalOutput, 1 };
	gpio_pin_config_t led_config_red = { kGPIO_DigitalOutput, 1 };
	gpio_pin_config_t led_config_green = { kGPIO_DigitalOutput, 1 };
	gpio_pin_config_t switch_config_sw3 = { kGPIO_DigitalInput, 0 };
	gpio_pin_config_t switch_config_sw2 = { kGPIO_DigitalInput, 0 };

	/* Sets the configuration */
	GPIO_PinInit(GPIOB, 21, &led_config_blue);
	GPIO_PinInit(GPIOB, 22, &led_config_red);
	GPIO_PinInit(GPIOE, 26, &led_config_green);
	GPIO_PinInit(GPIOA, 4, &switch_config_sw3);
	GPIO_PinInit(GPIOC, 6, &switch_config_sw2);


	xTaskCreate(led_green_task, "sw2", configMINIMAL_STACK_SIZE, 0, configMAX_PRIORITIES-1, 0);
	xTaskCreate(led_blue_task, "sw3", configMINIMAL_STACK_SIZE, 0, configMAX_PRIORITIES-2, 0);

	vTaskStartScheduler();

    /* Enter an infinite loop, just incrementing a counter. */
    for(;;)
    {

    }
    return 0 ;
}
