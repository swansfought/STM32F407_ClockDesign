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

//	START任务
#define START_TASK_PRIO				3		//任务优先级
#define START_STK_SIZE 				1024	//任务堆栈大小	
OS_TCB StartTaskTCB; //任务控制块	
CPU_STK START_TASK_STK[START_STK_SIZE];//任务堆栈	
void start_task(void *p_arg); //start_task任务函数接口

//	TOUCH任务
#define TOUCH_TASK_PRIO				4		//设置任务优先级
#define TOUCH_STK_SIZE				128		//任务堆栈大小
OS_TCB TouchTaskTCB; //任务控制块	
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE]; //任务堆栈
void touch_task(void *p_arg); //touch_task任务函数接口

//	EMWINDEMO任务
#define EMWINDEMO_TASK_PRIO			5
#define EMWINDEMO_STK_SIZE			2048
OS_TCB EmwindemoTaskTCB;
CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
void emwindemo_task(void *p_arg);

//	KEY任务
#define KEY_TASK_PRIO				6
#define KEY_STK_SIZE				128
OS_TCB KeyTaskTCB;
CPU_STK KEY_TASK_STK[KEY_STK_SIZE];
void key_task(void *p_arg);

//	LED0任务
#define LED0_TASK_PRIO				8
#define LED0_STK_SIZE				128
OS_TCB Led0TaskTCB;
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);

