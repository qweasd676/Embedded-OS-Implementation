/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                              uC/OS-II
*                                            EXAMPLE CODE
*
* Filename : main.c
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>

#include  "app_cfg.h"


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
static  OS_STK  StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE];
//resource 1
#define R1_PRIO 1 
#define R2_PRIO 4
//task1
#define TASK_STACKSIZE      2048
#define TASK1_PRIORITY      2
#define TASK1_ID            1
#define TASK1_Arrival       2
#define TASK1_exection      5
#define TASK1_period        30
//task2
#define TASK2_PRIORITY      3
#define TASK2_ID            2
#define TASK2_Arrival       3
#define TASK2_exection      3
#define TASK2_period        60
////task3
#define TASK3_PRIORITY      5
#define TASK3_ID            3
#define TASK3_Arrival       0
#define TASK3_exection      7
#define TASK3_period        90


OS_EVENT* R1;
OS_EVENT* R2;
static OS_STK       Task1_STK[TASK_STACKSIZE];
static OS_STK       Task2_STK[TASK_STACKSIZE];
static OS_STK       Task3_STK[TASK_STACKSIZE];

static void task1(void * p_arg, void* pdata);
static void task2(void * p_arg, void* pdata);
static void task3(void* p_arg, void* pdata);
static void mywait(int tick);

//�ŧitask

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  StartupTask (void  *p_arg);


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*
* Notes       : none
*********************************************************************************************************
*/

int  main (void)
{
#if OS_TASK_NAME_EN > 0u
    CPU_INT08U  os_err;
#endif


    CPU_IntInit();

    Mem_Init();                                                 /* Initialize Memory Managment Module                   */
    CPU_IntDis();                                               /* Disable all Interrupts                               */
    CPU_Init();                                                 /* Initialize the uC/CPU services                       */

    OSInit();                                                   /* Initialize uC/OS-II                                  */
     
                                                                /* Create the startup task                              */
                                                                /*                                                              
    OSTaskCreateExt( StartupTask,                              
                     0,
                    &StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE - 1u],
                     APP_CFG_STARTUP_TASK_PRIO,
                     APP_CFG_STARTUP_TASK_PRIO,
                    &StartupTaskStk[0u],
                     APP_CFG_STARTUP_TASK_STK_SIZE,
                     0u,
                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
                    
#if OS_TASK_NAME_EN > 0u
    OSTaskNameSet(         APP_CFG_STARTUP_TASK_PRIO,
                  (INT8U *)"Startup Task",
                           &os_err);
#endif
*/
    //create resource
    INT8U err;
    R1 = OSMutexCreate(R1_PRIO, &err);
    R2 = OSMutexCreate(R2_PRIO, &err);
    //�إ�task
    OSTaskCreateExt(task1,
        0,
        &Task1_STK[TASK_STACKSIZE - 1],
        TASK1_PRIORITY,
        TASK1_ID,
        &Task1_STK[0],
        TASK_STACKSIZE,
        0,
        (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR),
        TASK1_period, TASK1_Arrival);

    OSTaskCreateExt(task2,
        0,
        &Task2_STK[TASK_STACKSIZE - 1],
        TASK2_PRIORITY,
        TASK2_ID,
        &Task2_STK[0],
        TASK_STACKSIZE,
        0,
        (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR),
        TASK2_period, TASK2_Arrival);

    OSTaskCreateExt(task3,
        0,
        &Task3_STK[TASK_STACKSIZE - 1],
        TASK3_PRIORITY,
        TASK3_ID,
        &Task3_STK[0],
        TASK_STACKSIZE,
        0,
        (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR),
        TASK3_period, TASK3_Arrival);

    OSTimeSet(0);
    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)   */

    while (DEF_ON) {                                            /* Should Never Get Here.                               */
        ;
    }
}


/*
*********************************************************************************************************
*                                            STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'StartupTask()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  StartupTask (void *p_arg)
{
   (void)p_arg;

    OS_TRACE_INIT();                                            /* Initialize the uC/OS-II Trace recorder               */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
    
    
    APP_TRACE_DBG(("uCOS-III is Running...\n\r"));

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
        OSTimeDlyHMSM(0u, 0u, 1u, 0u);
		APP_TRACE_DBG(("Time: %d\n\r", OSTimeGet()));
    }
}

