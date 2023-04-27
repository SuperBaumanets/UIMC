// Includes
//=============================================================================================================================
#include "battery.h"
#include "st7735.h"
//=============================================================================================================================


//Declarations and definitions 
//=============================================================================================================================
extern ADC_HandleTypeDef hadc1;
extern screen_t scr1;

#define ADC_REF_VOL 			3.3			//Data sheet CPU (???)
#define ADC_MAX 					4095		//2^(n)-1 , n bits ADC 

//#define VOL_DIVISION						//Резистивный делитель
//=============================================================================================================================


//----------------------------------------------------------------------------
//@brief		Функция возвращает напряжение на источнике в [В]
float getBattVoltage()
{
	float outV;			//[В] - Выходное напряжение
	float adcN;			//Отсчет АЦП
	
	HAL_ADC_Start(&hadc1); 															//Запуск преобразования
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); 	//Ожидаем окончания преобразования
	adcN = HAL_ADC_GetValue(&hadc1); 										//Сохраняем значение АЦП в переменную adc   
		
#ifdef VOL_DIVISION
	/*
 >-------
				|
			 ###
		R1 ###
			 ###
	      |
Vin			|--------->
				|
			 ###
		R2 ###					VoutK
			 ###
				|	
>---------------->

VoutK = Vin * k, k < 1
	*/
	
	float k;					
	const float R1;
	const float R2;
	
	k = (float) R2 / (R1 + R2);
	outV = (float)( (adcN / k) * (ADC_REF_VOL / ADC_MAX) );
	return outV;	
#else

	//Разрешающая способность АЦП умножается на количество отсчетов полученное с АЦП
	outV = adcN * (ADC_REF_VOL / ADC_MAX);
	return outV;
	
#endif
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//@brief							Функция возвращает % заряда на батареи
//@param voltage			Напряжение снимаемое с источника					
uint8_t getBattPercent(float voltage)
{
	uint8_t percent;
	const float volscr_max = 1.8;
	const float volsrc_min = 0.8;
	
	if(volscr_max < voltage)
		return volscr_max;	
	if(volsrc_min > voltage)
		return volsrc_min;
	
	percent = ((voltage - volsrc_min) * 100) / (volscr_max - volsrc_min);
	
	if(percent < 0)
		return 0;
	if(percent > 100)
		return 100;
	
	return percent;
}
//----------------------------------------------------------------------------


//Различные варианты батарей
//================================================================================================================================================

