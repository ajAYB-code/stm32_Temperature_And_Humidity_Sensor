#include <string.h>
#include <stdarg.h>

#include "lcd_128x32.h"
#include "small_7.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_spi.h"
#include "util.h"
 
#define A0_SET		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1)//io_set(_GPIOA, PIN_8)
#define A0_CLR		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0)//io_clear(_GPIOA, PIN_8)

#define CS_SET		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1)//io_set(_GPIOB, PIN_6)
#define CS_CLR		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0)

#define RESET_SET	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1)//io_set(_GPIOA, PIN_6)
#define RESET_CLR	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0)//io_clear(_GPIOA, PIN_6)

#define SCR_WIDTH	128
#define SCR_HEIGHT	32

static unsigned char* font = (unsigned char *)Small_7;
static int char_x = 0;
static int char_y = 0;

static unsigned int contrast;
static int draw_mode = NORMAL;

static uint8_t  buffer[SCR_WIDTH*SCR_HEIGHT/8];		// 128x32 pixels / 8 pixels/byte = 512 bytes

static unsigned int auto_up = 1;

SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim2;

/**************************************************************************
 * Low level access to the LCD controller
 **************************************************************************/
// write command to lcd controller
static void wr_cmd(uint8_t cmd)
{
	A0_CLR;
	CS_CLR;
	HAL_SPI_Write_Byte(&hspi1, cmd);//spi_write_byte(_SPI1, cmd);
	CS_SET;
}

// write data to lcd controller
static void wr_dat(uint8_t dat)
{
	A0_SET;
	CS_CLR;
	HAL_SPI_Write_Byte(&hspi1, dat);//spi_write_byte(_SPI1, dat);
	CS_SET;
}

static void wr_data(uint8_t *dat, uint32_t n)
{
	A0_SET;
	CS_CLR;
	HAL_SPI_Transmit_IT(&hspi1, dat, n);//spi_write(_SPI1, dat, n);
	CS_SET;
}

static void lcd_update(void)
{
	// 4 128byte pages = 512bytes
    for (int i=0;i<4;i++) {
	    //page i
	    wr_cmd(0x00);      // set column low nibble 0
	    wr_cmd(0x10);      // set column hi  nibble 0
	    wr_cmd(0xB0+i);    // set page address  i
	
		wr_data(buffer+(i<<7), 128);
    }
}

/**************************************************************************
 * LCD reset, control and status interface
 **************************************************************************/
int lcd_reset()
{
	// in include/config.h
	// SPI pins : LCD_SCK   --> D13 --> PA5 : SPI1_SCK
	//            LCD_MOSI  --> D11 --> PA7 : SPI1_MOSI
	//spi_master_init(_SPI1, 20000000, SPI_MODE3);
	hspi1.Instance = SPI1;
	hspi1.Init.max_rate=20000000;
	hspi1.Init.cfg=SPI_MODE3;
	HAL_SPI_Init(&hspi1);

	RESET_SET;
	
	// Display reset
	A0_CLR;
	CS_SET;
	RESET_CLR;

	 htim2.Instance = TIM2;
	 HAL_TIM_wait_us(&htim2, 50);//timer_wait_us(_TIM5, 50, NULL);
	RESET_SET;
	HAL_TIM_wait_ms(&htim2, 5);//timer_wait_ms(_TIM5, 5, NULL);
	
	// Init sequence
//	uint8_t buf[]={0xAE, 0xA2, 0xA0, 0xC8, 0x22, 0x2F, 0x40, 0xAF, 0x81, 0x17, 0xA6};
//	lcd_cmds(buf, 11);
	
    wr_cmd(0xAE);   //  display off
    wr_cmd(0xA2);   //  bias voltage

    wr_cmd(0xA0);
    wr_cmd(0xC8);   //  colum normal

    wr_cmd(0x22);   //  voltage resistor ratio
    wr_cmd(0x2F);   //  power on
    //wr_cmd(0xA4);   //  LCD display ram
    wr_cmd(0x40);   // start line = 0
    wr_cmd(0xAF);     // display ON

    wr_cmd(0x81);   //  set contrast
    wr_cmd(0x17);   //  set contrast

    wr_cmd(0xA6);     // display normal
    
    return 0;
}

