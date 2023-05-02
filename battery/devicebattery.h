#ifndef DEVICEBATTERY_H_
#define DEVICEBATTERY_H_

//#define BATT_USER
#ifdef BATT_USER
	#define VOLTAGE_MAX // see datasheet
	#define VOLTAGE_MIN // see datasheet
#endif

//#define BATT_AA_R6
#ifdef BATT_AA_R6
	#define VOLTAGE_MAX 1.5
	#define VOLTAGE_MIN 0.8
#endif

#define BATT_AA_LR6
#ifdef BATT_AA_LR6
	#define VOLTAGE_MAX 1.5
	#define VOLTAGE_MIN 0.8
#endif

//#define BATT_AA_FR6
#ifdef BATT_AA_FR6
	#define VOLTAGE_MAX 1.5
	#define VOLTAGE_MIN 0.8
#endif

//#define BATT_AA_ZR6
#ifdef BATT_AA_ZR6
	#define VOLTAGE_MAX 1.65
	#define VOLTAGE_MIN ?? 
#endif

//#define BATT_AA_KR6
#ifdef BATT_AA_KR6
	#define VOLTAGE_MAX 1.25
	#define VOLTAGE_MIN ??
#endif

//#define BATT_AA_HR6
#ifdef BATT_AA_HR6
	#define VOLTAGE_MAX 1.25
	#define VOLTAGE_MIN ??
#endif

#endif // #ifndef DEVICEBATTERY