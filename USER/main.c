#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "sram.h"
#include "malloc.h"
#include "ILI93xx.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "rtc.h"
#include "timer.h"
#include "touch.h"
#include "GUI.h"
#include "includes.h"
#include "DIALOG.h"
#include "adc.h"
#include "lcd.h"
#include "interface.h"
#include "dock.h"
#include "alarm_clock.h"
#include "calendar.h"
#include "timer.h"

//	START����
#define START_TASK_PRIO				3		//�������ȼ�
#define START_STK_SIZE 				1024	//�����ջ��С	
OS_TCB StartTaskTCB; //������ƿ�	
CPU_STK START_TASK_STK[START_STK_SIZE];//�����ջ	
void start_task(void *p_arg); //start_task�������ӿ�

//	TOUCH����
#define TOUCH_TASK_PRIO				4		//�����������ȼ�
#define TOUCH_STK_SIZE				128		//�����ջ��С
OS_TCB TouchTaskTCB; //������ƿ�	
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE]; //�����ջ
void touch_task(void *p_arg); //touch_task�������ӿ�

//	EMWINDEMO����
#define EMWINDEMO_TASK_PRIO			5
#define EMWINDEMO_STK_SIZE			2048
OS_TCB EmwindemoTaskTCB;
CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
void emwindemo_task(void *p_arg);

//	KEY����
#define KEY_TASK_PRIO				6
#define KEY_STK_SIZE				128
OS_TCB KeyTaskTCB;
CPU_STK KEY_TASK_STK[KEY_STK_SIZE];
void key_task(void *p_arg);

//	LED0����
#define LED0_TASK_PRIO				8
#define LED0_STK_SIZE				128
OS_TCB Led0TaskTCB;
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);