int main(void){
	OS_ERR err;
	CPU_SR_ALLOC();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组配置
	delay_init(168);   //延时初始化
	uart_init(115200); //串口波特率设置
	TFTLCD_Init();     //初始化LCD
	TP_Init();	       //初始化触摸屏
	LED_Init();	       //LED初始化
	BEEP_Init();	   //蜂鸣器初始化	
	KEY_Init();		   //按键初始化
	_RTC_Init(); 	   //RTC初始化
	Adc_Init();        //内部温度传感器ADC初始化
	/*
	 *	定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft (us)
	 *	Ft = 定时器工作频率,单位:Mhz
	 */
 	TIM3_Int_Init(10000-1,8400-1);//1s
	
	
	
	FSMC_SRAM_Init();  //SRAM初始化	
	mem_init(SRAMIN);  //内部RAM初始化
	mem_init(SRAMEX);  //外部RAM初始化
	//mem_init(SRAMCCM);//CCM初始化
	OSInit(&err);//初始化UCOSIII
	OS_CRITICAL_ENTER();//进入临界区
	OSTaskCreate((OS_TCB*	 )&StartTaskTCB,//任务控制块
			    (CPU_CHAR* 	 )"start task",//开始函数(start_task)
                (OS_TASK_PTR )start_task,//任务函数
                (void* 		 )0,//传递给任务函数的参数
                (OS_PRIO	 )START_TASK_PRIO,//任务优先级
                (CPU_STK* 	 )&START_TASK_STK[0],//任务堆栈基地址
                (CPU_STK_SIZE)START_STK_SIZE/10,//任务堆栈深度限位
                (CPU_STK_SIZE)START_STK_SIZE,//任务堆栈大小
                (OS_MSG_QTY  )0,//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                (OS_TICK	 )0,//当使能时间片轮转时的时间片长度，为0时为默认长度，
                (void* 		 )0,//用户补充的存储区
                (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                (OS_ERR* 	 )&err);	//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);//开启UCOSIII
	while(1){ }
}

//开始任务函数
void start_task(void *p_arg){
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	CPU_Init();
/*必须加上-----------------------------------------------------------------------*/
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err); //统计任务                
#endif
#ifdef CPU_CFG_INT_DIS_MEAS_EN	 //如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);//使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
#endif		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);//开启CRC时钟
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init(); // STemWin初始化
/*------------------------------------------------------------------------------*/
	OS_CRITICAL_ENTER();//进入临界区
	//STemWin Demo任务	
	OSTaskCreate((OS_TCB*    )&EmwindemoTaskTCB,		(CPU_CHAR*   )"Emwindemo task", 		
                (OS_TASK_PTR )emwindemo_task, 			(void*       )0,					
                (OS_PRIO	 )EMWINDEMO_TASK_PRIO,      (CPU_STK*    )&EMWINDEMO_TASK_STK[0],	
                (CPU_STK_SIZE)EMWINDEMO_STK_SIZE/10,	(CPU_STK_SIZE)EMWINDEMO_STK_SIZE,		
                (OS_MSG_QTY  )0,					    (OS_TICK	 )0,  					
                (void*       )0,					    (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                (OS_ERR*     )&err);
	//触摸屏任务
	OSTaskCreate((OS_TCB*    )&TouchTaskTCB,            (CPU_CHAR*   )"Touch task", 		
                (OS_TASK_PTR )touch_task,               (void*       )0,					
                (OS_PRIO	 )TOUCH_TASK_PRIO,          (CPU_STK*    )&TOUCH_TASK_STK[0],	
                (CPU_STK_SIZE)TOUCH_STK_SIZE/10,        (CPU_STK_SIZE)TOUCH_STK_SIZE,		
                (OS_MSG_QTY  )0,	                    (OS_TICK	 )0,  					
                (void*       )0,						(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
			    (OS_ERR*     )&err);
	//KEY任务					 
	OSTaskCreate((OS_TCB*    )&KeyTaskTCB,			    (CPU_CHAR*   )"Key task",
                (OS_TASK_PTR )key_task, 				(void*       )0,					
                (OS_PRIO	 )KEY_TASK_PRIO,  	        (CPU_STK*    )&KEY_TASK_STK[0],	
                (CPU_STK_SIZE)KEY_STK_SIZE/10,	        (CPU_STK_SIZE)KEY_STK_SIZE,		
                (OS_MSG_QTY  )0,						(OS_TICK	 )0,  					
			    (void*       )0,						(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                (OS_ERR*     )&err);
	//LED0任务					 
	OSTaskCreate((OS_TCB*    )&Led0TaskTCB,			    (CPU_CHAR*   )"Led0 task",
                (OS_TASK_PTR )led0_task, 				(void*       )0,					
                (OS_PRIO	 )LED0_TASK_PRIO,  	        (CPU_STK*    )&LED0_TASK_STK[0],	
                (CPU_STK_SIZE)LED0_STK_SIZE/10,	        (CPU_STK_SIZE)LED0_STK_SIZE,		
                (OS_MSG_QTY  )0,						(OS_TICK	 )0,  					
			    (void*       )0,						(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                (OS_ERR*     )&err);
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);//挂起开始任务		 
	OS_CRITICAL_EXIT();//退出临界区
}

//EMWINDEMO任务
void emwindemo_task(void *p_arg){
	//GUI_CURSOR_Show(); //显示鼠标
    WM_SetCreateFlags(WM_CF_MEMDEV);//为重绘操作自动使用内存设备
	WM_SetDesktopColor(GUI_DARKCYAN);//设置桌面窗口颜色
	
	//	GUI_UC_SetEncodeUTF8(); //
	//更换皮肤
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
	 
	interface_win(); //时钟日期界面
	dock_win(); //菜单栏
	while(1){ 
		GUI_Delay(10); 
	} 
}

//TOUCH任务
void touch_task(void *p_arg){
	OS_ERR err;
	while(1){
		GUI_TOUCH_Exec();	
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//延时5ms
	}
	
}

//按键扫描任务
void key_task(void *pdata){	
	u8 key;		    						 
	while(1){
		//key=KEY_Scan(0);
		switch(key){
			case 1://挂起LED0任务，LED停止闪烁
				//OSTaskSuspend(LED0_TASK_PRIO);
				break;
			case 2://重新创建任务LED1
			//OSTaskCreate(led_1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);
				break;
			case 3://恢复LED0任务，LED恢复闪烁
				//OSTaskResume(LED0_TASK_PRIO);
				break;
			case 4://发送删除LED1任务请求，任务睡眠，无法恢复
				//OSTaskDelReq(LED1_TASK_PRIO);
				break;
		}
		delay_ms(20);
	}	
}


//LED0任务
void led0_task(void *p_arg){ // 用来提示程序的正常运行
	OS_ERR err;
	while(1){
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//延时500ms
	}
}


