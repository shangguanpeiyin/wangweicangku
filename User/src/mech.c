#include "mech.h"

// ���Դ���һ������(����һ���͹���)
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

CanRxMsg		 RX_message;	// Can1Funtion() �� R1_Task() ���õı���
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
	Mech.CatchAngle		  = 0;	  // 0��ȡ
	Mech.PutAngle		  = 260;
	Mech.DownAngle		  = 160;
	Mech.SteerAngle		  = 795;

	Zdrive_SetZero(1);
	Mech.UpDown->Limit.ZeroSpeed = -1000;

	Valve_Ctrl(CylinderCalculate(2, Open));	   // ����צ���ſ�
	Valve_Ctrl(CylinderCalculate(3, Open));
}

// ʹ��
void MechEnable()
{
	Mech.UpDown->enable	  = true;
	Mech.Steering->enable = true;
}

// Ѱ��
void FindZero()
{
	Mech.UpDown->mode  = DJ_ZERO;
	//	Mech.Steering->mode			  = Zdrive_FindZero;

	Mech.UpDown->begin = true;
	//	Mech.Steering->begin		  = true;

	//	Mech.Steering->mode			  = Zdrive_Position;
	//	Mech.Steering->ValueSet.angle = Mech.ZeroAngle;	   // 0�����ȡ��λ��
	//	Mech.Steering->begin		  = true;
	//	Zdrive_Check_place(Mech.Steering);

	Zdrive_SetZero(Mech.Steering->id);	  // ����Ϊ0�㡣

	OSTimeDly(10000);
}

int kongzhi = 0;
int wait	= 0;

