/*
 * lcd_gpio.c
 *
 *  Created on: 7 Oca 2021
 *      Author: Celil Yılmaz
 */

#include "lcd_gpio.h"
#include "gpio.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
 extern "C" {
#endif

static inline void LCD_GPIO_PulseEnable(LCD_t const * const lcd_ptr, uint8_t flag){

	LCD_GPIO_t const * const lcd_ptr_ = (LCD_GPIO_t const *)lcd_ptr;

		if(flag){

			HAL_GPIO_TogglePin(lcd_ptr_->ports.en_port, lcd_ptr_->pins.en_pin);

			HAL_Delay(0.05); // 50 microsecond
		}

	HAL_GPIO_TogglePin(lcd_ptr_->ports.en_port, lcd_ptr_->pins.en_pin);

		if(!flag)
			HAL_Delay(0.05); // 50 microsecond

}

static void LCD_GPIO_Write(LCD_t const * const lcd_ptr, uint8_t data){

	LCD_GPIO_t const * const lcd_ptr_ = (LCD_GPIO_t const *)lcd_ptr;

	HAL_GPIO_WritePin(lcd_ptr_->ports.rw_port, lcd_ptr_->pins.rw_pin, LCD_WRITE_PIN_STATE);

		#ifdef LCD_GPIO_4BIT_MODE

			for( int i = 0; i < 4; i++){

				HAL_GPIO_WritePin(lcd_ptr_->ports.data_ports[i], lcd_ptr_->pins.data_pins[i] , ((data >> i) & 0x01));


			}

		LCD_GPIO_PulseEnable(lcd_ptr, 1);

		#endif

		#ifdef LCD_GPIO_8BIT_MODE

			for( int i = 0; i < 8; i++){

				HAL_GPIO_WritePin(lcd_ptr_->ports.data_ports[i], lcd_ptr_->pins.data_pins[i] , ((data >> i) & 0x01));

			}

		LCD_GPIO_PulseEnable(lcd_ptr, 1);

		#endif

}


static uint8_t LCD_GPIO_CheckBusy(LCD_t const * const lcd_ptr){

	LCD_GPIO_t const * const lcd_ptr_ = (LCD_GPIO_t const *)lcd_ptr;

	HAL_GPIO_WritePin(lcd_ptr_->ports.rw_port, lcd_ptr_->pins.rw_pin, LCD_READ_PIN_STATE);
	HAL_GPIO_WritePin(lcd_ptr_->ports.rs_port, lcd_ptr_->pins.rs_pin, LCD_IR_PIN_STATE);

	GPIO_InitTypeDef GPIO_InitStruct1;

	GPIO_InitStruct1.Pin = LCD_D7_Pin;
	GPIO_InitStruct1.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct1.Pull = GPIO_NOPULL;
	GPIO_InitStruct1.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct1);

	uint8_t ret = 0;

		#ifdef LCD_GPIO_4BIT_MODE

			LCD_GPIO_PulseEnable(lcd_ptr, 0);

				if(HAL_GPIO_ReadPin(lcd_ptr_->ports.data_ports[3], lcd_ptr_->pins.data_pins[3]) == GPIO_PIN_SET )
					ret = 1;

				else
					ret = 0;

			LCD_GPIO_PulseEnable(lcd_ptr, 0);

			LCD_GPIO_PulseEnable(lcd_ptr, 1);

		#endif


		#ifdef LCD_GPIO_8BIT_MODE

			LCD_GPIO_PulseEnable(lcd_ptr, 0);

				if(HAL_GPIO_ReadPin(lcd_ptr_->ports.data_ports[7], lcd_ptr_->pins.data_pins[7]) == GPIO_PIN_SET )
					ret = 1;

				else
					ret = 0;

			LCD_GPIO_PulseEnable(lcd_ptr, 0);

			LCD_GPIO_PulseEnable(lcd_ptr, 1);

		#endif

	GPIO_InitStruct1.Mode = GPIO_MODE_OUTPUT_PP ;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct1);

	return (ret);

}

static void LCD_GPIO_WriteCommand(LCD_t const * const lcd_ptr, uint8_t command){

	LCD_GPIO_t const * const lcd_ptr_ = (LCD_GPIO_t const *)lcd_ptr;

	HAL_GPIO_WritePin(lcd_ptr_->ports.rs_port, lcd_ptr_->pins.rs_pin, LCD_IR_PIN_STATE);
	HAL_GPIO_WritePin(lcd_ptr_->ports.rw_port, lcd_ptr_->pins.rw_pin, LCD_WRITE_PIN_STATE);

		#ifdef LCD_GPIO_4BIT_MODE

			LCD_GPIO_Write(lcd_ptr, command >> 4);
			LCD_GPIO_Write(lcd_ptr, command & 0x0F);

			while( LCD_GPIO_CheckBusy(lcd_ptr) );

		#endif

		#ifdef LCD_GPIO_8BIT_MODE

			LCD_GPIO_Write(lcd_ptr, command);

			while( LCD_GPIO_CheckBusy(lcd_ptr) );

		#endif

}

