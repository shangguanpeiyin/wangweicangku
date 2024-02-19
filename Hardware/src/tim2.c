#include "tim2.h"

/**
 * @brief 0.1ms��ʱ�жϼ���
 */
void Tim2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef tim2_strcuture;
	tim2_strcuture.TIM_Period		 = 100 - 1;
	tim2_strcuture.TIM_Prescaler	 = 84 - 1;
	tim2_strcuture.TIM_CounterMode	 = TIM_CounterMode_Up;
	tim2_strcuture.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM2, &tim2_strcuture);

	NVIC_InitTypeDef NVIC_tim2_structure;
	NVIC_tim2_structure.NVIC_IRQChannel					  = TIM2_IRQn;
	NVIC_tim2_structure.NVIC_IRQChannelCmd				  = ENABLE;
	NVIC_tim2_structure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_tim2_structure.NVIC_IRQChannelSubPriority		  = 1;

	NVIC_Init(&NVIC_tim2_structure);

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

static int	Count  = 0;
static bool toggle = true;

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		if (toggle) {
			// ����toggle��ʱ�� ��֤���ͬʱ���������ֹ��
			CAN_QueueOut(CAN1, &CAN1_SendQueue);	//  �ظ�����  0.3ms һ��

		} else {
			CAN_QueueOut(CAN2, &CAN2_SendQueue);	// ���ƻ��� 0.3ms һ��
		}
		if (Count % 5 == 0)	   //
		{
			CAN_QueueOut(CAN2, &ZDrive_SendQueue);

			Count = 0;
		}
		if (Count % 5 == 1)	   // ʵ�ַ�ʱ����
		{
			CAN_QueueOut(CAN2, &DM_SendQueue);
		}
		toggle = !toggle;
		Count  = Count < 999 ? Count + 1 : 0;

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