void Seeding()
{
	if (Storage) {
		Valve_Ctrl(CylinderCalculate(2, Close));	// ����
		Valve_Ctrl(CylinderCalculate(3, Close));	// ��
		Valve_Ctrl(CylinderCalculate(4, Close));	// �������λ��
		OSTimeDly(5000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}									// �ֲ�����
		wait						= 0;	// 0��ʾû�еȴ���1��ʾ�ȴ�kongzhi��1
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // ��ֱ
		Mech.UpDown->ValueSet.angle = Mech.PutAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait						  = 0;
		kongzhi						  = 0;

		Mech.Steering->mode			  = Zdrive_Position;	// ��ת
		Mech.Steering->ValueSet.angle = Mech.SteerAngle;
		Mech.Steering->begin		  = true;
		Zdrive_Check_place(Mech.Steering);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // ��ֱ�����ȥ���磩
		Mech.UpDown->ValueSet.angle = Mech.DownAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(2, Open));	   // ����
		Valve_Ctrl(CylinderCalculate(3, Open));
		OSTimeDly(10000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait							= 0;
		kongzhi							= 0;

		Mech.UpDown->Limit.POS_rpmLimit = 1000;

		Mech.Steering->mode				= Zdrive_Position;	  // ��������һ��
		Mech.Steering->ValueSet.angle	= 0;				  // 0�����ȡ��λ��
		Mech.Steering->begin			= true;
		Mech.UpDown->mode				= DJ_POSITION;	  // ��ֱ
		Mech.UpDown->ValueSet.angle		= Mech.CatchAngle;
		Mech.UpDown->begin				= true;
		DJ_Check_place(Mech.UpDown);
		Zdrive_Check_place(Mech.Steering);

		Mech.UpDown->Limit.POS_rpmLimit = 3000;
		// �ص�ԭλ�ˣ��ٴ�ȡ�磨������븴�ƣ�΢�Ķ���

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(2, Close));	// ����
		Valve_Ctrl(CylinderCalculate(3, Close));
		Valve_Ctrl(CylinderCalculate(4, Open));	   // �������λ��
		OSTimeDly(5000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // ��ֱ
		Mech.UpDown->ValueSet.angle = Mech.PutAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait						  = 0;
		kongzhi						  = 0;

		Mech.Steering->mode			  = Zdrive_Position;	// ��ת
		Mech.Steering->ValueSet.angle = Mech.SteerAngle;
		Mech.Steering->begin		  = true;
		Zdrive_Check_place(Mech.Steering);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // ��ֱ�����ȥ���磩
		Mech.UpDown->ValueSet.angle = Mech.DownAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(2, Open));	   // ����
		Valve_Ctrl(CylinderCalculate(3, Open));
		OSTimeDly(5000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait							= 0;
		kongzhi							= 0;

		Mech.UpDown->Limit.POS_rpmLimit = 1000;

		Mech.Steering->mode				= Zdrive_Position;	  // ��������һ��
		Mech.Steering->ValueSet.angle	= 0;				  // 0�����ȡ��λ��
		Mech.Steering->begin			= true;
		Mech.UpDown->mode				= DJ_POSITION;	  // ��ֱ
		Mech.UpDown->ValueSet.angle		= Mech.CatchAngle;
		Mech.UpDown->begin				= true;
		DJ_Check_place(Mech.UpDown);
		Zdrive_Check_place(Mech.Steering);

		Mech.UpDown->Limit.POS_rpmLimit = 3000;
		// ���ˣ��������

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(2, Close));	// ����
		Valve_Ctrl(CylinderCalculate(3, Close));
		OSTimeDly(5000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // ��ֱ
		Mech.UpDown->ValueSet.angle = Mech.DownAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

	} else {
		Mech.UpDown->mode			= DJ_POSITION;	  // ��ֱ
		Mech.UpDown->ValueSet.angle = Mech.CatchAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(2, Open));	   // ����
		OSTimeDly(10000);
		Valve_Ctrl(CylinderCalculate(3, Open));	   // ��
		OSTimeDly(10000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // ��ֱ
		Mech.UpDown->ValueSet.angle = Mech.DownAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait						  = 0;
		kongzhi						  = 0;

		Mech.Steering->mode			  = Zdrive_Position;	// ��ת
		Mech.Steering->ValueSet.angle = Mech.SteerAngle;
		Mech.Steering->begin		  = true;
		Zdrive_Check_place(Mech.Steering);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(2, Close));	// ����
		Valve_Ctrl(CylinderCalculate(3, Close));
		OSTimeDly(10000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // ��ֱ,�γ���
		Mech.UpDown->ValueSet.angle = Mech.PutAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait						  = 0;
		kongzhi						  = 0;

		Mech.Steering->mode			  = Zdrive_Position;	// ��ת
		Mech.Steering->ValueSet.angle = 0;
		Mech.Steering->begin		  = true;
		Zdrive_Check_place(Mech.Steering);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // ��ֱ
		Mech.UpDown->ValueSet.angle = Mech.CatchAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(4, Close));	// �����������λ��

		// ѭ�����£�
		Valve_Ctrl(CylinderCalculate(2, Open));	   // ����
		OSTimeDly(10000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(3, Open));	   // ��
		OSTimeDly(10000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // ��ֱ
		Mech.UpDown->ValueSet.angle = Mech.DownAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait						  = 0;
		kongzhi						  = 0;

		Mech.Steering->mode			  = Zdrive_Position;	// ��ת
		Mech.Steering->ValueSet.angle = Mech.SteerAngle;
		Mech.Steering->begin		  = true;
		Zdrive_Check_place(Mech.Steering);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait	= 0;
		kongzhi = 0;

		Valve_Ctrl(CylinderCalculate(2, Close));	// ����
		Valve_Ctrl(CylinderCalculate(3, Close));
		OSTimeDly(10000);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // ��ֱ,�γ���
		Mech.UpDown->ValueSet.angle = Mech.PutAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait						  = 0;
		kongzhi						  = 0;

		Mech.Steering->mode			  = Zdrive_Position;	// ��ת
		Mech.Steering->ValueSet.angle = 0;
		Mech.Steering->begin		  = true;
		Zdrive_Check_place(Mech.Steering);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait						= 0;
		kongzhi						= 0;

		Mech.UpDown->mode			= DJ_POSITION;	  // ��ֱ
		Mech.UpDown->ValueSet.angle = Mech.CatchAngle;
		Mech.UpDown->begin			= true;
		DJ_Check_place(Mech.UpDown);

		while (kongzhi == 0) {
			wait = 1;
			if (kongzhi) {
				break;
			}
		}	 // �ֲ�����
		wait	= 0;
		kongzhi = 0;

		// ���
		Valve_Ctrl(CylinderCalculate(2, Open));	   // ����
		OSTimeDly(10000);
		Valve_Ctrl(CylinderCalculate(3, Open));	   // ��
		OSTimeDly(10000);
	}
}

// δ֪��������֮״̬������������һ������ �ļ��㡣IDΪ�����ڼ�λ
uint8_t CylinderCalculate(int Valve_Id, int command)
{
	switch (command) {
		case 1:
			CylinderControl = (1 << (8 - Valve_Id)) | CylinderControl;	  // ��λ ��1����������
			return CylinderControl;

		case 0:
			CylinderControl = ~(1 << (8 - Valve_Id)) & CylinderControl;	   // ��λ ��0����������
			return CylinderControl;
	}
	return 0;
}

