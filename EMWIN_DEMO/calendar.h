#ifndef _CALENDAR_H
#define _CALENDAR_H

#include "sys.h"
#include "wm.h"

static  int is_leap_year(int year);//�ж�����
static  int total_days(int year, int month);//����������
static  int arr_string(char arr[], int size);//����ת����
static  void print_calendar(int year, int month, WM_MESSAGE * pMsg);//��ӡ�����ص�����
WM_HWIN calendar_win(void);//��������

#endif
