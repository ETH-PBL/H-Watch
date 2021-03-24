
#ifndef DISP_SENSORS_H_
#define DISP_SENSORS_H_

//== Definitions =================================================================

#define VCOM_HI		0x40				// VCOM bit toggle
#define VCOM_LO		0x00

#define stringBig	0x01				// Set size of string
#define stringSmall 0x00

#define Clear		0x00
#define Draw		0x01

/* VCOM is an alternating signal that prevents a DC bias from being built up within the panel.
 * If this DC bias is allowed to accumulate, it will eventually reach the point where the state
 * of the liquid crystals in the panel cells cannot be changed by the panel electronics and
 * the cells will no longer change state.*/

//== Variables ===================================================================

extern uint8_t bit_lookup[8];			// Lookup table for easy bit access
extern uint8_t inv_char[8];
extern uint8_t char_lookup[8];
extern uint8_t reverse_bit[8];

extern uint8_t small_nb3[24];			//Lookup table for the Nb3 connectivity symbol
extern uint8_t big_nb4[120];			//Lookup table for the Nb4 connectivity symbol

extern uint8_t smallfont[256][24];		// Lookup table for the small font. Encoded ASCII-characters until position 255.
extern uint8_t bigfont[256][120];		// Lookup table for the big font. Encoded ASCII-characters until position 255.

extern uint8_t table[256];				// Lookup table for reverse byte

//== Functions ===================================================================
//== Display Functions ===========================================================

void Display_Task (void const * argument);

void screen_init(void);				// Initial screen: clear pixel map and activate pixel

void screen_on(void);				// enable display: GPIO PC4 = high
void screen_off(void);				// disable display GPIO PC4 = low

void screen_cs_set(void);			// set chip-select line of the display:	PC5
void screen_cs_reset(void);			// reset chip-select line of the display: PC5

// position (x/y) is the left lower corner of the character. charters, symbols will then build into up and right direction.
// (0/0) is in the top left corner if the display.

void screen_setPixel(uint8_t pos_x, uint8_t pos_y);		// Set one specific pixel at coordinate (x/y) in the screen_map matrix
void screen_resetPixel(uint8_t pos_x, uint8_t pos_y);	// clear one specific pixel at coordinate (x/y) in the screen_map matrix

void screen_drawSmallChar(uint8_t pos_x, uint8_t pos_y, uint8_t c);		// draw char according to ASCII-table. 16 pixels in height and 12 pixels in width
void screen_drawBigChar(uint8_t pos_x, uint8_t pos_y, uint8_t c);		// draw char according to ASCII-table. 40 pixels in height and 24 pixels in width

void screen_drawETH(uint8_t pos_x, uint8_t pos_y);						// Draw ETH logo with size 24 x 127 pixels
void screen_drawHealthAlarm(uint8_t pos_x, uint8_t pos_y);				// Draw health alarm symbol with size 24 x 24 pixels
void screen_drawPBL(uint8_t pos_x, uint8_t pos_y);						// Draw PBL logo with size 50 x 112 pixels
void screen_drawWatch(uint8_t pos_x, uint8_t pos_y);					// Draw Watchface with size 240 x 240 pixels

void screen_clearSmallChar(uint8_t pos_x, uint8_t pos_y, uint8_t c);
void screen_clearBigChar(uint8_t pos_x, uint8_t pos_y, uint8_t c);

void screen_drawSmallNb3(uint8_t pos_x, uint8_t pos_y);		// draw custom Nb connection symbol with 3 connectivity status bars
void screen_drawBigNb4(uint8_t pos_x, uint8_t pos_y);		// draw custom Nb connection symbol with 4 connectivity status bars

void screen_drawString(uint8_t pos_x, uint8_t pos_y, uint8_t size, uint8_t stringLenght, char* string);		// Plots strings in two sizes: size = 0 small, size = 1 big
void screen_clearString(uint8_t pos_x, uint8_t pos_y, uint8_t size, uint8_t stringLenght, char* string);

void screen_update(void);			// Update the changes of the screen_map matrix into the memory LCD
void set_screen_map_zero(void);		// Clear the screen_map. Every pixel is set to 1, therefore the whole screen appears white after updating the screen

uint8_t reverse_byte(uint8_t x);	// return reverse byte


#endif /* INT_SENSORS_H_ */
