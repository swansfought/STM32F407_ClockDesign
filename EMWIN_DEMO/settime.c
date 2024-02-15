#include "DIALOG.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "rtc.h"
#include "settime.h"
#include "delay.h"

#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x00)
#define ID_B_SET     	(GUI_ID_USER + 0x01)
#define ID_B_CANCEL     (GUI_ID_USER + 0x02)
#define ID_LW_0  		(GUI_ID_USER + 0x03)
#define ID_LW_1  		(GUI_ID_USER + 0x04)
#define ID_LW_2  		(GUI_ID_USER + 0x05)
#define ID_LW_3  		(GUI_ID_USER + 0x06) //周
#define ID_SHOW         (GUI_ID_USER + 0x07)
#define ID_TEXT_1 		(GUI_ID_USER + 0x08)
#define ID_TEXT_2 		(GUI_ID_USER + 0x09)
#define ID_TEXT_3 		(GUI_ID_USER + 0x0A)
#define ID_TEXT_4 		(GUI_ID_USER + 0x0B)//周

#define FRAMEWIN        (GUI_ID_USER + 0x0C)
#define B_SETDATE       (GUI_ID_USER + 0x0D)
#define B_SETTIME       (GUI_ID_USER + 0x0E)
#define B_CANCEL        (GUI_ID_USER + 0x0F)

