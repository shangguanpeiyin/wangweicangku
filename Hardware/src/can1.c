#include "can1.h"

void CAN1_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef	  gpio_initstruct;
	CAN_InitTypeDef		  CAN_initstruct;
	CAN_FilterInitTypeDef CAN_filterinitstruct;
	NVIC_InitTypeDef	  NVIC_initstruct;

	gpio_initstruct.GPIO_Mode  = GPIO_Mode_AF;
	gpio_initstruct.GPIO_Pin   = GPIO_Pin_11 | GPIO_Pin_12;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_initstruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &gpio_initstruct);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

	CAN_DeInit(CAN1);					// Deinitializes the CAN peripheral registers to their default reset values.
	CAN_StructInit(&CAN_initstruct);	// Fills each CAN_InitStruct member with its default value.

	/* CAN cell init */
	CAN_initstruct.CAN_TTCM = DISABLE;	  // ��ʱ�䴥��ͨ��ģʽ
	CAN_initstruct.CAN_ABOM = ENABLE;	 // �����CAN_MCR�Ĵ�����INRQλ��1�������0��һ����⵽128������11λ������λ�����˳�����״̬
	CAN_initstruct.CAN_AWUM		 = DISABLE;	   // ˯��ģʽ���������
	CAN_initstruct.CAN_NART		 = DISABLE;	   // ��ֹ�����Զ����ͣ���ֻ����һ�Σ����۽�����
	CAN_initstruct.CAN_RFLM		 = DISABLE;	   // ���Ĳ��������µĸ��Ǿɵ�
	CAN_initstruct.CAN_TXFP		 = DISABLE;	   // ����FIFO�����ȼ��ɱ�ʶ������
	CAN_initstruct.CAN_Mode		 = CAN_Mode_Normal;

	/* Seting BaudRate */
	CAN_initstruct.CAN_SJW		 = CAN_SJW_1tq;	   // ����ͬ����Ծ���Ϊһ��ʱ�䵥λ
	CAN_initstruct.CAN_BS1		 = CAN_BS1_9tq;	   // ʱ���1ռ��8��ʱ�䵥λ
	CAN_initstruct.CAN_BS2		 = CAN_BS2_4tq;	   // ʱ���2ռ��7��ʱ�䵥λ
	CAN_initstruct.CAN_Prescaler = 3;			   // ��Ƶϵ����Fdiv��
	CAN_Init(CAN1, &CAN_initstruct);			   // ��ʼ��CAN1
										//  can������=42M/��1+CAN_BS1+CAN_BS2��/CAN_Prescaler=42000/��1+9+4��/3 = 1Mbps
										//  CAN1ͨ�Ų�����=42MHZ

	CAN_filterinitstruct.CAN_FilterNumber		  = 0;
	CAN_filterinitstruct.CAN_FilterMode			  = CAN_FilterMode_IdMask;	  // 32λ ����ģʽ
	CAN_filterinitstruct.CAN_FilterScale		  = CAN_FilterScale_32bit;
	CAN_filterinitstruct.CAN_FilterIdHigh		  = ((0x01020501 << 3) & 0xffff0000) >> 16;
	CAN_filterinitstruct.CAN_FilterIdLow		  = (0x01020501 << 3) & 0xffff;	   // �ⲻ��׸����룬����Ͻ�
	// ǰ��24λ������ͬ��ֻ���ĺ��8λ( ���� 0x �����λ)
	CAN_filterinitstruct.CAN_FilterMaskIdHigh	  = 0xffff;
	CAN_filterinitstruct.CAN_FilterMaskIdLow	  = 0xf000;
	CAN_filterinitstruct.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;	// ����������0������0
	CAN_filterinitstruct.CAN_FilterActivation	  = ENABLE;
	CAN_FilterInit(&CAN_filterinitstruct);

	// CAN_filterinitstruct.CAN_FilterNumber		  = 1;
	// CAN_filterinitstruct.CAN_FilterMode			  = CAN_FilterMode_IdList;	  // �б�ģʽ
	// CAN_filterinitstruct.CAN_FilterScale		  = CAN_FilterScale_32bit;
	// CAN_filterinitstruct.CAN_FilterIdHigh		  = (0x01010000 << 5) >> 16;
	// CAN_filterinitstruct.CAN_FilterIdLow		  = (0x01010000 << 5) & 0xffff;
	// CAN_filterinitstruct.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;	// ����������1������0
	// CAN_filterinitstruct.CAN_FilterActivation	  = ENABLE;
	// CAN_FilterInit(&CAN_filterinitstruct);

	NVIC_initstruct.NVIC_IRQChannel					  = CAN1_RX0_IRQn;
	NVIC_initstruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_initstruct.NVIC_IRQChannelSubPriority		  = 3;
	NVIC_initstruct.NVIC_IRQChannelCmd				  = ENABLE;
	NVIC_Init(&NVIC_initstruct);

	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

void CAN1_RX0_IRQHandler()
{
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET) {
		CanRxMsg rx_message;
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		CAN_ClearFlag(CAN1, CAN_IT_FMP0);

		Can1Funtion(rx_message);	// ������
	}
}

// // �����λ����
// void SystemReset(void)
// {
// 	__set_FAULTMASK(1);	   // �ر������ж�
// 	NVIC_SystemReset();	   // ���������λ
// }