//----------------------------------------------------------------------------
void greenbattery(t_batt batt)
{	
	if(batt.crutch != 1)
		return;
	
	int16_t x0 = batt.x; 
	int16_t y0 = batt.y;  
	uint8_t percent = batt.prcnt;
	
	//Отрисовка зоны заряда батареи
	uint16_t widthMaxCharge;  //Длина области заряда батареи при 100% заряда	
	uint16_t widthCharge;			//Длина области заряда батареи в зависимости от percent
	uint16_t heightCharge;	 	//Ширина области заряда батареи 
	uint16_t colorCharge;	    //Цвет области заряда батареи 
	uint16_t dcolorCharge;	  //Темный цвет области заряда батареи   
	
	widthMaxCharge = 25;
	widthCharge = widthMaxCharge * percent / 100; 
	
	heightCharge = 12;
	
	colorCharge = set_Color16(0, 49, 5);
	dcolorCharge = set_Color16(0, 40, 5);
	
	plot_FillRectangle(x0 + 1, y0 + 1, widthCharge, heightCharge - 2, colorCharge);
	plot_FillRectangle(x0 + 1, y0 + heightCharge - 1, widthCharge, 2, dcolorCharge);
	
	
	//Отрисовка корпуса батареи
	uint16_t widthFrame;   	//Длина  корпуса батареи 
	uint16_t heightFrame;	 	//Ширина корпуса батареи 
	uint16_t colorFrame;    //Цвет корпуса
	
	widthFrame = widthMaxCharge + 2; 
	heightFrame = heightCharge + 2;
	colorFrame = 0xFFFF;
	
	plot_Rectangle(x0, y0, widthFrame, heightFrame, colorFrame);
	
	
	//Отрисовка контанкта батареи
	uint16_t widthContact;   	//Длина  контакта батареи 
	uint16_t heightContact;	 	//Ширина контакта батареи
	
	widthContact = 2;
	heightContact = heightCharge - 4;
	
	plot_FillRectangle(x0 + widthFrame, y0 + 3, widthContact, heightContact, colorFrame);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void gradientbattery(t_batt batt)
{	
	if(batt.crutch != 1)
		return;
	
	int16_t x0 = batt.x; 
	int16_t y0 = batt.y;  
	uint8_t percent = batt.prcnt;
		
	//Отрисовка зоны заряда батареи
	uint16_t widthMaxCharge;  //Длина области заряда батареи при 100% заряда	
	uint16_t widthCharge;			//Длина области заряда батареи в зависимости от percent
	uint16_t heightCharge;	 	//Ширина области заряда батареи 
	
	//Цвет области заряда батареи - цвета градиента  
	uint16_t colorChargeGreen;	    
	uint16_t colorChargeRed;
	uint16_t colorChargeYellow;
	
	uint16_t colorBckGrnd;
	
	widthMaxCharge = 25;
	widthCharge = widthMaxCharge * percent / 100; 
	
	heightCharge = 12;
	
	colorChargeGreen = set_Color16(0, 49, 5);
	colorChargeYellow = 0xFFE0;
	colorChargeRed = 0xF800;
	colorBckGrnd = scr1.color_bckgrnd;
	
	
	for(uint8_t i = 0; i < 12; i++)
	{
		plot_Line2Color(x0 + 1, y0 + 1 + i, (x0 + 1) + widthMaxCharge/2 - 1, y0 + 1 + i, colorChargeRed, colorChargeYellow);
		plot_Line2Color((x0 + 1) + widthMaxCharge/2, y0 + 1 + i, x0 + widthMaxCharge, y0 + 1 + i, colorChargeYellow, colorChargeGreen);
	}
	
	plot_FillRectangle(x0 + widthCharge, y0 + 1, widthMaxCharge - widthCharge, heightCharge, colorBckGrnd);
	
	
	//Отрисовка корпуса батареи
	uint16_t widthFrame;   	//Длина  корпуса батареи 
	uint16_t heightFrame;	 	//Ширина корпуса батареи 
	uint16_t colorFrame;    //Цвет корпуса
	
	widthFrame = widthMaxCharge + 2; 
	heightFrame = heightCharge + 2;
	colorFrame = 0xFFFF;
	
	plot_Rectangle(x0, y0, widthFrame, heightFrame, colorFrame);
	
	//Отрисовка контанкта батареи
	uint16_t widthContact;   	//Длина  контакта батареи 
	uint16_t heightContact;	 	//Ширина контакта батареи
	
	widthContact = 2;
	heightContact = heightCharge - 4;
	
	plot_FillRectangle(x0 + widthFrame, y0 + 3, widthContact, heightContact, colorFrame);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void fsgrnyllwrdbattery(t_batt batt)
{	
	if(batt.crutch != 1)
		return;
	
	int16_t x0 = batt.x; 
	int16_t y0 = batt.y;  
	uint8_t percent = batt.prcnt;
	
	//Отрисовка зоны заряда батареи
	uint16_t widthMaxCharge;  		//Длина области заряда батареи при 100% заряда	
	uint16_t widthChargeSeg;			//Длина одного сегмента заряда
	uint16_t heightChargeSeg;		  //Ширина одного сегмента заряда
	uint16_t heightCharge;	 			//Ширина области заряда батареи
	
	uint16_t colorChargeGreen;	    //Зеленый цвет заряда батареи 
	uint16_t colorChargeBrGreen;	  //Светло-зеленый цвет заряда батареи
	uint16_t colorChargeYellow;	    //Желтый цвет заряда батареи 
	uint16_t colorChargeRed;	    	//Красный цвет заряда батареи 
	
	widthMaxCharge = 25;
	widthChargeSeg = 5;
	heightCharge = 12;
	heightChargeSeg = 10;
	
	colorChargeGreen = set_Color16(0, 48, 5);   
	colorChargeBrGreen = set_Color16(19, 61, 0);
	colorChargeYellow = ST7735_COLOR_YELLOW;	    
	colorChargeRed = ST7735_COLOR_RED;	    	
	
	if( (percent <= 100) && (percent > 75) )
	{
		for(uint8_t i = 0; i < 4; i++)
				plot_FillRectangle(x0 + 2 + i + i*widthChargeSeg, y0 + 2, widthChargeSeg, heightChargeSeg, colorChargeGreen);
	}
	
	if( (percent <= 75) && (percent > 50) )
	{
		for(uint8_t i = 0; i < 3; i++)
				plot_FillRectangle(x0 + 2 + i + i*widthChargeSeg, y0 + 2, widthChargeSeg, heightChargeSeg, colorChargeBrGreen);
	}
	
	if( (percent <= 50) && (percent > 25) )
	{
		plot_FillRectangle(x0 + 2, y0 + 2, widthChargeSeg, heightChargeSeg, colorChargeYellow);
		plot_FillRectangle(x0 + 3 + widthChargeSeg, y0 + 2, widthChargeSeg, heightChargeSeg, colorChargeYellow);
	}
	
	if( (percent <= 25) && (percent > 0) )
				plot_FillRectangle(x0 + 2, y0 + 2, widthChargeSeg, heightChargeSeg, colorChargeRed);
	
	
	//Отрисовка корпуса батареи
	uint16_t widthFrame;   	//Длина  корпуса батареи 
	uint16_t heightFrame;	 	//Ширина корпуса батареи 
	uint16_t colorFrame;    //Цвет корпуса
	
	widthFrame = widthMaxCharge + 2; 
	heightFrame = heightCharge + 2;
	colorFrame = 0xFFFF;
	
	plot_Rectangle(x0, y0, widthFrame, heightFrame, colorFrame);
	
	
	//Отрисовка контанкта батареи
	uint16_t widthContact;   	//Длина  контакта батареи 
	uint16_t heightContact;	 	//Ширина контакта батареи
	
	widthContact = 2;
	heightContact = heightCharge - 4;
	
	plot_FillRectangle(x0 + widthFrame, y0 + 3, widthContact, heightContact, colorFrame);
}
//----------------------------------------------------------------------------

//================================================================================================================================================

//----------------------------------------------------------------------------
void init_batt(t_batt *ptrbatt, void (*ptrdesign)(t_batt), int16_t x0, int16_t y0)
{
	//Снимаемое с источника напряжение
	ptrbatt -> vout = getBattVoltage();
	
	//Процент заряда батареи
	ptrbatt -> prcnt  = getBattPercent(ptrbatt -> vout);
	
	ptrbatt -> x = x0;
	ptrbatt -> y = y0;
	
	ptrbatt -> design = *ptrdesign;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void plot_batt(t_batt batt)
{
	batt.crutch = 1;
	batt.design(batt);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void print_prcntbatt(t_batt batt, int16_t x0, int16_t y0, uint16_t color)
{
	set_cursor(x0, y0, 0);
	
	uint8_t prcnt = batt.prcnt;
	
	print_IntNum(prcnt, ST7735_COLOR_WHITE, 1, 1);
	
	// Подсчет количества цифр в числе prcnt
	uint8_t cnt = 0; 
  while (prcnt != 0)
  {
      prcnt = prcnt / 10;
      ++cnt;
  }
	
	uint8_t indent; // Расстояние от начала вывода цифр до символа '%'
	switch (cnt)
	{
		case 1: indent = 6;
						break;
		case 2: indent = 12;
						break;
		case 3: indent = 18;
						break;
	}
	
	set_cursor(x0 + indent, y0, 0);
	print_Char('%', color, 1, 1);
}
//----------------------------------------------------------------------------
