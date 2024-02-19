#include "main.h"

// 定义信号量

// OS_EVENT* XXXSem;
// INT8U*	  ErrorXXXSem;

extern CanRxMsg			RX_message;	   // Can1Funtion() 和 R1_Task() 共用的变量
extern volatile uint8_t ChooseFlag;
extern int				Cnt6;
extern uint8_t			SeedPaw1Cylinder;	 // 左。取秧气缸的位（倒数第几位）
extern uint8_t			SeedPaw2Cylinder;
extern int				A1;
extern int				A2;
extern int				kai;

// DJ顶部263，底端0.   Z:放苗位置： 783.795898
// DJ 放苗 160.0

int main(void)
{
	SystemInit();
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);
	CAN1_Init();
	CAN2_Init();
	Tim3_Init();
	Tim2_Init();

	CAN_InitSendQueue();
	param_Init();

	BEEP_Init();
	LED_Init();
	DJmotorInit();
	Zdrive_Init();

	Valve_Init();
	MechInit();	   // 需要修改（在最下面）

	OSInit();
	OSTaskCreate(Start_Task, (void*) 0, (OS_STK*) &START_TASK_STK[TASK_STK_SIZE - 1], START_TASK_PRIO);
	OSStart();
}

static void Start_Task(void* pdata)
{
	OS_CPU_SR cpu_sr = 0;
	pdata			 = pdata;

	OS_CPU_SysTickInit();	 // 开启OS时钟
	OS_ENTER_CRITICAL();	 // 进入临界区

	// 创建任务
	OSTaskCreate(TestOrder_Task, (void*) 0, (OS_STK*) &TESTORDER_TASK_STK[TASK_STK_SIZE - 1], TESTORDER_TASK_PRIO);	   // 测试 17

	OSTaskCreate(R1_Task, (void*) 0, (OS_STK*) &R1_TASK_STK[TASK_STK_SIZE - 1], R1_TASK_PRIO);			// 测试 15
	OSTaskCreate(Beep_Task, (void*) 0, (OS_STK*) &BEEP_TASK_STK[TASK_STK_SIZE - 1], BEEP_TASK_PRIO);	// 42
	OSTaskCreate(Led_Task, (void*) 0, (OS_STK*) &LED_TASK_STK[TASK_STK_SIZE - 1], LED_TASK_PRIO);		// 43

	OSTaskSuspend(START_TASK_PRIO);	   // 挂起起始任务

	OS_EXIT_CRITICAL();	   // 退出临界区
	OSTimeDly(100);
}

// 17
static void TestOrder_Task(void* pdata)
{
	pdata = pdata;
	while (1) { OSTimeDly(100); }
}

// 15
static void R1_Task(void* pdata)
{
	pdata = pdata;

	// 这里含有全局被动技能：上一次操作执行完毕之前，不得执行下一次操作。会杀死指令
	while (1) {
		switch (ChooseFlag) {
			case 1:
				MechEnable();	 // 使能
				ChooseFlag = 0xff;
				break;
			case 2:
				FindZero();	   // 寻零
				ChooseFlag = 0xff;
				break;

			case 3:
				Seeding();
				ChooseFlag = 0xff;
				break;

			default:
				break;
		}
		OSTimeDly(100);

		if (kai)
			Valve_Ctrl(CylinderCalculate(A1, A2));
		else {
		}

		if (kai == 6)
			Valve_Ctrl(255);
		if (kai == 7)
			Valve_Ctrl(0);
		if (kai == 8)
			Valve_Ctrl(1<<7);		
		if (kai == 9)
			Valve_Ctrl(1<<6);		
		if (kai == 10)
			Valve_Ctrl(1<<5);			
		
		
		
		
	}
}

// 42
static void Beep_Task(void* pdata)
{
	pdata = pdata;
	for (;;) {
		beeptask();
		OSTimeDly(1000);
	}
}

// 43
static void Led_Task(void* pdata)
{
	pdata = pdata;
	for (;;) {
		ledtask();
		OSTimeDly(5000);
	}
}
