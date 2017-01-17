/**
 * Test PIC 16f628a with Nokia 3310 LCD (PCD8544 GLCD) 
 * Source code written in MikroC language (v5.61 2012)
 *
 * @link http://www.EngMHatab.com
 * @author Mohammed Abu-Hatab 
 * @package mhatab
 * @version 1.0 
 * @copyright 2017 
 */
 
#include "../nokia3310_lcd.c"

#include "images/engmhatab.c"

const char* text = "EngMHatab.COM";

void main() {
    int counter ;
    TRISA = 0x00;
    TRISB = 0x00;

    CMCON = 0x07;

    PORTA = 0x00;
    PORTB = 0x00;

    nokia_lcd_init();

    while (1) {
        nokia_lcd_image(engmhatab); //engmhatab you can find here => #include "images/engmhatab.c"
        nokia_lcd_flash(1);
        nokia_lcd_clear();

        //print the string on the display
        // 14 char in line     -------------|-------------|=============|-------------|-------------|=============|
        nokia_lcd_out_message("    Hello!    I am Mohamed   Abu Hatab    Computer   ;) Engineer");
        DelayMs(2000);


        delay_char= 10 ;//10ms
        nokia_lcd_clear();


        nokia_lcd_goto_offset(2,1 * CHAR_WIDTH); // row 2 , column 1
        nokia_lcd_out_message(text);
        nokia_lcd_flash(1);

        nokia_lcd_clear();


        nokia_lcd_goto_offset(0, 1 * CHAR_WIDTH); // row 0 , col = 1
        nokia_lcd_out_message ("Test counter");
        nokia_lcd_goto_offset(4, 2 * CHAR_WIDTH); // row 4 , col = 2
        nokia_lcd_out_message ("Increment");
        counter = 0 ;
        while(counter < 15){
            nokia_lcd_clear_line(2);
            nokia_lcd_goto_offset(2, 6 * CHAR_WIDTH);
            nokia_lcd_out_int(counter);
            DelayMs(300);
            counter++;
        }

        nokia_lcd_goto_offset(4, 2 * CHAR_WIDTH);
        nokia_lcd_out_message ("Decrement");
        while(counter > -15){
            nokia_lcd_clear_line(2);
            nokia_lcd_goto_offset(2, 6 * CHAR_WIDTH);
            nokia_lcd_out_int(counter);
            DelayMs(300);
            counter--;
        }

        nokia_lcd_clear();
        DelayMs(500);
        nokia_lcd_goto_offset(2 , 2 * CHAR_WIDTH); // row 2 , column 0
        nokia_lcd_out_message("Thanks for     Watching");
        DelayMs(2000);

    }
}