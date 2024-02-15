#include "DIALOG.h"
#include "actip.h"

#define ID_FRAMEWIN_1     (GUI_ID_USER + 0x00) //����ɾ����ʾ����
#define ID_AC_SURE        (GUI_ID_USER + 0x01)
#define ID_AC_CANCEL      (GUI_ID_USER + 0x02)
#define ID_TIP            (GUI_ID_USER + 0x03)

//��Դ��
static const GUI_WIDGET_CREATE_INFO _aAcTipCreate[] = {
  { FRAMEWIN_CreateIndirect, "", ID_FRAMEWIN_1, 110, 40, 180, 125, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_AC_CANCEL, 24, 82, 51, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Sure", ID_AC_SURE, 100, 82, 51, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Are you sure delete it", ID_TIP, 10, 10, 150, 65, 0, 0x0, 0 },
};

//�������Ӵ��ڵ�ȫ��ֵ
extern WM_HWIN hSetAclock; // �������Ӿ��
extern  char*  aclock_arr[12];//ʵ������ֵ
extern  int    ac_sure;    //�Ѿ�ȷ�������Ӹ���
extern  int    ac_temp;    //��ʱ���Ӹ���
extern  int    ac_index;
extern  char*  ac_show[4]; //������ʾ������
extern  u8     ac_dele_flag;

u8 close_win;
u8 delet_ac_sure;//ȷ������һ�����ӱ�ɾ��

static void _cbAcTip(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  hItem = pMsg->hWin;
  switch (pMsg->MsgId) {
	  case WM_INIT_DIALOG:
		FRAMEWIN_SetTitleHeight(hItem, 2);
	    hItem = WM_GetDialogItem(pMsg->hWin, ID_TIP); //��ʾ��Ϣ
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
						  if(delet_ac_sure==1){ //����1
							 for(int i=0;i<12;i++){ //���ʵ������ֵ
								if(i<9) { *(aclock_arr+i)=*(aclock_arr+i+3);}
								else { *(aclock_arr+i)=0;}
							 }
							 for(int j=0;j<4;j++){ 
								if(j<3) { *(ac_show+j) = *(ac_show+j+1);}
								else { *(aclock_arr+j)=0;}
							 }
							 ac_dele_flag=ac_dele_flag>>1;//�����־λ XXX1 => 0XXX 
							 ac_sure--;//������-1
							 ac_temp--;//��ʱ��-1
							 ac_index = ac_sure*3;//ʵ������ֵ-3
						   }
						  
						   if(delet_ac_sure==2){//����2
							 for(int i=3;i<12;i++){
								if(i<9) { *(aclock_arr+i)=*(aclock_arr+i+3);}
								else { *(aclock_arr+i)=0;}
							 }
							 for(int j=1;j<4;j++){ 
								if(j<3) { *(ac_show+j) = *(ac_show+j+1);}
								else { *(aclock_arr+j)=0;}
							 }
							 ac_dele_flag=ac_dele_flag>>1;//�����־λ
							 ac_sure--;
							 ac_temp--;
							 ac_index = ac_sure*3;
						   }
						  
						  if(delet_ac_sure==3){//����3
							 for(int i=6;i<12;i++){
							 	if(i<9) { *(aclock_arr+i)=*(aclock_arr+i+3);}
								else { *(aclock_arr+i)=0;}
							 }
							 *(ac_show+2)= *(ac_show+3);
							 *(ac_show+3) = 0;
							
							 ac_dele_flag=ac_dele_flag>>1;//�����־λ
							 ac_sure--;
							 ac_temp--;
							 ac_index=ac_sure*3;
						   }
						  
						  if(delet_ac_sure==4){//����4
							 for(int i=9;i<12;i++){ *(aclock_arr+i)=0;} 
							 *(ac_show+3) = 0;
							 ac_dele_flag=ac_dele_flag>>1;//�����־λ
							 ac_sure--;
							 ac_temp--;
							 ac_index=ac_sure*3;
						 }    
						  
						 GUI_EndDialog(hItem,0);
						 close_win=0;//�����ⲿ���õ���
					  }break;
				  } break;
			case ID_AC_CANCEL: // Notifications sent by 'Cancel'
			  switch(NCode) {
				  case WM_NOTIFICATION_CLICKED:
					  if(close_win!=0) {
						 GUI_EndDialog(hItem,0);
						 close_win=0; //�����ⲿ���õ���
					  }break;
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
WM_HWIN ac_tip_win(void) {
  return  GUI_CreateDialogBox(_aAcTipCreate, GUI_COUNTOF(_aAcTipCreate), _cbAcTip, hSetAclock, 0, 0);
}