// 设置时间资源表
static const GUI_WIDGET_CREATE_INFO _aSetTimeCreate[] = {
  { FRAMEWIN_CreateIndirect, "", ID_FRAMEWIN_0, 0, 10, 400, 205, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Set", ID_B_SET, 308, 155, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_B_CANCEL, 22, 155, 60, 30, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "", ID_LW_0, 21, 28, 100, 110, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "", ID_LW_1, 147, 28, 100, 110, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "", ID_LW_2, 272, 28, 98, 110, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_SHOW, 114, 160, 160, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_1, 48, 6, 50, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_2, 172, 6, 50, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_3, 296, 6, 50, 20, 0, 0x0, 0 },
};

// 设置日期和资源表
static const GUI_WIDGET_CREATE_INFO _aSetDateCreate[] = {
  { FRAMEWIN_CreateIndirect, "", ID_FRAMEWIN_0, 0, 10, 400, 205, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Set", ID_B_SET, 308, 155, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_B_CANCEL, 22, 155, 60, 30, 0, 0x0, 0 }, 
  { LISTWHEEL_CreateIndirect, "", ID_LW_0, 8, 28, 85, 110, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "", ID_LW_1, 103, 28, 85, 110, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "", ID_LW_2, 198, 29, 85, 110, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "", ID_LW_3, 293, 28, 85, 110, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_SHOW, 95, 160, 200, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_1, 25, 6, 50, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_2, 123, 6, 50, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_3, 217, 6, 50, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_4, 312, 6, 50, 20, 0, 0x0, 0 }
};

//选择窗口对话框资源表
static const GUI_WIDGET_CREATE_INFO _aSelectCreate[] = {
  { FRAMEWIN_CreateIndirect, "", FRAMEWIN, 120, 35, 160, 150, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Set Date", B_SETDATE, 38, 12, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Set Time", B_SETTIME, 38, 54, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", B_CANCEL, 27, 98, 100, 30, 0, 0x0, 0 }
};

//变量声明
static char* _apYear[] = {"2022","2023","2024","2025","2026","2027","2028","2029","2030","2031",
						  "2032","2033","2034","2035","2036","2037","2038","2039","2040","2041",
                          "2042","2043","2044","2045","2046","2048","2049","2050"};
static char* _apSetYear[] = {"22","23","24","25","26","27","28","29","30","31","32","33","34",
	                        "35","36","37","38","39","40","41","42","43","44","45","46","48","49","50"};
static char* _apMonth[] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10","11", "12"};
static char* _apDay[] = {"1",  "2",  "3",  "4",  "5",  "6",  "7", "8",  "9",  "10", "11", "12", "13", 
						 "14", "15", "16", "17", "18", "19", "20","21", "22", "23", "24", "25", "26", 
                         "27", "28", "29", "30", "31"};
static char* _apWeek[] = {"Mon","Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
static char* _apSetWeek[] = {"1","2","3","4","5","6","7"};
static char* _apHour[] = {"00","01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", 
						  "12", "13","14", "15", "16", "17", "18", "19", "20", "21", "22", "23"};
static char* _apMSinute[] = {"00","01", "02", "03", "04", "05", "06", "07", "08", "09", "10", 
							 "11","12", "13", "14", "15", "16", "17", "18", "19", "20", "21", 
							 "22","23", "24", "25", "26", "27", "28", "29", "30", "31", "32", 
							 "33","34", "35", "36", "37", "38", "39", "40", "41", "42", "43",
							 "44", "45", "46", "47", "48", "49", "50", "51", "52", "53","54",
							 "55", "56", "57", "58", "59"};	
static char* _apShow_1[] = {"Year","Month","Day","Week"};
static char* _apShow_2[] = {"Hour","Minute","Second"};
static int  year,month,day,week,hour,minute,second;

//外部变量
extern WM_HWIN hShowWin;//显示窗口句柄	
extern u8 set_win;      //设置时间日期窗口最多一个
extern RTC_TimeTypeDef RTC_TimeStruct;
extern RTC_DateTypeDef RTC_DateStruct;	

WM_HWIN  hSetTime,hSetDate,hSelect;//设置时间、设置日期、选择设置句柄
char  now_time[30],temp[10]; //用来处理读入时间，便于显示
char* set_data[6];//存放设置的年月日（0-2），周时分（3-5）
u8   time_win=1;  //设置时间窗口最多一个
u8   date_win=1;  //设置日期窗口最多一个

/**
  * @brief 选择设置窗口回调函数
  * @param *pMsg 消息指针
  */
static void _cbSelect(WM_MESSAGE * pMsg){
  int     NCode;
  int     Id;
  hSelect = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    FRAMEWIN_SetTitleHeight(hSelect, 2);// Initialization of ''
    FRAMEWIN_SetClientColor(hSelect,0X00D3D3D3);
    hSelect = WM_GetDialogItem(pMsg->hWin, B_SETDATE);// Initialization of 'Set Date'
    BUTTON_SetFont(hSelect, GUI_FONT_COMIC18B_ASCII);
    hSelect = WM_GetDialogItem(pMsg->hWin, B_SETTIME);// Initialization of 'Set Time'
    BUTTON_SetFont(hSelect, GUI_FONT_COMIC18B_ASCII);
    hSelect = WM_GetDialogItem(pMsg->hWin, B_CANCEL);// Initialization of 'Cancel'
    BUTTON_SetFont(hSelect, GUI_FONT_COMIC18B_ASCII);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case B_SETDATE: //Set Date
      switch(NCode) {
		  case WM_NOTIFICATION_RELEASED:
			  if(date_win==1){
				set_date_win(); //设置日期窗口
				date_win=0; 
			  }
			 break;
	  }break;
    case B_SETTIME: //Set Time
      switch(NCode) {
		  case WM_NOTIFICATION_RELEASED:
			  if(time_win==1){
				 set_time_win();
				 time_win=0;  
			  }
			 break;
	  }break;
    case B_CANCEL: //Cancel
      switch(NCode) {
		  case WM_NOTIFICATION_RELEASED:
			  GUI_EndDialog(hSelect,0);//结束对话框
			  set_win=1;//允许创建对话框
			  break;
	  }break;
    }break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}
/**
  * @brief 选择设置对话框
  * @retval hItem资源表中第一个空间的句柄
  */
WM_HWIN select_win(void) {
  return GUI_CreateDialogBox(_aSelectCreate, GUI_COUNTOF(_aSelectCreate), _cbSelect,hShowWin, 0, 0);
}


/**
  * @brief listwheel重绘函数
  * @note 待优化
  * @param const WIDGET_ITEM_DRAW_INFO* pDrawItemInfo
  * @retval LISTWHEEL_OwnerDraw(pDrawItemInfo)或0
  */
static int _cbSelectDraw(const WIDGET_ITEM_DRAW_INFO* pDrawItemInfo){
	GUI_RECT aRect;
	switch(pDrawItemInfo->Cmd){
		case WIDGET_ITEM_DRAW_OVERLAY://覆盖绘制
			aRect.x0=pDrawItemInfo->x0;//获取控件坐标
			aRect.x1=pDrawItemInfo->x1;
			aRect.y1=pDrawItemInfo->y1;
			GUI_SetColor(0X00D3D3D3); 
			GUI_DrawLine(aRect.x0,(aRect.y1-25)/2,aRect.x1,(aRect.y1-25)/2);//画分割线
			GUI_DrawLine(aRect.x0,(aRect.y1+25)/2,aRect.x1,(aRect.y1+25)/2);
			break;
		default:
			return LISTWHEEL_OwnerDraw(pDrawItemInfo);
	}
	return 0;
}

/**
  * @brief 设置日期窗口回调函数
  * @note 
  * @param *pMsg 消息指针
  */
RTC_InitTypeDef RTC_InitStructure;
static void _cbSetDate(WM_MESSAGE * pMsg){
  int     NCode;
  int     Id;
  hSetDate = pMsg->hWin;
  switch (pMsg->MsgId) {
	   case WM_INIT_DIALOG:
		FRAMEWIN_SetTitleHeight(hSetDate, 1);
	    FRAMEWIN_SetClientColor(hSetDate,0X00D3D3D3);
		hSetDate = WM_GetDialogItem(pMsg->hWin, ID_B_SET);// 'Set'
		BUTTON_SetFont(hSetDate, GUI_FONT_COMIC18B_ASCII);
		hSetDate = WM_GetDialogItem(pMsg->hWin, ID_B_CANCEL);//'Cancel'
		BUTTON_SetFont(hSetDate, GUI_FONT_COMIC18B_ASCII);
		
		for(int i=0;i<4;i++){ //初始化列表轮
			hSetDate=WM_GetDialogItem(pMsg->hWin,ID_LW_0+i);
			LISTWHEEL_SetLineHeight(hSetDate,23);//列高
			LISTWHEEL_SetSnapPosition(hSetDate,(110-23)/2);//停留位置
			LISTWHEEL_SetFont(hSetDate,GUI_FONT_COMIC18B_ASCII);
			LISTWHEEL_SetTextAlign(hSetDate,GUI_TA_HCENTER|GUI_TA_VCENTER);
			LISTWHEEL_SetTextColor(hSetDate,LISTWHEEL_CI_UNSEL,0X191919);
			LISTWHEEL_SetTextColor(hSetDate,LISTWHEEL_CI_SEL,0X007DFE);
			LISTWHEEL_SetDeceleration(hSetDate,35); //滚动到停下的时间
			LISTWHEEL_SetOwnerDraw(hSetDate,_cbSelectDraw); //设置列表自定义重绘函数，待优化
			LISTWHEEL_SetSel(hSetDate,0);//待改进
		}
		
		//添加文本项列表轮
		hSetDate = WM_GetDialogItem(pMsg->hWin, ID_LW_0);
		for(int i=0;i<GUI_COUNTOF(_apYear);i++){ 
			LISTWHEEL_AddString(hSetDate, *(_apYear+i));
			LISTWHEEL_SetBkColor(hSetDate,i,GUI_GRAY);
		}
		hSetDate = WM_GetDialogItem(pMsg->hWin, ID_LW_1);
		for(int i=0;i<GUI_COUNTOF(_apMonth);i++){ 
			LISTWHEEL_AddString(hSetDate, *(_apMonth+i));
			LISTWHEEL_SetBkColor(hSetDate,i,GUI_GRAY);
		}	
		hSetDate = WM_GetDialogItem(pMsg->hWin, ID_LW_2);
		for(int i=0;i<GUI_COUNTOF(_apDay);i++){ 
			LISTWHEEL_AddString(hSetDate, *(_apDay+i));
			LISTWHEEL_SetBkColor(hSetDate,i,GUI_GRAY);
		}
		hSetDate = WM_GetDialogItem(pMsg->hWin, ID_LW_3);
		for(int i=0;i<GUI_COUNTOF(_apWeek);i++){ 
			LISTWHEEL_AddString(hSetDate, *(_apWeek+i));
			LISTWHEEL_SetBkColor(hSetDate,i,GUI_GRAY);
		}		
			
		for(int i=0;i<4;i++){ // Initialization of 'Year''Month' 'Day' 'Week'
			hSetDate = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1+i);
			TEXT_SetFont(hSetDate, GUI_FONT_COMIC18B_ASCII);
			TEXT_SetTextAlign(hSetDate, GUI_TA_HCENTER | GUI_TA_VCENTER);
			TEXT_SetText(hSetDate,*(_apShow_1+i));
		}
		// 显示信息
		hSetDate = WM_GetDialogItem(pMsg->hWin, ID_SHOW);
		TEXT_SetFont(hSetDate, GUI_FONT_COMIC18B_ASCII);
		TEXT_SetTextAlign(hSetDate, GUI_TA_HCENTER | GUI_TA_VCENTER);
		strcpy(now_time,""); //清空遗留数据
		strcpy(now_time,"now : 20");
		RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);//获取日期  
		sprintf(temp,"%d",RTC_DateStruct.RTC_Year); //年
		strcat(now_time,temp);
		strcat(now_time,"-");
		sprintf(temp,"%02d",RTC_DateStruct.RTC_Month);//月
		strcat(now_time,temp);
		strcat(now_time,"-");
		sprintf(temp,"%02d",RTC_DateStruct.RTC_Date);//日
		strcat(now_time,temp);
		strcat(now_time,"-week");
		sprintf(temp,"%d",RTC_DateStruct.RTC_WeekDay);//周
		strcat(now_time,temp);
		TEXT_SetText(hSetDate,now_time); //显示当前时间
		break;
	  case WM_NOTIFY_PARENT:
		Id    = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) {
		case ID_B_SET: //设置日期
		  switch(NCode) {
		  case WM_NOTIFICATION_RELEASED:
			RTC_Set_Date(atoi(*(_apSetYear+year)),atoi(*(_apMonth+month)),atoi(*(_apDay+day)),atoi(*(_apSetWeek+week))); //设置日期
			date_win=1;//允许创建设置时间窗口
			GUI_EndDialog(hSetDate,0);
			break;
		  }break;
		case ID_B_CANCEL: //'Cancel'
		  switch(NCode) {
		  case WM_NOTIFICATION_RELEASED:
			   GUI_EndDialog(hSetDate,0);
			   date_win=1;
			break;
		  }break;
		case ID_LW_0: // 年
			switch(NCode) {
				case WM_NOTIFICATION_CLICKED:
					hSetTime = WM_GetDialogItem(pMsg->hWin,ID_LW_0);
					LISTWHEEL_SetTextColor(hSetTime,LISTWHEEL_CI_SEL,0X191919);
					break;
				case WM_NOTIFICATION_RELEASED:
					break;
				case WM_NOTIFICATION_SEL_CHANGED: //项目已在对齐位置对齐
					hSetTime = WM_GetDialogItem(pMsg->hWin,ID_LW_0);
					LISTWHEEL_SetTextColor(hSetTime,LISTWHEEL_CI_SEL,0X007DFE);
					year = LISTWHEEL_GetPos(hSetTime); //获取当前值的索引值
					LISTWHEEL_SetSel(hSetTime,year);
					break;
			}break;
		case ID_LW_1: // 月
			switch(NCode) {
				case WM_NOTIFICATION_CLICKED:
					hSetTime = WM_GetDialogItem(pMsg->hWin,ID_LW_1);
					LISTWHEEL_SetTextColor(hSetTime,LISTWHEEL_CI_SEL,0X191919);
					break;
				case WM_NOTIFICATION_RELEASED:
					break;
				case WM_NOTIFICATION_SEL_CHANGED:
					hSetTime = WM_GetDialogItem(pMsg->hWin,ID_LW_1);
					LISTWHEEL_SetTextColor(hSetTime,LISTWHEEL_CI_SEL,0X007DFE);
					month = LISTWHEEL_GetPos(hSetTime);
					LISTWHEEL_SetSel(hSetTime,month);
					break;
			}break;
		case ID_LW_2: // 日
			switch(NCode) {
				case WM_NOTIFICATION_CLICKED:
					hSetTime = WM_GetDialogItem(pMsg->hWin,ID_LW_2);
					LISTWHEEL_SetTextColor(hSetTime,LISTWHEEL_CI_SEL,0X191919);
					break;
				case WM_NOTIFICATION_RELEASED:
					break;
				case WM_NOTIFICATION_SEL_CHANGED:
					hSetTime = WM_GetDialogItem(pMsg->hWin,ID_LW_2);
					LISTWHEEL_SetTextColor(hSetTime,LISTWHEEL_CI_SEL,0X007DFE);
					day = LISTWHEEL_GetPos(hSetTime);
					LISTWHEEL_SetSel(hSetTime,day);
					break;
			}break;
		case ID_LW_3: // 周
			switch(NCode) {
				case WM_NOTIFICATION_CLICKED:
					hSetTime = WM_GetDialogItem(pMsg->hWin,ID_LW_3);
					LISTWHEEL_SetTextColor(hSetTime,LISTWHEEL_CI_SEL,0X191919);
					break;
				case WM_NOTIFICATION_RELEASED:
					break;
				case WM_NOTIFICATION_SEL_CHANGED:
					hSetTime = WM_GetDialogItem(pMsg->hWin,ID_LW_3);
					LISTWHEEL_SetTextColor(hSetTime,LISTWHEEL_CI_SEL,0X007DFE);
					week = LISTWHEEL_GetPos(hSetTime);
					LISTWHEEL_SetSel(hSetTime,week);
					break;
			}break;
		}break;
	  default:
		WM_DefaultProc(pMsg);
		break;
	  }
}
/**
  * @brief 设置日期对话框
  * @retval hItem资源表中第一个空间的句柄
  */
