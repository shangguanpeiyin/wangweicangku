#include "controllist.h"

/**
 * @brief  initializ the can message control list
 * @param  CAN_MesgControlListMy Param doc
 * @param  CANx My Param doc
 */
void InitMesgControlList(MesgControlGrpType *CAN_MesgControlList, CAN_TypeDef *CANx)
{
    uint8_t NodeNumber = 0;
    if (CANx == CAN1)
    {
        Flag.CAN1_ControlList_Enable = ENABLE;
        NodeNumber = CAN1_NodeNumber;
    }
    else if (CANx == CAN2)
    {
        Flag.CAN2_ControlList_Enable = ENABLE;
        NodeNumber = CAN2_NodeNumber;
    }
    else
        return;
    for (int i = 0; i < NodeNumber; i++)
    {
        CAN_MesgControlList[i].SendSem = 0;
        CAN_MesgControlList[i].TimeOut = 0;
        CAN_MesgControlList[i].SendNumber = 0;
        CAN_MesgControlList[i].ReceiveNumber = 0;
        CAN_MesgControlList[i].SendQueue.Front = 0;
        CAN_MesgControlList[i].SendQueue.Rear = 0;
        CAN_MesgControlList[i].QueueFullTimeout = 0;
        CAN_MesgControlList[i].SendQueue.CAN_DataSend[0].InConGrpFlag = 0;
    }
}

/**
 * @brief ����Ҫ���ӵı��ķ����Ӧ���ӵĿ��ƿ���
 * @param  ControlList
 * @param  can_queue
 * @param  CANx
 */
void MesgConstrolList(MesgControlGrpType *ControlList, CAN_SendQueueType *can_queue, CAN_TypeDef *CANx)
{
    uint8_t NodeNumer;
    uint16_t ListID = 0xff;
    if (can_queue->CAN_DataSend[can_queue->Front].InConGrpFlag == false)
        return;

    if (CANx == CAN1)
    {
        NodeNumer = CAN1_NodeNumber;
    }
    else if (CANx == CAN2)
    {
        NodeNumer = CAN2_NodeNumber;
        ListID = can_queue->CAN_DataSend[can_queue->Front].ID - 0x301;//���Ŀ��ƿ�ӳ��(���������ضԽӲ��ֵĴ��룬��ʱ��Ҫ������ط��ı��Ľ�����Ӧ����)
    }
    else
        return;
    if (ListID < NodeNumer)//��ʱ����ĳ����ֻ���ڴ˴����� &&ControlListID != X
    {
        if (CAN_Queue_ifFull(&ControlList[ListID].SendQueue))
        {
            if (CANx == CAN1)
                Flag.CAN1SendQueueFULL = true;
            else
                Flag.CAN2SendQueueFULL = true;
        }
        else
        {
            ControlList[ListID].SendSem++;
            ControlList[ListID].SendNumber++;

            ControlList[ListID].SendQueue.CAN_DataSend[ControlList[ListID].SendQueue.Rear].ID = can_queue->CAN_DataSend[can_queue->Front].ID;
            ControlList[ListID].SendQueue.CAN_DataSend[ControlList[ListID].SendQueue.Rear].DLC = can_queue->CAN_DataSend[can_queue->Front].DLC;
            ControlList[ListID].SendQueue.CAN_DataSend[ControlList[ListID].SendQueue.Rear].InConGrpFlag = can_queue->CAN_DataSend[can_queue->Front].ID;
            memcpy(ControlList[ListID].SendQueue.CAN_DataSend[ControlList[ListID].SendQueue.Rear].Data,
                   can_queue->CAN_DataSend[can_queue->Front].Data,
                   sizeof(uint8_t) * ControlList[ListID].SendQueue.CAN_DataSend[ControlList[ListID].SendQueue.Rear].DLC);
            ControlList[ListID].SendQueue.Rear = (ControlList[ListID].SendQueue.Rear + 1) % CAN_QUEUESIZE;
        }
    }
}