int main(void){
	OS_ERR err;
	CPU_SR_ALLOC();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�жϷ�������
	delay_init(168);   //��ʱ��ʼ��
	uart_init(115200); //���ڲ���������
	TFTLCD_Init();     //��ʼ��LCD
	TP_Init();	       //��ʼ��������
	LED_Init();	       //LED��ʼ��
	BEEP_Init();	   //��������ʼ��	
	KEY_Init();		   //������ʼ��
	_RTC_Init(); 	   //RTC��ʼ��
	Adc_Init();        //�ڲ��¶ȴ�����ADC��ʼ��
	/*
	 *	��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft (us)
	 *	Ft = ��ʱ������Ƶ��,��λ:Mhz
	 */
 	TIM3_Int_Init(10000-1,8400-1);//1s
	
	
	
	FSMC_SRAM_Init();  //SRAM��ʼ��	
	mem_init(SRAMIN);  //�ڲ�RAM��ʼ��
	mem_init(SRAMEX);  //�ⲿRAM��ʼ��
	//mem_init(SRAMCCM);//CCM��ʼ��
	OSInit(&err);//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();//�����ٽ���
	OSTaskCreate((OS_TCB*	 )&StartTaskTCB,//������ƿ�
			    (CPU_CHAR* 	 )"start task",//��ʼ����(start_task)
                (OS_TASK_PTR )start_task,//������
                (void* 		 )0,//���ݸ��������Ĳ���
                (OS_PRIO	 )START_TASK_PRIO,//�������ȼ�
                (CPU_STK* 	 )&START_TASK_STK[0],//�����ջ����ַ
                (CPU_STK_SIZE)START_STK_SIZE/10,//�����ջ�����λ
                (CPU_STK_SIZE)START_STK_SIZE,//�����ջ��С
                (OS_MSG_QTY  )0,//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                (OS_TICK	 )0,//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                (void* 		 )0,//�û�����Ĵ洢��
                (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                (OS_ERR* 	 )&err);	//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);//����UCOSIII
	while(1){ }
}

//��ʼ������
void start_task(void *p_arg){
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	CPU_Init();
/*�������-----------------------------------------------------------------------*/
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err); //ͳ������                
#endif
#ifdef CPU_CFG_INT_DIS_MEAS_EN	 //���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);//ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
#endif		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);//����CRCʱ��
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init(); // STemWin��ʼ��
/*------------------------------------------------------------------------------*/
	OS_CRITICAL_ENTER();//�����ٽ���
	//STemWin Demo����	
	OSTaskCreate((OS_TCB*    )&EmwindemoTaskTCB,		(CPU_CHAR*   )"Emwindemo task", 		
                (OS_TASK_PTR )emwindemo_task, 			(void*       )0,					
                (OS_PRIO	 )EMWINDEMO_TASK_PRIO,      (CPU_STK*    )&EMWINDEMO_TASK_STK[0],	
                (CPU_STK_SIZE)EMWINDEMO_STK_SIZE/10,	(CPU_STK_SIZE)EMWINDEMO_STK_SIZE,		
                (OS_MSG_QTY  )0,					    (OS_TICK	 )0,  					
                (void*       )0,					    (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                (OS_ERR*     )&err);
	//����������
	OSTaskCreate((OS_TCB*    )&TouchTaskTCB,            (CPU_CHAR*   )"Touch task", 		
                (OS_TASK_PTR )touch_task,               (void*       )0,					
                (OS_PRIO	 )TOUCH_TASK_PRIO,          (CPU_STK*    )&TOUCH_TASK_STK[0],	
                (CPU_STK_SIZE)TOUCH_STK_SIZE/10,        (CPU_STK_SIZE)TOUCH_STK_SIZE,		
                (OS_MSG_QTY  )0,	                    (OS_TICK	 )0,  					
                (void*       )0,						(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
			    (OS_ERR*     )&err);
	//KEY����					 
	OSTaskCreate((OS_TCB*    )&KeyTaskTCB,			    (CPU_CHAR*   )"Key task",
                (OS_TASK_PTR )key_task, 				(void*       )0,					
                (OS_PRIO	 )KEY_TASK_PRIO,  	        (CPU_STK*    )&KEY_TASK_STK[0],	
                (CPU_STK_SIZE)KEY_STK_SIZE/10,	        (CPU_STK_SIZE)KEY_STK_SIZE,		
                (OS_MSG_QTY  )0,						(OS_TICK	 )0,  					
			    (void*       )0,						(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                (OS_ERR*     )&err);
	//LED0����					 
	OSTaskCreate((OS_TCB*    )&Led0TaskTCB,			    (CPU_CHAR*   )"Led0 task",
                (OS_TASK_PTR )led0_task, 				(void*       )0,					
                (OS_PRIO	 )LED0_TASK_PRIO,  	        (CPU_STK*    )&LED0_TASK_STK[0],	
                (CPU_STK_SIZE)LED0_STK_SIZE/10,	        (CPU_STK_SIZE)LED0_STK_SIZE,		
                (OS_MSG_QTY  )0,						(OS_TICK	 )0,  					
			    (void*       )0,						(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                (OS_ERR*     )&err);
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);//����ʼ����		 
	OS_CRITICAL_EXIT();//�˳��ٽ���
}

//EMWINDEMO����
void emwindemo_task(void *p_arg){
	//GUI_CURSOR_Show(); //��ʾ���
    WM_SetCreateFlags(WM_CF_MEMDEV);//Ϊ�ػ�����Զ�ʹ���ڴ��豸
	WM_SetDesktopColor(GUI_DARKCYAN);//�������洰����ɫ
	
	//	GUI_UC_SetEncodeUTF8(); //
	//����Ƥ��
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX); 
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	MENU_SetDefaultSkin(MENU_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
	 
	interface_win(); //ʱ�����ڽ���
	dock_win(); //�˵���
	while(1){ 
		GUI_Delay(10); 
	} 
}

//TOUCH����
void touch_task(void *p_arg){
	OS_ERR err;
	while(1){
		GUI_TOUCH_Exec();	
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//��ʱ5ms
	}
	
}

//����ɨ������
void key_task(void *pdata){	
	u8 key;		    						 
	while(1){
		//key=KEY_Scan(0);
		switch(key){
			case 1://����LED0����LEDֹͣ��˸
				//OSTaskSuspend(LED0_TASK_PRIO);
				break;
			case 2://���´�������LED1
			//OSTaskCreate(led_1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);
				break;
			case 3://�ָ�LED0����LED�ָ���˸
				//OSTaskResume(LED0_TASK_PRIO);
				break;
			case 4://����ɾ��LED1������������˯�ߣ��޷��ָ�
				//OSTaskDelReq(LED1_TASK_PRIO);
				break;
		}
		delay_ms(20);
	}	
}


//LED0����
void led0_task(void *p_arg){ // ������ʾ�������������
	OS_ERR err;
	while(1){
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//��ʱ500ms
	}
}


