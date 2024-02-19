#include "mech.h"

// 尝试创建一个对象(创建一个就够了)
struct {
	DJMOTOR*	   UpDown;
	ZdriveTypedef* Steering;
	float		   ZeroAngle;
	float		   CatchAngle;
	float		   PutAngle;
	float		   DownAngle;

	float SteerAngle;

} Mech;

uint8_t CylinderControl;

CanRxMsg		 RX_message;	// Can1Funtion() 和 R1_Task() 共用的变量
volatile uint8_t ChooseFlag;
int				 Cnt6;

int A1;
int A2;
int kai;
int Storage;

void MechInit()
{
	A1					  = 0;
	A2					  = 0;
	kai					  = 0;
	CylinderControl		  = 0x00;
	ChooseFlag			  = 0xff;
	Storage				  = 1;

	Mech.UpDown			  = &(DJmotor[1]);
	Mech.Steering		  = &(Zdrive[0]);

	Mech.UpDown->enable	  = false;
	Mech.UpDown->begin	  = false;
	Mech.Steering->enable = false;
	Mech.Steering->begin  = false;

	Mech.ZeroAngle		  = 0;
	Mech.CatchAngle		  = 0;	  // 0点取
	Mech.PutAngle		  = 260;
	Mech.DownAngle		  = 160;
	Mech.SteerAngle		  = 795;

	Zdrive_SetZero(1);
	Mech.UpDown->Limit.ZeroSpeed = -1000;

	Valve_Ctrl(CylinderCalculate(2, Open));	   // 保持爪子张开
	Valve_Ctrl(CylinderCalculate(3, Open));
}

// 使能
void MechEnable()
{
	Mech.UpDown->enable	  = true;
	Mech.Steering->enable = true;
}

// 寻零
void FindZero()
{
	Mech.UpDown->mode  = DJ_ZERO;
	//	Mech.Steering->mode			  = Zdrive_FindZero;

	Mech.UpDown->begin = true;
	//	Mech.Steering->begin		  = true;

	//	Mech.Steering->mode			  = Zdrive_Position;
	//	Mech.Steering->ValueSet.angle = Mech.ZeroAngle;	   // 0点就是取苗位置
	//	Mech.Steering->begin		  = true;
	//	Zdrive_Check_place(Mech.Steering);

	Zdrive_SetZero(Mech.Steering->id);	  // 设置为0点。

	OSTimeDly(10000);
}

int kongzhi = 0;
int wait	= 0;

