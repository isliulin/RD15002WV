#include  <includes.h>

/***************************************************************/
//任务控制块
static  OS_TCB   AppTaskTASK1TCB;
static  OS_TCB   AppTaskTASK2TCB;
static  OS_TCB   AppTaskTASK3TCB;
//任务堆栈
static  CPU_STK  AppTaskTASK1Stk[APP_CFG_TASK_TASK1_STK_SIZE];
static  CPU_STK  AppTaskTASK2Stk[APP_CFG_TASK_TASK2_STK_SIZE];
static  CPU_STK  AppTaskTASK3Stk[APP_CFG_TASK_TASK3_STK_SIZE];

/***************************************************************/
PQueueInfo pUart3QueueInfo;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//开始任务
void  AppTaskStart (void *p_arg)
{
	OS_ERR  err;
	CPU_Init();//初始化cpu，禁用中断，开启时间戳等
	BSP_Tick_Init();//设置滴答定时器
/*------------------------------*/		 
	OSTaskCreate((OS_TCB       *)&AppTaskTASK1TCB,              
                 (CPU_CHAR     *)"task1",
                 (OS_TASK_PTR   )task1, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_TASK1_PRIO,
                 (CPU_STK      *)&AppTaskTASK1Stk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_TASK1_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_TASK1_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
/*------------------------------*/
	OSTaskCreate((OS_TCB       *)&AppTaskTASK2TCB,              
                 (CPU_CHAR     *)"task2",
                 (OS_TASK_PTR   )task2, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_TASK2_PRIO,
                 (CPU_STK      *)&AppTaskTASK2Stk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_TASK2_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_TASK2_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);  
/*-------------------------------*/
	OSTaskCreate((OS_TCB       *)&AppTaskTASK3TCB,              
                 (CPU_CHAR     *)"task3",
                 (OS_TASK_PTR   )task3, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_TASK3_PRIO,
                 (CPU_STK      *)&AppTaskTASK3Stk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_TASK3_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_TASK3_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
				 
	//创建好其他任务后就删除自己
    OSTaskDel((OS_TCB*)0,&err);
}




















///////////////////////////////////////////////////////////////////////////////////////////////////////////
//用户任务1
void task1(void)
{
	OS_ERR      err;
	while(1)
	{
		OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_DLY,&err);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
void task2(void)
{
	OS_ERR      err;
	
//	RTC_Config(extrnal);
//	set_time(0x15,0x12,0x28,0x23,0x59,0x50,0x01,RTC_H12_PM);
//	set_alarm(0,1,0);
//	printf("%s\r\n",get_time());
	
    while(1)
    {
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_DLY,&err);
		//printf("%s\r\n",get_time());
    }
}

 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void task3(void)
{
	OS_ERR      err;
	pUart3QueueInfo = malloc(sizeof(CQueueInfo));//申请内存
	memset(pUart3QueueInfo, 0, sizeof(CQueueInfo));
	
	bsp_io_init();//IO口
	UsartConfig();//串口设置配置
	SW_12V(1);//电源
	SW_5V(1);//正负电源，用于检测回波Z
	FreqModuleInit();//测频率模块初始化
	LTC2402Init();
	voltage_adc_init();
	GetFreq(1);
	SW_VW(1);
	get_time();
	
	while(1)
	{
		AppMain();
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_DLY,&err);//10毫秒检测一次是否传输完毕
	}
	
	
}



//			SW_12V(0);//电源
//			SW_5V(0);//正负电源，用于检测回波
//			OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_DLY,&err);
//			ConfigPINToListen();
//			PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);
//			SetClock();//配置各级CPU时钟
//			bsp_io_init();//IO口
//			SW_12V(1);//电源
//			SW_5V(1);//正负电源，用于检测回波
//			//OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_DLY,&err);
//			UsartConfig();//串口设置配置
//			FreqModuleInit();//测频率模块初始化
//			LTC2402Init();
//			voltage_adc_init();
//			GetFreq(1);
//			read_dev_id(dev_ID);//读取ID