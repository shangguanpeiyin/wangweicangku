#include "cylinder.h"

#include "delay.h"

// imitate I2C

CyLinderStruct Cyl;	   //{}}
char		   CYL_table[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//									  Lh             Paw  Ld        Tg    Rh
// pc0 sclk ; pc1 io
void		   CYL_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin	  = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure);

	Set_CYL8CLK();		// GPIO_SetBits(GPIOC, GPIO_Pin_0)
	Reset_CYL8CLK();	// GPIO_ResetBits(GPIOC, GPIO_Pin_0)
}

void CYL_Ctrl(u8 IO, u8 status)	   // ��ô�����޸�һ��byte�е�һλ
{
	IO = 8 - IO;				  // ��λ��0����λ��8����һ��˳��
	for (u8 i = 0; i < 8; i++)	  // ��Ϊ��8λ��������ѭ��8��
	{
		Reset_CYL8CLK();	// switch analog clock

		if (i == IO)	// �ı䵱ǰλ
		{
			CYL_table[i] = status;
		}

		// Send signal based on current settings
		if (CYL_table[i] & 0x01)
			Set_CYLData();	  // GPIO_SetBits(GPIOC, GPIO_Pin_1)
		else
			Reset_CYLData();	// GPIO_ResetBits(GPIOC, GPIO_Pin_1)

		Set_CYL8CLK();
	}
}

void Valve_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin	  = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	Set_CYLCLK();		// 1
	Reset_CYLDATA();	// 0��������
}

/**
 * @brief: Valve control by bits
 */
void Valve_Ctrl(u8 Data)
{
	u8 i;
	if (Data > 0xFF)
		Data = 0XFF;
	for (i = 0; i < 8; i++) {
		Reset_CYLCLK();
		if (Data & 0x01)		// ����������һλΪ1
			Set_CYLDATA();		// 1
		else					// ���Ϊ0
			Reset_CYLDATA();	// 0
		Set_CYLCLK();			// ��ʱ�ӣ�0
		Data >>= 1;				// ����һλ
	}
	Set_CYLDATA();
}

char YTL_table[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void YTL_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin	  = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	Set_YTLCLK();	   // PA9 = 1
	Reset_YTLCLK();	   // PA9 = 0
}

// ytl board;
void YTL_Ctrl(u8 Data)	  // ��ô�����޸�һ��byte�е�һλ
{
	////// 	u8 i;
	//////	if (Data > 0xFF)
	//////		Data = 0XFF;
	//////	for (i = 0; i < 8; i++) {
	//////			Reset_YTLCLK();
	//////		if (Data & 0x01)		// ����������һλΪ1
	//////			Set_YTLDATA();		// 1
	//////		else					// ���Ϊ0
	//////		Reset_YTLDATA();	// 0
	//////		Set_YTLCLK();		// ��ʱ�ӣ�0
	//////		Data >>= 1;				// ����һλ
	//////	}
	//////Set_YTLDATA();
	//////
	//////
	//	IO = 8 - IO;		//��λ��0����λ��8����һ��˳��
	//	for(u8 i=0;i<8;i++)		//��Ϊ��8λ��������ѭ��8��
	//	{
	//		Reset_YTLCLK();  // switch analog clock
	//
	//		if(i==IO)		//�ı䵱ǰλ
	//		{
	//			YTL_table[i] = status;
	//		}

	//		// Send signal based on current settings
	//		if(YTL_table[i])
	//			Set_YTLDATA();
	//		else
	//			Reset_YTLDATA();
	//
	//		Set_YTLCLK();
	//	}

	Reset_YTLCLK();	   // PA9 = 0

	for (u8 i = 0; i < 8; i++) {
		if ((Data >> i) & 0x01) {
			Set_YTLDATA();	  // 1
		} else {
			Reset_YTLDATA();	// 0
		}

		Delay_us(2);
		Set_YTLCLK();	 // ��ʱ�ӣ�0

		Delay_us(2);
		Reset_YTLCLK();
	}
}
