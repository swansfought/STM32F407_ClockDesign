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

#define ID_FRAMEWIN_1    (GUI_ID_USER + 0x15) //����ɾ�����ڿؼ�
#define ID_AC_CLOSE      (GUI_ID_USER + 0x16)
#define ID_AC_CANCEL     (GUI_ID_USER + 0x17)
#define ID_TIP           (GUI_ID_USER + 0x18)

// ��Դ��
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

//���Ӵ���������Դ��
static const GUI_WIDGET_CREATE_INFO _aAcTipCreate[] = {
  { FRAMEWIN_CreateIndirect, "", ID_FRAMEWIN_1, 110, 40, 180, 125, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_AC_CANCEL, 20, 82, 55, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Close", ID_AC_CLOSE, 96, 82, 55, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Alarm Clock Time!", ID_TIP, 10, 10, 150, 65, 0, 0x0, 0 },
};

//�ⲿAlarmģ������ӱ���
extern  char*  aclock_arr[12];//ʵ������ֵ���ܡ�ʱ����
extern  int    ac_sure;      //��ȷ�������Ӹ���

//��������
int    t_h,t_m,t_w,t_s; //RTC��ȡ��ʱ���֡��ܡ���
int    ac_h,ac_m,ac_w;  //����ʱ���֡���
int    d_year,d_month,d_day;//��ȡ��������,��"����"ʹ��!!!
char   t_show[10],t_ac[10];//��ȡ������ʱ�䡢����ֵ
int    alarm_stop = 1;  //�����־��Ĭ�Ͻ�ֹ
u8     close_ac_win = 1;//���ѵ������һ��
short  temperature;     //�¶�ֵ
int    refresh_cycle;   //�¶�ˢ������
RTC_TimeTypeDef  RTC_TimeStruct; //RTC�ṹ������
RTC_DateTypeDef  RTC_DateStruct;
WM_HWIN  hShowWin,hHour,hMinute,hSecond,hYear,hMonth,hDay,hWeek;//��ʾ���ڡ�ʱ���֡��롢�ꡢ�¡��վ��
WM_HWIN  hClose;//���嵯�����

/**
  * @brief ������ص�����
  * @note ��ʾ��Ϣ��ƥ������
  * @param WM_MESSAGE * pMsg
  */
