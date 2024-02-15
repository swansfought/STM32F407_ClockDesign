#include "DIALOG.h"
#include "alarm_clock.h"
#include "string.h"
#include "stdlib.h"

#define ID_FRAMEWIN_0      (GUI_ID_USER + 0x00) //闹钟窗口
#define ID_B_ADD           (GUI_ID_USER + 0x01)
#define ID_B_OK            (GUI_ID_USER + 0x02)
#define ID_B_CANCEL        (GUI_ID_USER + 0x03)
#define ID_D_AC_1          (GUI_ID_USER + 0x04)
#define ID_D_AC_2          (GUI_ID_USER + 0x05)
#define ID_D_AC_3          (GUI_ID_USER + 0x06)
#define ID_D_AC_4          (GUI_ID_USER + 0x07)
#define ID_LW_WEEK         (GUI_ID_USER + 0x08)
#define ID_LW_HOUR         (GUI_ID_USER + 0x09)
#define ID_LW_MINUTE       (GUI_ID_USER + 0x0A)
#define ID_TEXT_0          (GUI_ID_USER + 0x0B)
#define ID_TEXT_1          (GUI_ID_USER + 0x0C)
#define ID_AC_1       	   (GUI_ID_USER + 0x0D)
#define ID_AC_2            (GUI_ID_USER + 0x0E)
#define ID_AC_3            (GUI_ID_USER + 0x0F)
#define ID_AC_4            (GUI_ID_USER + 0x10)

#define ID_FRAMEWIN_1      (GUI_ID_USER + 0x11) //闹钟删除提示窗口
#define ID_AC_SURE         (GUI_ID_USER + 0x12)
#define ID_AC_CANCEL       (GUI_ID_USER + 0x13)
#define ID_TIP             (GUI_ID_USER + 0x14)

//设置闹钟资源表												 
static const GUI_WIDGET_CREATE_INFO _aAlarmClockCreate[] = {
  { FRAMEWIN_CreateIndirect, "", ID_FRAMEWIN_0, 0, 10, 400, 205, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Add your alarm", ID_B_ADD, 34, 155, 120, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OK", ID_B_OK, 306, 155, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_B_CANCEL, 210, 155, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Delete", ID_D_AC_1, 210, 54, 60, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Delete", ID_D_AC_2, 306, 54, 60, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Delete", ID_D_AC_3, 210, 114, 60, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Delete", ID_D_AC_4, 306, 114, 60, 20, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "Week", ID_LW_WEEK, 10, 24, 60, 120, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "Hour", ID_LW_HOUR, 74, 24, 55, 120, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "Minute", ID_LW_MINUTE, 133, 24, 55, 120, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Select", ID_TEXT_0, 67, 4, 60, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Added", ID_TEXT_1, 262, 4, 60, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "", ID_AC_1, 200, 29, 82, 25, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "", ID_AC_2, 296, 29, 82, 25, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "", ID_AC_3, 200, 89, 82, 25, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_AC_4, 296, 89, 82, 25, 0, 0x64, 0 },
};

//删除闹钟提醒窗口资源表
static const GUI_WIDGET_CREATE_INFO _aAcTipCreate[] = {
  { FRAMEWIN_CreateIndirect, "", ID_FRAMEWIN_1, 110, 40, 180, 125, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_AC_CANCEL, 20, 78, 55, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Sure", ID_AC_SURE, 96, 78, 55, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Sure delete it ...", ID_TIP, 10, 10, 150, 65, 0, 0x0, 0 },
};


