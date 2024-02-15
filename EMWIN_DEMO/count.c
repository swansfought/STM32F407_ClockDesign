#include "DIALOG.h"
#include "count.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "beep.h"
#include "delay.h"

#define ID_FRAMEWIN_0     (GUI_ID_USER + 0x00)
#define ID_TXT_HOUR       (GUI_ID_USER + 0x01)
#define ID_TXT_MINUTE     (GUI_ID_USER + 0x02)
#define ID_TXT_SECOND     (GUI_ID_USER + 0x03)
#define ID_LW_HOUR        (GUI_ID_USER + 0x06)
#define ID_LW_MINUTE      (GUI_ID_USER + 0x07)
#define ID_LW_SECOND      (GUI_ID_USER + 0x08)
#define ID_B_EXIT         (GUI_ID_USER + 0x09)
#define ID_B_CLOSE        (GUI_ID_USER + 0x0A)
#define ID_B_START        (GUI_ID_USER + 0x0B)
#define ID_TXY            (GUI_ID_USER + 0x0C)

#define ID_FRAMEWIN_1    (GUI_ID_USER + 0x10)
#define ID_TEXT_0        (GUI_ID_USER + 0x11)
#define ID_TEXT_1        (GUI_ID_USER + 0x12)
#define ID_TEXT_2        (GUI_ID_USER + 0x13)