// ���󽮵��λ��ģʽ�Ƿ�λ
void DJ_Check_place(DJMOTORpointer DJ)
{
	int ReachCnt = 0;
	//	int DuZhuanCnt = 0;

	if (DJ->mode == DJ_POSITION) {
		while (1) {
			// ����Ƿ�λ
			if (ABS(DJ->ValueNow.angle - DJ->ValueSet.angle) < 2.0f) {	  // �ۼ�С���ż�����Ϊ��λ
				if (ReachCnt++ > 999) {
					//					DuZhuanCnt = 0;
					ReachCnt  = 0;
					DJ->begin = false;	  // ����ر�
					break;
				}
			} else
				ReachCnt = 0;	 // ��ֹ ����żȻ�ɹ�

			// // ����Ƿ��ת
			// if (ABS(DJ->ValueNow.angle - DJ->ValuePre.angle) < DuZhuan_THRESHOLD) {
			// 	if (DuZhuanCnt++ > 60000) {
			// 		DuZhuanCnt = 0;
			// 		ReachCnt   = 0;
			// 		// AnswerError(IdTransmiter(DJ->id), DuZhuan);
			// 		while (1) {
			// 			Beep.BeepOnNum++;
			// 			OSTimeDly(500);	   // �����ˣ�������һֱ�죬����ͣ
			// 		}
			// 	}
			// } else
			// 	DuZhuanCnt = 0;	   // ��ֹ żȻ����
		}
	}
}

// ���Zdrive�ĵ��λ��ģʽ�Ƿ�λ
void Zdrive_Check_place(ZdriveTypedef* Zdri)
{
	int ReachCnt = 0;
	//	int DuZhuanCnt = 0;

	if (Zdri->mode == Zdrive_Position) {
		while (1) {
			// ����Ƿ�λ
			if (ABS(Zdri->ValueSet.angle - Zdri->ValueReal.angle) < Reach_THRESHOLD) {	  // �ۼ�С���ż�����Ϊ��λ
				if (ReachCnt++ > 999) {
					//					DuZhuanCnt	= 0;
					ReachCnt = 0;
					//					Zdri->begin = false;
					break;
				}
			} else
				ReachCnt = 0;	 // ��ֹ ����żȻ�ɹ�

			// // ����Ƿ��ת
			// if ((ABS(Zdri->ValuePre.angle - Zdri->ValueReal.angle) < Reach_THRESHOLD)) {
			// 	if (DuZhuanCnt++ > 60000) {
			// 		DuZhuanCnt = 0;
			// 		ReachCnt   = 0;
			// 		// AnswerError(IdTransmiter(DJ->id), DuZhuan);
			// 		while (1) {
			// 			Beep.BeepOnNum++;
			// 			OSTimeDly(500);	   // �����ˣ�������һֱ�죬����ͣ
			// 		}
			// 	}
			// } else
			// 	DuZhuanCnt = 0;	   // ��ֹ żȻ����
		}
	}
}

void CanBack(CanRxMsg rx_message)	 // ����ԭ����
{
	CanTxMsg message;

	message.IDE	  = CAN_Id_Extended;
	message.ExtId = 0x05020100 + (rx_message.ExtId & 0x0000000f);
	message.DLC	  = 2;
	message.RTR	  = CAN_RTR_Data;

	if (message.ExtId == 0x05020107) {	  // ��ID��Ҫ���� OK
		message.Data[0] = 'O';
		message.Data[1] = 'K';
	} else {	// ����ID����ԭ����
		message.Data[0] = rx_message.Data[0];
		message.Data[1] = rx_message.Data[1];
	}

	CAN_Transmit(CAN1, &message);
}

void Can1Funtion(CanRxMsg rx_message)
{
	RX_message = rx_message;	// ������Ϣ

	// ���ݲ�ͬ��Э��IDִ�в�ͬ�Ĳ���
	if (rx_message.ExtId == 0x01020501 && rx_message.Data[0] == 'M')	// ʹ��/ʧ��
		ChooseFlag = 1;

	if (rx_message.ExtId == 0x01020502 && rx_message.Data[0] == 'Z')	// Ѱ��
		ChooseFlag = 2;

	if (rx_message.ExtId == 0x01020503)	   // ���÷��丩����/��ת��/�ٶ�
		ChooseFlag = 3;

	if (rx_message.ExtId == 0x01020504 && rx_message.Data[0] == 'F')	// ���㷢��
		ChooseFlag = 4;

	if (rx_message.ExtId == 0x01020505 && rx_message.Data[0] == 'B')	// ����ȡ���צ����
		ChooseFlag = 5;

	if (rx_message.ExtId == 0x01020506 && rx_message.Data[0] == 'S')	// ȡ��/������
	{
		ChooseFlag = 6;
		Cnt6++;
	}

	if (rx_message.ExtId == 0x01020507 && rx_message.Data[0] == 'I' && rx_message.Data[1] == 'P')	 // �ص���ʼλ��
		ChooseFlag = 7;

	if (rx_message.ExtId == 0x010205FF && rx_message.Data[0] == 'R' && rx_message.Data[1] == 'S')	 // ���Ӹ�λ
	{
		__set_FAULTMASK(1);	   // �ر����е��жϣ�ȷ��ִ�и�λʱ�����жϴ��
		NVIC_SystemReset();	   // ϵͳ�����λ�����úõ�����Ĵ���Ҳһ��λ
	}
}
