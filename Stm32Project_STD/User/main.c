#include "stm32f10x.h"//stm32头文件
#include "math.h"
#include "SysTick.h"
#include "LED.h"
#include "key.h"
#include "oled.h"
#include "mpu6050.h"
#include "ADC.h"

u8 GUI_1[]="BAT_VAL:      V";
u8 GUI_2[]="A_X:    G_X:";
u8 GUI_3[]="A_Y:    G_Y:";
u8 GUI_4[]="A_Z:    G_Z:";
uint16_t ACC_X,ACC_Y,ACC_Z,GYRO_X,GYRO_Y,GYRO_Z;

void GUI(void)
{
	OLED_Clear();
	
	OLED_ShowCHinese(20,6,7);//作
	OLED_ShowCHinese(36,6,8);//者
	OLED_ShowChar(52,6,':');
	OLED_ShowCHinese(60,6,0);//白
	OLED_ShowCHinese(76,6,1);//明
	OLED_ShowCHinese(92,6,2);//宇
	
	OLED_ShowCHinese(32,0,3);//直
	OLED_ShowCHinese(48,0,4);//立
	OLED_ShowCHinese(64,0,5);//小
	OLED_ShowCHinese(80,0,6);//车
	
	delay_ms(5000);
	
	OLED_Clear();
	
	OLED_ShowString(0,0,GUI_1);
	OLED_ShowString(0,2,GUI_2);
	OLED_ShowString(0,4,GUI_3);
	OLED_ShowString(0,6,GUI_4);
}
void BAT_Show(void)
{
		OLED_ShowNum(72,0,(u32)Read_ADC_data(ADC1),1,16);
		OLED_ShowChar(80,0,'.');
		OLED_ShowNum(88,0,(u32)(100*Read_ADC_data(ADC1))%100,2,16);
}


int abs_1(uint16_t num)
{
	int num1;
	if(num&0x8000)
	{
		num&=0x7FFF;
		num--;
		num^=0x7FFF;
		num1=-num;
	}
	else
	{
		num1=num;
	}
	return num1;
}

void acc_dis(u8 X,u8 Y,uint16_t num)
{
		if(num&0x8000)
		{
			OLED_ShowChar(X,Y,'-');
			num&=0x7FFF;
			num--;
			num^=0x7FFF;	
		}
		else
		{
			OLED_ShowChar(X,Y,' ');
		}
		
		num=num*0.598;
		if(num>9999)
		{
			num=9999;
		}
		OLED_ShowNum(X+8,Y,num/1000,1,16);
		OLED_ShowChar(X+16,Y,'.');
		OLED_ShowNum(X+24,Y,num%1000,2,16);

}

void GYRO_dis(u8 X,u8 Y,uint16_t num)
{
	if(num&0x8000)
	{
		OLED_ShowChar(X,Y,'-');
		num&=0x7FFF;
		num--;
		num^=0x7FFF;	
	}
	else
	{
		OLED_ShowChar(X,Y,' ');
	}
	num=2000*num/32768;
	OLED_ShowNum(X+8,Y,num,3,16);
}

void MPU_Show(void)
{
	uint16_t angle;
	
	ACC_X=getdate(ACCEL_XOUT_H);
	ACC_Y=getdate(ACCEL_YOUT_H);
	ACC_Z=getdate(ACCEL_ZOUT_H);
	GYRO_X=getdate(GYRO_XOUT_H);
	GYRO_Y=getdate(GYRO_YOUT_H);
	GYRO_Z=getdate(GYRO_ZOUT_H);
	
	angle=sqrt(abs_1(ACC_X)*abs_1(ACC_X)+abs_1(ACC_Y)*abs_1(ACC_Y))/ACC_Z;
	
	if(angle>0)
	{
		OLED_ShowNum(32,2,angle,5,16);
	}
	else
	{
		OLED_ShowChar(32,2,'-');
		OLED_ShowNum(40,2,-angle,5,16);
	}
//	acc_dis(32,2,ACC_X);
//	acc_dis(32,4,ACC_Y);
//	acc_dis(32,6,ACC_Z);
//	
//	GYRO_dis(96,2,GYRO_X);
//	GYRO_dis(96,4,GYRO_Y);
//	GYRO_dis(96,6,GYRO_Z);
	

}
void INIT(void)
{
	SysTick_Init(72);
	LED_Init();
	KEY_Init();
	OLED_Init();
	iic_config();
	InitMPU6050();
	ADC_DeInit(ADC1);
  ADC_init();
}
int main(void)//主程序
{
	INIT();
	GUI();
	while(1)
	{	
		BAT_Show();
		MPU_Show();
	}
}