//资源表
static const GUI_WIDGET_CREATE_INFO _aCountCreate[] = {
  { FRAMEWIN_CreateIndirect, "", ID_FRAMEWIN_0, 20, 0, 360, 225, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Hour", ID_TXT_HOUR, 25, 12, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Minute", ID_TXT_MINUTE, 139, 12, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Second", ID_TXT_SECOND, 251, 12, 80, 20, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "", ID_LW_HOUR, 5, 38, 110, 100, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "", ID_LW_MINUTE, 120, 38, 110, 100, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "", ID_LW_SECOND, 234, 38, 110, 100, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Exit", ID_B_EXIT, 35, 175, 70, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Close", ID_B_CLOSE, 144, 175, 70, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Start", ID_B_START, 251, 175, 70, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Select Count Value", ID_TXY, 103, 146, 152, 22, 0, 0x0, 0 },
};

//倒计时窗口
static const GUI_WIDGET_CREATE_INFO _aShowCountCreate[] = {
  { FRAMEWIN_CreateIndirect, "", ID_FRAMEWIN_1, 0,43, 359, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_0, 5, -4, 110, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_1, 120, -4, 110, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_2, 234, -4, 110, 100, 0, 0x0, 0 },
};

//变量声明
static char* _apHour[] = {"00","01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", 
						  "12", "13","14", "15", "16", "17", "18", "19", "20", "21", "22", "23"};
static char* _apMSinute[] = {"00","01", "02", "03", "04", "05", "06", "07", "08", "09", "10", 
							 "11","12", "13", "14", "15", "16", "17", "18", "19", "20", "21", 
							 "22","23", "24", "25", "26", "27", "28", "29", "30", "31", "32", 
							 "33","34", "35", "36", "37", "38", "39", "40", "41", "42", "43",
							 "44", "45", "46", "47", "48", "49", "50", "51", "52", "53","54",
							 "55", "56", "57", "58", "59"};	
//外部变量
extern u8 count_win; //计数窗口最多一个
extern WM_HWIN hShowWin; //显示窗口句柄	
extern int time_it_flag; //TIM3中断标志
							 
static int  hour,minute,second,h_back,m_back,s_back;//时分秒指针，以及他们的备份
static int  is_start; //确定是否开始倒计时，默认关闭
u8  show_win = 1;//显示窗口最多一个
int clos_show_flag; //连续点击两次关闭显示窗口
WM_HWIN cHour,cMinute,cSecond,hCount,hShowCount;; //时分秒句柄
							 
/**
  * @brief 倒计时回调函数
  * @param WM_MESSAGE * pMsg
  */
static void _cbCount(WM_MESSAGE * pMsg) {
  int     NCode;
  int     Id;
  char    show_arr[10]={0};
  hCount = pMsg->hWin;
  switch (pMsg->MsgId) {
	  case WM_INIT_DIALOG:
		FRAMEWIN_SetTitleHeight(hCount, 2);
	    FRAMEWIN_SetClientColor(hCount,0X00D3D3D3);
	    WM_CreateTimer(WM_GetClientWindow(pMsg->hWin), 0, 100, 0); //创建定时器
		// Initialization of 'Hour'
		hCount = WM_GetDialogItem(pMsg->hWin, ID_TXT_HOUR);
		TEXT_SetFont(hCount, GUI_FONT_COMIC24B_ASCII);
		TEXT_SetTextAlign(hCount, GUI_TA_HCENTER | GUI_TA_VCENTER);
		// Initialization of 'Minute'
		hCount = WM_GetDialogItem(pMsg->hWin, ID_TXT_MINUTE);
		TEXT_SetFont(hCount, GUI_FONT_COMIC24B_ASCII);
		TEXT_SetTextAlign(hCount, GUI_TA_HCENTER | GUI_TA_VCENTER);
		// Initialization of 'Second'
		hCount = WM_GetDialogItem(pMsg->hWin, ID_TXT_SECOND);
		TEXT_SetFont(hCount, GUI_FONT_COMIC24B_ASCII);
		TEXT_SetTextAlign(hCount, GUI_TA_HCENTER | GUI_TA_VCENTER);
		// Initialization of 'Listwheel'
		for(int i=0;i<3;i++){ //初始化列表轮
			hCount=WM_GetDialogItem(pMsg->hWin,ID_LW_HOUR+i);
			LISTWHEEL_SetLineHeight(hCount,100);//列高
			LISTWHEEL_SetSnapPosition(hCount,2);//停留位置
			LISTWHEEL_SetFont(hCount,GUI_FONT_D48X64);
			LISTWHEEL_SetTextAlign(hCount,GUI_TA_HCENTER|GUI_TA_VCENTER);
			LISTWHEEL_SetTextColor(hCount,LISTWHEEL_CI_UNSEL,0X191919);
			LISTWHEEL_SetTextColor(hCount,LISTWHEEL_CI_SEL,0X007DFE);
			LISTWHEEL_SetDeceleration(hCount,35); //滚动到停下的时间
			LISTWHEEL_SetSel(hCount,0);//待改进
		}
		
		//添加文本项列表轮
		cHour = WM_GetDialogItem(pMsg->hWin, ID_LW_HOUR);
		for(int i=0;i<GUI_COUNTOF(_apHour);i++){ 
			LISTWHEEL_AddString(cHour, *(_apHour+i));
			LISTWHEEL_SetBkColor(cHour,i,GUI_GRAY);
		}
		cMinute = WM_GetDialogItem(pMsg->hWin, ID_LW_MINUTE);
		for(int i=0;i<GUI_COUNTOF(_apMSinute);i++){ 
			LISTWHEEL_AddString(cMinute, *(_apMSinute+i));
			LISTWHEEL_SetBkColor(cMinute,i,GUI_GRAY);
		}	
		cSecond = WM_GetDialogItem(pMsg->hWin, ID_LW_SECOND);
		for(int i=0;i<GUI_COUNTOF(_apMSinute);i++){ 
			LISTWHEEL_AddString(cSecond, *(_apMSinute+i));
			LISTWHEEL_SetBkColor(cSecond,i,GUI_GRAY);
		}
		// Initialization of 'Exit'
		hCount = WM_GetDialogItem(pMsg->hWin, ID_B_EXIT);
		BUTTON_SetFont(hCount, GUI_FONT_COMIC18B_ASCII);
		// Initialization of 'Suspend'
		hCount = WM_GetDialogItem(pMsg->hWin, ID_B_CLOSE);
		BUTTON_SetFont(hCount, GUI_FONT_COMIC18B_ASCII);
		// Initialization of 'Continue'
		hCount = WM_GetDialogItem(pMsg->hWin, ID_B_START);
		BUTTON_SetFont(hCount, GUI_FONT_COMIC18B_ASCII);
		// Initialization of 'Select Count Value'
		hCount = WM_GetDialogItem(pMsg->hWin, ID_TXY);
		TEXT_SetFont(hCount, GUI_FONT_COMIC18B_ASCII);
		TEXT_SetTextAlign(hCount, GUI_TA_HCENTER | GUI_TA_VCENTER);
		break;
//			strcpy(show_arr,*(_apMSinute+second));
//			hShowCount = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
//			TEXT_SetText(hShowCount,show_arr);
		
		
	 case WM_TIMER: //定时器消息
		if((is_start==1)&&(time_it_flag==1)){ 
			strcpy(show_arr,*(_apMSinute+hour)); // 刷新显示，防止窗口再打开出现时分不显示问题
			hShowCount = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
			TEXT_SetText(hShowCount,show_arr); 
			strcpy(show_arr,*(_apMSinute+minute));
			hShowCount = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
			TEXT_SetText(hShowCount,show_arr); 
			
			if(atoi(*(_apMSinute+second))>0){ // s/=0
				strcpy(show_arr,*(_apMSinute+second));
				hShowCount = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
				TEXT_SetText(hShowCount,show_arr); 
				second--;//指向下一个
			}else { // s=0
				if(atoi(*(_apMSinute+minute))==0){ // m=0
					if(atoi(*(_apHour+hour))==0){ // h=0
						is_start = 0; //结束倒计时
						//clos_show_flag = 0;//清除按下次数
						alarm_beep(); //倒计时到
					}else { // h=/=0
						minute=59; //指向59继续倒计时
						second=59; 
						hour--; //指向下一个
						strcpy(show_arr,*(_apMSinute+hour));
						hShowCount = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
						TEXT_SetText(hShowCount,show_arr); 
						strcpy(show_arr,*(_apMSinute+minute));
						hShowCount = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
						TEXT_SetText(hShowCount,show_arr); 
						strcpy(show_arr,*(_apMSinute+second));
						hShowCount = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
						TEXT_SetText(hShowCount,show_arr); 
					}							
				}else { // m=/=0
						second=59;//指向59继续倒计时
						minute--;
						strcpy(show_arr,*(_apMSinute+minute));
						hShowCount = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
						TEXT_SetText(hShowCount,show_arr); 
						strcpy(show_arr,*(_apMSinute+second));
						hShowCount = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
						TEXT_SetText(hShowCount,show_arr); 
				}
			}
			time_it_flag=0;//TIM3中断标志符清零
		}
		WM_RestartTimer(pMsg->Data.v,200);//200ms刷新一次，这里只用来刷新显示，具体更新依赖TIM3
		break;
	 case WM_NOTIFY_PARENT:
		Id    = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) {
			case ID_LW_HOUR: //'ID_LW_HOUR'
			  switch(NCode) {
			  case WM_NOTIFICATION_CLICKED:
				cHour = WM_GetDialogItem(pMsg->hWin,ID_LW_HOUR);
				LISTWHEEL_SetTextColor(cHour,LISTWHEEL_CI_SEL,0X191919);
				break;
			  case WM_NOTIFICATION_SEL_CHANGED:
				cHour = WM_GetDialogItem(pMsg->hWin,ID_LW_HOUR);
				LISTWHEEL_SetTextColor(cHour,LISTWHEEL_CI_SEL,0X007DFE);
				hour = LISTWHEEL_GetPos(cHour);
			    h_back = hour;
				LISTWHEEL_SetSel(cHour,hour);
				break;
			  }break;
			case ID_LW_MINUTE: // 'ID_LW_MINUTE'
			  switch(NCode) {
			  case WM_NOTIFICATION_CLICKED:
				cMinute = WM_GetDialogItem(pMsg->hWin,ID_LW_MINUTE);
				LISTWHEEL_SetTextColor(cMinute,LISTWHEEL_CI_SEL,0X191919);
				break;
			  case WM_NOTIFICATION_SEL_CHANGED:
				cMinute = WM_GetDialogItem(pMsg->hWin,ID_LW_MINUTE);
				LISTWHEEL_SetTextColor(cMinute,LISTWHEEL_CI_SEL,0X007DFE);
				minute = LISTWHEEL_GetPos(cMinute);
			    m_back = minute;
				LISTWHEEL_SetSel(cMinute,minute);
				break;
			  }break;
			case ID_LW_SECOND: //'ID_LW_SECOND'
			  switch(NCode) {
			  case WM_NOTIFICATION_CLICKED:
				cSecond = WM_GetDialogItem(pMsg->hWin,ID_LW_SECOND);
				LISTWHEEL_SetTextColor(cSecond,LISTWHEEL_CI_SEL,0X191919);
				break;
			  case WM_NOTIFICATION_SEL_CHANGED:
				cSecond = WM_GetDialogItem(pMsg->hWin,ID_LW_SECOND);
				LISTWHEEL_SetTextColor(cSecond,LISTWHEEL_CI_SEL,0X007DFE);
				second = LISTWHEEL_GetPos(cSecond);
			    s_back = second;
				LISTWHEEL_SetSel(cSecond,second);
				break;
			  }break;
			case ID_B_EXIT: //'Exit'
			  switch(NCode) {
				  case WM_NOTIFICATION_RELEASED:
					GUI_EndDialog(hCount,0);
					count_win=1;//允许创建倒计时窗口
				    clos_show_flag = 0;//清除按下次数34hour = h_back; //恢复原先指向值
					minute = m_back;
					second = s_back; 
					break;
			  }break;
			case ID_B_CLOSE: //'Close'
			  switch(NCode) {
			  case WM_NOTIFICATION_RELEASED:
				  if(show_win==0){ 
					 clos_show_flag++;
					 if(clos_show_flag==2){ //连续点击两次关闭显示窗口
						 clos_show_flag = 0;//清除按下次数
						 hour = h_back; 
						 minute = m_back;//恢复原先指向值
						 second = s_back; 
						 GUI_EndDialog(hShowCount,0);
						 show_win = 1;//允许创建
					}
				}break;
			  }break;
			case ID_B_START: //'Start'
			  switch(NCode) {
			  case WM_NOTIFICATION_RELEASED:
				  if((atoi(*(_apHour+hour))!=0)||(atoi(*(_apHour+minute))!=0)||(atoi(*(_apHour+second))!=0)){ //至少保证有一个数已设置
					  showCount_win();//创建显示窗口
					  show_win = 0; 
					  is_start = 1;//开始计数  
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
  * @brief 设置倒计时值对话框
  * @retval hItem资源表中第一个空间的句柄
  */
WM_HWIN counter_win(void) {
    return GUI_CreateDialogBox(_aCountCreate, GUI_COUNTOF(_aCountCreate), _cbCount, hShowWin, 0, 0);
}

/**
  * @brief 倒计时到铃声
  * @note BEEP响...响.响.
  */
static void alarm_beep(void){
	for(int i=0;i<5;i++){
		BEEP=!BEEP;
		//LED1=!LED1;
		if(i==0){ delay_ms(1000);}
		else delay_ms(70);
		BEEP=!BEEP;
		//LED1=!LED1;
		delay_ms(50);
	}
}

/**
  * @brief 倒计时显示回调函数
  * @param WM_MESSAGE * pMsg
  */
static void _cbShowCount(WM_MESSAGE * pMsg) {
  hShowCount = pMsg->hWin;
  switch (pMsg->MsgId) {
	  case WM_INIT_DIALOG:
		FRAMEWIN_SetTitleHeight(hShowCount, 2);
		for(int i=0;i<3;i++){
			hShowCount = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0+i);
			TEXT_SetFont(hShowCount, GUI_FONT_D48X64);
			TEXT_SetBkColor(hShowCount,GUI_GRAY);
			TEXT_SetTextAlign(hShowCount, GUI_TA_HCENTER | GUI_TA_VCENTER);
		}
		break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief 倒计时对话框
  * @retval hItem资源表中第一个空间的句柄
  */
WM_HWIN showCount_win(void) {
  return GUI_CreateDialogBox(_aShowCountCreate, GUI_COUNTOF(_aShowCountCreate), _cbShowCount, hCount, 0, 0);
}


