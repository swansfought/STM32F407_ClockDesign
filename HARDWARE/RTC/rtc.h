#ifndef __RTC_H
#define __RTC_H	 
#include "sys.h" 

u8 _RTC_Init(void);//初始化
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);//时间设置
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week);//日期设置

#endif

















