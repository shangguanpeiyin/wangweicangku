
#include "Zdrive.h"

ZdriveTypedef	   Zdrive[USE_ZDRIVE_NUM];
ZdriveParamTypedef zdriveParam;

/**************��ʼ������**************/
void Zdrive_Init(void)
{
	// TODO:��������У��
	zdriveParam.GearRadio	   = 1;
	zdriveParam.ReductionRatio = 2.f;	 // �ܼ��ٱ�
	for (int i = 0; i < USE_ZDRIVE_NUM; i++) {
		Zdrive[i].id			   = i + 1;
		Zdrive[i].errClearFlag	   = false;
		Zdrive[i].Param			   = zdriveParam;
		Zdrive[i].enable		   = false;
		Zdrive[i].begin			   = false;
		Zdrive[i].mode			   = Zdrive_Disable;
		Zdrive[i].ValueSet.speed   = 0;
		Zdrive[i].Argum.lockAngle  = 0;
		Zdrive[i].PVTMode.PVTStage = 0;
	}
};

/**************���ú���**************/
void Zdrive_SetMode(float mode, u8 id)	  // 0x3D
{
	if (IncZDriveRear == ZDrive_SendQueue.Front) {
		Flag.CAN2SendQueueFULL++;
		return;
	}

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0] = 0x3D;
	memcpy(&(ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[1]), &mode, sizeof(float));

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 5;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;
}

void Zdrive_SetSpeed(float speed, u8 id)	// 0x45
{
	if (IncZDriveRear == ZDrive_SendQueue.Front) {
		Flag.CAN2SendQueueFULL++;
		return;
	}

	speed														 /= 60.f;	 // rpm => r/s
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0]  = 0x45;
	memcpy(&(ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[1]), &speed, sizeof(float));

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 5;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;
};

void Zdrive_SetPosition(float position, u8 id)	  // 0x47
{
	if (IncZDriveRear == ZDrive_SendQueue.Front) {
		Flag.CAN2SendQueueFULL++;
		return;
	}

	position													 /= 360.f;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0]  = 0x47;
	memcpy(&(ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[1]), &position, sizeof(float));

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 5;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;
};

void Zdrive_SetAcceleration(float acceleration, u8 id)	  // 0x29
{
	if (IncZDriveRear == ZDrive_SendQueue.Front) {
		Flag.CAN2SendQueueFULL++;
		return;
	}

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0] = 0x29;
	memcpy(&(ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[1]), &acceleration, sizeof(float));

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 5;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;
}
void Zdrive_SetDeceleration(float deceleration, u8 id)	  // 0x2B
{
	if (IncZDriveRear == ZDrive_SendQueue.Front) {
		Flag.CAN2SendQueueFULL++;
		return;
	}

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0] = 0x2B;
	memcpy(&(ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[1]), &deceleration, sizeof(float));

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 5;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;
}

void Zdrive_SetZero(u8 id)
{
	if (IncZDriveRear == ZDrive_SendQueue.Front) {
		Flag.CAN2SendQueueFULL++;
		return;
	}

	Zdrive[id - 1].ValueSet.angle								 = 0;

	u8 pos														 = 0;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0] = 0x5F;
	memcpy(&(ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[1]), &pos, sizeof(float));

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 5;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;
}

void Zdrive_Set_p_PID(float p_p, float p_i, u8 id)	  // 0x21,0x23
{
	if (IncZDriveRear == ZDrive_SendQueue.Front) {
		Flag.CAN2SendQueueFULL++;
		return;
	}

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0] = 0x21;
	memcpy(&(ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[1]), &p_p, sizeof(float));

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 5;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0]	  = 0x23;
	memcpy(&(ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[1]), &p_i, sizeof(float));

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 5;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;
}

void Zdrive_Set_v_PID(float v_p, float v_i, u8 id)	  // 0x25,0x27
{
	if (IncZDriveRear == ZDrive_SendQueue.Front) {
		Flag.CAN2SendQueueFULL++;
		return;
	}

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0] = 0x25;
	memcpy(&(ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[1]), &v_p, sizeof(float));

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 5;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0]	  = 0x27;
	memcpy(&(ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[1]), &v_i, sizeof(float));

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 5;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;
}

