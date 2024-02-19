/**
 * @file queue.c
 * @brief
 *  ע�����ǻ����Ķ��к����ļ�
 * @author ZhiHang_FU (fu_zhihang@126.com)
 * @version 1.0
 * @date
 * @copyright Copyright (c) 2023  the author of this program
 */
#include "queue.h"

CAN_SendQueueType CAN1_SendQueue, CAN2_SendQueue, VESC_SendQueue, ZDrive_SendQueue, DM_SendQueue; // CAN�ڱ��ķ��Ͷ���  ����VESC��ͨ��CAN2���͵�
MesgControlGrpType CAN1_MesgControlList[CAN1_NodeNumber], CAN2_MesgControlList[CAN2_NodeNumber]; // ���Ŀ��Ʊ�

/**
 * @brief Initializ CAN message queue
 */
void CAN_InitSendQueue(void)
{
    CAN1_SendQueue.Front = CAN1_SendQueue.Rear = 0;
    CAN2_SendQueue.Front = CAN2_SendQueue.Rear = 0;
    VESC_SendQueue.Front = VESC_SendQueue.Rear = 0;
    ZDrive_SendQueue.Front = ZDrive_SendQueue.Rear = 0;
    DM_SendQueue.Front = DM_SendQueue.Rear = 0;
    VESC_SendQueue.IDE = CAN_ID_EXT;
    ZDrive_SendQueue.IDE = CAN_ID_STD;
    DM_SendQueue.IDE = CAN_ID_STD;
}

/**
 * @brief Judge whether the CAN queue is empty
 * @param  can_queueMy Param doc
 * @return true
 * @return false
 */
bool CAN_Queue_ifEmpty(CAN_SendQueueType *can_queue)
{
    return ((can_queue->Front == can_queue->Rear));
}

/**
 * @brief Judge whether the CAN queue is full
 * @param  can_queueMy Param doc
 * @return true
 * @return false
 */
bool CAN_Queue_ifFull(CAN_SendQueueType *can_queue)
{
    return (((can_queue->Rear + 1) % CAN_QUEUESIZE) == can_queue->Front);
}

/**
 * @brief  Out of CAN queue
 * @param  CANx   select which CAN channel
 * @param  can_queue
 */
void CAN_QueueOut(CAN_TypeDef *CANx, CAN_SendQueueType *can_queue)
{
    CanTxMsg tx_message; // CAN���ķ��ͽṹ����ʱ����

    if (CAN_Queue_ifEmpty(can_queue))
    {
        Flag.CANSendQueueEMPTY = true;
        return;
    }

    if (CANx == CAN1) // �ظ�����
    {
        MesgConstrolList(CAN1_MesgControlList, can_queue, CAN1);
        tx_message.IDE = CAN_ID_EXT; // ��չ֡ ��������
        tx_message.ExtId = can_queue->CAN_DataSend[can_queue->Front].ID;
    }

    else if (CANx == CAN2)
    {
        tx_message.IDE = can_queue->IDE; // ��չ֡
        if (can_queue->IDE == CAN_ID_STD)
        {
            tx_message.StdId = can_queue->CAN_DataSend[can_queue->Front].ID;
        }
        else if (can_queue->IDE == CAN_ID_EXT)
        {
            if ((can_queue->CAN_DataSend[can_queue->Front].ID & 0xf0000000) == 0xf0000000) // ������Ϊ������VESC
            {
                can_queue->CAN_DataSend[can_queue->Front].ID &= 0x0fffffff;
            }
            tx_message.ExtId = can_queue->CAN_DataSend[can_queue->Front].ID;
        }
        MesgConstrolList(CAN2_MesgControlList, can_queue, CAN2);
        // // can2 ��Ӧ����VESC�����ĵ��
        //  if ((can_queue->CAN_DataSend[can_queue->Front].ID & 0xf0000000) == 0xf0000000) // ������Ϊ������
        // {
        //     can_queue->CAN_DataSend[can_queue->Front].ID &= 0x0fffffff;
        //     tx_message.IDE = CAN_ID_EXT; // ��չ֡
        //     tx_message.ExtId = can_queue->CAN_DataSend[can_queue->Front].ID;
        // }

        // else
        // {
        //     tx_message.IDE = CAN_ID_STD; // �������ʱΪ��zdirveд��
        //     tx_message.StdId = can_queue->CAN_DataSend[can_queue->Front].ID;
        // }
        // MesgConstrolList(CAN2_MesgControlList, can_queue, CAN2);
    }

    tx_message.DLC = can_queue->CAN_DataSend[can_queue->Front].DLC;
    tx_message.RTR = CAN_RTR_Data; // ����֡
    memcpy(tx_message.Data, can_queue->CAN_DataSend[can_queue->Front].Data, tx_message.DLC * sizeof(uint8_t));

    can_queue->Front = (can_queue->Front + 1) % CAN_QUEUESIZE;

    CAN_Transmit(CANx, &tx_message);
}

// /**
//  * @brief  Out of CAN queue
//  * @param  CANx   select which CAN channel
//  * @param  can_queue
//  */
// void CAN_QueueOut(CAN_TypeDef *CANx, CAN_SendQueueType *can_queue)
// {
//     CanTxMsg tx_message; // CAN���ķ��ͽṹ����ʱ����

//     if (CAN_Queue_ifEmpty(can_queue))
//     {
//         Flag.CANSendQueueEMPTY = true;
//         return;
//     }

//     if (can_queue->CAN_DataSend[can_queue->Front].ID < 0x400) // can1ͨ����Ӧ����DJMotor
//     {
//         tx_message.IDE = CAN_ID_STD; // ��׼֡
//         tx_message.StdId = can_queue->CAN_DataSend[can_queue->Front].ID;
//     }

//     // can2 ��Ӧ����VESC�����ĵ��
//     else if ((can_queue->CAN_DataSend[can_queue->Front].ID & 0xf0000000) == 0xf0000000) // ������Ϊ������
//     {
//         can_queue->CAN_DataSend[can_queue->Front].ID &= 0x0fffffff;
//         tx_message.IDE = CAN_ID_EXT; // ��չ֡
//         tx_message.ExtId = can_queue->CAN_DataSend[can_queue->Front].ID;
//     }

//     else
//     {
//         tx_message.IDE = CAN_ID_STD; // �������ʱΪ��zdirveд��
//         tx_message.StdId = can_queue->CAN_DataSend[can_queue->Front].ID;
//     }

//     if (CANx == CAN1)
//     {
//         MesgConstrolList(CAN1_MesgControlList, can_queue, CAN1);
//         tx_message.IDE = CAN_ID_EXT; // ��չ֡ ��������
//         tx_message.ExtId = can_queue->CAN_DataSend[can_queue->Front].ID;
//     }
//     else if (CANx == CAN2)
//         MesgConstrolList(CAN2_MesgControlList, can_queue, CAN2);

//     tx_message.DLC = can_queue->CAN_DataSend[can_queue->Front].DLC;
//     tx_message.RTR = CAN_RTR_Data; // ����֡
//     memcpy(tx_message.Data, can_queue->CAN_DataSend[can_queue->Front].Data, tx_message.DLC * sizeof(uint8_t));

//     can_queue->Front = (can_queue->Front + 1) % CAN_QUEUESIZE;

//     CAN_Transmit(CANx, &tx_message);
// }
