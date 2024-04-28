#ifndef _LCD_128x32_H_
#define _LCD_128x32_H_

#include "include/board.h"

/** Draw mode
  * NORMAl
  * XOR set pixel by xor the screen
  */
enum {NORMAL, XOR};

/** Bitmap
 */
typedef struct _Bitmap {
    uint32_t width;
    uint32_t height;
    uint32_t bytes_per_line;
    uint8_t* data;
} Bitmap;

int lcd_reset();

void lcd_set_contrast(unsigned int o);
unsigned int lcd_get_contrast(void);

int lcd_width();
int lcd_height();

unsigned int lcd_get_autoup();
void lcd_set_autoup(unsigned int update);

void lcd_setmode(int mode);
void lcd_invert(unsigned int o);

void locate(int x, int y);

void cls(void);

void line(int x0, int y0, int x1, int y1, int color);
void rect(int x0, int y0, int x1, int y1, int color);
void fillrect(int x0, int y0, int x1, int y1, int color);
void circle(int x0, int y0, int r, int color);
void fillcircle(int x, int y, int r, int color);

void bitmap(Bitmap *bm, int x, int y);

void lcd_putc(int value);
void lcd_puts(char *s);
void lcd_printf(const char* fmt, ...);

#endif