void Zdrive_SetVLimit(float vlimit, u8 id)	  // 0x15
{
	if (IncZDriveRear == ZDrive_SendQueue.Front) {
		Flag.CAN2SendQueueFULL++;
		return;
	}

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0] = 0x15;
	memcpy(&(ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[1]), &vlimit, sizeof(float));

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 5;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;
}
/**************ѯ�ʺ���*************/
void Zdrive_AskSpeed(u8 id)	   // 0x5C
{
	if (IncZDriveRear == ZDrive_SendQueue.Front) {
		Flag.CAN2SendQueueFULL++;
		return;
	}

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0]	  = 0x5C;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 1;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;
};

void Zdrive_AskPosition(u8 id)	  // 0x5E
{
	if (IncZDriveRear == ZDrive_SendQueue.Front) {
		Flag.CAN2SendQueueFULL++;
		return;
	}

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0]	  = 0x5E;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 1;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;
};

void Zdrive_AskErr(u8 id)	 // 0x40
{
	if (IncZDriveRear == ZDrive_SendQueue.Front) {
		Flag.CAN2SendQueueFULL++;
		return;
	}

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0]	  = 0x40;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 1;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;
}

void Zdrive_AskCurrent(u8 id)	 // 0x52
{
	if (IncZDriveRear == ZDrive_SendQueue.Front) {
		Flag.CAN2SendQueueFULL++;
		return;
	}

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0]	  = 0x52;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 1;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;
}

void Zdrive_AskVoltage(u8 id)
{
	if (IncZDriveRear == ZDrive_SendQueue.Front) {
		Flag.CAN2SendQueueFULL++;
		return;
	}

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0]	  = 0x62;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 1;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;
}

void Zdrive_AskMode(u8 id)
{
	if (IncZDriveRear == ZDrive_SendQueue.Front) {
		Flag.CAN2SendQueueFULL++;
		return;
	}

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0]	  = 0x3C;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 1;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;
}

void Zdrive_Ask_p_PID(u8 id)	// 0x20,0x22
{
	if (IncZDriveRear == ZDrive_SendQueue.Front) {
		Flag.CAN2SendQueueFULL++;
		return;
	}

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0]	  = 0x20;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 1;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0]	  = 0x22;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 1;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;
}

