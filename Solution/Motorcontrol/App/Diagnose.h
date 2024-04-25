#ifndef DIAGNOSE_H
#define DIAGNOSE_H

typedef struct
{
	uint8_t SendDoneFlag;
	uint16_t SendDoneCnt;
	
}DIAG_INFOR_DEF;

typedef enum
{
	Condition_Meet = 0,
	Condition_NoMeet,
	
}STATUS_CONDITION_NOFAULT;

typedef enum
{
	Enable_DiagOutput = 0,
	Disable_DiagOutput,
}STATUS_DIAG_OUTPUT;

extern DIAG_INFOR_DEF Diag_Infor_DryRotor;
extern DIAG_INFOR_DEF Diag_Infor_StallOrOc;
extern DIAG_INFOR_DEF Diag_Infor_Ot;
extern DIAG_INFOR_DEF Diag_Infor_LossSpeed;
extern DIAG_INFOR_DEF Diag_Infor;
extern PRIOR_FAULTCODE_DEF Confirm_Prior(FAULT_ALL_TYPE_DEF *pFault_All_Type);
extern void PwmOut_DiagInfor(void);
extern void Duty_DiagInfor(uint16_t HighLeverTime,uint16_t LowLeverTime,PORT_ID_t port,PORT_GPIONO_t gpioNo,DIAG_INFOR_DEF *pDiag_Infor);
extern STATUS_CONDITION_NOFAULT Condition_Diag_NoFault(void);
extern STATUS_DIAG_OUTPUT Get_Status_DiagOutput(void);

#endif