void Seeding()
{
	if (Storage) {
		Valve_Ctrl(CylinderCalculate(2, Close));	// 夹苗
		Valve_Ctrl(CylinderCalculate(3, Close));	// 右
		Valve_Ctrl(CylinderCalculate(4, Close));	// 调整存放位置
		OSTimeDly(5000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}									// 分布控制
		wait						= 0;	// 0表示没有等待，1表示等待kongzhi置1
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // 垂直
		Mech.UpDown->ValueSet.angle = Mech.PutAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait						  = 0;
		kongzhi						  = 0;

		Mech.Steering->mode			  = Zdrive_Position;	// 回转
		Mech.Steering->ValueSet.angle = Mech.SteerAngle;
		Mech.Steering->begin		  = true;
		Zdrive_Check_place(Mech.Steering);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // 垂直（插进去放苗）
		Mech.UpDown->ValueSet.angle = Mech.DownAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(2, Open));	   // 存苗
		Valve_Ctrl(CylinderCalculate(3, Open));
		OSTimeDly(10000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait							= 0;
		kongzhi							= 0;

		Mech.UpDown->Limit.POS_rpmLimit = 1000;

		Mech.Steering->mode				= Zdrive_Position;	  // 回来再拿一次
		Mech.Steering->ValueSet.angle	= 0;				  // 0点就是取苗位置
		Mech.Steering->begin			= true;
		Mech.UpDown->mode				= DJ_POSITION;	  // 垂直
		Mech.UpDown->ValueSet.angle		= Mech.CatchAngle;
		Mech.UpDown->begin				= true;
		DJ_Check_place(Mech.UpDown);
		Zdrive_Check_place(Mech.Steering);

		Mech.UpDown->Limit.POS_rpmLimit = 3000;
		// 回到原位了，再次取苗（上面代码复制，微改动）

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(2, Close));	// 夹苗
		Valve_Ctrl(CylinderCalculate(3, Close));
		Valve_Ctrl(CylinderCalculate(4, Open));	   // 调整存放位置
		OSTimeDly(5000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // 垂直
		Mech.UpDown->ValueSet.angle = Mech.PutAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait						  = 0;
		kongzhi						  = 0;

		Mech.Steering->mode			  = Zdrive_Position;	// 回转
		Mech.Steering->ValueSet.angle = Mech.SteerAngle;
		Mech.Steering->begin		  = true;
		Zdrive_Check_place(Mech.Steering);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // 垂直（插进去放苗）
		Mech.UpDown->ValueSet.angle = Mech.DownAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(2, Open));	   // 存苗
		Valve_Ctrl(CylinderCalculate(3, Open));
		OSTimeDly(5000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait							= 0;
		kongzhi							= 0;

		Mech.UpDown->Limit.POS_rpmLimit = 1000;

		Mech.Steering->mode				= Zdrive_Position;	  // 回来再拿一次
		Mech.Steering->ValueSet.angle	= 0;				  // 0点就是取苗位置
		Mech.Steering->begin			= true;
		Mech.UpDown->mode				= DJ_POSITION;	  // 垂直
		Mech.UpDown->ValueSet.angle		= Mech.CatchAngle;
		Mech.UpDown->begin				= true;
		DJ_Check_place(Mech.UpDown);
		Zdrive_Check_place(Mech.Steering);

		Mech.UpDown->Limit.POS_rpmLimit = 3000;
		// 到此，二次完毕

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(2, Close));	// 夹苗
		Valve_Ctrl(CylinderCalculate(3, Close));
		OSTimeDly(5000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // 垂直
		Mech.UpDown->ValueSet.angle = Mech.DownAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

	} else {
		Mech.UpDown->mode			= DJ_POSITION;	  // 垂直
		Mech.UpDown->ValueSet.angle = Mech.CatchAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(2, Open));	   // 放苗
		OSTimeDly(10000);
		Valve_Ctrl(CylinderCalculate(3, Open));	   // 右
		OSTimeDly(10000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // 垂直
		Mech.UpDown->ValueSet.angle = Mech.DownAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait						  = 0;
		kongzhi						  = 0;

		Mech.Steering->mode			  = Zdrive_Position;	// 回转
		Mech.Steering->ValueSet.angle = Mech.SteerAngle;
		Mech.Steering->begin		  = true;
		Zdrive_Check_place(Mech.Steering);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(2, Close));	// 夹苗
		Valve_Ctrl(CylinderCalculate(3, Close));
		OSTimeDly(10000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // 垂直,拔出来
		Mech.UpDown->ValueSet.angle = Mech.PutAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait						  = 0;
		kongzhi						  = 0;

		Mech.Steering->mode			  = Zdrive_Position;	// 回转
		Mech.Steering->ValueSet.angle = 0;
		Mech.Steering->begin		  = true;
		Zdrive_Check_place(Mech.Steering);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // 垂直
		Mech.UpDown->ValueSet.angle = Mech.CatchAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(4, Close));	// 调整存苗机构位置

		// 循环如下：
		Valve_Ctrl(CylinderCalculate(2, Open));	   // 放苗
		OSTimeDly(10000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(3, Open));	   // 右
		OSTimeDly(10000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // 垂直
		Mech.UpDown->ValueSet.angle = Mech.DownAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait						  = 0;
		kongzhi						  = 0;

		Mech.Steering->mode			  = Zdrive_Position;	// 回转
		Mech.Steering->ValueSet.angle = Mech.SteerAngle;
		Mech.Steering->begin		  = true;
		Zdrive_Check_place(Mech.Steering);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(2, Close));	// 夹苗
		Valve_Ctrl(CylinderCalculate(3, Close));
		OSTimeDly(10000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // 垂直,拔出来
		Mech.UpDown->ValueSet.angle = Mech.PutAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait						  = 0;
		kongzhi						  = 0;

		Mech.Steering->mode			  = Zdrive_Position;	// 回转
		Mech.Steering->ValueSet.angle = 0;
		Mech.Steering->begin		  = true;
		Zdrive_Check_place(Mech.Steering);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // 垂直
		Mech.UpDown->ValueSet.angle = Mech.CatchAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // 分布控制
		wait	= 0;
		kongzhi = 0;

		// 最后
		Valve_Ctrl(CylinderCalculate(2, Open));	   // 放苗
		OSTimeDly(10000);
		Valve_Ctrl(CylinderCalculate(3, Open));	   // 右
		OSTimeDly(10000);
	}
}

// 未知其他气缸之状态，而单独开关一个气缸 的计算。ID为正数第几位
uint8_t CylinderCalculate(int Valve_Id, int command)
{
	switch (command) {
		case 1:
			CylinderControl = (1 << (8 - Valve_Id)) | CylinderControl;	  // 此位 置1，其他不变
			return CylinderControl;

		case 0:
			CylinderControl = ~(1 << (8 - Valve_Id)) & CylinderControl;	   // 此位 置0，其他不变
			return CylinderControl;
	}
	return 0;
}

