#include "DIALOG.h"
#include "actip.h"

#define ID_FRAMEWIN_1     (GUI_ID_USER + 0x00) //闹钟删除提示窗口
#define ID_AC_SURE        (GUI_ID_USER + 0x01)
#define ID_AC_CANCEL      (GUI_ID_USER + 0x02)
#define ID_TIP            (GUI_ID_USER + 0x03)

//资源表
static const GUI_WIDGET_CREATE_INFO _aAcTipCreate[] = {
  { FRAMEWIN_CreateIndirect, "", ID_FRAMEWIN_1, 110, 40, 180, 125, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_AC_CANCEL, 24, 82, 51, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Sure", ID_AC_SURE, 100, 82, 51, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Are you sure delete it", ID_TIP, 10, 10, 150, 65, 0, 0x0, 0 },
};

//设置闹钟窗口的全局值
extern WM_HWIN hSetAclock; // 设置闹钟句柄
extern  char*  aclock_arr[12];//实际闹钟值
extern  int    ac_sure;    //已经确定的闹钟个数
extern  int    ac_temp;    //临时闹钟个数
extern  int    ac_index;
extern  char*  ac_show[4]; //用于显示的闹钟
extern  u8     ac_dele_flag;

u8 close_win;
u8 delet_ac_sure;//确定是哪一个闹钟被删除

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
		TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
		break;
	  case WM_NOTIFY_PARENT:
		Id    = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) {
			case ID_AC_SURE: // Notifications sent by 'Sure'
			  switch(NCode) {
				  case WM_NOTIFICATION_CLICKED:
					  if(close_win!=0){
						  if(delet_ac_sure==1){ //闹钟1
							 for(int i=0;i<12;i++){ //清除实际闹钟值
								if(i<9) { *(aclock_arr+i)=*(aclock_arr+i+3);}
								else { *(aclock_arr+i)=0;}
							 }
							 for(int j=0;j<4;j++){ 
								if(j<3) { *(ac_show+j) = *(ac_show+j+1);}
								else { *(aclock_arr+j)=0;}
							 }
							 ac_dele_flag=ac_dele_flag>>1;//清除标志位 XXX1 => 0XXX 
							 ac_sure--;//闹钟数-1
							 ac_temp--;//临时数-1
							 ac_index = ac_sure*3;//实际闹钟值-3
						   }
						  
						   if(delet_ac_sure==2){//闹钟2
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
						   }
						  
						  if(delet_ac_sure==3){//闹钟3
							 for(int i=6;i<12;i++){
							 	if(i<9) { *(aclock_arr+i)=*(aclock_arr+i+3);}
								else { *(aclock_arr+i)=0;}
							 }
							 *(ac_show+2)= *(ac_show+3);
							 *(ac_show+3) = 0;
							
							 ac_dele_flag=ac_dele_flag>>1;//清除标志位
							 ac_sure--;
							 ac_temp--;
							 ac_index=ac_sure*3;
						   }
						  
						  if(delet_ac_sure==4){//闹钟4
							 for(int i=9;i<12;i++){ *(aclock_arr+i)=0;} 
							 *(ac_show+3) = 0;
							 ac_dele_flag=ac_dele_flag>>1;//清除标志位
							 ac_sure--;
							 ac_temp--;
							 ac_index=ac_sure*3;
						 }    
						  
						 GUI_EndDialog(hItem,0);
						 close_win=0;//允许外部启用弹窗
					  }break;
				  } break;
			case ID_AC_CANCEL: // Notifications sent by 'Cancel'
			  switch(NCode) {
				  case WM_NOTIFICATION_CLICKED:
					  if(close_win!=0) {
						 GUI_EndDialog(hItem,0);
						 close_win=0; //允许外部启用弹窗
					  }break;
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
WM_HWIN ac_tip_win(void) {
  return  GUI_CreateDialogBox(_aAcTipCreate, GUI_COUNTOF(_aAcTipCreate), _cbAcTip, hSetAclock, 0, 0);
}

