/**
 * Nokia 3310 LCD (PCD8544) Controller  
 * Source code written in MikroC language (v5.61 2012)
 *
 * @link http://www.EngMHatab.com
 * @author Mohammed Abu-Hatab 
 * @package mhatab
 * @version 1.0 
 * @copyright 2017 
 */

#include "pcd8544.h"          

// main constants  
#define COMMAND_BYTE    0
#define DATA_BYTE       1
#define CHAR_WIDTH      6
#define MAX_ROWS        6 
#define MAX_COLS        84
#define MAX_CHAR_INLINE 14

char delay_char = 200; //200 ms
int temp_index;

static char init_cmd [] = {CHIP_ACTIVE, BIAS_SYSTEM,
    VOLTAGE_OPERATION, DISPLAY_SEGMENTS_ON, DISPLAY_BLANK,
    DISPLAY_NORMAL_MODE, WRITE_HORIZONTAL_MODE};

/**
* Delay Time in ms (millisecond) 
* 18 Bytes of ROM usage
* 
* @param int delay ,value[0-65535]
*/
void DelayMs(unsigned int delay) { 
    while (delay > 0) {
        Delay_500us(); //Delay_500us this function take 9 Bytes of ROM usage
        Delay_500us(); 
        delay--;
    }
}

/**
* Write Byte on Nokia LCD
* 25 Bytes of ROM usage
* 
* @param char temp_data 
*/
void nokia_lcd_write_byte(char temp_data) {
    char data_index;

    nokia_cse = 0; // chip enabled

    for (data_index = 0; data_index < 8; data_index++) {
        nokia_sdin = 1;
        if ((temp_data & 0x80) == 0) nokia_sdin = 0;

        nokia_sclk = 1;

        temp_data <<= 1;

        nokia_sclk = 0;

    }
    nokia_cse = 1; // chip disabled
}
  
/**
* Send command to PCD8544
* 7 Bytes of ROM usage
* 
* @param char temp_data 
*/
void nokia_lcd_cmd(char temp_data) {  
    nokia_dc = COMMAND_BYTE;
    nokia_lcd_write_byte(temp_data);
}

/**
* Send Data to PCD8544
* 7 Bytes of ROM usage
* 
* @param char temp_data 
*/
void nokia_lcd_data(char temp_data) { 
    nokia_dc = DATA_BYTE;
    nokia_lcd_write_byte(temp_data);
}

/**
* Goto offset (Y ,X)  
* 17 Bytes of ROM usage
* 
* @param char y_axis ,value [0-5]
* @param char x_axis ,value [0-83]
*/
void nokia_lcd_goto_offset(char y_axis, char x_axis) {  
	nokia_lcd_cmd((Y_ADDRESS | (y_axis & 0x07))); // Y-axis value : 0100 0yyy ; 0 <= Y <= 5 
	nokia_lcd_cmd((X_ADDRESS | (x_axis & 0x7f))); // X-axis value : 1xxx xxxx ; 0 <= X <= 83  
}
 
/**
* Write Char on Nokia LCD  
* 45 Bytes of ROM usage
* 
* @param char temp_char  
*/
void nokia_lcd_out_char(char temp_char) { 
    // this right 6 bytes foreach char  = 5 bytes + space   
    char temp_index;

    temp_char -= 32;
    for (temp_index = 0; temp_index < 5; temp_index++) {
        nokia_lcd_data(English_Font[temp_char][temp_index]); // send data to nokia
    }
    nokia_lcd_data(0x00); // space between char
    DelayMs(delay_char);
}
 
/**
* Write Message on Nokia LCD  
* 27 Bytes of ROM usage
* 
* @param const char* message  
*/
void nokia_lcd_out_message(const char* message) {  
    while (*message) // Look for end of string
        nokia_lcd_out_char(*message++);  
}
 
/**
* Write Unsigned Integer Number on Nokia LCD  
* 67 Bytes of ROM usage
* 
* @param unsigned int num , value [0,65535]  
*/
void nokia_lcd_out_unsigned_int(unsigned int num) { 
    //num = 135
    char numCache [6];
    unsigned int num_mod;
    unsigned int num_div;
    char digit_index = 0;
    num_div = num;
    do {
        num_mod = num_div % 10; // 5 ,3,1
        num_div = num_div / 10; // 13,1,0 
        numCache[digit_index] = num_mod + 48; //char + (32 + 16) = char + 48
        digit_index++;
    } while (num_div > 0);

    while (digit_index > 0) {
        digit_index--;
        nokia_lcd_out_char(numCache[digit_index]);
    }

}
 
/**
* Write Integer Number on Nokia LCD  
* 35 Bytes of ROM usage
* 
* @param int num , value [-32765,32767]  
*/
void nokia_lcd_out_int(int num) {
    if (num < 0) {
        nokia_lcd_out_char('-');
        num *= -1;
    }
    nokia_lcd_out_unsigned_int(num);
}
 
/**
* Draw Image Bitmap on Nokia LCD  
* 41 Bytes of ROM usage
* 
* @param const char* img , array of 504 bytes 
*/
void nokia_lcd_image(const char* img) {
    nokia_lcd_goto_offset(0, 0);
    for (temp_index = 0; temp_index < 504; temp_index++) {
        nokia_lcd_data(img[temp_index]);
    }
}
 
/**
* Flash Nokia LCD
* 25 Bytes of ROM usage
* 
* @param char flash_number , value [0,255]
*/
void nokia_lcd_flash(char flash_number) { 
    while (flash_number > 0) {
        DelayMs(1500);
        nokia_lcd_cmd(DISPLAY_INVERSE_MODE);
        DelayMs(1500);
        nokia_lcd_cmd(DISPLAY_NORMAL_MODE);
        flash_number--;
    }
}
 
/**
* Clear All Pixels in DDRAM of Nokia LCD
* 19 Bytes of ROM usage
*/
void nokia_lcd_clear() {  
	char temp_row;
	char temp_column;

    for (temp_row = 0; temp_row < MAX_ROWS; temp_row++) {
        for (temp_column = 0; temp_column < MAX_COLS; temp_column++) {
			 nokia_lcd_data(0x00);
        } 
    }
}

/**
* Clear Line of Nokia LCD
* 16 Bytes of ROM usage
*
* @param char temp_row , value [0,5]
*/
void nokia_lcd_clear_line(char temp_row) {  
    
	char temp_column;
	nokia_lcd_goto_offset(temp_row,0);

	for (temp_column = 0; temp_column < MAX_COLS; temp_column++) {
		 nokia_lcd_data(0x00);
	} 
}

/**
* Initialize Nokia LCD
* 27 Bytes of ROM usage
*/
void nokia_lcd_init() {  
    char temp_cmd_index;
    // The serial interface is initialized 
    nokia_cse = 1; // chip disabled

    /*The RES input must be < 0.3 VDD when VDD reaches VDDmin
        (or higher) within a maximum time of 100 ms after VDD
        goes HIGH , from data-sheet page 15 */
    nokia_res = 0; // reset chip 
    DelayMs(250);
    nokia_res = 1;

    for (temp_cmd_index = 0; temp_cmd_index < 7; temp_cmd_index++) {
        nokia_lcd_cmd(init_cmd[temp_cmd_index]);
    }
    nokia_lcd_clear();  
    nokia_lcd_goto_offset(0, 0); 
}
