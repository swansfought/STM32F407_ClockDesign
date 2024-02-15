#include "DIALOG.h"
#include "interface.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "delay.h"
#include "rtc.h"
#include "beep.h"
#include "led.h"
#include "adc.h"

#define ID_FRAMEWIN_0    (GUI_ID_USER + 0x00)
#define ID_SPOT_1_UP     (GUI_ID_USER + 0x01)
#define ID_SPOT_1_DOWN   (GUI_ID_USER + 0x02)
#define ID_SPOT_2_UP     (GUI_ID_USER + 0x03)
#define ID_SPOT_2_DOWN   (GUI_ID_USER + 0x04)

#define ID_T_YMS         (GUI_ID_USER + 0x05) 
#define ID_YEAR        	 (GUI_ID_USER + 0x06)
#define ID_MONTH       	 (GUI_ID_USER + 0x07)
#define ID_DAY       	 (GUI_ID_USER + 0x08)
#define ID_HOUR        	 (GUI_ID_USER + 0x09)
#define ID_MINUTE        (GUI_ID_USER + 0x0A)
#define ID_SECOND        (GUI_ID_USER + 0x0B)
#define ID_TXT      	 (GUI_ID_USER + 0x0C)
#define ID_SHOW_TEMP     (GUI_ID_USER + 0x0D)
#define ID_SHOW_WEEK     (GUI_ID_USER + 0x0E)
#define ID_WEEK          (GUI_ID_USER + 0x0F)
#define ID_TEMP          (GUI_ID_USER + 0x10)

#define ID_FRAMEWIN_1    (GUI_ID_USER + 0x15) //闹钟删除窗口控件
#define ID_AC_CLOSE      (GUI_ID_USER + 0x16)
#define ID_AC_CANCEL     (GUI_ID_USER + 0x17)
#define ID_TIP           (GUI_ID_USER + 0x18)

// 资源表
static const GUI_WIDGET_CREATE_INFO _aShowInterfaceCreate[] = {
  { FRAMEWIN_CreateIndirect, "", ID_FRAMEWIN_0, 40, 10, 400, 225, 0, 0x0, 0 },
  { TEXT_CreateIndirect, ".", ID_SPOT_1_UP, 93,-5, 60, 60, 0, 0x64, 0 },
  { TEXT_CreateIndirect, ".", ID_SPOT_1_DOWN, 93, 20, 60, 60, 0, 0x0, 0 },
  { TEXT_CreateIndirect, ".", ID_SPOT_2_UP, 235, -5, 60, 60, 0, 0x0, 0 },
  { TEXT_CreateIndirect, ".", ID_SPOT_2_DOWN, 235, 20, 60, 60, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Year      Month      Day", ID_T_YMS, 48, 111, 300, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Simplicity", ID_TXT, 100, 175, 200, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Temp 10/s", ID_SHOW_TEMP, 299, 155, 80, 35, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Week", ID_SHOW_WEEK, 20, 155, 60, 35, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TEMP, 310, 175, 60, 35, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_YEAR, 68, 130, 50, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_MONTH, 178, 130, 50, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_DAY, 282, 130, 50, 20, 0, 0x0, 0 }, 
  { TEXT_CreateIndirect, "", ID_WEEK, 26, 175, 40, 35, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_HOUR, 3, 0, 110, 115, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_MINUTE, 138, 0, 110, 115, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_SECOND, 278, 0, 110, 115, 0, 0x0, 0 },
};

//闹钟窗口提醒资源表
static const GUI_WIDGET_CREATE_INFO _aAcTipCreate[] = {
  { FRAMEWIN_CreateIndirect, "", ID_FRAMEWIN_1, 110, 40, 180, 125, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_AC_CANCEL, 20, 82, 55, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Close", ID_AC_CLOSE, 96, 82, 55, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Alarm Clock Time!", ID_TIP, 10, 10, 150, 65, 0, 0x0, 0 },
};

