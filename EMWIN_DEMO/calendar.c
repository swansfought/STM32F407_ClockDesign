#include "DIALOG.h"
#include "calendar.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)
#define ID_B_CANCEL    (GUI_ID_USER + 0x01)
#define ID_DROP_YEAR   (GUI_ID_USER + 0x02)
#define ID_DROP_MONTH  (GUI_ID_USER + 0x03)
#define ID_0  (GUI_ID_USER + 0x06)
#define ID_1  (GUI_ID_USER + 0x07)
#define ID_2  (GUI_ID_USER + 0x08)
#define ID_3  (GUI_ID_USER + 0x09)
#define ID_4  (GUI_ID_USER + 0x0A)
#define ID_5  (GUI_ID_USER + 0x0B)
#define ID_6  (GUI_ID_USER + 0x0C)
#define ID_7  (GUI_ID_USER + 0x0D)
#define ID_8  (GUI_ID_USER + 0x0E)
#define ID_9  (GUI_ID_USER + 0x0F)
#define ID_10 (GUI_ID_USER + 0x10)
#define ID_11 (GUI_ID_USER + 0x11)
#define ID_12 (GUI_ID_USER + 0x12)
#define ID_13 (GUI_ID_USER + 0x13)
#define ID_14 (GUI_ID_USER + 0x14)
#define ID_15 (GUI_ID_USER + 0x15)
#define ID_16 (GUI_ID_USER + 0x16)
#define ID_17 (GUI_ID_USER + 0x17)
#define ID_18 (GUI_ID_USER + 0x18)
#define ID_19 (GUI_ID_USER + 0x19)
#define ID_20 (GUI_ID_USER + 0x1A)
#define ID_21 (GUI_ID_USER + 0x1B)
#define ID_22 (GUI_ID_USER + 0x1C)
#define ID_23 (GUI_ID_USER + 0x1D)
#define ID_24 (GUI_ID_USER + 0x1E)
#define ID_25 (GUI_ID_USER + 0x1F)
#define ID_26 (GUI_ID_USER + 0x20)
#define ID_27 (GUI_ID_USER + 0x21)
#define ID_28 (GUI_ID_USER + 0x22)
#define ID_29 (GUI_ID_USER + 0x23)
#define ID_30 (GUI_ID_USER + 0x24)
#define ID_31 (GUI_ID_USER + 0x25)
#define ID_32 (GUI_ID_USER + 0x26)
#define ID_33 (GUI_ID_USER + 0x27)
#define ID_34 (GUI_ID_USER + 0x28)
#define ID_35 (GUI_ID_USER + 0x29)
#define ID_36 (GUI_ID_USER + 0x2A)
#define ID_37 (GUI_ID_USER + 0x2B)
#define ID_38 (GUI_ID_USER + 0x2C)
#define ID_39 (GUI_ID_USER + 0x2D)
#define ID_40 (GUI_ID_USER + 0x2E)
#define ID_41 (GUI_ID_USER + 0x2F)
#define ID_42 (GUI_ID_USER + 0x30)
#define ID_43 (GUI_ID_USER + 0x31)
#define ID_44 (GUI_ID_USER + 0x32)
#define ID_45 (GUI_ID_USER + 0x34)
#define ID_46 (GUI_ID_USER + 0x35)
#define ID_47 (GUI_ID_USER + 0x36)
#define ID_48 (GUI_ID_USER + 0x37)
#define ID_49 (GUI_ID_USER + 0x38)

