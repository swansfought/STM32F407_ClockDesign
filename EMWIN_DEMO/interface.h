#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "sys.h"
#include "WM.h"

static void alarm_beep(void);//闹钟铃
WM_HWIN interface_win(void);//界面窗口
WM_HWIN ac_clos_win(void);//关闭闹钟窗口

#endif