//外部Alarm模块的闹钟变量
extern  char*  aclock_arr[12];//实际闹钟值，周、时、分
extern  int    ac_sure;      //已确定的闹钟个数

//变量声明
int    t_h,t_m,t_w,t_s; //RTC读取的时、分、周、秒
int    ac_h,ac_m,ac_w;  //闹钟时、分、周
int    d_year,d_month,d_day;//读取的年月日,供"日历"使用!!!
char   t_show[10],t_ac[10];//读取的日期时间、闹钟值
int    alarm_stop = 1;  //响铃标志，默认禁止
u8     close_ac_win = 1;//提醒弹窗最多一个
short  temperature;     //温度值
int    refresh_cycle;   //温度刷新周期
RTC_TimeTypeDef  RTC_TimeStruct; //RTC结构体声明
RTC_DateTypeDef  RTC_DateStruct;
WM_HWIN  hShowWin,hHour,hMinute,hSecond,hYear,hMonth,hDay,hWeek;//显示窗口、时、分、秒、年、月、日句柄
WM_HWIN  hClose;//响铃弹窗句柄

/**
  * @brief 主界面回调函数
  * @note 显示信息、匹配闹钟
  * @param WM_MESSAGE * pMsg
  */
static void _cbShowInterface(WM_MESSAGE * pMsg) {
  hShowWin = pMsg->hWin;
  short temp = 0;
  switch (pMsg->MsgId) {
	  case WM_INIT_DIALOG:
		FRAMEWIN_SetTitleHeight(hShowWin, 1);
	    FRAMEWIN_SetClientColor(hShowWin,0X00D3D3D3);//GUI_CYAN
		WM_CreateTimer(WM_GetClientWindow(pMsg->hWin), 0, 100, 0); //创建定时器 
	    for(int i=0;i<4;i++){ //	:
			hShowWin = WM_GetDialogItem(pMsg->hWin, ID_SPOT_1_UP+i);
			TEXT_SetFont(hShowWin, GUI_FONT_D48X64);
			TEXT_SetTextAlign(hShowWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
		}
	    //  年
		hYear = WM_GetDialogItem(pMsg->hWin, ID_YEAR);
		TEXT_SetFont(hYear, GUI_FONT_COMIC18B_ASCII);
		TEXT_SetTextAlign(hYear, GUI_TA_HCENTER | GUI_TA_VCENTER);
		// 	月
		hMonth = WM_GetDialogItem(pMsg->hWin, ID_MONTH);
		TEXT_SetFont(hMonth, GUI_FONT_COMIC18B_ASCII);
		TEXT_SetTextAlign(hMonth, GUI_TA_HCENTER | GUI_TA_VCENTER);
		// 	日
		hDay = WM_GetDialogItem(pMsg->hWin, ID_DAY);
		TEXT_SetFont(hDay, GUI_FONT_COMIC18B_ASCII);
		TEXT_SetTextAlign(hDay, GUI_TA_HCENTER | GUI_TA_VCENTER);
		// 	'周'
		hWeek = WM_GetDialogItem(pMsg->hWin, ID_WEEK);
		TEXT_SetFont(hWeek, GUI_FONT_COMIC18B_ASCII);
		TEXT_SetTextAlign(hWeek, GUI_TA_HCENTER | GUI_TA_VCENTER);
		// 	小时
		hHour = WM_GetDialogItem(pMsg->hWin, ID_HOUR);
		TEXT_SetTextAlign(hHour, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hHour, GUI_FONT_D48X64);
		//	分钟
		hMinute = WM_GetDialogItem(pMsg->hWin, ID_MINUTE);
		TEXT_SetTextAlign(hMinute, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hMinute, GUI_FONT_D48X64);
		// 	秒
		hSecond = WM_GetDialogItem(pMsg->hWin, ID_SECOND);
		TEXT_SetTextAlign(hSecond, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hSecond, GUI_FONT_D48X64);
		// 'Year                 Month                 Day'
		hShowWin = WM_GetDialogItem(pMsg->hWin, ID_T_YMS);
		TEXT_SetFont(hShowWin, GUI_FONT_COMIC24B_ASCII);
		TEXT_SetTextAlign(hShowWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
		
		for(int j=0;j<5;j++){ //Simplicity、Temp、Week、温度值、周值
			hShowWin = WM_GetDialogItem(pMsg->hWin, ID_TXT+j);
			TEXT_SetFont(hShowWin, GUI_FONT_16B_ASCII);
			TEXT_SetTextAlign(hShowWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
		}
		break;
	  case WM_TIMER://定时器消息  
/*--------------------------------------------------显示实时时间模块（RTC）--------------------------------------------------*/
		RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);//获取时间
	    sprintf(t_show,"%02d",RTC_TimeStruct.RTC_Hours); //时
	    TEXT_SetText(hHour,t_show);
		sprintf(t_show,"%02d",RTC_TimeStruct.RTC_Minutes); //分
	    TEXT_SetText(hMinute,t_show);
		sprintf(t_show,"%02d",RTC_TimeStruct.RTC_Seconds); //秒
	    TEXT_SetText(hSecond,t_show); 
	    
		RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);//获取日期
	    strcpy(t_show,"20");
		sprintf(t_ac,"%d",RTC_DateStruct.RTC_Year); //年
	    d_year = atoi(t_ac)+2000; 
	    strcat(t_show,t_ac);//小处理
	    TEXT_SetText(hYear,t_show); 
		sprintf(t_show,"%02d",RTC_DateStruct.RTC_Month);//月
	    TEXT_SetText(hMonth,t_show); 
	    d_month = atoi(t_show);
	    sprintf(t_show,"%02d",RTC_DateStruct.RTC_Date);//日
	    TEXT_SetText(hDay,t_show);
	    d_day = atoi(t_show);
		sprintf(t_show,"%d",RTC_DateStruct.RTC_WeekDay);//周   
		TEXT_SetText(hWeek,t_show); 
/*--------------------------------------------------闹钟判断模块（RTC和已设置闹钟）--------------------------------------------------*/
		if(ac_sure>0){ //有闹钟才开始判断
			sprintf((char*)t_ac,"%d",RTC_DateStruct.RTC_WeekDay);  
			t_w = atoi(t_ac);//读取到的周
			sprintf((char*)t_ac,"%d",RTC_TimeStruct.RTC_Hours);
			t_h = atoi(t_ac);//读取到的时
			sprintf((char*)t_ac,"%d",RTC_TimeStruct.RTC_Minutes);
			t_m = atoi(t_ac);// 读取到的分
			sprintf((char*)t_ac,"%d",RTC_TimeStruct.RTC_Seconds);
			t_s = atoi(t_ac);// 读取到的秒
			for(int i=0;i<ac_sure;i++){ //遍历四个闹钟的时分秒，实际情况存在未满四个
				ac_w = atoi(*(aclock_arr+3*i));
				ac_h = atoi(*(aclock_arr+1+3*i)); 
				ac_m = atoi(*(aclock_arr+2+3*i));
				if((ac_w==t_w)&&(ac_h==t_h)&&(ac_m==t_m)&&(t_s==0)){ alarm_stop=0;} //匹配成功，允许响铃
			}
			if(alarm_stop==0){ //判断闹钟符
				if(close_ac_win==1){
					ac_clos_win(); //提醒弹窗
					close_ac_win=0;
				}
				alarm_beep(); //响铃
				if((atoi(t_ac)>5)){ //未关闭闹钟和弹窗情况
					alarm_stop=1; // 停止闹铃
					if(close_ac_win==0){ //检查窗口是否关闭
						close_ac_win=1;
						GUI_EndDialog(hClose,0);
					}
				}
			}
		}
/*--------------------------------------------------显示温度模块（ADC）10秒刷新一次--------------------------------------------------*/
		if((refresh_cycle==0)||(refresh_cycle==20)){ 
		    temperature=Get_Temprate();//获取温度值
			if(temperature<0){ // 温度<0的情况
				temperature=-temperature;
				temp = temperature/1000; //整数部分
				sprintf(t_show,"%d",temp);
				strcat(t_show,".");
				temp = temperature%100;
				sprintf(t_ac,"%d",temp);
				strcat(t_show,t_ac);
				strcat(t_show,"C");
				hShowWin = WM_GetDialogItem(pMsg->hWin, ID_TEMP);
				TEXT_SetText(hShowWin,t_show); //显示温度
			}else {
				temp = temperature/1000; //整数部分
				sprintf(t_show,"%d",temp);
				strcat(t_show,".");
				temp = temperature%100;//小数部分
				sprintf(t_ac,"%d",temp);
				strcat(t_show,t_ac);
				strcat(t_show,"C");
				hShowWin = WM_GetDialogItem(pMsg->hWin, ID_TEMP);
				TEXT_SetText(hShowWin,t_show); //显示温度
			}
		   refresh_cycle=0;
		}
		refresh_cycle++;
	    WM_RestartTimer(pMsg->Data.v,500);//500ms刷新一次
	  default:
		WM_DefaultProc(pMsg);
		break;
	  }
  }


/**
  * @brief 闹钟铃声
  * @note BEEP响停响停
  */
static void alarm_beep(void){
	for(int i=0;i<5;i++){
		BEEP=!BEEP;
		//LED1=!LED1;
		delay_ms(70);
		BEEP=!BEEP;
		//LED1=!LED1;
		delay_ms(50);
	}
}

/**
  * @brief 以对话框的方式间接创建控件
  * @retval hItem资源表中第一个空间的句柄
  */
WM_HWIN interface_win(void) {
  return GUI_CreateDialogBox(_aShowInterfaceCreate, GUI_COUNTOF(_aShowInterfaceCreate), _cbShowInterface, WM_HBKWIN, 0, 0);
}


/**
  * @brief 关闭闹钟窗回调函数
  * @param WM_MESSAGE * pMsg
  */
static void _cbAcClos(WM_MESSAGE * pMsg) {
  int     NCode;
  int     Id;
  hClose = pMsg->hWin;
  switch (pMsg->MsgId) {
	  case WM_INIT_DIALOG:
		FRAMEWIN_SetTitleHeight(hClose, 2);
		FRAMEWIN_SetClientColor(hClose,0X00D3D3D3);
	    hClose = WM_GetDialogItem(pMsg->hWin, ID_TIP); //提示信息
	    TEXT_SetTextAlign(hClose, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hClose, GUI_FONT_COMIC18B_ASCII);
	  
	    hClose = WM_GetDialogItem(pMsg->hWin, ID_AC_CANCEL);
		BUTTON_SetFont(hClose, GUI_FONT_COMIC18B_ASCII);
	    hClose = WM_GetDialogItem(pMsg->hWin, ID_AC_CLOSE);
		BUTTON_SetFont(hClose, GUI_FONT_COMIC18B_ASCII);
		break;
	  case WM_NOTIFY_PARENT:
		Id    = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) {
			case ID_AC_CLOSE: //'Close'
			  switch(NCode) {
				  case WM_NOTIFICATION_CLICKED:
						 alarm_stop=1; //停止响铃
						 close_ac_win=1;//允许弹窗弹出
						 GUI_EndDialog(hClose,0);
				  } break;
			case ID_AC_CANCEL: //'Cancel'
			  switch(NCode) {
				  case WM_NOTIFICATION_CLICKED:
						 GUI_EndDialog(hClose,0);
				  } break;
			  }break;
	   default:
		  WM_DefaultProc(pMsg);
	   break;
	  }
}

/**
  * @brief 闹钟关闭对话框
  * @retval hItem资源表中第一个空间的句柄
  */
WM_HWIN ac_clos_win(void) {
  return  GUI_CreateDialogBox(_aAcTipCreate, GUI_COUNTOF(_aAcTipCreate), _cbAcClos, hShowWin, 0, 0);
}

