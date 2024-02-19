#ifndef _ZDRIVE_H
#define _ZDRIVE_H

#include "includes.h"
#include "mathFunc.h"
#include "mech2.h"
#include "param.h"
#include "pid.h"
#include "queue.h"
#include "stdbool.h"
#include "stm32f4xx.h"
#include "stm32f4xx_can.h"

#define Zdrive_Zero_THRESHOLD 10

typedef enum {
	// mod,0ʧ��,1����ģʽ,2�ٶ�ģʽ,3λ��ģʽ,4����ģʽ,5������У׼,6���������Բ���,7������ƫ��У׼,8VKУ׼,9��������,10��������,11�������,12ɲ��,13Ѱ��
	Zdrive_Disable = 0,
	Zdrive_Current,
	Zdrive_Speed,
	Zdrive_Position,
	Zdrive_Test,
	Zdrive_RVCalibration,	 // 5
	Zdrive_EncoderLineCalibration,
	Zdrive_EncoudeOffsetCalibration,
	Zdrive_VKCalibration,
	Zdrive_SaveSetting,
	Zdrive_EraseSetting,
	Zdrive_ClearErr,	// ��������
	Zdrive_Brake,
	Zdrive_FindZero	   // Ѱ��
} Zdrive_Mode;
typedef enum {
	// err:0��,1�͵�ѹ,2����ѹ,3��������,4������,5����,6�������,7��й���,8����������,9��������ƥ��,10KVУ׼ʧ��,11ģʽ���Ϸ�,12��������,13����
	Zdrive_Well,
	Zdrive_InsufficientVoltage,
	Zdrive_OverVoltage,
	Zdrive_InstabilityCurrent,
	Zdrive_OverCurrent,
	Zdrive_OverSpeed,
	Zdrive_ExcessiveR,
	Zdrive_ExcessiveInductence,
	Zdrive_LoseEncoder1,
	Zdrive_PolesErr,
	Zdrive_VKCalibrationErr,
	Zdrive_ModeErr,			// ģʽ���Ϸ�
	Zdrive_ParameterErr,	// ��������
	Zdrive_Hot
} Zdrive_Err;
typedef struct {
	float current;
	float speed;
	float angle;
} ZdriveValueTypedef;

typedef struct {
	bool timeout;
	bool stuck;
	int	 FindZeroCnt;

	Zdrive_Err err;
} ZdriveStatusTypedef;

typedef struct {
	uint32_t lastRxTim;
	uint32_t timeoutTicks;

	uint32_t stuckcnt;		 // �жϸõ���Ƿ��ת���������ط���Ϣ����
	uint16_t DuanLianCnt;	 // ������Ƿ����

	float lockAngle;
	float vlimit;
	float FindZeroSpeed;	// Ѱ���ٶ�
} ZdriveArgumTypedef;

typedef struct {
	float GearRadio;
	float ReductionRatio;
} ZdriveParamTypedef;

typedef struct {
	bool  PVTModeFlag;
	u8	  PVTStage, PVTArrivedCnt;
	float ReachTime[3];
	float TargetSpeed[3];
	float TargetAngle[3];
} ZdrivePVTModeTypedef;

typedef struct {
	float p_p;
	float p_i;
	float v_p;
	float v_i;
} ZdrivePIDTypedef;

typedef struct {
	int					 id;
	bool				 enable;
	bool				 begin;
	bool				 Brake;
	bool				 errClearFlag;
	Zdrive_Mode			 mode;
	Zdrive_Mode			 modeRead;
	ZdriveParamTypedef	 Param;
	ZdrivePIDTypedef	 PID, PIDRead;
	ZdriveValueTypedef	 ValueSet, ValueReal, ValuePre;
	ZdriveStatusTypedef	 Status;
	ZdriveArgumTypedef	 Argum;
	ZdrivePVTModeTypedef PVTMode;
} ZdriveTypedef;

#define USE_ZDRIVE_NUM 1
extern ZdriveTypedef Zdrive[USE_ZDRIVE_NUM];	// pay attention when id=2(Zdrive[1])

void Zdrive_Init(void);

void Zdrive_SetMode(float mode, u8 id);
void Zdrive_SetSpeed(float speed, u8 id);
void Zdrive_SetPosition(float position, u8 id);
void Zdrive_SetAcceleration(float acceleration, u8 id);
void Zdrive_SetDeceleration(float deceleration, u8 id);
void Zdrive_Set_p_PID(float p_p, float p_i, u8 id);
void Zdrive_Set_v_PID(float v_p, float v_i, u8 id);
void Zdrive_SetZero(u8 id);	   // λ������
void Zdrive_SetVLimit(float vlimit, u8 id);
void Zdrive_FindingZero(u8 id);

void Zdrive_AskErr(u8 id);
void Zdrive_AskSpeed(u8 id);
void Zdrive_AskPosition(u8 id);
void Zdrive_AskVoltage(u8 id);
void Zdrive_AskMode(u8 id);
void Zdrive_Ask_p_PID(u8 id);
void Zdrive_Ask_v_PID(u8 id);
void Zdrive_AskVLimit(u8 id);

void ZdriveFunction(void);
void ZdriveReceiveHandler(CanRxMsg rx_message);

#endif
