#ifndef _ALARM_CLOCK_H
#define _ALARM_CLOCK_H	

#include "sys.h"
#include "wm.h"

static char* ac_sort(char* week);//确定星期
static void op_ac_data(int is_ok);//确定闹钟
static void set_aclock(WM_MESSAGE * pMsg);//闹钟回显回调函数
WM_HWIN alarm_clock_win(void);//设置闹钟窗口
WM_HWIN ac_tip_win(void);//提醒窗口

#endif

