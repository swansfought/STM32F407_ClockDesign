#ifndef _ALARM_CLOCK_H
#define _ALARM_CLOCK_H	

#include "sys.h"
#include "wm.h"

static char* ac_sort(char* week);//ȷ������
static void op_ac_data(int is_ok);//ȷ������
static void set_aclock(WM_MESSAGE * pMsg);//���ӻ��Իص�����
WM_HWIN alarm_clock_win(void);//�������Ӵ���
WM_HWIN ac_tip_win(void);//���Ѵ���

#endif