//变量声明
static char* _apWeek[] = {"Mon","Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
static char* _apHour[] = {"00","01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", 
						  "12", "13","14", "15", "16", "17", "18", "19", "20", "21", "22", "23"};
static char* _apMinute[] = {"00","01", "02", "03", "04", "05", "06", "07", "08", "09", "10", 
							 "11","12", "13", "14", "15", "16", "17", "18", "19", "20", "21", 
							 "22","23", "24", "25", "26", "27", "28", "29", "30", "31", "32", 
							 "33","34", "35", "36", "37", "38", "39", "40", "41", "42", "43",
							 "44", "45", "46", "47", "48", "49", "50", "51", "52", "53","54", "55", "56", "57", "58", "59"};						 
extern  WM_HWIN hShowWin;//显示界面句柄
extern u8   alarm_win; //设置闹钟窗口最多一个
u8     dele_ac_win = 1;//删除提醒窗口最多一个
u8     delet_ac_sure; //确定 哪一个闹钟被删除
int    w_index,h_index,m_index,ac_index;//列轮表和闹钟索引
char*  aclock_arr[12];//实际闹钟值，周、小时、分
char*  ac_show[4];  //再次打开窗口显示的闹钟
int    ac_temp;     //临时闹钟个数
int    ac_sure;    //已经确定的闹钟个数
int    need_flash; //删除闹钟刷新符
WM_HWIN  hSetAclock;//设置闹钟句柄					 
/**************************************************************
 *四个闹钟标识符，主要用于限制删除提醒弹窗的弹出
 *  闹钟1：0 0001  闹钟2：0 0010  闹钟3：0 0100 闹钟2：0 1000 
 *************************************************************/
u8     ac_dele_flag;  
				 
/**
  * @brief 闹钟回显回调函数
  * @note 闹钟最多四个,否则进不来
  * @param WM_MESSAGE * pMsg
  */						 
static void set_aclock(WM_MESSAGE * pMsg){
	WM_HWIN hItem;
	char  arr[10]={0};
	strcpy(arr,*(_apWeek+w_index));//添加便于文本显示的闹钟格式
	strcat(arr," ");
	strcat(arr,*(_apHour+h_index));
	strcat(arr,":");
	strcat(arr,*(_apMinute+m_index));
	hItem = WM_GetDialogItem(pMsg->hWin, ID_AC_1+ac_temp);
	TEXT_SetText(hItem,arr); //实时显示设置的闹钟值
	
	char* str=malloc((strlen(arr)+1)*sizeof(char)); //存储已设置闹钟，用于再次打开设置闹钟窗口显示
	if(str!=NULL){
		for(int i=0;i<=10;i++){ str[i] = arr[i];}
		str[strlen(arr)] = 0;
		ac_show[ac_temp] = str;//存至显示数组中
	}
	
	ac_dele_flag+=1<<ac_temp;//添加闹钟标识符，便于后期锁定删除
	ac_temp++;
	
	// 添加实际用来匹配的周、时、分
	*(aclock_arr+ac_index) = ac_sort(*(_apWeek+w_index));//week 
	ac_index++;
	*(aclock_arr+ac_index) = *(_apHour+h_index); //hour
	ac_index++;
	*(aclock_arr+ac_index) = *(_apMinute+m_index);//min
	ac_index++;
}

/**
  * @brief 确定星期，便于闹钟匹配
  * @param char* week
  * @retval char *str
  */
static char *ac_sort(char* week){
	if(week==*(_apWeek))  { return "1";}
	if(week==*(_apWeek+1)){ return "2";}
	if(week==*(_apWeek+2)){ return "3";}
	if(week==*(_apWeek+3)){ return "4";}
	if(week==*(_apWeek+4)){ return "5";}
	if(week==*(_apWeek+5)){ return "6";}
	if(week==*(_apWeek+6)){ return "7";}
	return 0;
}

/**
  * @brief 确定闹钟和闹钟标识符
  * @param int is_ok
  */
static void op_ac_data(int is_ok){
	if(is_ok){ ac_sure=ac_temp;}
	else{
		for(int i=ac_sure;i<4;i++){  //清空未添加的
			*(ac_show+i) = 0;
			ac_dele_flag = ac_dele_flag>>ac_sure;//清除标记
		}
		ac_temp = ac_sure;
		for(int j=ac_sure*3;j<12;j++){ *(aclock_arr+j) = 0;} //清空未添加的
		ac_index = ac_sure*3;
	}
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
  * @brief 闹钟窗口回调函数
  * @note 
  * @param *pMsg 消息指针
  */
static void _cbAlarmClock(WM_MESSAGE * pMsg) {
	int     NCode;
	int     Id;
	hSetAclock = pMsg->hWin;
	switch (pMsg->MsgId) {
		case WM_INIT_DIALOG:
			WM_CreateTimer(WM_GetClientWindow(pMsg->hWin), 0, 100, 0); //创建定时器 
			FRAMEWIN_SetTitleHeight(hSetAclock, 2);// Initialization of 'Set Alarm Clocks'
			FRAMEWIN_SetTextAlign(hSetAclock, GUI_TA_HCENTER | GUI_TA_VCENTER);
			FRAMEWIN_SetFont(hSetAclock, GUI_FONT_COMIC18B_ASCII);
            FRAMEWIN_SetClientColor(hSetAclock,0X00D3D3D3);
			// Initialization of 'Add Alarm Clock'
			hSetAclock = WM_GetDialogItem(pMsg->hWin, ID_B_ADD);
			BUTTON_SetFont(hSetAclock, GUI_FONT_COMIC18B_ASCII);
			// Initialization of 'OK'
			hSetAclock = WM_GetDialogItem(pMsg->hWin, ID_B_OK);
			BUTTON_SetFont(hSetAclock, GUI_FONT_COMIC18B_ASCII);
			// Initialization of 'Cancel'
			hSetAclock = WM_GetDialogItem(pMsg->hWin, ID_B_CANCEL);
			BUTTON_SetFont(hSetAclock, GUI_FONT_COMIC18B_ASCII);
			
			for(int i=0;i<4;i++){ // Initialization of 'AC_X'
				hSetAclock = WM_GetDialogItem(pMsg->hWin, ID_D_AC_1+i);
				BUTTON_SetFont(hSetAclock, GUI_FONT_COMIC18B_ASCII);
			}			
			
			for(int i=0;i<4;i++){ // Initialization of ID_AC_X'
				hSetAclock = WM_GetDialogItem(pMsg->hWin, ID_AC_1+i);
				TEXT_SetFont(hSetAclock, GUI_FONT_COMIC18B_ASCII);
				TEXT_SetTextAlign(hSetAclock, GUI_TA_HCENTER | GUI_TA_VCENTER);
			}
			
			for(int i=0;i<3;i++){ //初始化列表轮
				hSetAclock=WM_GetDialogItem(pMsg->hWin,ID_LW_WEEK+i);
				LISTWHEEL_SetLineHeight(hSetAclock,22);//列高
				LISTWHEEL_SetSnapPosition(hSetAclock,(120-22)/2);//停留位置
				LISTWHEEL_SetFont(hSetAclock,GUI_FONT_COMIC18B_ASCII);
				LISTWHEEL_SetTextAlign(hSetAclock,GUI_TA_HCENTER|GUI_TA_VCENTER);
				LISTWHEEL_SetTextColor(hSetAclock,LISTWHEEL_CI_UNSEL,0X191919);
				LISTWHEEL_SetTextColor(hSetAclock,LISTWHEEL_CI_SEL,0X007DFE);
				LISTWHEEL_SetDeceleration(hSetAclock,35); //滚动到停下的时间
				LISTWHEEL_SetOwnerDraw(hSetAclock,_cbSelectDraw); //设置列表自定义重绘函数，待优化
				LISTWHEEL_SetSel(hSetAclock,0);//待改进
			}
			
			// Week添加文本项
			hSetAclock = WM_GetDialogItem(pMsg->hWin, ID_LW_WEEK);
			for(int i=0;i<GUI_COUNTOF(_apWeek);i++){ 
				LISTWHEEL_AddString(hSetAclock, *(_apWeek+i));
				LISTWHEEL_SetBkColor(hSetAclock,i,GUI_GRAY);
			}
			// Hour添加文本项
			hSetAclock = WM_GetDialogItem(pMsg->hWin, ID_LW_HOUR);
			for(int i=0;i<GUI_COUNTOF(_apHour);i++){ 
				LISTWHEEL_AddString(hSetAclock, *(_apHour+i));
				LISTWHEEL_SetBkColor(hSetAclock,i,GUI_GRAY);
			}	
			// Minute添加文本项
			hSetAclock = WM_GetDialogItem(pMsg->hWin, ID_LW_MINUTE);
			for(int i=0;i<GUI_COUNTOF(_apMinute);i++){ 
				LISTWHEEL_AddString(hSetAclock, *(_apMinute+i));
				LISTWHEEL_SetBkColor(hSetAclock,i,GUI_GRAY);
			}
			//Select
			hSetAclock = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
			TEXT_SetTextAlign(hSetAclock, GUI_TA_HCENTER | GUI_TA_VCENTER);
			TEXT_SetFont(hSetAclock, GUI_FONT_COMIC18B_ASCII);
			//Added
			hSetAclock = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
			TEXT_SetTextAlign(hSetAclock, GUI_TA_HCENTER | GUI_TA_VCENTER);
			TEXT_SetFont(hSetAclock, GUI_FONT_COMIC18B_ASCII);
			
			for(int i=0;i<ac_sure;i++){ //显示已设置的闹钟 
				hSetAclock = WM_GetDialogItem(pMsg->hWin, ID_AC_1+i);
				TEXT_SetText(hSetAclock,*(ac_show+i));
			}
			break;
		case WM_TIMER://定时器消息  
			if(need_flash==1){
				for(int i=0;i<4;i++){ //显示已设置的闹钟 
					hSetAclock = WM_GetDialogItem(pMsg->hWin, ID_AC_1+i);
					if(i<ac_sure) TEXT_SetText(hSetAclock,*(ac_show+i));
					else TEXT_SetText(hSetAclock,"");
				}
				need_flash=0;
			}
			WM_RestartTimer(pMsg->Data.v,50);//50ms新一次
			break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id){
				case ID_B_CANCEL: 
					switch(NCode) {
						case WM_NOTIFICATION_RELEASED://关闭窗口
							alarm_win = 1;//允许闹钟窗口创建
							op_ac_data(0);//清除已设置的闹钟
							GUI_EndDialog(hSetAclock,0);
							break;
					}break;
				case ID_B_OK: 
					switch(NCode) {
						case WM_NOTIFICATION_RELEASED:
							alarm_win = 1;
							op_ac_data(1);//确定已设置的闹钟
							GUI_EndDialog(hSetAclock,0);
							break;
					}break;
				case ID_B_ADD://添加闹钟
					switch(NCode) {
						case WM_NOTIFICATION_RELEASED: 
							if(ac_temp<4){ set_aclock(pMsg);} //最多四个
							break;
					}break;
				case ID_LW_WEEK: //选择周
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							hSetAclock = WM_GetDialogItem(pMsg->hWin,ID_LW_WEEK);
							LISTWHEEL_SetTextColor(hSetAclock,LISTWHEEL_CI_SEL,0X191919);
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED: //项目已在对齐位置对齐
							hSetAclock = WM_GetDialogItem(pMsg->hWin,ID_LW_WEEK);
							LISTWHEEL_SetTextColor(hSetAclock,LISTWHEEL_CI_SEL,0X007DFE);
							w_index = LISTWHEEL_GetPos(hSetAclock); //获取当前值的索引值
							LISTWHEEL_SetSel(hSetAclock,w_index);
							break;
					}break;
				case ID_LW_HOUR: //选择时
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							hSetAclock = WM_GetDialogItem(pMsg->hWin,ID_LW_HOUR);
							LISTWHEEL_SetTextColor(hSetAclock,LISTWHEEL_CI_SEL,0X191919);
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
							hSetAclock = WM_GetDialogItem(pMsg->hWin,ID_LW_HOUR);
							LISTWHEEL_SetTextColor(hSetAclock,LISTWHEEL_CI_SEL,0X007DFE);
							h_index = LISTWHEEL_GetPos(hSetAclock);
							LISTWHEEL_SetSel(hSetAclock,h_index);
							break;
					}break;
				case ID_LW_MINUTE://选择分
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							hSetAclock = WM_GetDialogItem(pMsg->hWin,ID_LW_MINUTE);
							LISTWHEEL_SetTextColor(hSetAclock,LISTWHEEL_CI_SEL,0X191919);
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
							hSetAclock = WM_GetDialogItem(pMsg->hWin,ID_LW_MINUTE);
							LISTWHEEL_SetTextColor(hSetAclock,LISTWHEEL_CI_SEL,0X007DFE);
							m_index = LISTWHEEL_GetPos(hSetAclock);
							LISTWHEEL_SetSel(hSetAclock,m_index);
							break;
					}break;
				case ID_D_AC_1: //删除闹钟1
					switch(NCode) {
						case WM_NOTIFICATION_RELEASED:
							if(dele_ac_win==1 && (ac_dele_flag&0X01) && ac_sure>0){
								delet_ac_sure=1;//删除闹钟1
								ac_tip_win();//提示是否确定删除，删除工作在其中
								dele_ac_win=0;//禁止提醒弹窗弹出
								
							}break;
					}break;
				case ID_D_AC_2: //删除闹钟2
					switch(NCode) {
						case WM_NOTIFICATION_RELEASED:
							if(dele_ac_win==1 && (ac_dele_flag&0X02) && ac_sure>1){
								delet_ac_sure=2;
								ac_tip_win();
								dele_ac_win=0;
							}break;
					}break;
				case ID_D_AC_3: //删除闹钟3
					switch(NCode) {
						case WM_NOTIFICATION_RELEASED:
							if(dele_ac_win==1 && (ac_dele_flag&0X04) && ac_sure>2){
								delet_ac_sure=3; 
								ac_tip_win();
								dele_ac_win=0;
							}break;
					}break;
				case ID_D_AC_4: //删除闹钟4
					switch(NCode) {
						case WM_NOTIFICATION_RELEASED:
							if(dele_ac_win==1 && (ac_dele_flag&0X08) && ac_sure>3){
								delet_ac_sure=4;
								ac_tip_win();
								dele_ac_win=0;
							}break;
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
WM_HWIN alarm_clock_win(void) {
	return GUI_CreateDialogBox(_aAlarmClockCreate, GUI_COUNTOF(_aAlarmClockCreate), &_cbAlarmClock, hShowWin, 0, 0); 
}

/**
  * @brief 删除提醒弹窗回调函数
  * @note 闹钟的删除和相应标志位的清除都在其中
  * @param WM_MESSAGE * pMsg
  */
static void _cbAcTip(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  hItem = pMsg->hWin;
  switch (pMsg->MsgId) {
	  case WM_INIT_DIALOG:
		FRAMEWIN_SetTitleHeight(hItem, 2);
	    hItem = WM_GetDialogItem(pMsg->hWin, ID_TIP); //提示信息
	    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
		hItem = WM_GetDialogItem(pMsg->hWin, ID_AC_SURE); //按键
		BUTTON_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
	    hItem = WM_GetDialogItem(pMsg->hWin, ID_AC_CANCEL); //按键
		BUTTON_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
		break;
	  case WM_NOTIFY_PARENT:
		Id    = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) {
			case ID_AC_SURE: //'Sure'
			  switch(NCode) {
				  case WM_NOTIFICATION_CLICKED:
					if(delet_ac_sure==1){ //闹钟1
						for(int i=0;i<12;i++){ //清除实际闹钟值
							if(i<9) { *(aclock_arr+i)=*(aclock_arr+i+3);}
							else { *(aclock_arr+i)=0;}
						}
						for(int j=0;j<4;j++){ // 删除闹钟显示值
							if(j<3) { *(ac_show+j) = *(ac_show+j+1);}
							else { *(aclock_arr+j)=0;}
						}
						ac_dele_flag=ac_dele_flag>>1;//清除标志位
						ac_sure--;//闹钟数-1
						ac_temp--;//临时数-1
						ac_index = ac_sure*3;//实际闹钟值-3(周时分)
						need_flash=1;//需要刷新
					}

					if(delet_ac_sure==2){ //闹钟2
						for(int i=3;i<12;i++){
							if(i<9) { *(aclock_arr+i)=*(aclock_arr+i+3);}
							else { *(aclock_arr+i)=0;}
						}
						for(int j=1;j<4;j++){ 
							if(j<3) { *(ac_show+j) = *(ac_show+j+1);}
							else { *(aclock_arr+j)=0;}
						}
						ac_dele_flag=ac_dele_flag>>1;//清除标志位
						ac_sure--;
						ac_temp--;
						ac_index = ac_sure*3;
						need_flash=1;
					}

					if(delet_ac_sure==3){ //闹钟3
						for(int i=6;i<12;i++){
							if(i<9) { *(aclock_arr+i)=*(aclock_arr+i+3);}
							else { *(aclock_arr+i)=0;}
						}
						*(ac_show+2)= *(ac_show+2);
						*(ac_show+3) = 0;

						ac_dele_flag=ac_dele_flag>>1;//清除标志位
						ac_sure--;
						ac_temp--;
						ac_index=ac_sure*3;
						need_flash=1;
					}

					if(delet_ac_sure==4){ //闹钟4
						for(int i=9;i<12;i++){ *(aclock_arr+i)=0;} 
						*(ac_show+3) = 0;
						ac_dele_flag=ac_dele_flag>>1;//清除标志位
						ac_sure--;
						ac_temp--;
						ac_index=ac_sure*3;
						need_flash=1;
					} 
					GUI_EndDialog(hItem,0);
					dele_ac_win=1;//允许外部启用弹窗
					break;
				  }break;
			case ID_AC_CANCEL: // 'Cancel'
			  switch(NCode) {
				  case WM_NOTIFICATION_CLICKED: 
					 GUI_EndDialog(hItem,0);
					 dele_ac_win=1; //允许外部启用弹窗
					 break;
			  }break;
     default:
	    WM_DefaultProc(pMsg);
	    break;
	   }
   }
}

/**
  * @brief 闹钟关闭对话框
  * @retval hItem资源表中第一个空间的句柄
  */
WM_HWIN ac_tip_win(void) {
  return  GUI_CreateDialogBox(_aAcTipCreate, GUI_COUNTOF(_aAcTipCreate), _cbAcTip, hSetAclock, 0, 0);
}


