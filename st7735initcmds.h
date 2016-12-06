#ifndef _ST7735INITCMDS_H_
#define _ST7735INITCMDS_H_

#include <avr/pgmspace.h>

#include "st7735.h"

#define DELAY_FLAG 0x80

static const uint8_t PROGMEM st7735_blue_init[] = {	// Initialization commands for 7735B screens
    18,                       		// 18 commands in list:
    ST7735_SWRESET, DELAY_FLAG,  	//  1: Software reset, no args, w/delay
      	50,                     	//     50 ms delay
    ST7735_SLPOUT, DELAY_FLAG,  	//  2: Out of sleep mode, no args, w/delay
      	255,                    	//     255 = 500 ms delay
    ST7735_COLMOD, 1 | DELAY_FLAG,  //  3: Set color mode, 1 arg + delay:
      	0x05,                   	//     16-bit color
      	10,                     	//     10 ms delay
    ST7735_FRMCTR1, 3 | DELAY_FLAG, //  4: Frame rate control, 3 args + delay:
      	0x00,                   	//     fastest refresh
      	0x06,                   	//     6 lines front porch
      	0x03,                   	//     3 lines back porch
      	10,                     	//     10 ms delay
    ST7735_MADCTL, 1,		  		//  5: Memory access ctrl (directions), 1 arg:
      	0x08,                   	//     Row addr/col addr, bottom to top refresh
    ST7735_DISSET5, 2,  			//  6: Display settings #5, 2 args, no delay:
      	0x15,                   	//     1 clk cycle nonoverlap, 2 cycle gate
                              		//     rise, 3 cycle osc equalize
      	0x02,                   	//     Fix on VTL
    ST7735_INVCTR, 1,		  		//  7: Display inversion control, 1 arg:
      	0x0,                    	//     Line inversion
    ST7735_PWCTR1, 2 | DELAY_FLAG,  //  8: Power control, 2 args + delay:
      	0x02,                   	//     GVDD = 4.7V
      	0x70,                   	//     1.0uA
      	10,                     	//     10 ms delay
    ST7735_PWCTR2, 1,  				//  9: Power control, 1 arg, no delay:
      	0x05,                   	//     VGH = 14.7V, VGL = -7.35V
    ST7735_PWCTR3, 2,  				// 10: Power control, 2 args, no delay:
      	0x01,                   	//     Opamp current small
      	0x02,                   	//     Boost frequency
    ST7735_VMCTR1, 2 | DELAY_FLAG,  // 11: Power control, 2 args + delay:
      	0x3C,                   	//     VCOMH = 4V
      	0x38,                   	//     VCOML = -1.1V
      	10,                     	//     10 ms delay
    ST7735_PWCTR6, 2,  				// 12: Power control, 2 args, no delay:
      	0x11, 0x15,
    ST7735_GMCTRP1, 16,  			// 13: Magical unicorn dust, 16 args, no delay:
	    0x09, 0x16, 0x09, 0x20, 	//     (seriously though, not sure what
	    0x21, 0x1B, 0x13, 0x19, 	//      these config values represent)
	    0x17, 0x15, 0x1E, 0x2B,
	    0x04, 0x05, 0x02, 0x0E,
    ST7735_GMCTRN1, 16 | DELAY_FLAG,// 14: Sparkles and rainbows, 16 args + delay:
      	0x0B, 0x14, 0x08, 0x1E, 	//     (ditto)
      	0x22, 0x1D, 0x18, 0x1E,
      	0x1B, 0x1A, 0x24, 0x2B,
      	0x06, 0x06, 0x02, 0x0F,
      	10,                     	//     10 ms delay
    ST7735_CASET, 4,  				// 15: Column addr set, 4 args, no delay:
      	0x00, 0x02,             	//     XSTART = 2
      	0x00, 0x81,             	//     XEND = 129
    ST7735_RASET, 4,  				// 16: Row addr set, 4 args, no delay:
      	0x00, 0x02,             	//     XSTART = 1
      	0x00, 0x81,             	//     XEND = 160
    ST7735_NORON, DELAY_FLAG,  		// 17: Normal display on, no args, w/delay
      	10,                     	//     10 ms delay
    ST7735_DISPON, DELAY_FLAG,  	// 18: Main screen turn on, no args, w/delay
      	255							//     255 = 500 ms delay
};

