#include "can2.h"

void CAN2_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_FilterInitTypeDef CAN_Filter_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
    RCC_AHB1PeriphClockCmd(Rcc_AHB1_CAN_Gpio_Port, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = CAN2_RX_PIN | CAN2_TX_PIN;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(CAN2_GPIO_PORT, &GPIO_InitStructure);

    GPIO_PinAFConfig(CAN2_GPIO_PORT, CAN_AF_GpioSourePin_RX, GPIO_AF_CAN2); // ��ʼ��GPIO�ĸ��ù���
    GPIO_PinAFConfig(CAN2_GPIO_PORT, CAN_AF_GpioSourePin_TX, GPIO_AF_CAN2);

    CAN_InitStructure.CAN_TTCM = DISABLE;         // ��ʱ�䴥��ͨ��ģʽ
    CAN_InitStructure.CAN_ABOM = ENABLE;         // �����CAN_MCR�Ĵ�����INRQλ��1�������0��һ����⵽128������11λ������λ�����˳�����״̬
    CAN_InitStructure.CAN_AWUM = DISABLE;         // ˯��ģʽ���������
    CAN_InitStructure.CAN_NART = DISABLE;         // ��ֹ�����Զ����ͣ���ֻ����һ�Σ����۽�����
    CAN_InitStructure.CAN_RFLM = DISABLE;         // ���Ĳ��������µĸ��Ǿɵ�
    CAN_InitStructure.CAN_TXFP = DISABLE;         // ����FIFO�����ȼ��ɱ�ʶ������
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; // CANӲ������������ģʽ

    /* Seting BaudRate */
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq; // ����ͬ����Ծ���Ϊһ��ʱ�䵥λ
    CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq; // ʱ���1ռ��8��ʱ�䵥λ
    CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq; // ʱ���2ռ��7��ʱ�䵥λ
    CAN_InitStructure.CAN_Prescaler = 3;     // ��Ƶϵ����Fdiv��
    CAN_Init(CAN2, &CAN_InitStructure);      // ��ʼ��CAN1
                                             //  can_BaudRate=42M/��1+CAN_BS1+CAN_BS2��/CAN_Prescaler=42000/��1+9+4��/3 = 1Mbps
    // F4��CAN1��CAN2�ڵ���APB1�ϣ�ϵͳƵ��Ϊ42MHZ.����ʱ��APB2Ϊ84MHZ.��F1��F4ʱ�ӵ�Ƶ���ǲ�һ����

    /*------------------CAN FIFO DJMOTOR----------------------*/
    CAN_Filter_InitStructure.CAN_FilterNumber = 14;                   // ѡ��14 �ο�README.md������һ��
    CAN_Filter_InitStructure.CAN_FilterMode = CAN_FilterMode_IdList;  // �б�ģʽ
    CAN_Filter_InitStructure.CAN_FilterScale = CAN_FilterScale_16bit; // 16λ������
    CAN_Filter_InitStructure.CAN_FilterIdHigh = 0x201 << 5;
    CAN_Filter_InitStructure.CAN_FilterIdLow = 0x202 << 5;
    CAN_Filter_InitStructure.CAN_FilterMaskIdHigh = 0x203 << 5;
    CAN_Filter_InitStructure.CAN_FilterMaskIdLow = 0x204 << 5;
    CAN_Filter_InitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_Filter_InitStructure.CAN_FilterActivation = ENABLE; // ����FIFO0
    CAN_FilterInit(&CAN_Filter_InitStructure);

    CAN_Filter_InitStructure.CAN_FilterNumber = 15;
    CAN_Filter_InitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
    CAN_Filter_InitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
    CAN_Filter_InitStructure.CAN_FilterIdHigh = 0x205 << 5;
    CAN_Filter_InitStructure.CAN_FilterIdLow = 0x206 << 5;
    CAN_Filter_InitStructure.CAN_FilterMaskIdHigh = 0x207 << 5;
    CAN_Filter_InitStructure.CAN_FilterMaskIdLow = 0x208 << 5;
    CAN_Filter_InitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
    CAN_Filter_InitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_Filter_InitStructure);


    //**********Zdrive**********//
	CAN_Filter_InitStructure.CAN_FilterNumber = 20;
	CAN_Filter_InitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_Filter_InitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_Filter_InitStructure.CAN_FilterIdHigh = 0x001 << 5;
	CAN_Filter_InitStructure.CAN_FilterIdLow = 0x002 << 5;
	CAN_Filter_InitStructure.CAN_FilterMaskIdHigh = 0x003 << 5;
	CAN_Filter_InitStructure.CAN_FilterMaskIdLow = 0x004 << 5;
	CAN_Filter_InitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_Filter_InitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_Filter_InitStructure);

//    /*-------------------------------------------*/
//    CAN_Filter_InitStructure.CAN_FilterNumber = 24;
//    CAN_Filter_InitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
//    CAN_Filter_InitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
//    // ģʽ
//    CAN_Filter_InitStructure.CAN_FilterIdLow = (0x054) << 5;
//    CAN_Filter_InitStructure.CAN_FilterMaskIdLow = (0x0FF) << 5;
//    CAN_Filter_InitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
//    CAN_Filter_InitStructure.CAN_FilterActivation = ENABLE;
//    CAN_FilterInit(&CAN_Filter_InitStructure);

    
    /*-------------------------------------------*/
    /*-------------------------------------------*/


    NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // ��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        // �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
    CAN_ITConfig(CAN2, CAN_IT_FMP1, ENABLE);
}

/**
 * @brief CAN2 FIFO0���Ľ����жϺ���
 * ����ע�⺯��CAN_GetFlagStatus() �Ĳ��� CAN_IT_FMP0 �ǹ����־
 * CAN_FLAG_FMP0: FIFO 0 Message Pending Flag
 */
void CAN2_RX0_IRQHandler(void)
{

    if (CAN_GetITStatus(CAN2, CAN_IT_FMP0) != RESET)
    {
        CanRxMsg Rx_message0;
        CAN_Receive(CAN2, CAN_FIFO0, &Rx_message0);
		
        DJ_ReceiveData_CAN2(Rx_message0); // ��
//		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
//		CAN_ClearFlag(CAN2, CAN_IT_FMP0);
        // Flag.ScopeStartFlag=true;
    }
}

void CAN2_RX1_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN2, CAN_IT_FMP1) != RESET)
    {
        CanRxMsg Rx_message1;
        CAN_Receive(CAN2, CAN_FIFO1, &Rx_message1);

        if ((Rx_message1.IDE == CAN_ID_STD) && (Rx_message1.RTR == CAN_RTR_DATA)) // IDE
        {
            ZdriveReceiveHandler(Rx_message1);
        }
    }
}
