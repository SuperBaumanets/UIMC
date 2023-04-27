#ifndef BATTERY_H_
#define BATTERY_H_

//Includes
//=============================================================================================================================
#include "stm32f4xx_hal.h"
//=============================================================================================================================

//Variables
//=============================================================================================================================
typedef struct c_batt
{
	int16_t x; 
	int16_t y;
	
	float vout;
	uint8_t prcnt;
	
	void (*design)(struct c_batt);
	void (*init_batt)(struct c_batt, int16_t, int16_t);
	
	//In order you can not call the drawing of battery icons through the battery icon decoration function
	uint8_t crutch;
}t_batt;
//=============================================================================================================================


//Functions 
//================================================================================================================================================

//@brief														Function initializes the battery parameters and writes them to the structure.
//																	initializes parameters:
//																	icon drawing position
//																	battery design
//																	battery charge in percent
//																	battery charge in volts
//@param *ptrbatt										pointer to battery object				
//@param (*ptrdesign)(t_batt)				battery icon design function pointer
//@param x0													coordinate x0 of start point
//@param y0													coordinate y0 of start point
void init_batt(t_batt *ptrbatt, void (*ptrdesign)(t_batt), int16_t x0, int16_t y0);


//@brief 							Draw the battery icon. 
//@param batt					Battery object
void plot_batt(t_batt batt);

//@brief 							Print the battery percent. 
//										Default the width and height size of the "pixel" is 1.
//@param batt					battery object
//@param color				Ñolor of the symbol
//@param x0						coordinate x 
//@param y0						coordinate y
void print_prcntbatt(t_batt batt, int16_t x0, int16_t y0, uint16_t color);


//Battery icon design function
//-----------------------------------------------------------------------------------------------------------------------------
//@brief					greenbattery() - The charge display area is a green area 
//								that decreases in length depending on the percent of battery charge.
//@param batt			battery object
void greenbattery(t_batt batt);


//@brief					gradientbattery() - The charge display area is a gradient area(0-50%-100% : Red-Yellow-Green)
//								that decreases in length depending on the percentage of battery charge.
//@param batt			battery object
void gradientbattery(t_batt batt);


//@brief					fsgrnyllwrdbattery() - The charge display area includes four segments:
//								100% - 75% : 4 green segments
//								75% - 50%  : 3 bright-green segments
//								50% - 25%  : 2 yellow segments
//								25% - 0%   : 1 red segments
//@param batt			battery object
void fsgrnyllwrdbattery(t_batt batt);
//-----------------------------------------------------------------------------------------------------------------------------

//================================================================================================================================================
#endif // #ifndef BATTERY