static void _cbShowInterface(WM_MESSAGE * pMsg) {
  hShowWin = pMsg->hWin;
  short temp = 0;
  switch (pMsg->MsgId) {
	  case WM_INIT_DIALOG:
		FRAMEWIN_SetTitleHeight(hShowWin, 1);
	    FRAMEWIN_SetClientColor(hShowWin,0X00D3D3D3);//GUI_CYAN
		WM_CreateTimer(WM_GetClientWindow(pMsg->hWin), 0, 100, 0); //������ʱ�� 
	    for(int i=0;i<4;i++){ //	:
			hShowWin = WM_GetDialogItem(pMsg->hWin, ID_SPOT_1_UP+i);
			TEXT_SetFont(hShowWin, GUI_FONT_D48X64);
			TEXT_SetTextAlign(hShowWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
		}
	    //  ��
		hYear = WM_GetDialogItem(pMsg->hWin, ID_YEAR);
		TEXT_SetFont(hYear, GUI_FONT_COMIC18B_ASCII);
		TEXT_SetTextAlign(hYear, GUI_TA_HCENTER | GUI_TA_VCENTER);
		// 	��
		hMonth = WM_GetDialogItem(pMsg->hWin, ID_MONTH);
		TEXT_SetFont(hMonth, GUI_FONT_COMIC18B_ASCII);
		TEXT_SetTextAlign(hMonth, GUI_TA_HCENTER | GUI_TA_VCENTER);
		// 	��
		hDay = WM_GetDialogItem(pMsg->hWin, ID_DAY);
		TEXT_SetFont(hDay, GUI_FONT_COMIC18B_ASCII);
		TEXT_SetTextAlign(hDay, GUI_TA_HCENTER | GUI_TA_VCENTER);
		// 	'��'
		hWeek = WM_GetDialogItem(pMsg->hWin, ID_WEEK);
		TEXT_SetFont(hWeek, GUI_FONT_COMIC18B_ASCII);
		TEXT_SetTextAlign(hWeek, GUI_TA_HCENTER | GUI_TA_VCENTER);
		// 	Сʱ
		hHour = WM_GetDialogItem(pMsg->hWin, ID_HOUR);
		TEXT_SetTextAlign(hHour, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hHour, GUI_FONT_D48X64);
		//	����
		hMinute = WM_GetDialogItem(pMsg->hWin, ID_MINUTE);
		TEXT_SetTextAlign(hMinute, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hMinute, GUI_FONT_D48X64);
		// 	��
		hSecond = WM_GetDialogItem(pMsg->hWin, ID_SECOND);
		TEXT_SetTextAlign(hSecond, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetFont(hSecond, GUI_FONT_D48X64);
		// 'Year                 Month                 Day'
		hShowWin = WM_GetDialogItem(pMsg->hWin, ID_T_YMS);
		TEXT_SetFont(hShowWin, GUI_FONT_COMIC24B_ASCII);
		TEXT_SetTextAlign(hShowWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
		
		for(int j=0;j<5;j++){ //Simplicity��Temp��Week���¶�ֵ����ֵ
			hShowWin = WM_GetDialogItem(pMsg->hWin, ID_TXT+j);
			TEXT_SetFont(hShowWin, GUI_FONT_16B_ASCII);
			TEXT_SetTextAlign(hShowWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
		}
		break;
	  case WM_TIMER://��ʱ����Ϣ  
/*--------------------------------------------------��ʾʵʱʱ��ģ�飨RTC��--------------------------------------------------*/
		RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);//��ȡʱ��
	    sprintf(t_show,"%02d",RTC_TimeStruct.RTC_Hours); //ʱ
	    TEXT_SetText(hHour,t_show);
		sprintf(t_show,"%02d",RTC_TimeStruct.RTC_Minutes); //��
	    TEXT_SetText(hMinute,t_show);
		sprintf(t_show,"%02d",RTC_TimeStruct.RTC_Seconds); //��
	    TEXT_SetText(hSecond,t_show); 
	    
		RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);//��ȡ����
	    strcpy(t_show,"20");
		sprintf(t_ac,"%d",RTC_DateStruct.RTC_Year); //��
	    d_year = atoi(t_ac)+2000; 
	    strcat(t_show,t_ac);//С����
	    TEXT_SetText(hYear,t_show); 
		sprintf(t_show,"%02d",RTC_DateStruct.RTC_Month);//��
	    TEXT_SetText(hMonth,t_show); 
	    d_month = atoi(t_show);
	    sprintf(t_show,"%02d",RTC_DateStruct.RTC_Date);//��
	    TEXT_SetText(hDay,t_show);
	    d_day = atoi(t_show);
		sprintf(t_show,"%d",RTC_DateStruct.RTC_WeekDay);//��   
		TEXT_SetText(hWeek,t_show); 
/*--------------------------------------------------�����ж�ģ�飨RTC�����������ӣ�--------------------------------------------------*/
		if(ac_sure>0){ //�����Ӳſ�ʼ�ж�
			sprintf((char*)t_ac,"%d",RTC_DateStruct.RTC_WeekDay);  
			t_w = atoi(t_ac);//��ȡ������
			sprintf((char*)t_ac,"%d",RTC_TimeStruct.RTC_Hours);
			t_h = atoi(t_ac);//��ȡ����ʱ
			sprintf((char*)t_ac,"%d",RTC_TimeStruct.RTC_Minutes);
			t_m = atoi(t_ac);// ��ȡ���ķ�
			sprintf((char*)t_ac,"%d",RTC_TimeStruct.RTC_Seconds);
			t_s = atoi(t_ac);// ��ȡ������
			for(int i=0;i<ac_sure;i++){ //�����ĸ����ӵ�ʱ���룬ʵ���������δ���ĸ�
				ac_w = atoi(*(aclock_arr+3*i));
				ac_h = atoi(*(aclock_arr+1+3*i)); 
				ac_m = atoi(*(aclock_arr+2+3*i));
				if((ac_w==t_w)&&(ac_h==t_h)&&(ac_m==t_m)&&(t_s==0)){ alarm_stop=0;} //ƥ��ɹ�����������
			}
			if(alarm_stop==0){ //�ж����ӷ�
				if(close_ac_win==1){
					ac_clos_win(); //���ѵ���
					close_ac_win=0;
				}
				alarm_beep(); //����
				if((atoi(t_ac)>5)){ //δ�ر����Ӻ͵������
					alarm_stop=1; // ֹͣ����
					if(close_ac_win==0){ //��鴰���Ƿ�ر�
						close_ac_win=1;
						GUI_EndDialog(hClose,0);
					}
				}
			}
		}
/*--------------------------------------------------��ʾ�¶�ģ�飨ADC��10��ˢ��һ��--------------------------------------------------*/
		if((refresh_cycle==0)||(refresh_cycle==20)){ 
		    temperature=Get_Temprate();//��ȡ�¶�ֵ
			if(temperature<0){ // �¶�<0�����
				temperature=-temperature;
				temp = temperature/1000; //��������
				sprintf(t_show,"%d",temp);
				strcat(t_show,".");
				temp = temperature%100;
				sprintf(t_ac,"%d",temp);
				strcat(t_show,t_ac);
				strcat(t_show,"C");
				hShowWin = WM_GetDialogItem(pMsg->hWin, ID_TEMP);
				TEXT_SetText(hShowWin,t_show); //��ʾ�¶�
			}else {
				temp = temperature/1000; //��������
				sprintf(t_show,"%d",temp);
				strcat(t_show,".");
				temp = temperature%100;//С������
				sprintf(t_ac,"%d",temp);
				strcat(t_show,t_ac);
				strcat(t_show,"C");
				hShowWin = WM_GetDialogItem(pMsg->hWin, ID_TEMP);
				TEXT_SetText(hShowWin,t_show); //��ʾ�¶�
			}
		   refresh_cycle=0;
		}
		refresh_cycle++;
	    WM_RestartTimer(pMsg->Data.v,500);//500msˢ��һ��
	  default:
		WM_DefaultProc(pMsg);
		break;
	  }
  }


/**
  * @brief ��������
  * @note BEEP��ͣ��ͣ
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
  * @brief �ԶԻ���ķ�ʽ��Ӵ����ؼ�
  * @retval hItem��Դ���е�һ���ռ�ľ��
  */
WM_HWIN interface_win(void) {
  return GUI_CreateDialogBox(_aShowInterfaceCreate, GUI_COUNTOF(_aShowInterfaceCreate), _cbShowInterface, WM_HBKWIN, 0, 0);
}


/**
  * @brief �ر����Ӵ��ص�����
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
	    hClose = WM_GetDialogItem(pMsg->hWin, ID_TIP); //��ʾ��Ϣ
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
						 alarm_stop=1; //ֹͣ����
						 close_ac_win=1;//����������
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
  * @brief ���ӹرնԻ���
  * @retval hItem��Դ���е�һ���ռ�ľ��
  */
WM_HWIN ac_clos_win(void) {
  return  GUI_CreateDialogBox(_aAcTipCreate, GUI_COUNTOF(_aAcTipCreate), _cbAcClos, hShowWin, 0, 0);
}

