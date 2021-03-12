/*
 * lcd.h
 *
 *  Created on: 5 Oca 2021
 *      Author: Celil Yılmaz
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"

#ifdef __cplusplus
 extern "C" {
#endif


#define LCD_IR_PIN_STATE						((uint8_t)0x00)
#define LCD_DR_PIN_STATE						((uint8_t)0x01)
#define LCD_WRITE_PIN_STATE						((uint8_t)0x00)
#define LCD_READ_PIN_STATE						((uint8_t)0x01)

#define LCD_CLEAR 								((uint8_t)0x01)
#define LCD_RETURN								((uint8_t)0x02)

#define LCD_BACKLIGHT_ON        				((uint8_t)0x08)
#define LCD_BACKLIGHT_OFF       				((uint8_t)0x00)

#define LCD_ENTRYMODESET_BASE					((uint8_t)0x04)
#define SELECT_INC								((uint8_t)0x02)
#define SELECT_DEC								((uint8_t)0x00)
#define SELECT_S								((uint8_t)0x01)

#define LCD_DISPLAYCONTROL_BASE					((uint8_t)0x08)
#define SELECT_D								((uint8_t)0x04) // D -> display on/off
#define SELECT_C								((uint8_t)0x02) // C -> cursor on/off
#define SELECT_B								((uint8_t)0x01) // B -> blinking on/off

#define LCD_CUR_SHIFT_BASE						((uint8_t)0x10)
#define SELECT_DISPLAY_SHIFT					((uint8_t)0x08) // SC = 1 -> Display Shift, SC = 0 -> Cursor Move
#define SELECT_CURSOR_MOVE						((uint8_t)0x00) // SC = 1 -> Display Shift, SC = 0 -> Cursor Move
#define SELECT_SHIFT_RIGHT						((uint8_t)0x04) // RL = 1 -> Shift to the Right, RL = 0 -> Shift to the Left
#define SELECT_SHIFT_LEFT						((uint8_t)0x00) // RL = 1 -> Shift to the Right, RL = 0 -> Shift to the Left


#define FUNC_SET_BASE							((uint8_t)0x20)
#define SELECT_8_BITS							((uint8_t)0x10) // DL = 1 -> 8-bits, DL = 0 -> 4-bits
#define SELECT_4_BITS							((uint8_t)0x00) // DL = 1 -> 8-bits, DL = 0 -> 4-bits
#define SELECT_2_LINES							((uint8_t)0x08) // N = 1 -> 2 lines, N = 0 -> 1 line
#define SELECT_1_LINE							((uint8_t)0x00) // N = 1 -> 2 lines, N = 0 -> 1 line
#define SELECT_10_DOTS							((uint8_t)0x04) // F = 1 -> 5*10 dots, F = 0 -> 5*8 dots
#define SELECT_8_DOTS							((uint8_t)0x00) // F = 1 -> 5*10 dots, F = 0 -> 5*8 dots

#define LCD_SET_CGRAM_ADDR						((uint8_t)0x40)
#define LCD_SET_DDRAM_ADDR						((uint8_t)0x80)

#define BASE_DECIMAL							((uint8_t)10)
#define BASE_HEX								((uint8_t)16)
#define BASE_OCTAL								((uint8_t)8)

typedef struct lcd_vtbl_ lcd_vtbl;	/* forward declaration */

/* SUPER CLASS LCD */

typedef struct {

	lcd_vtbl const * lcd_vptr;

}LCD_t;

/* virtual table */

struct lcd_vtbl_{

	void (*LCD_WriteCommand)(LCD_t const * const lcd_ptr, uint8_t command);
	void (*LCD_Write)(LCD_t const * const lcd_ptr, uint8_t data);
	void (*LCD_PrintString)(LCD_t const * const lcd_ptr, const char * str);
	void (*LCD_PrintNumber)(LCD_t const * const lcd_ptr, const int x, const uint8_t base);
	void (*LCD_PrintChar)(LCD_t const * const lcd_ptr, const uint8_t chr);
	void (*LCD_GoTo)(LCD_t const * const lcd_ptr, const uint8_t row, const uint8_t column);
	void (*LCD_DefChar)(LCD_t const * const lcd_ptr, const uint8_t num, const uint8_t * udg_num);
	void (*LCD_ReturnHome)(LCD_t const * const lcd_ptr);
	void (*LCD_Clear)(LCD_t const * const lcd_ptr);

};

// LCD CONSTRUCTOR

void LCD_Constructor(LCD_t * const lcd_ptr);

void LCD_WriteCommand(LCD_t const * const lcd_ptr, uint8_t command);
void LCD_Write(LCD_t const * const lcd_ptr, uint8_t data);
void LCD_PrintString(LCD_t const * const lcd_ptr, const char * str);
void LCD_PrintNumber(LCD_t const * const lcd_ptr, const int x, const uint8_t base);
void LCD_PrintChar(LCD_t const * const lcd_ptr, const uint8_t chr);
void LCD_GoTo(LCD_t const * const lcd_ptr, const uint8_t row, const uint8_t column);
void LCD_DefChar(LCD_t const * const lcd_ptr, const uint8_t num, const uint8_t * udg_num);
void LCD_ReturnHome(LCD_t const * const lcd_ptr);
void LCD_Clear(LCD_t const * const lcd_ptr);


#ifdef __cplusplus
}
#endif

#endif /* LCD_H_ */
