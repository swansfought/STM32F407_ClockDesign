#include "DIALOG.h"
#include "dock.h"
#include "alarm_clock.h"
#include "count.h"
#include "calendar.h"
#include "settime.h"

#define ID_FRAMEWIN_0     (GUI_ID_USER + 0x00) //控件ID
#define ID_B_ALARM        (GUI_ID_USER + 0x01)
#define ID_B_COUNT        (GUI_ID_USER + 0x02)
#define ID_B_CALEN        (GUI_ID_USER + 0x03)
#define ID_B_SETTING      (GUI_ID_USER + 0x04)

// dock资源表	
static const GUI_WIDGET_CREATE_INFO _aDockCreate[] = {
  { FRAMEWIN_CreateIndirect, "",  		  ID_FRAMEWIN_0, 92,   245, 295, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,	 "Alarm", 	  ID_B_ALARM, 	 8,    6,   60,  50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, 	 "Count", 	  ID_B_COUNT, 	 78,   6,   60,  50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, 	 "Calen", 	  ID_B_CALEN,    148,  6,   60,  50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,	 "Set",   ID_B_SETTING, 	 218,  6 ,  60,  50, 0, 0x0, 0 },
};

//变量声明
u8 alarm_win = 1;//闹钟窗口最多一个
u8 count_win = 1;//计数窗口最多一个
u8 calen_win = 1;//日历窗口最多一个
u8 set_win = 1;  //设置窗口最多一个

/**
  * @brief 菜单栏回调函数
  * @param *pMsg 消息指针
  */
static void _cbDock(WM_MESSAGE * pMsg) {
	WM_HWIN  hItem;
	int      NCode;
	int      Id;
	switch (pMsg->MsgId) {
		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			FRAMEWIN_SetTitleHeight(hItem, 1);
		    FRAMEWIN_SetClientColor(hItem,0X00D3D3D3);
			//初始化'Alarm'
			hItem = WM_GetDialogItem(pMsg->hWin, ID_B_ALARM);
			BUTTON_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
			
			//初始化'Count'
			hItem = WM_GetDialogItem(pMsg->hWin, ID_B_COUNT);
			BUTTON_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
			
			//初始化'Calendar'
			hItem = WM_GetDialogItem(pMsg->hWin, ID_B_CALEN);
			BUTTON_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
			
			//初始化'Button'
			hItem = WM_GetDialogItem(pMsg->hWin, ID_B_SETTING);
			BUTTON_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
			break;		
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc); //获取控件的ID号
			NCode = pMsg->Data.v; // 获取事件类型
			switch(Id) {
				case ID_B_ALARM:
					switch(NCode) {
					    case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							if(alarm_win==1){  //闹钟窗口
								alarm_clock_win();
								alarm_win = 0;
							}
							break;
					}break;	
				case ID_B_COUNT:
					switch(NCode) {
						case WM_NOTIFICATION_RELEASED:
							if(count_win==1){  //倒计时窗口
								counter_win();
								count_win = 0;
							}
							break;
					}break;
				case ID_B_CALEN:
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							if(calen_win==1){  //日历窗口
								calendar_win();
								calen_win = 0;
							}
							break;
					}break;
				case ID_B_SETTING:
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							if(set_win==1){ //设置时间日期选择窗口
								select_win();
								set_win = 0;
							}
							break;
					}break;
			}break;
		default:
			WM_DefaultProc(pMsg);
			break;
  }
}

/**
  * @brief 以对话框的方式间接创建控件 
  * @retval hItem资源表中第一个空间的句柄
  */
WM_HWIN dock_win(void) {
	return GUI_CreateDialogBox(_aDockCreate, GUI_COUNTOF(_aDockCreate), &_cbDock, WM_HBKWIN, 0, 0);
}

