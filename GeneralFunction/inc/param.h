#ifndef _PARAM_H
#define _PARAM_H
#include "DJmotor.h"
#include "stdbool.h"
#include "stm32f4xx.h"

typedef struct {
	volatile bool StepCatchFlag;	// �����־λ
	volatile bool FireSpeedCheckFlag;
	volatile bool FireSpeedArrviedFlag;
	volatile bool TakeRingFlag;		   // ȡ����־λ
	volatile bool TakeRingOverFlag;	   // ȡ����ɱ�־λ ��������Ƿ���Ҫ

	volatile bool CAN1_ControlList_Enable;
	volatile bool CAN2_ControlList_Enable;
	volatile bool CAN1SendQueueFULL;
	volatile bool CAN2SendQueueFULL;
	volatile bool CANSendQueueEMPTY;

	// TODO: ����ʵ����Ҫ��ӱ�־λ
} FlagType;

typedef struct {
	volatile bool AllowFlag;
	uint16_t	  BeepOnNum;
	volatile bool error;
	uint8_t		  error_cnt;
} BeepStructType;

typedef struct {
	volatile bool Normal;
	volatile bool Stuck;
	volatile bool CANErr;
	volatile bool Timeout;
	volatile bool CloseAll;
} LEDStructType;

extern FlagType		  Flag;
extern BeepStructType Beep;
extern LEDStructType  Led;
// extern Usart_StructType usart;

void param_Init(void);

#endif
