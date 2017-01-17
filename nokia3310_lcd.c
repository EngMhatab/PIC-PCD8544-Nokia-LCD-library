/**
 * Pic interfacing with Nokia 3310 LCD (PCD8544 GLCD) 
 * Source code written in MikroC language (v5.61 2012)
 *
 * @link http://www.EngMHatab.com
 * @author Mohammed Abu-Hatab 
 * @package mhatab
 * @version 1.0 
 * @copyright 2017 
 */
 
//================= PIC PINs ================================ Nokia PINs 
//                              	//   3.3 v          pin 1 
#define nokia_sclk  PORTA.f6 		// nokia lcd sclk	pin 2
#define nokia_sdin  PORTA.f7 		// nokia lcd sdin	pin 3
#define nokia_dc    PORTA.f0    	// nokia lcd d/c	pin 4
#define nokia_cse   PORTA.f1 		// nokia lcd cse	pin 5
//                              	// ground           pin 6
//                                  //                  pin 7 connect with 0.33uF "+" --|(-- "-" to ground
#define nokia_res   PORTA.f2 		// nokia lcd res	pin 8
 
// if you don't have ( 3.3v ) you can following this theory :( voltage divider theory)
// 3.3 v  <==>  +5v __( --->|--- diode 1n4001 )__( --->|--- diode 1n4001 )__( --->|--- diode 1n4001 )__.____ to pin1  
//                                                                                                     |____(10k ohm)___ to ground

        
#include "fonts/english_5x7.c"    
#include "controllers/pcd8544.c"