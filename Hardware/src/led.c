#include "led.h"
// LED IO ��ʼ��

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;                  // ����ṹ�����
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // ʹ�� GPIOF ʱ��

    GPIO_InitStructure.GPIO_Pin = LED_R_GPIO_PIN | LED_Y_GPIO_PIN | LED_B_GPIO_PIN | LED_G_GPIO_PIN; // LED0 �� LED1 ��Ӧ IO ��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                                                    // ��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                                                   // �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                                               // 100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                                     // ����
    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);                                                   // ��ʼ�� GPIO
    GPIO_SetBits(LED_GPIO_PORT, LED_R_GPIO_PIN | LED_Y_GPIO_PIN | LED_B_GPIO_PIN | LED_G_GPIO_PIN);  // ��λ������
}

void ledtask(void)
{

    if (Led.Normal)
    {
        LED_light_water();
    }
    if (Led.Stuck)
    {
        LED_RED_TOGGLE; // ��ת
        Led.Stuck--;
    }
    if (Led.Timeout)
    {
        LED_YELLOW_TOGGLE; // ��ʱ����
        Led.Timeout--;
    }
    if (Led.CANErr)
    {
        LED_BLUE_TOGGLE;
        Led.CANErr--;
    }
}

void LED_light_water(void)
{
    LED_RED_TOGGLE;
    OSTimeDly(1000);
    LED_RED_TOGGLE;
    LED_BLUE_TOGGLE;
    OSTimeDly(1000);
    LED_BLUE_TOGGLE;
    LED_YELLOW_TOGGLE;
    OSTimeDly(1000);
    LED_YELLOW_TOGGLE;
    LED_GREEN_TOGGLE;
    OSTimeDly(1000);
    LED_GREEN_TOGGLE;
}