void lcd_set_contrast(unsigned int o)
{
    contrast = o;
    wr_cmd(0x81);      //  set volume
    wr_cmd(o & 0x3F);
}

unsigned int lcd_get_contrast(void)
{
    return(contrast);
}

int lcd_width()
{
    return SCR_WIDTH;
}

int lcd_height()
{
    return SCR_HEIGHT;
}

void lcd_invert(unsigned int o)
{
    if(o == 0) wr_cmd(0xA6);
    else wr_cmd(0xA7);
}

void lcd_setmode(int mode)
{
    draw_mode = mode;
}

unsigned int lcd_get_autoup()
{
	return auto_up;
}

void lcd_set_autoup(unsigned int update)
{
	auto_up = update;
	if (auto_up) lcd_update();
}

/**************************************************************************
 * LCD drawing interface
 **************************************************************************/
static void pixel(int x, int y, int color)
{
    // first check parameter
    if(x >= SCR_WIDTH || y >= SCR_HEIGHT || x < 0 || y < 0) return;

    if(draw_mode == NORMAL) {
        if(color == 0)
            buffer[x + ((y/8) * SCR_WIDTH)] &= ~(1 << (y%8));  // erase pixel
        else
            buffer[x + ((y/8) * SCR_WIDTH)] |= (1 << (y%8));   // set pixel
    } else { // XOR mode
        if(color == 1)
            buffer[x + ((y/8) * SCR_WIDTH)] ^= (1 << (y%8));   // xor pixel
    }
}

void locate(int x, int y)
{
    char_x = x;
    char_y = y;
}

void cls(void)
{
    memset(buffer, 0x00, SCR_WIDTH*SCR_HEIGHT/8);  // clear display buffer
    locate(0, 0);
    lcd_update();
}


void line(int x0, int y0, int x1, int y1, int color)
{
    int   dx = 0, dy = 0;
    int   dx_sym = 0, dy_sym = 0;
    int   dx_x2 = 0, dy_x2 = 0;
    int   di = 0;

    dx = x1-x0;
    dy = y1-y0;

    //  if (dx == 0) {        /* vertical line */
    //      if (y1 > y0) vline(x0,y0,y1,color);
    //      else vline(x0,y1,y0,color);
    //      return;
    //  }

    if (dx > 0) {
        dx_sym = 1;
    } else {
        dx_sym = -1;
    }
    //  if (dy == 0) {        /* horizontal line */
    //      if (x1 > x0) hline(x0,x1,y0,color);
    //      else  hline(x1,x0,y0,color);
    //      return;
    //  }

    if (dy > 0) {
        dy_sym = 1;
    } else {
        dy_sym = -1;
    }

    dx = dx_sym*dx;
    dy = dy_sym*dy;

    dx_x2 = dx*2;
    dy_x2 = dy*2;

    if (dx >= dy) {
        di = dy_x2 - dx;
        while (x0 != x1) {

            pixel(x0, y0, color);
            x0 += dx_sym;
            if (di<0) {
                di += dy_x2;
            } else {
                di += dy_x2 - dx_x2;
                y0 += dy_sym;
            }
        }
        pixel(x0, y0, color);
    } else {
        di = dx_x2 - dy;
        while (y0 != y1) {
            pixel(x0, y0, color);
            y0 += dy_sym;
            if (di < 0) {
                di += dx_x2;
            } else {
                di += dx_x2 - dy_x2;
                x0 += dx_sym;
            }
        }
        pixel(x0, y0, color);
    }
    if(auto_up) lcd_update();
}

