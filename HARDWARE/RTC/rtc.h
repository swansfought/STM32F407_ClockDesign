#ifndef __RTC_H
#define __RTC_H	 
#include "sys.h" 

u8 _RTC_Init(void);//��ʼ��
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);//ʱ������
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week);//��������

#endif

