static const uint8_t PROGMEM st7735_red_init1[] = { // Init for 7735R, part 1 (red or green tab)
    15,                       	// 15 commands in list:
    ST7735_SWRESET,	DELAY_FLAG, //  1: Software reset, 0 args, w/delay
      	150,                    //     150 ms delay
    ST7735_SLPOUT, DELAY_FLAG,  //  2: Out of sleep mode, 0 args, w/delay
      	255,                    //     500 ms delay
    ST7735_FRMCTR1, 3,  		//  3: Frame rate ctrl - normal mode, 3 args:
      	0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3,  		//  4: Frame rate control - idle mode, 3 args:
      	0x01, 0x2C, 0x2D,      	//     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6,  		//  5: Frame rate ctrl - partial mode, 6 args:
      	0x01, 0x2C, 0x2D,       //     Dot inversion mode
      	0x01, 0x2C, 0x2D,       //     Line inversion mode
    ST7735_INVCTR, 1,  			//  6: Display inversion ctrl, 1 arg, no delay:
      	0x07,                   //     No inversion
    ST7735_PWCTR1, 3,  			//  7: Power control, 3 args, no delay:
      	0xA2,
      	0x02,                   //     -4.6V
      	0x84,                   //     AUTO mode
    ST7735_PWCTR2, 1,  			//  8: Power control, 1 arg, no delay:
      	0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735_PWCTR3, 2,  			//  9: Power control, 2 args, no delay:
      	0x0A,                   //     Opamp current small
      	0x00,                   //     Boost frequency
    ST7735_PWCTR4, 2,  			// 10: Power control, 2 args, no delay:
      	0x8A,                   //     BCLK/2, Opamp current small & Medium low
      	0x2A,
    ST7735_PWCTR5, 2,  			// 11: Power control, 2 args, no delay:
      	0x8A, 0xEE,
    ST7735_VMCTR1, 1,  			// 12: Power control, 1 arg, no delay:
      	0x0E,
    ST7735_INVOFF, 0,  			// 13: Don't invert display, no args, no delay
    ST7735_MADCTL, 1,  			// 14: Memory access control (directions), 1 arg:
      	0xC8,                   //     row addr/col addr, bottom to top refresh
    ST7735_COLMOD, 1,  			// 15: set color mode, 1 arg, no delay:
      	0x05					//     16-bit color
  };

static const uint8_t PROGMEM st7735_red_init_green2[] = { // Init for 7735R, part 2 (green tab only)
    2,                        	//  2 commands in list:
    ST7735_CASET, 4,  			//  1: Column addr set, 4 args, no delay:
      	0x00, 0x02,            	//     XSTART = 0
      	0x00, 0x7F+0x02,        //     XEND = 127
    ST7735_RASET, 4,  			//  2: Row addr set, 4 args, no delay:
      	0x00, 0x01,             //     XSTART = 0
      	0x00, 0x9F+0x01			//     XEND = 159
 };

 static const uint8_t PROGMEM st7735_red_init_red2[] = { // Init for 7735R, part 2 (red tab only)
    2,                 	//  2 commands in list:
    ST7735_CASET, 4,  	//  1: Column addr set, 4 args, no delay:
      	0x00, 0x00,     //     XSTART = 0
      	0x00, 0x7F,     //     XEND = 127
    ST7735_RASET, 4,  	//  2: Row addr set, 4 args, no delay:
      	0x00, 0x00,     //     XSTART = 0
      	0x00, 0x9F,     //     XEND = 159
};

static const uint8_t PROGMEM st7735_red_init_green1442[] = {// Init for 7735R, part 2 (green 1.44 tab)
    2,                    	//  2 commands in list:
    ST7735_CASET, 4,  		//  1: Column addr set, 4 args, no delay:
      	0x00, 0x00,         //     XSTART = 0
      	0x00, 0x7F,         //     XEND = 127
    ST7735_RASET, 4,  		//  2: Row addr set, 4 args, no delay:
      	0x00, 0x00,         //     XSTART = 0
      	0x00, 0x7F          //     XEND = 127
};

static const uint8_t PROGMEM st7735_red_init3[] = { // Init for 7735R, part 3 (red or green tab)
    4,                        		//  4 commands in list:
    ST7735_GMCTRP1, 16, 			//  1: Magical unicorn dust, 16 args, no delay:
      	0x02, 0x1c, 0x07, 0x12,
      	0x37, 0x32, 0x29, 0x2d,
      	0x29, 0x25, 0x2B, 0x39,
      	0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16, 			//  2: Sparkles and rainbows, 16 args, no delay:
      	0x03, 0x1d, 0x07, 0x06,
      	0x2E, 0x2C, 0x29, 0x2D,
      	0x2E, 0x2E, 0x37, 0x3F,
      	0x00, 0x00, 0x02, 0x10,
    ST7735_NORON, DELAY_FLAG, 		//  3: Normal display on, no args, w/delay
      	10,                     	//     10 ms delay
    ST7735_DISPON, DELAY_FLAG, 		//  4: Main screen turn on, no args w/delay
      	100                  		//     100 ms delay
};

#endif
