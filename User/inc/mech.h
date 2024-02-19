#ifndef _NEW_MECH_H
#define _NEW_MECH_H

#include "DJmotor.h"
#include "Zdrive.h"
#include "beep.h"
#include "cylinder.h"
#include "mathFunc.h"
#include "mech2.h"
#include "stm32f4xx.h"
#include "stm32f4xx_can.h"

// ����
void MechInit(void);	// ������ʼ��

void Can1Funtion(CanRxMsg rx_message);	  // can1�жϴ�����
void MechEnable(void);					  // ʹ��or ʧ��
void FindZero(void);					  // Ѱ��
void Seeding(void);

uint8_t CylinderCalculate(int Paw_Id, int command);

void DJ_Check_place(DJMOTORpointer DJ);
void Zdrive_Check_place(ZdriveTypedef* Zdri);

#endif
