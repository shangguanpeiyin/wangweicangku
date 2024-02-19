#include "beep.h"

void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_Structure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_Structure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_Structure.GPIO_OType = GPIO_OType_PP;
	GPIO_Structure.GPIO_Pin	  = GPIO_Pin_8;
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Structure);

	BEEP_OFF;

	Beep.AllowFlag = true;

	Beep.BeepOnNum = true;
}

void Start_Music(void)
{
	//    music_2();
	//    SysTick->CTRL &= ~(0x01<<0);	//�ر�ϵͳʱ�Ӷ�ʱ����
	BEEP_ON;	// ����������
	delay_ms(100);
	BEEP_OFF;	 // �رշ�����
	delay_ms(60);
	BEEP_ON;
	delay_ms(50);
	BEEP_OFF;
	delay_ms(50);
	BEEP_ON;
	delay_ms(50);
	BEEP_OFF;
}

void beeptask(void)
{
	if (Beep.AllowFlag) {
		if (Beep.BeepOnNum) {
			BEEP_ON;
			OSTimeDly(600);
			BEEP_OFF;
			Beep.BeepOnNum--;
		}
		if (Beep.error) {
			BEEP_ON;
			OSTimeDly(1200);
			BEEP_OFF;
			OSTimeDly(600);
			BEEP_ON;
			OSTimeDly(600);
			BEEP_OFF;

			OSTimeDly(2500);

			BEEP_ON;
			OSTimeDly(1200);
			BEEP_OFF;
			OSTimeDly(600);
			BEEP_ON;
			OSTimeDly(600);
			BEEP_OFF;
			Beep.error--;
		}
	} else {
		Beep.BeepOnNum = 0;
	}
	//		̫����
	//        if(Beep.answer_master_flag) // ÿ�ε���answer_master�������������
	//        {
	//            BEEP_ON;
	//            OSTimeDly(300);
	//            BEEP_OFF;
	//            Beep.answer_master_flag = false;
	//        }
}

void Sound(u16 frq)
{
	u32 time;
	if (frq != 1000) {
		//		time = 500000/((u32)frq);
		time = 1000000 / ((u32) frq);
		BEEP_ON;	// �򿪷�����--�����Լ���Ӳ�����������ͨ�����ǿ��Ʒ�������gpio������1

		delay_us(time);

		BEEP_OFF;	 // �رշ�����--�����Լ���Ӳ�����������ͨ�����ǿ��Ʒ�������gpio������0
		delay_us(time);
	} else
		delay_us(300);
}

/**
 * @brief ������ǰ���䣬��Ī�Ǹ��� ������ һ������������?��
 */
void music_2(void)
{
	uint16_t music[] = {
		N_3, N_3, N_4, N_5, N_5, N_4, N_3, N_2, s_0, N_1, N_1, N_2, N_3, H_3, L_2, N_2,
	};
	uint8_t time[16] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2};
	u32		yanshi;
	u16		i, e;
	yanshi = 22;	// 10;  4;  2
	for (i = 0; i < sizeof(music) / sizeof(music[0]); i++) {
		for (e = 0; e < ((u16) time[i]) * music[i] / yanshi; e++) { Sound((u32) music[i]); }
	}
}