// 检测大疆电机位置模式是否到位
void DJ_Check_place(DJMOTORpointer DJ)
{
	int ReachCnt = 0;
	//	int DuZhuanCnt = 0;

	if (DJ->mode == DJ_POSITION) {
		while (1) {
			// 检测是否到位
			if (ABS(DJ->ValueNow.angle - DJ->ValueSet.angle) < 2.0f) {	  // 累计小于门槛后，视为到位
				if (ReachCnt++ > 999) {
					//					DuZhuanCnt = 0;
					ReachCnt  = 0;
					DJ->begin = false;	  // 电机关闭
					break;
				}
			} else
				ReachCnt = 0;	 // 防止 积累偶然成功

			// // 检测是否堵转
			// if (ABS(DJ->ValueNow.angle - DJ->ValuePre.angle) < DuZhuan_THRESHOLD) {
			// 	if (DuZhuanCnt++ > 60000) {
			// 		DuZhuanCnt = 0;
			// 		ReachCnt   = 0;
			// 		// AnswerError(IdTransmiter(DJ->id), DuZhuan);
			// 		while (1) {
			// 			Beep.BeepOnNum++;
			// 			OSTimeDly(500);	   // 报错了，蜂鸣器一直响，不用停
			// 		}
			// 	}
			// } else
			// 	DuZhuanCnt = 0;	   // 防止 偶然积累
		}
	}
}

// 检测Zdrive的电机位置模式是否到位
void Zdrive_Check_place(ZdriveTypedef* Zdri)
{
	int ReachCnt = 0;
	//	int DuZhuanCnt = 0;

	if (Zdri->mode == Zdrive_Position) {
		while (1) {
			// 检测是否到位
			if (ABS(Zdri->ValueSet.angle - Zdri->ValueReal.angle) < Reach_THRESHOLD) {	  // 累计小于门槛后，视为到位
				if (ReachCnt++ > 999) {
					//					DuZhuanCnt	= 0;
					ReachCnt = 0;
					//					Zdri->begin = false;
					break;
				}
			} else
				ReachCnt = 0;	 // 防止 积累偶然成功

			// // 检测是否堵转
			// if ((ABS(Zdri->ValuePre.angle - Zdri->ValueReal.angle) < Reach_THRESHOLD)) {
			// 	if (DuZhuanCnt++ > 60000) {
			// 		DuZhuanCnt = 0;
			// 		ReachCnt   = 0;
			// 		// AnswerError(IdTransmiter(DJ->id), DuZhuan);
			// 		while (1) {
			// 			Beep.BeepOnNum++;
			// 			OSTimeDly(500);	   // 报错了，蜂鸣器一直响，不用停
			// 		}
			// 	}
			// } else
			// 	DuZhuanCnt = 0;	   // 防止 偶然积累
		}
	}
}

void CanBack(CanRxMsg rx_message)	 // 反馈原报文
{
	CanTxMsg message;

	message.IDE	  = CAN_Id_Extended;
	message.ExtId = 0x05020100 + (rx_message.ExtId & 0x0000000f);
	message.DLC	  = 2;
	message.RTR	  = CAN_RTR_Data;

	if (message.ExtId == 0x05020107) {	  // 此ID需要回馈 OK
		message.Data[0] = 'O';
		message.Data[1] = 'K';
	} else {	// 其余ID反馈原报文
		message.Data[0] = rx_message.Data[0];
		message.Data[1] = rx_message.Data[1];
	}

	CAN_Transmit(CAN1, &message);
}

void Can1Funtion(CanRxMsg rx_message)
{
	RX_message = rx_message;	// 传送消息

	// 根据不同的协议ID执行不同的操作
	if (rx_message.ExtId == 0x01020501 && rx_message.Data[0] == 'M')	// 使能/失能
		ChooseFlag = 1;

	if (rx_message.ExtId == 0x01020502 && rx_message.Data[0] == 'Z')	// 寻零
		ChooseFlag = 2;

	if (rx_message.ExtId == 0x01020503)	   // 设置发射俯仰角/回转角/速度
		ChooseFlag = 3;

	if (rx_message.ExtId == 0x01020504 && rx_message.Data[0] == 'F')	// 定点发射
		ChooseFlag = 4;

	if (rx_message.ExtId == 0x01020505 && rx_message.Data[0] == 'B')	// 左右取球夹爪动作
		ChooseFlag = 5;

	if (rx_message.ExtId == 0x01020506 && rx_message.Data[0] == 'S')	// 取秧/放秧动作
	{
		ChooseFlag = 6;
		Cnt6++;
	}

	if (rx_message.ExtId == 0x01020507 && rx_message.Data[0] == 'I' && rx_message.Data[1] == 'P')	 // 回到初始位置
		ChooseFlag = 7;

	if (rx_message.ExtId == 0x010205FF && rx_message.Data[0] == 'R' && rx_message.Data[1] == 'S')	 // 板子复位
	{
		__set_FAULTMASK(1);	   // 关闭所有的中断，确保执行复位时不被中断打断
		NVIC_SystemReset();	   // 系统软件复位，配置好的外设寄存器也一起复位
	}
}