void Zdrive_Ask_v_PID(u8 id)	// 0x24,0x26
{
	if (IncZDriveRear == ZDrive_SendQueue.Front) {
		Flag.CAN2SendQueueFULL++;
		return;
	}

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0]	  = 0x24;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 1;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;

	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].Data[0]	  = 0x26;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].ID			  = id;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].DLC		  = 1;
	ZDrive_SendQueue.CAN_DataSend[ZDrive_SendQueue.Rear].InConGrpFlag = false;
	ZDrive_SendQueue.Rear											  = IncZDriveRear;
}
/**************�ӿں���**************/
u16	  StuckCnt		 = 0;
bool  FirstFlag		 = 0;
bool  PIDFlag		 = false;
float Pos_SetCur[8]	 = {0, 0, 0, 0, 0, 0, 0, 0};
float Pos_LockCur[8] = {0, 0, 0, 0, 0, 0, 0, 0};
void  ZdriveFunction(void)
{
	int id = FireArmId - 1;	   // ֻ�õ�����һ�����
	{
		if (Zdrive[id].enable) {
			if (Zdrive[id].begin) {
				Zdrive[id].Argum.lockAngle = Zdrive[id].ValueReal.angle;
				switch (Zdrive[id].mode) {
					case Zdrive_Speed: {
						if (Zdrive[id].modeRead != Zdrive_Speed)
							Zdrive_SetMode((float) Zdrive_Speed, id + 1);
						Zdrive_SetSpeed(Zdrive[id].ValueSet.speed, id + 1);
						break;
					}
					case Zdrive_Position: {
						if (Zdrive[id].modeRead != Zdrive_Position)
							Zdrive_SetMode((float) Zdrive_Position, id + 1);

						if (Zdrive[id].PVTMode.PVTModeFlag == false)	// ����λ��ģʽ
						{
							if (Pos_SetCur[id] != Zdrive[id].ValueSet.angle) {
								Zdrive_SetPosition(Zdrive[id].ValueSet.angle * Zdrive[id].Param.ReductionRatio, id + 1);
								Pos_SetCur[id] = Zdrive[id].ValueSet.angle;
							}
						} else	  // PVTģʽ
						{
							// ����ʹ��
							Zdrive_SetAcceleration((float) 0, id + 1);
							Zdrive_SetDeceleration(Zdrive[id].PVTMode.ReachTime[Zdrive[id].PVTMode.PVTStage], id + 1);
							Zdrive_SetSpeed(Zdrive[id].PVTMode.TargetSpeed[Zdrive[id].PVTMode.PVTStage], id + 1);
							Zdrive_SetPosition(
								Zdrive[id].PVTMode.TargetAngle[Zdrive[id].PVTMode.PVTStage] * Zdrive[id].Param.ReductionRatio,
								id + 1);
							if (ABS(Zdrive[id].ValueReal.angle
									- Zdrive[id].PVTMode.TargetAngle[Zdrive[id].PVTMode.PVTStage] * Zdrive[id].Param.ReductionRatio)
								< 1.0f) {
								Zdrive[id].PVTMode.PVTArrivedCnt++;
								if (Zdrive[id].PVTMode.PVTArrivedCnt >= 50) {
									Zdrive[id].PVTMode.PVTArrivedCnt = 0;
									Zdrive[id].PVTMode.PVTStage++;
									if (Zdrive[id].PVTMode.PVTStage == 3) {
										Zdrive[id].PVTMode.PVTStage	   = 0;
										Zdrive[id].PVTMode.PVTModeFlag = false;
										Zdrive[id].ValueSet.angle = Zdrive[id].ValueReal.angle / Zdrive[id].Param.ReductionRatio;
									}
								}
							}

							// ����ʹ��
							//							if(FirstFlag==0)
							//							{
							//								Zdrive_SetAcceleration((float)0,id+1);
							//								Zdrive_SetDeceleration(Zdrive[id].PVTMode.ReachTime[Zdrive[id].PVTMode.PVTStage],id+1);
							//								Zdrive_SetSpeed(Zdrive[id].PVTMode.TargetSpeed[Zdrive[id].PVTMode.PVTStage],id+1);
							//								Zdrive_SetPosition(Zdrive[id].PVTMode.TargetAngle[Zdrive[id].PVTMode.PVTStage]*Zdrive[id].Param.ReductionRatio,id+1);
							//								FirstFlag=1;
							//							}
							//							if(ABS(Zdrive[id].ValueReal.angle-Zdrive[id].PVTMode.TargetAngle[Zdrive[id].PVTMode.PVTStage]*Zdrive[id].Param.ReductionRatio)<5.0f)
							//							{
							//								Zdrive[id].PVTMode.PVTArrivedCnt++;
							//								if(Zdrive[id].PVTMode.PVTArrivedCnt>=50)
							//								{
							//									Zdrive[id].PVTMode.PVTArrivedCnt=0;
							//									Zdrive[id].PVTMode.PVTModeFlag=false;
							//									FirstFlag=0;
							//									Zdrive[id].ValueSet.angle=Zdrive[id].PVTMode.TargetAngle[Zdrive[id].PVTMode.PVTStage]/Zdrive[id].Param.ReductionRatio;
							//									Zdrive_SetAcceleration((float)20,id+1);
							//									Zdrive_SetDeceleration((float)10,id+1);
							//								}
							//							}
							//
						}
						break;
					}
					default:
						break;
				}
			} else {
				if (Zdrive[id].modeRead != Zdrive_Position)
					Zdrive_SetMode((float) Zdrive_Position, id + 1);
				if (ABS(Zdrive[id].ValueReal.angle - Zdrive[id].Argum.lockAngle * Zdrive[id].Param.ReductionRatio) > 2.0f)
					if (Pos_LockCur[id] != Zdrive[id].Argum.lockAngle) {
						Zdrive_SetPosition(Zdrive[id].Argum.lockAngle, id + 1);
						Pos_LockCur[id] = Zdrive[id].Argum.lockAngle;
					}
			}
		} else {
			if (Zdrive[id].modeRead != Zdrive_Disable)
				Zdrive_SetMode((float) Zdrive_Disable, id + 1);
		}

		//		if(PIDFlag)
		//		{
		//			Zdrive_Set_p_PID(Zdrive[id].PID.p_p,Zdrive[id].PID.p_i,id+1);
		//			Zdrive_Set_v_PID(Zdrive[id].PID.v_p,Zdrive[id].PID.v_i,id+1);
		//			PIDFlag=false;
		//		}

		Zdrive_AskErr(id + 1);
		Zdrive_AskCurrent(id + 1);
		Zdrive_AskSpeed(id + 1);
		Zdrive_AskPosition(id + 1);
		Zdrive_AskMode(id + 1);
		//		Zdrive_Ask_p_PID(id+1);
		//		Zdrive_Ask_v_PID(id+1);

		if (Zdrive[id].Argum.DuanLianCnt++ > 200)
			AnswerError(IdTransmiter(FireArmId), DuanLian);	   // ����ֻ����FireArmId����id+1
	}
};

