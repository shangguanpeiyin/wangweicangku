#ifndef _DJMOTOR_H
#define _DJMOTOR_H
#include "can2.h"
#include "mathFunc.h"
#include "mech2.h"
#include "pid.h"
#include "stdbool.h"
#include "stm32f4xx.h"
#include "stm32f4xx_can.h"

/*-------------------define typedef enum-----------------*/

// // ��������ʹ�ö��ٸ��󽮣�DJ�����
// #define USE_M2006_NUM 6
// #define USE_M3508_NUM 0
// #define USE_DJ_NUM	  (USE_M2006_NUM + USE_M3508_NUM)	 // �󽮵������
#define USE_DJ_NUM 8	// �󽮵������

// ������ٱ�
#define M2006_RATIO 36
#define M3508_RAITO 19

#define ZERO_DISTANCE_THRESHOLD	 15
#define DuZhuan_THRESHOLD		 1.0f
#define Reach_THRESHOLD			 3.0f
#define SETLOCK_MAXDIS_THRESHOLD 10

// �������ģʽö����
enum Motor_Mode {
	DJ_RPM,	   // 0
	DJ_POSITION,
	DJ_ZERO,	// 2
	DJ_CURRENT,
};

// ���״̬����
typedef struct {
	volatile int16_t current;	 // ����

	volatile float	 angle;			// �����Ƕ�
	volatile int16_t speed;			// �������ٶȣ�ת��ת�٣�
	volatile float	 real_speed;	// ������ٶ�
	volatile float	 current_A;
	volatile int16_t pluse_read;		// ת�ӻ�е�Ƕȣ�������
	volatile int16_t pluse_distance;	// ��ǰ���������ϴ�������֮��
	volatile int32_t pluse_total;		// �ۼ�����
	volatile int8_t	 temperature;		// ���ڴ�C610���Բ��ᷴ���¶����ݡ�C620��DATA[6]�����¶�
	int16_t			 SetCurrent;
} DJMotorValue;

// �������ʵ�����
typedef struct {
	uint16_t PlusePerRound;		// ���ñ�����ÿȦ������
	uint8_t	 ReductionRatio;	// ������ٱ�
	float	 GearRatio;			// ����������
	uint32_t StdId;				// ID��
	uint32_t IDE;				// ��׼or��չ֡
	uint32_t RTR;				// ����orԶ��֡
	uint8_t	 DLC;
	int16_t	 Current_Limit;	   // ��ͬ��������Ƶ���
} DJMotorParam;

typedef struct {
	volatile int32_t  pluse_lock;	 // ��λ���ۼ�����
	volatile uint32_t LastRxTime;	 // ���յ������ʱ���¼
	uint16_t		  ZeroCnt;		 // Ѱ�������
	uint16_t		  PawLockCnt;
	uint16_t		  DuZhuanCnt;	 // ��ת������
	uint32_t		  MaxDistance;
	uint16_t		  Counter;
} DJMotorArgum;

// ��ֵ���ƽṹ��
typedef struct {
	// ���Ʊ�־λ
	bool	RPMLimitFlag;	 // �ٶ�ģʽ�µ��ٶ�����
	bool	PosAngleLimitFlag;
	bool	POS_rpm_Flag;	 // λ��ģʽ�µ��ٶȻ�pid�������Ʊ�־λ
	bool	CurrentLimitFlag;
	// ��ֵ����
	float	MaxAngle;
	float	MinAngle;
	int16_t RPMMaxSpeed;
	int32_t POS_rpmLimit;	 // λ��ģʽ�µ���������ٶ� ����ʵ�����Ƕ�pid�����м�����������
	int16_t MaxCurrent;
	int16_t ZeroSpeed;	  // Ѱ������ٶ�
	int16_t ZeroCurrent;
	int16_t SetLockCurrent;
	bool	isLooseStuck;
} DJMotorLimit;

// �쳣��Ӧ�ṹ��
typedef struct {
	bool RleaseWhenStuckFlag;	  // ����ת���Ƿ��ͷ�
	bool StuckDetectionFlag;	  // ��ת���
	bool TimeoutDetectionFlag;	  // ��ʱ���

	uint16_t DuanLianCnt;	 // ������Ƿ����

	uint16_t StuckCount;
	uint16_t TimeoutCount;
} DJMotorError;

// ���ִ��״̬���
typedef struct {
	volatile bool ArrrivedFlag;
	volatile bool ZeroFlag;
	volatile bool OvertimeFlag;
	volatile bool StuckFlag;
	volatile bool isSetZero;

} DJMotorStatus;

// �������ṹ��
typedef struct {
	uint8_t		  id;	 // 1-8
	volatile bool enable;
	volatile bool begin;
	uint8_t		  mode;
	float		  I_A;
	DJMotorParam  Param;
	DJMotorValue  ValueSet, ValueNow, ValuePre;
	DJMotorArgum  Argum;
	DJMotorLimit  Limit;

	DJMotorError  Error;
	DJMotorStatus StatusFlag;

	PID_Type POS_PID, RPM_PID;

	float Cylinder;			// ����������ж�Ӧ���ף����ﴢ�����׵ı�ţ�8λ�е����ڼ�λ��
	float SteeringAngle;	// �����������ȡ���������ﴢ���Ӧ��ת��
} DJMOTOR, *DJMOTORpointer;
/*-------------end of define typedef enum-----------------*/

/*-------------------ȫ������ �� ��������-------------------*/
extern DJMOTOR DJmotor[8];

void DJmotorInit(void);
void DJ_ReceiveData_CAN2(CanRxMsg Rx_message0);
void DJ_Position_Calculate(DJMOTORpointer motor);
void DJ_SetZero(DJMOTORpointer motor);

void DJ_LockPosition(DJMOTORpointer motor);
void DJ_CurretnTransmit(DJMOTORpointer motor, uint8_t i);
void DJ_SpeedMode(DJMOTORpointer motor);
void DJ_PositionMode(DJMOTORpointer motor);
void DJ_ZeroMode(DJMOTORpointer motor);
void DJ_CurretnTransmit(DJMOTORpointer motor, uint8_t i);
void Monitoring(DJMOTORpointer motor);

void DJ_Func(void);
/*---------------------------------------------------------*/

#endif
