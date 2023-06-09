// Includes
//=============================================================================================================================
#include "battery.h"
#include "devicebattery.h"
#include "st7735.h"
//=============================================================================================================================


//Declarations and definitions 
//=============================================================================================================================
extern ADC_HandleTypeDef hadc1;
extern screen_t scr1;

#define ADC_REF_VOL 			3.3			//Data sheet CPU (???)
#define ADC_MAX 					4095		//2^(n)-1 , n bits ADC 

//#define VOL_DIVISION						//����������� ��������
//=============================================================================================================================


//----------------------------------------------------------------------------
//@brief		return the voltage at the source in [V]
float batt_GetVoltage()
{
	float outV;			//[�] - �������� ����������
	float adcN;			//������ ���
	
	HAL_ADC_Start(&hadc1); 															//������ ��������������
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); 	//������� ��������� ��������������
	adcN = HAL_ADC_GetValue(&hadc1); 										//��������� �������� ��� � ���������� adc   
		
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

	//����������� ����������� ��� ���������� �� ���������� �������� ���������� � ���
	outV = adcN * (ADC_REF_VOL / ADC_MAX);
	return outV;
	
#endif
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//@brief							������� ���������� % ������ �� �������
//@param voltage			���������� ��������� � ���������					
uint8_t batt_GetPercent(float voltage)
{
	uint8_t percent;
	
	const float vltgMax = VOLTAGE_MAX;
	const float vltgMin = VOLTAGE_MIN;
	
	if(vltgMax < voltage)
		voltage = vltgMax;	
	if(vltgMin > voltage)
		return voltage; // to correct
	
	percent = ((voltage - vltgMin) * 100) / (vltgMax - vltgMin);
	
	if(percent < 0)
		return 0;
	if(percent > 100)
		return 100;
	
	return percent;
}
//----------------------------------------------------------------------------


//��������� �������� �������
//================================================================================================================================================

