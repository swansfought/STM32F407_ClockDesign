
#ifndef __ADC_H
#define __ADC_H	

#include "sys.h" 
							   
void Adc_Init(void);//ADCͨ����ʼ��
u16  Get_Adc(u8 ch); //���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(u8 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ  
short Get_Temprate(void);//��ȡ�¶�
#endif 















