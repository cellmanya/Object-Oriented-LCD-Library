/*
 * lcd_i2c.h
 *
 *  Created on: 6 Oca 2021
 *      Author: Celil Yılmaz
 */

#ifndef LCD_I2C_H_
#define LCD_I2C_H_


#include <stdint.h>
#include "lcd.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define LCD_BIT_RS                  	((uint8_t)0x01U)
#define LCD_BIT_RW                 		((uint8_t)0x02U)
#define LCD_BIT_EN                 		((uint8_t)0x04U)
#define LCD_BIT_BACKLIGHT_ON       		((uint8_t)0x08U)
#define LCD_BIT_BACKLIGHT_OFF      		((uint8_t)0x00U)
#define SLAVE_ADDR						((uint8_t)0x4EU) // Default Address

#define LCD_I2C_ENTRYMODESET_CFG		(LCD_ENTRYMODESET_BASE | SELECT_INC)
#define LCD_I2C_DISPLAYCONTROL_CFG 		(LCD_DISPLAYCONTROL_BASE | SELECT_D)
#define LCD_I2C_CUR_SHIFT_CFG 			(LCD_CUR_SHIFT_BASE | SELECT_SHIFT_RIGHT)
#define FUNC_I2C_SET_BASE_CFG			(FUNC_SET_BASE | SELECT_2_LINES | SELECT_8_DOTS)

#define LCD16
//#define LCD20

typedef struct {

	LCD_t super; // Inheriting the superclass LCD
	I2C_HandleTypeDef * i2c_handle;
	uint16_t slave_address;

}LCD_I2C_t;


/* constructor prototype */
void LCD_I2C_Constructor(LCD_I2C_t * const lcd_ptr,
						 I2C_HandleTypeDef * i2c_handle,
						 const uint16_t slave_address);


#ifdef __cplusplus
}
#endif

#endif /* LCD_I2C_H_ */