//资源表
static const GUI_WIDGET_CREATE_INFO _aCalendarCreate[] = {
	{ FRAMEWIN_CreateIndirect, "", ID_FRAMEWIN_0, 0, 0, 400, 225, 0, 0x64, 0 },
	{ BUTTON_CreateIndirect, "Cancel", ID_B_CANCEL, 313, 178, 60, 30, 0, 0x0,  0 },
	{ DROPDOWN_CreateIndirect, "Year", ID_DROP_YEAR, 137, 185, 80, 20, 1, 0x0, 0 },
	{ DROPDOWN_CreateIndirect, "Month", ID_DROP_MONTH, 232, 185, 60, 20, 1, 0x0, 0 },
	{ TEXT_CreateIndirect, "Tue", ID_0, 102, 5, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "Mon", ID_1, 62,  5, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Wed", ID_2, 142, 5, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "Thu", ID_3, 182, 5, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Fri", ID_4, 222, 5, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "Sat", ID_5, 262, 5, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Sun", ID_6, 302, 5, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_7,   62,  32, 35, 23, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "", ID_8,   102,  32, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_9,   142, 32, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_10,  182, 32, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_11,  222, 32, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_12,  262, 32, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_13,  302, 32, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_14,  62,  59, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_15,  102,  59, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_16, 142, 59, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_17, 182, 59, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_18, 222, 59, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_19, 262, 59, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_20, 302, 59, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_21, 62,  86, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_22, 102,  86, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_23, 142, 86, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_24, 182, 86, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_25, 222, 86, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_26, 262, 86, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_27, 302, 86, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_28, 62,  113, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_29, 102,  113, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_30, 142, 113, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_31, 182, 113, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_32, 222, 113, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_33, 262, 113, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_34, 302, 113, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_35, 62,  140, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_36, 102,  140, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_37, 142, 140, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_38, 182, 140, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_39, 222, 140, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_40, 262, 140, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_41, 302, 140, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_42, 62,  167, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_43, 102,  167, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_44, 142, 167, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_45, 182, 167, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_46, 222, 167, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "", ID_47, 262, 167, 35, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_48, 302, 167, 35, 23, 0, 0x0, 0 },{ TEXT_CreateIndirect, "Calendar", ID_49, 25, 176, 80, 30, 0, 0x0, 0 },
};

// 变量声明 
static char* _apYear[] = { "2000","2001","2002","2003","2004","2005", "2006","2007","2008","2009",
	                       "2010","2011","2012","2013","2014","2015","2016","2017","2018","2019",
	                       "2020","2021","2022","2023","2024","2025","2026","2027","2028","2029",
	                       "2030","2031","2032","2033","2034","2035","2036","2037","2038","2039",
                           "2040","2041","2042","2043","2044","2045","2046","2048","2049","2050",
						   "2051","2052","2053","2054","2054","2056","2057","2058","2059","2060"};
static char* _apMonth[] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10","11", "12"};
static char* _apDay[] = {"1",  "2",  "3",  "4",  "5",  "6",  "7", "8",  "9",  "10", "11", "12", "13", 
						 "14", "15", "16", "17", "18", "19", "20","21", "22", "23", "24", "25", "26", 
                         "27", "28", "29", "30", "31"};

//外部变量
extern  WM_HWIN hShowWin;//显示界面句柄
extern int d_year,d_month,d_day;//读取的年月日,供日历使用
extern u8 calen_win; //设置日历窗口最多一个
static int leap_year[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 }; 
static int nonleap_year[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

/**
  * @brief 求当年当月之前的总天数
  * @note 以1900年为起始年
  * @param int year, int month
  * @retval total
  */
static int total_days(int year, int month) {
	int total = 0;
	for (int i = 1900; i < year; i++) { //今年之前有多少天
		if (is_leap_year(i)) total += 366; 
		else total += 365; 
	}
	if (is_leap_year(year)) { //加上今年当月之前的天数
		for (int j = 0; j < month - 1; j++){ total += leap_year[j]; } 
	}else {
		for (int i = 0; i < month - 1; i++) { total += nonleap_year[i]; } 
	}
	return total;
}

static int is_leap_year(int year) { return ((year%4==0 && year%100!=0) || year%100==0);}

/**
  * @brief 字符数组转整型
  * @param char arr[], int size
  * @retval temp
  */
static int arr_string(char arr[], int size){
	int temp = 0;
	char* str = malloc((size + 1) * sizeof(char));
	if (str != NULL){
		for( int i = 0; i <= size; i++) { str[i] = arr[i]; } // <
		str[size] = 0; //不要忘掉'/0'
		temp = atoi(str);
	}
	return temp;
}

/**
  * @brief 打印日历
  * @note 当前年的突出显示
  * @param int year, int month,WM_MESSAGE * pMsg
  */
static void print_calendar(int year, int month,WM_MESSAGE * pMsg){
	WM_HWIN hItem;
	int total = total_days(year,month);//当年当月之前的总天数
	int current = total % 7; //月开始处
    
	hItem = WM_GetDialogItem(pMsg->hWin,ID_7);
	for(int i=0;i<current;i++){ //补空格
		hItem = WM_GetDialogItem(pMsg->hWin,ID_7 + i);
		TEXT_SetText(hItem,""); 
	}
	if (is_leap_year(year)){ //闰年 
		for (int j=0; j<42-current; j++){ //ID_7->ID_48
			if(j < leap_year[month-1]){
				hItem = WM_GetDialogItem(pMsg->hWin,ID_7+current+j);//设置天数
				//突出显示当天
				if(year==d_year && month==d_month && d_day== atoi(*(_apDay + j))){ 
					TEXT_SetFont(hItem, GUI_FONT_COMIC24B_ASCII);
					TEXT_SetBkColor(hItem,GUI_GRAY);
				}else {
					TEXT_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
					TEXT_SetBkColor(hItem,0X00D3D3D3);
				}
			
				TEXT_SetText(hItem,*(_apDay + j));
			}else{
				hItem = WM_GetDialogItem(pMsg->hWin,ID_7+current+j);//后面的全部补0
				TEXT_SetText(hItem,""); 
			}
		}
	}else { 
		for (int j=0; j<42-current; j++){ 
			if(j < nonleap_year[month - 1]){
				hItem = WM_GetDialogItem(pMsg->hWin,ID_7+current+j);
				//突出显示当天
				if(year==d_year && month==d_month && d_day== atoi(*(_apDay + j))){ 
					TEXT_SetFont(hItem, GUI_FONT_COMIC24B_ASCII);
					TEXT_SetBkColor(hItem,GUI_GRAY);
				}else {
					TEXT_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
					TEXT_SetBkColor(hItem,0X00D3D3D3);
				}
				TEXT_SetText(hItem,*(_apDay + j));
			}else{
				hItem = WM_GetDialogItem(pMsg->hWin,ID_7+current+j);
				TEXT_SetText(hItem,""); 
			}
		}
	}
}

/**
  * @brief 日历窗口回调函数
  * @note 
  * @param *pMsg 消息指针
  */
static void _cbCalendar(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  int 	  _index;
  int  	  c_year,c_month;//用于选择年月日
  char	  temp_year[10] = {0};
  char    temp_month[10] = {0};
  hItem = pMsg->hWin;
  switch (pMsg->MsgId){
	  case WM_INIT_DIALOG:
		FRAMEWIN_SetTitleHeight(hItem, 2);
		FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		FRAMEWIN_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
		FRAMEWIN_SetClientColor(hItem,0X00D3D3D3);

		hItem = WM_GetDialogItem(pMsg->hWin, ID_B_CANCEL);// Initialization of 'Cancel'
		BUTTON_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
	  
		for(int i=0;i<=50;i++){ // Initialization of 'ID_X'
			hItem=WM_GetDialogItem(pMsg->hWin,ID_0+i);
			TEXT_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		}
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROP_YEAR);// Initialization of 'Year'
		DROPDOWN_SetListHeight(hItem, 80);
		DROPDOWN_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
		DROPDOWN_SetScrollbarWidth(hItem,22);
		DROPDOWN_SetUpMode(hItem,1);//向上展开
	    DROPDOWN_SetItemSpacing(hItem,5);//各项目之间的间距
		DROPDOWN_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
		for(int i=0; i<GUI_COUNTOF(_apYear); i++){ //添加年份
			DROPDOWN_AddString(hItem, *(_apYear+i));
			if((d_year)==atoi(*(_apYear+i))) { DROPDOWN_SetSel(hItem,i+1);}// 设置当前选定内容
		}

		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROP_MONTH);// Initialization of 'Month'
		DROPDOWN_SetListHeight(hItem, 80);
		DROPDOWN_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
		DROPDOWN_SetScrollbarWidth(hItem,22);
		DROPDOWN_SetUpMode(hItem,1);
		DROPDOWN_SetItemSpacing(hItem,5);//各项目之间的间距
		DROPDOWN_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
		for(int j=0; j<GUI_COUNTOF(_apMonth); j++){ //添加月份
			DROPDOWN_AddString(hItem, *(_apMonth+j));
			if(d_month==atoi(*(_apMonth+j))) { DROPDOWN_SetSel(hItem,j+1);}// 设置当前选定内容
		}	
		
		print_calendar(d_year, d_month,pMsg);//初始化日历
		
		break;
	  case WM_NOTIFY_PARENT:
		Id    = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) {
		case ID_B_CANCEL: //Cancel
		  switch(NCode) {
			  case WM_NOTIFICATION_RELEASED:
				 calen_win = 1; //允许创建日历窗口
				 GUI_EndDialog(hItem,0);
				 break;
			}break;
		case ID_DROP_YEAR: //Year
		  switch(NCode) {
			  case WM_NOTIFICATION_CLICKED:
				break;
			  case WM_NOTIFICATION_RELEASED:
				break;
			  case WM_NOTIFICATION_SEL_CHANGED: 
				  hItem = WM_GetDialogItem(pMsg->hWin,ID_DROP_YEAR);
				  _index = DROPDOWN_GetSel(hItem);
				  DROPDOWN_GetItemText(hItem,_index,temp_year,GUI_COUNTOF(temp_year)); //获取年份
				  c_year = arr_string(temp_year,strlen(temp_year));//当前年份
				  hItem = WM_GetDialogItem(pMsg->hWin,ID_DROP_MONTH);
			      _index = DROPDOWN_GetSel(hItem);
				  DROPDOWN_GetItemText(hItem,_index,temp_month,GUI_COUNTOF(temp_month)); //获取月份
				  c_month = arr_string(temp_month,strlen(temp_month));//当前月份
				  print_calendar(c_year,c_month,pMsg); //打印日历
				  break;
			  }break;
		case ID_DROP_MONTH: //Month
		  switch(NCode) {
			  case WM_NOTIFICATION_CLICKED:
				break;
			  case WM_NOTIFICATION_RELEASED:
				break;
			  case WM_NOTIFICATION_SEL_CHANGED: 
				 hItem = WM_GetDialogItem(pMsg->hWin,ID_DROP_YEAR);
				 _index = DROPDOWN_GetSel(hItem);
				 DROPDOWN_GetItemText(hItem,_index,temp_year,GUI_COUNTOF(temp_year));
				 c_year= arr_string(temp_year,strlen(temp_year));// 当前年份
				 hItem = WM_GetDialogItem(pMsg->hWin,ID_DROP_MONTH);
			     _index = DROPDOWN_GetSel(hItem);
				 DROPDOWN_GetItemText(hItem,_index,temp_month,GUI_COUNTOF(temp_month));
				 c_month = arr_string(temp_month,strlen(temp_month));// 当前月份
				 print_calendar(c_year,c_month,pMsg); //打印日历
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
WM_HWIN calendar_win(void) {
  return GUI_CreateDialogBox(_aCalendarCreate, GUI_COUNTOF(_aCalendarCreate), _cbCalendar, hShowWin, 0, 0);
}