//task1�������c
void task1(void* p_arg, void*pdata) {
    (void)p_arg;
    INT8U err;
    OSTCBCur->execution = TASK1_exection;
    if (OSTimeGet() < TASK1_Arrival) {
        OSTimeDly(OSTCBCur->task_arrival - OSTimeGet());
    }
    if (OSTimeGet() == 0) {
        printf("%d\tTask %d \n", OSTimeGet(), OSTCBCur->OSTCBId);
    }
    while (1) {
        mywait(1);
        //OSSchedLock();      //NPCS
        printf("%d\tTask %d get R1",OSTimeGet(), OSTCBCur->OSTCBId);
        OSMutexPend(R1,0,&err);
        mywait(3);
       /* printf("%d\tTask %d get R2", OSTimeGet(), OSTCBCur->OSTCBId);
        OSMutexPend(R2, 0, &err);
        mywait(2);*/
        /*printf("%d\tTask %d release R2", OSTimeGet(), OSTCBCur->OSTCBId);
        OSMutexPost(R2);*/
        printf("%d\tTask %d release R1", OSTimeGet(), OSTCBCur->OSTCBId);
        OSMutexPost(R1);
        //OSSchedUnlock();    //NPCS
        OS_Sched(); 
        mywait(1);
        int time = OSTCBCur->response;      
        OSTCBCur->response = 0;         //�M���g�L�ɶ�

        if (TASK1_period - time == 0) {   
            OS_Sched();
        }
        else {
            OSTimeDly(TASK1_period - time);
        }
        
    }
}
//task2�������c
void task2(void* p_arg, void* pdata) {
    (void)p_arg;
    INT8U err;
    OSTCBCur->execution = TASK2_exection;
    if (OSTimeGet() < TASK2_Arrival) {
        OSTimeDly(OSTCBCur->task_arrival - OSTimeGet());
    }
    if (OSTimeGet() == 0) {
        printf("%d\tTask %d \n", OSTimeGet(), OSTCBCur->OSTCBId);
    }
    while (1) {   
        //OSSchedLock();      //NPCS
        mywait(3);
        //OSSchedUnlock();    //NPCS
        if (TASK2_period - OSTCBCur->response == 0) {   //comptime�p�g�L���ɶ�
            OS_Sched();
        }
        else {
            OSTimeDly(TASK2_period - OSTCBCur->response);
        }
        OSTCBCur->response = 0;
    }
}
//task3�������c
void task3(void* p_arg, void* pdata) {
    (void)p_arg;
    INT8U err;

    OSTCBCur->execution = TASK3_exection;
    if (OSTimeGet() < TASK3_Arrival) {
        OSTimeDly(OSTCBCur->task_arrival - OSTimeGet());
    }
    if (OSTimeGet() == 0) {
        printf("%d\tTask %d \n", OSTimeGet(), OSTCBCur->OSTCBId);
    }
    while (1) {
        mywait(1);
        //OSSchedLock();      //NPCS
        printf("%d\tTask %d get R2", OSTimeGet(), OSTCBCur->OSTCBId);
        OSMutexPend(R2, 0, &err);
        mywait(5);
        //printf("%d\tTask %d get R1", OSTimeGet(), OSTCBCur->OSTCBId);
        //OSMutexPend(R1, 0, &err);
        //mywait(2);
        //printf("%d\tTask %d release R1", OSTimeGet(), OSTCBCur->OSTCBId);
        //OSMutexPost(R1);
        printf("%d\tTask %d release R2", OSTimeGet(), OSTCBCur->OSTCBId);
        OSMutexPost(R2);
        //OSSchedUnlock();    //NPCS
        OS_Sched();
        mywait(1);
        int time = OSTCBCur->response;
        OSTCBCur->response = 0;           //�M���g�L�ɶ�
        if (TASK3_period - time == 0) {   //comptime�p�g�L���ɶ�
            //printf("back\n");
            OS_Sched();
        }
        else {
            OSTimeDly(TASK3_period - time);
        }
    }
}

//��������ɶ�
void mywait(int tick) {

#if OS_CRITICAL_METHOD==3;
    OS_CPU_SR cpu_sr = 0;
#endif
    while (1)
    {
        if (tick <= OSTCBCur->execution_frequency)
            break;
    }
    OS_ENTER_CRITICAL();
    OSTCBCur->CompTime += OSTCBCur->execution_frequency;
    OSTCBCur->execution_frequency = 0;
    OS_EXIT_CRITICAL();

}