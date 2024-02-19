#include "delay.h"

/** 
  * @brief  ��ʱ������
  */
	
void Delay_ms(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=42000;
		while(a--);
	}
}

/** 
  * @brief  ��ʱ��΢��
  */
void Delay_us(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=40;
		while(a--);
	}
}

/** 
  * @brief  ��ʱ��ָ��������
  */

void Delay(u16 t)
{
	while(t--);
}

 

 
u32 my_us = 21;     //��21��������ʱ1us
u32 my_ms = 21000;  //��21000��������ʱ1ms
 
void Delay_Init(void)
{
	//systick��ʱ��Ƶ�ʣ�168/8 = 21MHZ    
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	
	//����systickʱ��Դ���ж�	SystemCoreClock = 168000 000  1ms�����ж�
	//SysTick_Config(SystemCoreClock/1000);  //��ʼ��systick,ʱ��ΪHCLK,�������ж�
 
}
 
 
//����u32 nusֵ��Χ��0~798915
void delay_us(u32 nus)
{
	u32 temp;
	
	//������װֵ
	SysTick->LOAD = my_us*nus - 1;
	//����VALΪ0
	SysTick->VAL = 0x00;	
	//����ʱ��
	SysTick->CTRL |= (0x01<<0);
	
	do
	{
		temp = SysTick->CTRL;  //temp ��ʮ��Ϊ1,��˵ʱ������0
	}while(  temp & (0x01<<0) && !(temp & (0x01<<16)) );
			//�ж϶�ʱ���Ƿ���   �жϼ����Ƿ�0
	
	//�ض�ʱ��
	SysTick->CTRL &= ~(0x01<<0);	
 
}
 
//����u32 nmsֵ��Χ��0~798
void delay_ms(u32 nms)
{
 
	u32 temp;
	
	//������װֵ
	SysTick->LOAD = my_ms*nms - 1;
	//����VALΪ0
	SysTick->VAL = 0x00;
	
	//����ʱ��
	SysTick->CTRL |= (0x01<<0);
	
	
	do
	{
		temp = SysTick->CTRL;  //temp ��ʮ��Ϊ1,��˵ʱ������0
	}while(  temp & (0x01<<0) && !(temp & (0x01<<16)) );
			//�ж϶�ʱ���Ƿ���   �жϼ����Ƿ�0
	
	//�ض�ʱ��
	SysTick->CTRL &= ~(0x01<<0);
 
}
 
 
void delay_s(int n)
{
	int i;
	for(i=0; i<n; i++)
	{
		delay_ms(500);
		delay_ms(500);
	}

}

