#ifndef _CALENDAR_H
#define _CALENDAR_H

#include "sys.h"
#include "wm.h"

static  int is_leap_year(int year);//判断闰年
static  int total_days(int year, int month);//计算总天数
static  int arr_string(char arr[], int size);//数组转整型
static  void print_calendar(int year, int month, WM_MESSAGE * pMsg);//打印日历回调函数
WM_HWIN calendar_win(void);//日历窗口

#endif