//----------------------------------------------------------------------------
void greenbattery(t_batt batt)
{	
	if(batt.flagFrstPlotBatt != 1)
		return;
	
	int16_t x0 = batt.x; 
	int16_t y0 = batt.y;  
	uint8_t percent = batt.prcnt;
	
	//��������� ���� ������ �������
	uint16_t widthMaxCharge;  //����� ������� ������ ������� ��� 100% ������	
	uint16_t widthCharge;			//����� ������� ������ ������� � ����������� �� percent
	uint16_t heightCharge;	 	//������ ������� ������ ������� 
	uint16_t colorCharge;	    //���� ������� ������ ������� 
	uint16_t dcolorCharge;	  //������ ���� ������� ������ �������   
	
	widthMaxCharge = 25;
	widthCharge = widthMaxCharge * percent / 100; 
	
	heightCharge = 12;
	
	colorCharge = ST7735S_SetColor16(0, 49, 5);
	dcolorCharge = ST7735S_SetColor16(0, 40, 5);
	
	ST7735S_DrawFillRectangle(x0 + 1, y0 + 1, widthCharge, heightCharge - 2, colorCharge);
	ST7735S_DrawFillRectangle(x0 + 1, y0 + heightCharge - 1, widthCharge, 2, dcolorCharge);
	
	
	//��������� ������� �������
	uint16_t widthFrame;   	//�����  ������� ������� 
	uint16_t heightFrame;	 	//������ ������� ������� 
	uint16_t colorFrame;    //���� �������
	
	widthFrame = widthMaxCharge + 2; 
	heightFrame = heightCharge + 2;
	colorFrame = 0xFFFF;
	
	ST7735S_DrawRectangle(x0, y0, widthFrame, heightFrame, colorFrame);
	
	
	//��������� ��������� �������
	uint16_t widthContact;   	//�����  �������� ������� 
	uint16_t heightContact;	 	//������ �������� �������
	
	widthContact = 2;
	heightContact = heightCharge - 4;
	
	ST7735S_DrawFillRectangle(x0 + widthFrame, y0 + 3, widthContact, heightContact, colorFrame);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void gradientbattery(t_batt batt)
{	
	if(batt.flagFrstPlotBatt != 1)
		return;
	
	int16_t x0 = batt.x; 
	int16_t y0 = batt.y;  
	uint8_t percent = batt.prcnt;
		
	//��������� ���� ������ �������
	uint16_t widthMaxCharge;  //����� ������� ������ ������� ��� 100% ������	
	uint16_t widthCharge;			//����� ������� ������ ������� � ����������� �� percent
	uint16_t heightCharge;	 	//������ ������� ������ ������� 
	
	//���� ������� ������ ������� - ����� ���������  
	uint16_t colorChargeGreen;	    
	uint16_t colorChargeRed;
	uint16_t colorChargeYellow;
	
	uint16_t colorBckGrnd;
	
	widthMaxCharge = 25;
	widthCharge = widthMaxCharge * percent / 100; 
	
	heightCharge = 12;
	
	colorChargeGreen = ST7735S_SetColor16(0, 49, 5);
	colorChargeYellow = 0xFFE0;
	colorChargeRed = 0xF800;
	colorBckGrnd = scr1.color_bckgrnd;
	
	
	for(uint8_t i = 0; i < 12; i++)
	{
		ST7735S_DrawLine2Color(x0 + 1, y0 + 1 + i, (x0 + 1) + widthMaxCharge/2 - 1, y0 + 1 + i, colorChargeRed, colorChargeYellow);
		ST7735S_DrawLine2Color((x0 + 1) + widthMaxCharge/2, y0 + 1 + i, x0 + widthMaxCharge, y0 + 1 + i, colorChargeYellow, colorChargeGreen);
	}
	
	ST7735S_DrawFillRectangle(x0 + widthCharge, y0 + 1, widthMaxCharge - widthCharge, heightCharge, colorBckGrnd);
	
	
	//��������� ������� �������
	uint16_t widthFrame;   	//�����  ������� ������� 
	uint16_t heightFrame;	 	//������ ������� ������� 
	uint16_t colorFrame;    //���� �������
	
	widthFrame = widthMaxCharge + 2; 
	heightFrame = heightCharge + 2;
	colorFrame = 0xFFFF;
	
	ST7735S_DrawRectangle(x0, y0, widthFrame, heightFrame, colorFrame);
	
	//��������� ��������� �������
	uint16_t widthContact;   	//�����  �������� ������� 
	uint16_t heightContact;	 	//������ �������� �������
	
	widthContact = 2;
	heightContact = heightCharge - 4;
	
	ST7735S_DrawFillRectangle(x0 + widthFrame, y0 + 3, widthContact, heightContact, colorFrame);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void fsgrnyllwrdbattery(t_batt batt)
{	
	if(batt.flagFrstPlotBatt != 1)
		return;
	
	int16_t x0 = batt.x; 
	int16_t y0 = batt.y;  
	uint8_t percent = batt.prcnt;
	
	//��������� ���� ������ �������
	uint16_t widthMaxCharge;  		//����� ������� ������ ������� ��� 100% ������	
	uint16_t widthChargeSeg;			//����� ������ �������� ������
	uint16_t heightChargeSeg;		  //������ ������ �������� ������
	uint16_t heightCharge;	 			//������ ������� ������ �������
	
	uint16_t colorChargeGreen;	    //������� ���� ������ ������� 
	uint16_t colorChargeBrGreen;	  //������-������� ���� ������ �������
	uint16_t colorChargeYellow;	    //������ ���� ������ ������� 
	uint16_t colorChargeRed;	    	//������� ���� ������ ������� 
	
	widthMaxCharge = 25;
	widthChargeSeg = 5;
	heightCharge = 12;
	heightChargeSeg = 10;
	
	colorChargeGreen = ST7735S_SetColor16(0, 48, 5);   
	colorChargeBrGreen = ST7735S_SetColor16(19, 61, 0);
	colorChargeYellow = ST7735_COLOR_YELLOW;	    
	colorChargeRed = ST7735_COLOR_RED;	    	
	
	if( (percent <= 100) && (percent > 75) )
	{
		for(uint8_t i = 0; i < 4; i++)
				ST7735S_DrawFillRectangle(x0 + 2 + i + i*widthChargeSeg, y0 + 2, widthChargeSeg, heightChargeSeg, colorChargeGreen);
	}
	
	if( (percent <= 75) && (percent > 50) )
	{
		for(uint8_t i = 0; i < 3; i++)
				ST7735S_DrawFillRectangle(x0 + 2 + i + i*widthChargeSeg, y0 + 2, widthChargeSeg, heightChargeSeg, colorChargeBrGreen);
	}
	
	if( (percent <= 50) && (percent > 25) )
	{
		ST7735S_DrawFillRectangle(x0 + 2, y0 + 2, widthChargeSeg, heightChargeSeg, colorChargeYellow);
		ST7735S_DrawFillRectangle(x0 + 3 + widthChargeSeg, y0 + 2, widthChargeSeg, heightChargeSeg, colorChargeYellow);
	}
	
	if( (percent <= 25) && (percent > 0) )
				ST7735S_DrawFillRectangle(x0 + 2, y0 + 2, widthChargeSeg, heightChargeSeg, colorChargeRed);
	
	
	//��������� ������� �������
	uint16_t widthFrame;   	//�����  ������� ������� 
	uint16_t heightFrame;	 	//������ ������� ������� 
	uint16_t colorFrame;    //���� �������
	
	widthFrame = widthMaxCharge + 2; 
	heightFrame = heightCharge + 2;
	colorFrame = 0xFFFF;
	
	ST7735S_DrawRectangle(x0, y0, widthFrame, heightFrame, colorFrame);
	
	
	//��������� ��������� �������
	uint16_t widthContact;   	//�����  �������� ������� 
	uint16_t heightContact;	 	//������ �������� �������
	
	widthContact = 2;
	heightContact = heightCharge - 4;
	
	ST7735S_DrawFillRectangle(x0 + widthFrame, y0 + 3, widthContact, heightContact, colorFrame);
}
//----------------------------------------------------------------------------


//================================================================================================================================================

//----------------------------------------------------------------------------
void batt_Init(t_batt *ptrbatt, void (*ptrdesign)(t_batt), int16_t x0, int16_t y0)
{
	//��������� � ��������� ����������
	ptrbatt -> vout = 0;
	
	//������� ������ �������
	ptrbatt -> prcnt  = 0;
	
	ptrbatt -> x = x0;
	ptrbatt -> y = y0;
	
	ptrbatt -> design = *ptrdesign;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void batt_ChrctrstcProscessing(t_batt *ptrbatt)
{
	ptrbatt -> vout = batt_GetVoltage();
	ST7735S_SetCursor(50, 50, 0);
	ST7735S_PrintFloatNum(ptrbatt -> vout, ST7735_COLOR_WHITE, 1, 1);
	ptrbatt -> prcnt = batt_GetPercent(ptrbatt -> vout);
}
	
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void batt_Plot(t_batt batt)
{
	batt.flagFrstPlotBatt = 1;
	batt.design(batt);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void batt_PrintPercent(t_batt batt, int16_t x0, int16_t y0, uint16_t color)
{
	ST7735S_SetCursor(x0, y0, 0);
	
	uint8_t prcnt = batt.prcnt;
	
	ST7735S_PrintIntNum(prcnt, ST7735_COLOR_WHITE, 1, 1);
	
	// ������� ���������� ���� � ����� prcnt
	uint8_t cnt = 0; 
  while(prcnt != 0)
  {
      prcnt = prcnt / 10;
      ++cnt;
  }
	
	uint8_t indent; // ���������� �� ������ ������ ���� �� ������� '%'
	switch (cnt)
	{
		case 1: indent = 6;
						ST7735S_SetCursor(x0 + indent, y0, 0);
						ST7735S_PrintChar('%', color, 1, 1);
						break;
		case 2: indent = 12;
						ST7735S_SetCursor(x0 + indent, y0, 0);
						ST7735S_PrintChar('%', color, 1, 1);
						break;
		case 3: indent = 18;
						ST7735S_SetCursor(x0 + indent, y0, 0);
						ST7735S_PrintChar('%', color, 1, 1);
						break;
	}
}
//----------------------------------------------------------------------------