void rect(int x0, int y0, int x1, int y1, int color)
{

    if (x1 > x0) line(x0,y0,x1,y0,color);
    else  line(x1,y0,x0,y0,color);

    if (y1 > y0) line(x0,y0,x0,y1,color);
    else line(x0,y1,x0,y0,color);

    if (x1 > x0) line(x0,y1,x1,y1,color);
    else  line(x1,y1,x0,y1,color);

    if (y1 > y0) line(x1,y0,x1,y1,color);
    else line(x1,y1,x1,y0,color);

    if(auto_up) lcd_update();
}

void fillrect(int x0, int y0, int x1, int y1, int color)
{
    int l,c,i;
    if(x0 > x1) {
        i = x0;
        x0 = x1;
        x1 = i;
    }

    if(y0 > y1) {
        i = y0;
        y0 = y1;
        y1 = i;
    }

    for(l = x0; l<= x1; l ++) {
        for(c = y0; c<= y1; c++) {
            pixel(l,c,color);
        }
    }
    if(auto_up) lcd_update();
}



void circle(int x0, int y0, int r, int color)
{
    int draw_x0, draw_y0;
    int draw_x1, draw_y1;
    int draw_x2, draw_y2;
    int draw_x3, draw_y3;
    int draw_x4, draw_y4;
    int draw_x5, draw_y5;
    int draw_x6, draw_y6;
    int draw_x7, draw_y7;
    int xx, yy;
    int di;

    if (r == 0) {       /* no radius */
        return;
    }

    draw_x0 = draw_x1 = x0;
    draw_y0 = draw_y1 = y0 + r;
//    if (draw_y0 < lcd_height()) {
        pixel(draw_x0, draw_y0, color);     /* 90 degree */
//    }

    draw_x2 = draw_x3 = x0;
    draw_y2 = draw_y3 = y0 - r;
//    if (draw_y2 >= 0) {
        pixel(draw_x2, draw_y2, color);    /* 270 degree */
//    }

    draw_x4 = draw_x6 = x0 + r;
    draw_y4 = draw_y6 = y0;
//    if (draw_x4 < lcd_width()) {
        pixel(draw_x4, draw_y4, color);     /* 0 degree */
//    }

    draw_x5 = draw_x7 = x0 - r;
    draw_y5 = draw_y7 = y0;
//    if (draw_x5>=0) {
        pixel(draw_x5, draw_y5, color);     /* 180 degree */
//    }

    if (r == 1) {
        return;
    }

    di = 3 - 2*r;
    xx = 0;
    yy = r;
    while (xx < yy) {

        if (di < 0) {
            di += 4*xx + 6;
        } else {
            di += 4*(xx - yy) + 10;
            yy--;
            draw_y0--;
            draw_y1--;
            draw_y2++;
            draw_y3++;
            draw_x4--;
            draw_x5++;
            draw_x6--;
            draw_x7++;
        }
        xx++;
        draw_x0++;
        draw_x1--;
        draw_x2++;
        draw_x3--;
        draw_y4++;
        draw_y5++;
        draw_y6--;
        draw_y7--;

//        if ( (draw_x0 <= lcd_width()) && (draw_y0>=0) ) {
            pixel(draw_x0, draw_y0, color);
//        }

//        if ( (draw_x1 >= 0) && (draw_y1 >= 0) ) {
            pixel(draw_x1, draw_y1, color);
//        }

//        if ( (draw_x2 <= lcd_width()) && (draw_y2 <= lcd_height()) ) {
            pixel(draw_x2, draw_y2, color);
//        }

//        if ( (draw_x3 >=0 ) && (draw_y3 <= lcd_height()) ) {
            pixel(draw_x3, draw_y3, color);
//        }

//        if ( (draw_x4 <= lcd_width()) && (draw_y4 >= 0) ) {
            pixel(draw_x4, draw_y4, color);
//        }

//        if ( (draw_x5 >= 0) && (draw_y5 >= 0) ) {
            pixel(draw_x5, draw_y5, color);
//        }
//        if ( (draw_x6 <= lcd_width()) && (draw_y6 <= lcd_height()) ) {
            pixel(draw_x6, draw_y6, color);
//        }
//        if ( (draw_x7 >= 0) && (draw_y7 <= lcd_height()) ) {
            pixel(draw_x7, draw_y7, color);
//        }
    }
    if(auto_up) lcd_update();
}

