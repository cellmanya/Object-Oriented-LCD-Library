/*
 * lcd_i2c.c
 *
 *  Created on: 6 Oca 2021
 *      Author: Celil Yılmaz
 */
#include "lcd_i2c.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
 extern "C" {
#endif


static void LCD_I2C_WriteCommand(LCD_t const * const lcd_ptr, uint8_t command){

	LCD_I2C_t const * const lcd_ptr_ = (LCD_I2C_t const *)lcd_ptr;

	uint8_t cmd_arr[4];

	uint8_t cmd_L, cmd_H;

	while(((HAL_StatusTypeDef)HAL_I2C_IsDeviceReady( lcd_ptr_->i2c_handle, lcd_ptr_->slave_address, 1, HAL_MAX_DELAY)) != HAL_OK);

		cmd_L = ((command<<4) & 0xF0);
		cmd_H = (command & 0xF0);

		cmd_arr[0] =  LCD_BIT_EN | (cmd_H);
		cmd_arr[1] =  LCD_BACKLIGHT_ON | (cmd_H);
		cmd_arr[2] =  LCD_BIT_EN | (cmd_L);
		cmd_arr[3] =  LCD_BACKLIGHT_ON | (cmd_L);

	HAL_I2C_Master_Transmit( lcd_ptr_->i2c_handle, lcd_ptr_->slave_address, (uint8_t *)cmd_arr, sizeof cmd_arr, HAL_MAX_DELAY );

}


static void LCD_I2C_Write(LCD_t const * const lcd_ptr, uint8_t data){


	LCD_I2C_t const * const lcd_ptr_ = (LCD_I2C_t const *)lcd_ptr;

	uint8_t data_arr[4];

	uint8_t data_L, data_H;

	while(((HAL_StatusTypeDef)HAL_I2C_IsDeviceReady( lcd_ptr_->i2c_handle, lcd_ptr_->slave_address, 1, HAL_MAX_DELAY)) != HAL_OK);

		data_L = ((data<<4) & 0xF0);
		data_H = (data & 0xF0);

		data_arr[0] = LCD_BIT_RS | LCD_BIT_EN | (data_H);
		data_arr[1] = LCD_BIT_RS | LCD_BACKLIGHT_ON | (data_H);
		data_arr[2] = LCD_BIT_RS | LCD_BIT_EN | (data_L);
		data_arr[3] = LCD_BIT_RS | LCD_BACKLIGHT_ON | (data_L);

	HAL_I2C_Master_Transmit( lcd_ptr_->i2c_handle, lcd_ptr_->slave_address, (uint8_t *)data_arr, sizeof data_arr, HAL_MAX_DELAY );

}


static inline void LCD_I2C_PrintString(LCD_t const * const lcd_ptr, const char * str){

	for( int i = 0; i < strlen(str); i++ ){

			LCD_I2C_Write(lcd_ptr, str[i]);
	}

}


static void LCD_I2C_PrintNumber(LCD_t const * const lcd_ptr, const int x, const uint8_t base){

	char buffer[12];

		if( base == BASE_DECIMAL )
			sprintf(buffer, "%d", x);

		else if( base == BASE_HEX )
			sprintf(buffer, "%X", x);

		else if( base == BASE_OCTAL )
			sprintf(buffer, "%o", x);

	LCD_I2C_PrintString(lcd_ptr, buffer);

}


static inline void LCD_I2C_PrintChar(LCD_t const * const lcd_ptr, const uint8_t chr){

	LCD_I2C_Write(lcd_ptr, chr);

}

static void LCD_I2C_GoTo(LCD_t const * const lcd_ptr, const uint8_t row, const uint8_t column){

	const uint8_t LCD_16[] = {0x00, 0x40, 0x10, 0x50};
	const uint8_t LCD_20[] = {0x00, 0x40, 0x14, 0x54};

	assert_param(row < 4);
	assert_param(column <= 20);

		#ifdef LCD16

			LCD_I2C_WriteCommand(lcd_ptr, LCD_SET_DDRAM_ADDR + LCD_16[row] + column);

		#endif

		#ifdef LCD20

			LCD_I2C_WriteCommand(lcd_ptr, LCD_SET_DDRAM_ADDR + LCD_20[row] + column);

		#endif

}

static void LCD_I2C_DefChar(LCD_t const * const lcd_ptr, const uint8_t num, const uint8_t * udg_num){

	assert_param(num < 8);

	LCD_I2C_WriteCommand(lcd_ptr, LCD_SET_CGRAM_ADDR | ((num & 0x07) * 8));

		for( int i = 0; i < 8; i++ )
			LCD_I2C_Write(lcd_ptr, udg_num[i]);


}

static inline void LCD_I2C_ReturnHome(LCD_t const * const lcd_ptr){

	LCD_I2C_WriteCommand(lcd_ptr, (uint8_t)LCD_RETURN);

}


static inline void LCD_I2C_Clear(LCD_t const * const lcd_ptr){

	LCD_I2C_WriteCommand(lcd_ptr, (uint8_t)LCD_CLEAR);
}


void LCD_I2C_Constructor(LCD_I2C_t * const lcd_ptr, I2C_HandleTypeDef * i2c_handle, const uint16_t slave_address){

	static struct lcd_vtbl_ const vtbl = {

			&LCD_I2C_WriteCommand,
			&LCD_I2C_Write,
			&LCD_I2C_PrintString,
			&LCD_I2C_PrintNumber,
			&LCD_I2C_PrintChar,
			&LCD_I2C_GoTo,
			&LCD_I2C_DefChar,
			&LCD_I2C_ReturnHome,
			&LCD_I2C_Clear
	};

	LCD_Constructor(&lcd_ptr->super);

	lcd_ptr->super.lcd_vptr = &vtbl;
	lcd_ptr->i2c_handle = i2c_handle;
	lcd_ptr->slave_address = slave_address;

	/* Initialization Process of the LCD referring to its datasheet */

	HAL_Delay(50);  // wait for >40ms
	LCD_I2C_WriteCommand(&lcd_ptr->super, 0x30);
	HAL_Delay(5);  // wait for >4.1ms
	LCD_I2C_WriteCommand(&lcd_ptr->super, 0x30);
	HAL_Delay(1);  // wait for >100us
	LCD_I2C_WriteCommand(&lcd_ptr->super, 0x30);
	HAL_Delay(10);
	LCD_I2C_WriteCommand(&lcd_ptr->super, 0x20);  // 4bit mode
	HAL_Delay(10);

	/* Configuration setups */
	LCD_I2C_WriteCommand(&lcd_ptr->super, FUNC_I2C_SET_BASE_CFG);
	HAL_Delay(1);
	LCD_I2C_WriteCommand(&lcd_ptr->super, LCD_I2C_ENTRYMODESET_CFG);
	HAL_Delay(1);
	LCD_I2C_WriteCommand(&lcd_ptr->super, LCD_I2C_DISPLAYCONTROL_CFG);
	HAL_Delay(1);
	LCD_I2C_WriteCommand(&lcd_ptr->super, LCD_I2C_CUR_SHIFT_CFG);
	HAL_Delay(1);

	LCD_I2C_ReturnHome(&lcd_ptr->super);
	HAL_Delay(1);
	LCD_I2C_Clear(&lcd_ptr->super);
	HAL_Delay(1);

}

#ifdef __cplusplus
}
#endif
