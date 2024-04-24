// Terminal functions

#ifndef _TERM_IO_H_
#define _TERM_IO_H_

#include "../inc/stm32f4xx_hal_uart.h"

// ****************************************************************************
// Data types

// Terminal input mode (parameter of p_term_in and term_getch())
#define TERM_INPUT_DONT_WAIT      	0
#define TERM_INPUT_WAIT           	1

// ANSI color & effect code array
#define CL_DEFAULT					0
#define CL_BLACK					30
#define CL_RED 						31
#define CL_GREEN					32
#define CL_YELLOW					33
#define CL_BLUE						34
#define CL_MAGENTA					35
#define CL_CYAN						36
#define CL_WHITE					37

#define CL_BRIGHT					1
#define CL_UNDERLINE				4
#define CL_BLINK					5
#define CL_REVERSE					7

enum
{
	KC_UP = 256,
	KC_DOWN,
	KC_LEFT,
	KC_RIGHT,
	KC_HOME,
	KC_END,
	KC_PAGEUP,
	KC_PAGEDOWN,
	KC_ENTER,
	KC_TAB,
	KC_BACKSPACE,
	KC_SUPPR,
	KC_INSERT,
	KC_ESC,
	KC_F1,
	KC_F2,
	KC_F3,
	KC_F4,
	KC_F5,
	KC_F6,
	KC_F7,
	KC_F8,
	KC_F9,
	KC_F10,
	KC_F11,
	KC_F12,
	KC_EOT,
	KC_UNKNOWN
};

// ****************************************************************************
// Exported functions

// Terminal initialization
void term_init(UART *u, unsigned rows, unsigned cols);

// Terminal output functions
void term_clrscr();
void term_clreol();
void term_color(unsigned color, unsigned effect);


void term_gotoxy(unsigned x, unsigned y);
void term_up(unsigned delta);
void term_down(unsigned delta);
void term_left(unsigned delta);
void term_right(unsigned delta);

unsigned term_get_rows();
unsigned term_get_cols();
unsigned term_get_cx();
unsigned term_get_cy();

int term_getc(void);
void term_putc(char ch );
void term_puts(const char* str);
void term_printf(const char* fmt, ...);

int readline(char *prompt_str, char *buf, unsigned maxlength);

#endif /* #ifndef __TERM_IO_H__ */