void fillcircle(int x, int y, int r, int color)
{
    int i,up;
    up = auto_up;
    auto_up = 0;   // off
    for (i = 0; i <= r; i++)
        circle(x,y,i,color);
    auto_up = up;
    if(auto_up) lcd_update();
}


void bitmap(Bitmap *bm, int x, int y)
{
    uint32_t h,v,b;
    char d;

    for(v=0; v < bm->height; v++) {   // lines
        for(h=0; h < bm->width; h++) { // pixel
            if(h + x > 127) break;
            if(v + y > 31) break;
            d = bm->data[bm->bytes_per_line * v + ((h & 0xF8) >> 3)];
            b = 0x80 >> (h & 0x07);
            if((d & b) == 0) {
                pixel(x+h,y+v,0);
            } else {
                pixel(x+h,y+v,1);
            }
        }
    }

}
/**************************************************************************
 * LCD text functions
 **************************************************************************/
static void character(int x, int y, int c)
{
    int hor,vert,offset,bpl,j,i,b;
    unsigned char* zeichen;
    unsigned char z,w;

    if ((c < 31) || (c > 127)) return;   // test char range

    // read font parameter from start of array
    offset = font[0];                    // bytes / char
    hor = font[1];                       // get hor size of font
    vert = font[2];                      // get vert size of font
    bpl = font[3];                       // bytes per line

    if (char_x + hor > lcd_width()) {
        char_x = 0;
        char_y = char_y + vert;
        if (char_y >= lcd_height() - font[2]) {
            char_y = 0;
        }
    }

    zeichen = &font[((c -32) * offset) + 4]; // start of char bitmap
    w = zeichen[0];                          // width of actual char
    // construct the char into the buffer
    for (j=0; j<vert; j++) {  //  vert line
        for (i=0; i<hor; i++) {   //  horz line
            z =  zeichen[bpl * i + ((j & 0xF8) >> 3)+1];
            b = 1 << (j & 0x07);
            if (( z & b ) == 0x00) {
                pixel(x+i,y+j,0);
            } else {
                pixel(x+i,y+j,1);
            }

        }
    }

    char_x += w;
}

void lcd_putc(int value)
{
    if (value == '\n') {    // new line
        char_x = 0;
        char_y = char_y + font[2];
        if (char_y >= lcd_height() - font[2]) {
            char_y = 0;
        }
    } else {
        character(char_x, char_y, value);
        if (auto_up) lcd_update();
    }
}

void lcd_puts(char *s)
{
	unsigned int tmp = auto_up;
	auto_up = 0;
	
	if (s)
		while (*s) lcd_putc(*s++);
	
	auto_up = tmp;
	if (auto_up) lcd_update();
}

void lcd_printf(const char* fmt, ...)
{
	va_list        ap;
	char          *p;
	char           ch;
	unsigned long  ul;
	char           s[512];
	
	unsigned int tmp = auto_up;
	auto_up = 0;
	
	va_start(ap, fmt);
	while (*fmt != '\0') {
		if (*fmt =='%') {
			switch (*++fmt) {
				case '%':
					lcd_putc('%');
					break;
				case 'c':
					ch = va_arg(ap, int);
					lcd_putc(ch);
					break;
				case 's':
					p = va_arg(ap, char *);
					lcd_puts(p);
					break;
				case 'd':
					ul = va_arg(ap, long);
					if ((long)ul < 0) {
						lcd_putc('-');
						ul = -(long)ul;
					}
					num2str(s, ul, 10);
					lcd_puts(s);
					break;
				case 'u':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 10);
					lcd_puts(s);
					break;
				case 'x':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 16);
					lcd_puts(s);
					break;
				default:
				    lcd_putc(*fmt);
			}
		} else lcd_putc(*fmt);
		fmt++;
	}
	va_end(ap);
	
	auto_up = tmp;
	if (auto_up) lcd_update();
}