static void LCD_GPIO_PrintString(LCD_t const * const lcd_ptr, const char * str){

	LCD_GPIO_t const * const lcd_ptr_ = (LCD_GPIO_t const *)lcd_ptr;

	HAL_GPIO_WritePin(lcd_ptr_->ports.rs_port, lcd_ptr_->pins.rs_pin, LCD_DR_PIN_STATE);
	HAL_GPIO_WritePin(lcd_ptr_->ports.rw_port, lcd_ptr_->pins.rw_pin, LCD_WRITE_PIN_STATE);

		#ifdef LCD_GPIO_4BIT_MODE

			for( int i = 0; i < strlen(str); i++){

				HAL_GPIO_WritePin(lcd_ptr_->ports.rs_port, lcd_ptr_->pins.rs_pin, LCD_DR_PIN_STATE);

				LCD_GPIO_Write(lcd_ptr, str[i] >> 4);
				LCD_GPIO_Write(lcd_ptr, str[i] & 0x0F);

				while( LCD_GPIO_CheckBusy(lcd_ptr) );

			}

		#else

			for( int i = 0; i < strlen(str); i++){

				HAL_GPIO_WritePin(lcd_ptr_->ports.rs_port, lcd_ptr_->pins.rs_pin, LCD_DR_PIN_STATE);

				LCD_GPIO_Write(lcd_ptr, str[i]);

				while( LCD_GPIO_CheckBusy(lcd_ptr) );

			}

		#endif

}

static void LCD_GPIO_PrintNumber(LCD_t const * const lcd_ptr, const int x, const uint8_t base){

	char buffer[12];

		if( base == BASE_DECIMAL )
			sprintf(buffer, "%d", x);

		else if( base == BASE_HEX )
			sprintf(buffer, "%X", x);

		else if( base == BASE_OCTAL )
			sprintf(buffer, "%o", x);

	LCD_GPIO_PrintString(lcd_ptr, buffer);

}

static void LCD_GPIO_PrintChar(LCD_t const * const lcd_ptr, const uint8_t chr){

	LCD_GPIO_t const * const lcd_ptr_ = (LCD_GPIO_t const *)lcd_ptr;

	HAL_GPIO_WritePin(lcd_ptr_->ports.rs_port, lcd_ptr_->pins.rs_pin, LCD_DR_PIN_STATE);
	HAL_GPIO_WritePin(lcd_ptr_->ports.rw_port, lcd_ptr_->pins.rw_pin, LCD_WRITE_PIN_STATE);

		#ifdef LCD_GPIO_4BIT_MODE

			LCD_GPIO_Write(lcd_ptr, chr >> 4);
			LCD_GPIO_Write(lcd_ptr, chr & 0x0F);

			while( LCD_GPIO_CheckBusy(lcd_ptr) );

		#endif

		#ifdef LCD_GPIO_8BIT_MODE

			LCD_GPIO_Write(lcd_ptr, chr);

			while( LCD_GPIO_CheckBusy(lcd_ptr) );

		#endif

}


static void LCD_GPIO_GoTo(LCD_t const * const lcd_ptr, const uint8_t row, const uint8_t column){

	const uint8_t LCD_16[] = {0x00, 0x40, 0x10, 0x50};
	const uint8_t LCD_20[] = {0x00, 0x40, 0x14, 0x54};

	assert_param(row < 4);
	assert_param(column <= 20);

		#ifdef LCD16
			LCD_GPIO_WriteCommand(lcd_ptr, LCD_SET_DDRAM_ADDR + LCD_16[row] + column);

		#endif

		#ifdef LCD20
			LCD_GPIO_WriteCommand(lcd_ptr, LCD_SET_DDRAM_ADDR + LCD_20[row] + column);

		#endif
}


static void LCD_GPIO_DefChar(LCD_t const * const lcd_ptr, const uint8_t num, const uint8_t * udg_num){

	assert_param(num < 8);

	LCD_GPIO_WriteCommand(lcd_ptr, LCD_SET_CGRAM_ADDR | ((num & 0x07) * 8));

		for( int i = 0; i < 8; i++ )
			LCD_GPIO_Write(lcd_ptr, udg_num[i]);


}

static inline void LCD_GPIO_ReturnHome(LCD_t const * const lcd_ptr){

	LCD_GPIO_WriteCommand(lcd_ptr, (uint8_t)LCD_RETURN);

}

static inline void LCD_GPIO_Clear(LCD_t const * const lcd_ptr){

	LCD_GPIO_WriteCommand(lcd_ptr, (uint8_t)LCD_CLEAR);

}


void LCD_GPIO_Constructor(LCD_GPIO_t * const lcd_ptr, LCD_GPIO_Ports_t ports, LCD_GPIO_Pins_t pins){

	static struct lcd_vtbl_ const vtbl = {

			&LCD_GPIO_WriteCommand,
			&LCD_GPIO_Write,
			&LCD_GPIO_PrintString,
			&LCD_GPIO_PrintNumber,
			&LCD_GPIO_PrintChar,
			&LCD_GPIO_GoTo,
			&LCD_GPIO_DefChar,
			&LCD_GPIO_ReturnHome,
			&LCD_GPIO_Clear
	};

	LCD_Constructor(&lcd_ptr->super);

	lcd_ptr->super.lcd_vptr = &vtbl;
	lcd_ptr->ports = ports;
	lcd_ptr->pins = pins;


	LCD_GPIO_WriteCommand(&lcd_ptr->super, FUNC_SET_BASE_CFG);
	LCD_GPIO_WriteCommand(&lcd_ptr->super, LCD_ENTRYMODESET_CFG);
	LCD_GPIO_WriteCommand(&lcd_ptr->super, LCD_DISPLAYCONTROL_CFG);
	LCD_GPIO_WriteCommand(&lcd_ptr->super, LCD_CUR_SHIFT_CFG);

	LCD_GPIO_Clear(&lcd_ptr->super);

}

#ifdef __cplusplus
}
#endif
