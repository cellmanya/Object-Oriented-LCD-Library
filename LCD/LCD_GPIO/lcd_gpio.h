/*
 * lcd_gpio.h
 *
 *  Created on: 7 Oca 2021
 *      Author: Celil Yılmaz
 */

#ifndef LCD_GPIO_H_
#define LCD_GPIO_H_


#include <stdint.h>
#include "lcd.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define LCD_ENTRYMODESET_CFG		(LCD_ENTRYMODESET_BASE | SELECT_INC)
#define LCD_DISPLAYCONTROL_CFG 		(LCD_DISPLAYCONTROL_BASE | SELECT_D)
#define LCD_CUR_SHIFT_CFG 			(LCD_CUR_SHIFT_BASE | SELECT_SHIFT_RIGHT)
#define FUNC_SET_BASE_CFG			(FUNC_SET_BASE | SELECT_2_LINES | SELECT_8_DOTS | SELECT_8_BITS)

//#define LCD_GPIO_4BIT_MODE
#define LCD_GPIO_8BIT_MODE
#define LCD16
//#define LCD20

typedef struct{

	GPIO_TypeDef * rs_port;
	GPIO_TypeDef * rw_port;
	GPIO_TypeDef * en_port;
	GPIO_TypeDef * data_ports[8];

}LCD_GPIO_Ports_t;

typedef struct{

	uint16_t rs_pin;
	uint16_t rw_pin;
	uint16_t en_pin;
	uint16_t data_pins[8];

}LCD_GPIO_Pins_t;


typedef struct {

	LCD_t super; // Inheriting the superclass LCD
	LCD_GPIO_Ports_t ports;
	LCD_GPIO_Pins_t pins;

}LCD_GPIO_t;


/* constructor prototype */
void LCD_GPIO_Constructor(LCD_GPIO_t * const lcd_ptr, LCD_GPIO_Ports_t ports, LCD_GPIO_Pins_t pins);

#ifdef __cplusplus
}
#endif

#endif /* LCD_GPIO_H_ */