/*
static void lcd_cmd(uint8_t c)
{
	A0_CLR;
	CS_CLR;
	spi_write(_SPI1, &c, 1);
	CS_SET;
}

static void lcd_cmds(uint8_t *buf, uint32_t n)
{
	A0_CLR;
	CS_CLR;
	spi_write(_SPI1, buf, n);
	CS_SET;
}

static void lcd_datum(uint8_t d)
{
	A0_SET;
	CS_CLR;
	spi_write(_SPI1, &d, 1);
	CS_SET;
}

static void lcd_data(uint8_t *buf, uint32_t n)
{
	A0_SET;
	CS_CLR;
	spi_write(_SPI1, buf, n);
	CS_SET;
}

int lcd_reset()
{
	// LCD_A0    --> D7  --> PA8 : GPIO
	io_configure(_GPIOA, PIN_8, PIN_MODE_OUTPUT|PIN_OPT_RESISTOR_NONE|PIN_OPT_OUTPUT_PUSHPULL|PIN_OPT_OUTPUT_SPEED_MEDIUM, NULL);
	// LCD_CS_N  --> D10 --> PB6 : GPIO
	io_configure(_GPIOB, PIN_6, PIN_MODE_OUTPUT|PIN_OPT_RESISTOR_NONE|PIN_OPT_OUTPUT_PUSHPULL|PIN_OPT_OUTPUT_SPEED_MEDIUM, NULL);
	// LCD_RESET --> D12 --> PA6 : GPIO
	io_configure(_GPIOA, PIN_6, PIN_MODE_OUTPUT|PIN_OPT_RESISTOR_NONE|PIN_OPT_OUTPUT_PUSHPULL|PIN_OPT_OUTPUT_SPEED_MEDIUM, NULL);
	// SPI pins : LCD_SCK   --> D13 --> PA5 : SPI1_SCK
	//            LCD_MOSI  --> D11 --> PA7 : SPI1_MOSI
	io_configure(_GPIOA, PIN_5|PIN_7, PIN_MODE_ALTFUNC|PIN_OPT_RESISTOR_NONE|PIN_OPT_OUTPUT_PUSHPULL|PIN_OPT_OUTPUT_SPEED_MEDIUM|PIN_OPT_AF5, NULL);

	spi_master_init(_SPI1, 20000000, SPI_MODE3);
	
	// Display reset
	A0_CLR;
	CS_SET;
	RESET_CLR;
	timer_wait_us(_TIM5, 50, NULL);
	RESET_SET;
	timer_wait_ms(_TIM5, 5, NULL);
	
	// Init sequence
	uint8_t buf[]={0xAE, 0xA2, 0xA0, 0xC8, 0x22, 0x2F, 0x40, 0xAF, 0x81, 0x17, 0xA6};
	lcd_cmds(buf, 11);
	
#if 0
    lcd_cmd(0xAE);   //  display off
    lcd_cmd(0xA2);   //  bias voltage

    lcd_cmd(0xA0);
    lcd_cmd(0xC8);   //  colum normal

    lcd_cmd(0x22);   //  voltage resistor ratio
    lcd_cmd(0x2F);   //  power on
    //lcd_cmd(0xA4);   //  LCD display ram
    lcd_cmd(0x40);   // start line = 0
    lcd_cmd(0xAF);     // display ON

    lcd_cmd(0x81);   //  set contrast
    lcd_cmd(0x17);   //  set contrast

    lcd_cmd(0xA6);     // display normal
#endif
}
*/