WM_HWIN set_date_win(void) {
  return GUI_CreateDialogBox(_aSetDateCreate, GUI_COUNTOF(_aSetDateCreate), _cbSetDate, hShowWin, 0, 0);
}



/**
  * @brief 设置时间窗口回调函数
  * @note 
  * @param *pMsg 消息指针
  */
static void _cbSetTime(WM_MESSAGE * pMsg) {
  int     NCode;
  int     Id;
  hSetTime = pMsg->hWin;
  switch (pMsg->MsgId) {
	   case WM_INIT_DIALOG:
			FRAMEWIN_SetTitleHeight(hSetTime, 2);
	        FRAMEWIN_SetClientColor(hSetTime,0X00D3D3D3);
			WM_CreateTimer(WM_GetClientWindow(pMsg->hWin), 0, 100, 0); //创建定时器
			hSetTime = WM_GetDialogItem(pMsg->hWin, ID_B_SET);//'Set'
			BUTTON_SetFont(hSetTime, GUI_FONT_COMIC18B_ASCII);
			hSetTime = WM_GetDialogItem(pMsg->hWin, ID_B_CANCEL);//'Cancel'
			BUTTON_SetFont(hSetTime, GUI_FONT_COMIC18B_ASCII);

			for(int i=0;i<3;i++){ //初始化列表轮
				hSetTime=WM_GetDialogItem(pMsg->hWin,ID_LW_0+i);
				LISTWHEEL_SetLineHeight(hSetTime,23);//列高
				LISTWHEEL_SetSnapPosition(hSetTime,(110-23)/2);//停留位置
				LISTWHEEL_SetFont(hSetTime,GUI_FONT_COMIC18B_ASCII);
				LISTWHEEL_SetTextAlign(hSetTime,GUI_TA_HCENTER|GUI_TA_VCENTER);
				LISTWHEEL_SetTextColor(hSetTime,LISTWHEEL_CI_UNSEL,0X191919);
				LISTWHEEL_SetTextColor(hSetTime,LISTWHEEL_CI_SEL,0X007DFE);
				LISTWHEEL_SetDeceleration(hSetTime,35); //滚动到停下的时间
				LISTWHEEL_SetOwnerDraw(hSetTime,_cbSelectDraw); //设置列表自定义重绘函数，待优化
				LISTWHEEL_SetSel(hSetTime,0);//待改进
			}
			//添加文本项列表轮
			hSetTime = WM_GetDialogItem(pMsg->hWin, ID_LW_0);
			for(int i=0;i<GUI_COUNTOF(_apHour);i++){ 
				LISTWHEEL_AddString(hSetTime, *(_apHour+i));
				LISTWHEEL_SetBkColor(hSetTime,i,GUI_GRAY);
			}
			hSetTime = WM_GetDialogItem(pMsg->hWin, ID_LW_1);
			for(int i=0;i<GUI_COUNTOF(_apMSinute);i++){
				LISTWHEEL_AddString(hSetTime, *(_apMSinute+i));
				LISTWHEEL_SetBkColor(hSetTime,i,GUI_GRAY);
			}	
			hSetTime = WM_GetDialogItem(pMsg->hWin, ID_LW_2);
			for(int i=0;i<GUI_COUNTOF(_apMSinute);i++){ 
				LISTWHEEL_AddString(hSetTime, *(_apMSinute+i));
				LISTWHEEL_SetBkColor(hSetTime,i,GUI_GRAY);
			}	

			for(int i=0;i<3;i++){ // Initialization of 'Week''Hour' 'Minute'
				hSetTime = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1+i);
				TEXT_SetFont(hSetTime, GUI_FONT_COMIC18B_ASCII);
				TEXT_SetTextAlign(hSetTime, GUI_TA_HCENTER | GUI_TA_VCENTER);
				TEXT_SetText(hSetTime,*(_apShow_2+i));
			}
			break;
	  case WM_TIMER://定时器消息
/*--------------------------------------------------实时显示时间--------------------------------------------------*/
			hSetTime = WM_GetDialogItem(pMsg->hWin, ID_SHOW);
			TEXT_SetFont(hSetTime, GUI_FONT_COMIC18B_ASCII);
			TEXT_SetTextAlign(hSetTime, GUI_TA_HCENTER | GUI_TA_VCENTER);
			strcpy(now_time,"");//清空遗留数据
			strcpy(now_time,"now : ");
			RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);//获取时间
			sprintf(temp,"%02d",RTC_TimeStruct.RTC_Hours); //时
			strcat(now_time,temp);
			strcat(now_time,"-");
			sprintf(temp,"%02d",RTC_TimeStruct.RTC_Minutes); //分
			strcat(now_time,temp);
			strcat(now_time,"-");
			sprintf(temp,"%02d",RTC_TimeStruct.RTC_Seconds);//秒 
			strcat(now_time,temp);
			TEXT_SetText(hSetTime,now_time); //显示当前时间
			WM_RestartTimer(pMsg->Data.v,50);//50ms刷新一次
	        break;
	  case WM_NOTIFY_PARENT:
		Id    = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) {
		case ID_B_SET: // Set
		  switch(NCode) {
		  case WM_NOTIFICATION_RELEASED:
			  RTC_Set_Time(atoi(*(_apHour+hour)),atoi(*(_apMSinute+minute)),atoi(*(_apMSinute+second)),RTC_H12_AM); //设置时间
			  time_win=1;  //允许创建设置时间窗口
			  GUI_EndDialog(hSetTime,0);
			  break;
		  }break;
		case ID_B_CANCEL: //'Cancel'
		  switch(NCode) {
		  case WM_NOTIFICATION_RELEASED:
			   time_win=1;
			   GUI_EndDialog(hSetTime,0);
			break;
		  }break;
		case ID_LW_0: // 时
			switch(NCode) {
				case WM_NOTIFICATION_CLICKED:
					hSetTime = WM_GetDialogItem(pMsg->hWin,ID_LW_0);
					LISTWHEEL_SetTextColor(hSetTime,LISTWHEEL_CI_SEL,0X191919);
					break;
				case WM_NOTIFICATION_RELEASED:
					break;
				case WM_NOTIFICATION_SEL_CHANGED: //项目已在对齐位置对齐
					hSetTime = WM_GetDialogItem(pMsg->hWin,ID_LW_0);
					LISTWHEEL_SetTextColor(hSetTime,LISTWHEEL_CI_SEL,0X007DFE);
					hour = LISTWHEEL_GetPos(hSetTime); //获取当前值的索引值
					LISTWHEEL_SetSel(hSetTime,hour);
					break;
			}break;
		case ID_LW_1: // 分
			switch(NCode) {
				case WM_NOTIFICATION_CLICKED:
					hSetTime = WM_GetDialogItem(pMsg->hWin,ID_LW_1);
					LISTWHEEL_SetTextColor(hSetTime,LISTWHEEL_CI_SEL,0X191919);
					break;
				case WM_NOTIFICATION_RELEASED:
					break;
				case WM_NOTIFICATION_SEL_CHANGED:
					hSetTime = WM_GetDialogItem(pMsg->hWin,ID_LW_1);
					LISTWHEEL_SetTextColor(hSetTime,LISTWHEEL_CI_SEL,0X007DFE);
					minute = LISTWHEEL_GetPos(hSetTime); //获取当前值的索引值
					LISTWHEEL_SetSel(hSetTime,minute);
					break;
			}break;
		case ID_LW_2: // 秒
			switch(NCode) {
				case WM_NOTIFICATION_CLICKED:
					hSetTime = WM_GetDialogItem(pMsg->hWin,ID_LW_2);
					LISTWHEEL_SetTextColor(hSetTime,LISTWHEEL_CI_SEL,0X191919);
					break;
				case WM_NOTIFICATION_RELEASED:
					break;
				case WM_NOTIFICATION_SEL_CHANGED:
					hSetTime = WM_GetDialogItem(pMsg->hWin,ID_LW_2);
					LISTWHEEL_SetTextColor(hSetTime,LISTWHEEL_CI_SEL,0X007DFE);
					second = LISTWHEEL_GetPos(hSetTime); //获取当前值的索引值
					LISTWHEEL_SetSel(hSetTime,second);
					break;
			}break;
		}break;
	  default:
		WM_DefaultProc(pMsg);
		break;
	  }
}

/**
  * @brief 设置时间对话框
  * @retval hItem资源表中第一个空间的句柄
  */
WM_HWIN set_time_win(void) {
  return GUI_CreateDialogBox(_aSetTimeCreate, GUI_COUNTOF(_aSetTimeCreate), _cbSetTime, hShowWin, 0, 0);
}