bool ReceiveFlag;
void ZdriveReceiveHandler(CanRxMsg rx_message)
{
	u8 NodeID					  = rx_message.StdId;
	u8 CommandID				  = rx_message.Data[0];

	Zdrive[NodeID - 1].Status.err = Zdrive_Well;				   // û�з����������������
	Zdrive[NodeID - 1].ValuePre = Zdrive[NodeID - 1].ValueReal;	   // ��ʱ���� ��� ֮ǰ���ݣ�Ȼ���ʱ���ݽ��и���
	switch (CommandID) {
		case 0x3C:	  // Mode
		{
			float tempmode;
			memcpy(&tempmode, &rx_message.Data[1], sizeof(float));
			Zdrive[NodeID - 1].modeRead = (Zdrive_Mode) tempmode;
			break;
		}
		case 0x40:	  // Error
		{
			float temperr;
			memcpy(&temperr, &rx_message.Data[1], sizeof(float));
			Zdrive[NodeID - 1].Status.err = (Zdrive_Err) temperr;	 // �д�͸�д
			break;
		}
		case 0x52:	  // Current
		{
			memcpy(&(Zdrive[NodeID - 1].ValueReal.current), &rx_message.Data[1], sizeof(float));
			break;
		}
		case 0x5C:	  // Speed(rpm)
		{
			memcpy(&(Zdrive[NodeID - 1].ValueReal.speed), &rx_message.Data[1], sizeof(float));
			Zdrive[NodeID - 1].ValueReal.speed *= 60;
			break;
		}
		case 0x5E:	  // Position
		{
			memcpy(&(Zdrive[NodeID - 1].ValueReal.angle), &rx_message.Data[1], sizeof(float));
			Zdrive[NodeID - 1].ValueReal.angle
				= Zdrive[NodeID - 1].ValueReal.angle * (360.f / Zdrive[NodeID - 1].Param.ReductionRatio);
			break;
		}
		case 0x20:	  // p_p
		{
			memcpy(&(Zdrive[NodeID - 1].PIDRead.p_p), &rx_message.Data[1], sizeof(float));
			break;
		}
		case 0x22:	  // p_i
		{
			memcpy(&(Zdrive[NodeID - 1].PIDRead.p_i), &rx_message.Data[1], sizeof(float));
			break;
		}
		case 0x24:	  // v_p
		{
			memcpy(&(Zdrive[NodeID - 1].PIDRead.v_p), &rx_message.Data[1], sizeof(float));
			break;
		}
		case 0x26:	  // v_i
		{
			memcpy(&(Zdrive[NodeID - 1].PIDRead.v_i), &rx_message.Data[1], sizeof(float));
			break;
		}
		default:
			break;
	}

	Zdrive[NodeID - 1].Argum.DuanLianCnt = 0;
};
