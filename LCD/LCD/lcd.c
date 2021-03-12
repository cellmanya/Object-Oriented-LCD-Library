/*
 * lcd.c
 *
 *  Created on: 5 Oca 2021
 *      Author: Celil Yılmaz
 */

#include "lcd.h"

#ifdef __cplusplus
 extern "C" {
#endif

							/* INLINE FUNCTION DEFINITIONS */

inline void LCD_WriteCommand(LCD_t const * const lcd_ptr, uint8_t command){

	return (*lcd_ptr->lcd_vptr->LCD_WriteCommand)(lcd_ptr, command);
}

inline void LCD_Write(LCD_t const * const lcd_ptr, uint8_t data){

	return (*lcd_ptr->lcd_vptr->LCD_Write)(lcd_ptr, data);
}

inline void LCD_PrintString(LCD_t const * const lcd_ptr, const char * str){

	return (*lcd_ptr->lcd_vptr->LCD_PrintString)(lcd_ptr, str);
}

inline void LCD_PrintNumber(LCD_t const * const lcd_ptr, const int x, const uint8_t base){

	return (*lcd_ptr->lcd_vptr->LCD_PrintNumber)(lcd_ptr, x, base);
}

inline void LCD_PrintChar(LCD_t const * const lcd_ptr, const uint8_t chr){

	return (*lcd_ptr->lcd_vptr->LCD_PrintChar)(lcd_ptr, chr);
}

inline void LCD_GoTo(LCD_t const * const lcd_ptr, const uint8_t row, const uint8_t column){

	return (*lcd_ptr->lcd_vptr->LCD_GoTo)(lcd_ptr, row, column);
}

inline void LCD_DefChar(LCD_t const * const lcd_ptr, const uint8_t num, const uint8_t * udg_num){

	return (*lcd_ptr->lcd_vptr->LCD_DefChar)(lcd_ptr, num, udg_num);
}

inline void LCD_ReturnHome(LCD_t const * const lcd_ptr){

	return (*lcd_ptr->lcd_vptr->LCD_ReturnHome)(lcd_ptr);
}

inline void LCD_Clear(LCD_t const * const lcd_ptr){

	return (*lcd_ptr->lcd_vptr->LCD_Clear)(lcd_ptr);
}

void LCD_Constructor(LCD_t * const lcd_ptr){


	static struct lcd_vtbl_ const vtbl = {

			&LCD_WriteCommand,
			&LCD_Write,
			&LCD_PrintString,
			&LCD_PrintNumber,
			&LCD_PrintChar,
			&LCD_GoTo,
			&LCD_DefChar,
			&LCD_ReturnHome,
			&LCD_Clear
	};

	lcd_ptr->lcd_vptr = &vtbl;

}

#ifdef __cplusplus
}
#endif
