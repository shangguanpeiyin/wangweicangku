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

// // 决定具体使用多少个大疆（DJ）电机
// #define USE_M2006_NUM 6
// #define USE_M3508_NUM 0
// #define USE_DJ_NUM	  (USE_M2006_NUM + USE_M3508_NUM)	 // 大疆电机总数
#define USE_DJ_NUM 8	// 大疆电机总数

// 电机减速比
#define M2006_RATIO 36
#define M3508_RAITO 19

#define ZERO_DISTANCE_THRESHOLD	 15
#define DuZhuan_THRESHOLD		 1.0f
#define Reach_THRESHOLD			 2.0f
#define SETLOCK_MAXDIS_THRESHOLD 10

// 电机工作模式枚举体
enum Motor_Mode {
	DJ_RPM,	   // 0
	DJ_POSITION,
	DJ_ZERO,	// 2
	DJ_CURRENT,
};

// 电机状态参数
typedef struct {
	volatile int16_t current;	 // 用神

	volatile float	 angle;			// 机构角度
	volatile int16_t speed;			// 输入轴速度（转子转速）
	volatile float	 real_speed;	// 输出轴速度
	volatile float	 current_A;
	volatile int16_t pluse_read;		// 转子机械角度，脉冲数
	volatile int16_t pluse_distance;	// 当前脉冲数与上次脉冲数之差
	volatile int32_t pluse_total;		// 累计脉冲
	volatile int8_t	 temperature;		// 对于大疆C610而言不会反馈温度数据。C620是DATA[6]返回温度
	int16_t			 SetCurrent;
} DJMotorValue;

// 电机物理实体参数
typedef struct {
	uint16_t PlusePerRound;		// 内置编码器每圈脉冲数
	uint8_t	 ReductionRatio;	// 电机减速比
	float	 GearRatio;			// 机构齿数比
	uint32_t StdId;				// ID号
	uint32_t IDE;				// 标准or扩展帧
	uint32_t RTR;				// 数据or远程帧
	uint8_t	 DLC;
	int16_t	 Current_Limit;	   // 不同电机的限制电流
} DJMotorParam;

typedef struct {
	volatile int32_t  pluse_lock;	 // 锁位置累计脉冲
	volatile uint32_t LastRxTime;	 // 接收电调报文时间记录
	uint16_t		  ZeroCnt;		 // 寻零计数器
	uint16_t		  PawLockCnt;
	uint16_t		  DuZhuanCnt;	 // 堵转计数器
	uint32_t		  MaxDistance;
	uint16_t		  Counter;
} DJMotorArgum;

// 数值限制结构体
typedef struct {
	// 限制标志位
	bool	RPMLimitFlag;	 // 速度模式下的速度限制
	bool	PosAngleLimitFlag;
	bool	POS_rpm_Flag;	 // 位置模式下的速度环pid计算限制标志位
	bool	CurrentLimitFlag;
	// 数值限制
	float	MaxAngle;
	float	MinAngle;
	int16_t RPMMaxSpeed;
	int32_t POS_rpmLimit;	 // 位置模式下的最大限制速度 这里实际上是对pid计算中间量进行限制
	int16_t MaxCurrent;
	int16_t ZeroSpeed;	  // 寻零最大速度
	int16_t ZeroCurrent;
	int16_t SetLockCurrent;
	bool	isLooseStuck;
} DJMotorLimit;

// 异常反应结构体
typedef struct {
	bool RleaseWhenStuckFlag;	  // 　堵转后是否释放
	bool StuckDetectionFlag;	  // 堵转监控
	bool TimeoutDetectionFlag;	  // 超时检测

	uint16_t DuanLianCnt;	 // 检测电机是否断连

	uint16_t StuckCount;
	uint16_t TimeoutCount;
} DJMotorError;

// 电机执行状态情况
typedef struct {
	volatile bool ArrrivedFlag;
	volatile bool ZeroFlag;
	volatile bool OvertimeFlag;
	volatile bool StuckFlag;
	volatile bool isSetZero;

} DJMotorStatus;

// 电机整体结构体
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

	float Cylinder;			// 如果这个电机有对应气缸，这里储存气缸的编号（8位中倒数第几位）
	float SteeringAngle;	// 如果这个电机是取球电机，这里储存对应回转角
} DJMOTOR, *DJMOTORpointer;
/*-------------end of define typedef enum-----------------*/

/*-------------------全局类型 和 函数声明-------------------*/
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
