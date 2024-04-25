#ifndef EVENTHANDLE_H
#define EVENTHANDLE_H

typedef struct
{
	float PowerActive;
	q15 Power;
	q15 PowerActive_LPF;
	q15 PowerReactive;
	q15 PowerReactive_LPF;
	q15 PowerApparent;
	q15 PowerApparent_LPF;
	q15 PowerApparent_Real;
	q15 BusCurrent;
	q15 BusCurrent_Pu;
	q15 BusCurrent_LPF;
	q15 Target_BusCurrent;
	q15 BusVoltage;
	float BusVoltageReal;
	float BusCurrentReal;
	float BusCurrentReal_LPF;
	q15 Coeff_LPF;
	q15 TemperSample;
	q15 TemperSample_LPF;
	float PowerReal;
	float PowerRealtimeComp;
}HWSAMPLE_PARAM;

typedef struct
{
	float PowerActive;
	q15 Power;
	q15 PowerActive_LPF;
	q15 PowerReactive;
	q15 PowerReactive_LPF;
	q15 PowerApparent;
	q15 PowerApparent_LPF;
	q15 PowerApparent_Real;
	q15 BusCurrent;
	q15 BusCurrent_Pu;
	q15 BusCurrent_LPF;
	q15 Target_BusCurrent;
	q15 BusVoltage;
	float BusVoltageReal;
	float BusCurrentReal;
	float BusCurrentReal_LPF;
	q15 Coeff_LPF;
	q15 TemperSample;
	q15 TemperSample_LPF;
	float PowerReal;
	float PowerRealtimeComp;
}RECONSTRUCT_PARAM;

#define RECONSTRUCT_ENABLE		(0)
	
#define POWER_COEFF_LPF 		(0.92)
#define FREQ_COEFF_LPF			(31129)
#define BUSCURRENT_COEFF_LPF	(31129)
extern q15 BusCurrent_Target;
extern q15 ElecFreq_LPF;
extern HWSAMPLE_PARAM		HWSample;
extern RECONSTRUCT_PARAM	Reconstruct;
extern q15 Get_CurrentRealtime(HWSAMPLE_PARAM *pHWSample);
extern void EventHandle_1ms(void);
extern q15 BusCurrentReconstructCalc(Foc_ObjType *pFocObj,RECONSTRUCT_PARAM *pReconstruct);
extern q15 Get_BusCurrentLimit(Foc_ObjType *pFocObj);
#endif