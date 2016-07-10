

 
/**




**/

#include <rthw.h>
#include <rtthread.h>    
#include <stdio.h>
#include "NUC472_442.h"
#include"Config.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "weight.h"
#include "pidctrl.h"
#include "ds18b20.h" 

#ifndef ABS
#define ABS(a)	   (((a) < 0) ? -(a) : (a))
#endif

#define WINDUP_ON(_pid)         (_pid->features & PID_ENABLE_WINDUP)
#define DEBUG_ON(_pid)          (_pid->features & PID_DEBUG)
#define SAT_MIN_ON(_pid)        (_pid->features & PID_OUTPUT_SAT_MIN)
#define SAT_MAX_ON(_pid)        (_pid->features & PID_OUTPUT_SAT_MAX)
#define HIST_ON(_pid)           (_pid->features & PID_INPUT_HIST)


#ifdef USING_PID



/*****external temperature sample module *********/

 void ConfigHWTemperatureSample()
 {
  
  	 GPIO_SetMode(PA, BIT1, GPIO_MODE_QUASI); //ds18b20 IO


 }








/*****external SSR ctrl module*********/





























 void testdelay()
 {
   SYS_UnlockReg(); 																				                                
CLK_EnableModuleClock(TMR2_MODULE);
CLK_SetModuleClock(TMR2_MODULE,CLK_CLKSEL1_TMR2SEL_HXT, 0);
 GPIO_SetMode(PD, BIT14, GPIO_MODE_QUASI); //ds18b20 IO
 	while(1)
	{ 
	
	PD14=0;
   // TIMER_Delay(TIMER2, 500);
	  delayus_soft(50);
	PD14=1;
   // TIMER_Delay(TIMER2, 500);
	  delayus_soft(50);		  

	 }
 
 }



  
 	PID_t pid;
	float result,kj;	
//PID
static char thread_PID_stack[1024];
struct rt_thread thread_PID;
void rt_thread_entry_PID(void* parameter)
{  
	
	 /*
	  	 比例因子=1；
		 积分因子=0；
		 微分因子=0；

	  
	  */	
 
 	//  pid_init(&pid, 1, 0, 0);
	  //pid_enable_feature(&pid, , 0)		
	/*目标值=70*/		 
    //  pid_set(&pid, 30);
	 
	 while(1)
	 {
 
	 DS1820main() ;

	// result = pid_calculate(&pid,kj, 1) ;
	

	 rt_thread_delay(100); // 线程CPU主动让出	

	 }




}


 void init_PID_thread(char priority,int tick)
 {
//------- init Door thread
    rt_thread_init(&thread_PID,
                   "PID",
                   rt_thread_entry_PID,
                   RT_NULL,
                   &thread_PID_stack[0],
                   sizeof(thread_PID_stack),priority,tick);
    rt_thread_startup(&thread_PID);

 }


















void pid_enable_feature(PID_t *pid, unsigned int feature, float value)
{
    pid->features |= feature;

    switch (feature) {
        case PID_ENABLE_WINDUP:
            /* integral windup is in absolute output units, so scale to input units */
            pid->intmax = ABS(value / pid->ki);
            break;
        case PID_DEBUG:
            break;
        case PID_OUTPUT_SAT_MIN:
            pid->sat_min = value;
            break;
        case PID_OUTPUT_SAT_MAX:
            pid->sat_max = value;
            break;
        case PID_INPUT_HIST:
            break;
    }
}

/**
 *
 * @param pid
 * @param kp
 * @param ki
 * @param kd
 */
void pid_init(PID_t *pid, float kp, float ki, float kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;

	pid->sp = 0;
	pid->error_previous = 0;
	pid->integral = 0;

    pid->features = 0;

    if (DEBUG_ON(pid))
        printf("setpoint,value,P,I,D,error,i_total,int_windup\n");
}

void pid_set(PID_t *pid, float sp)
{
	pid->sp = sp;
	pid->error_previous = 0;
	pid->integral = 0;
}

/**
 *
 * @param pid
 * @param val  NOW VALUE
 * @param dt   
 * @return
 */
float pid_calculate(PID_t *pid, float val, float dt)
{
	float i,d, error, total;

	error = pid->sp - val;
	i = pid->integral + (error * dt);
	d = (error - pid->error_previous) / dt;

    total = (error * pid->kp) + (i * pid->ki) + (d * pid->kd);

    if (DEBUG_ON(pid))
        printf("%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%d\n", 
                pid->sp,val,
                (error * pid->kp), (i * pid->ki), (d * pid->kd),
                error, pid->integral, ABS(i) == pid->intmax);

    if ( WINDUP_ON(pid) ) {
        if ( i < 0 )
            i = ( i < -pid->intmax ? -pid->intmax : i );
        else
   		    i = ( i < pid->intmax ? i : pid->intmax );
    }
    pid->integral = i;

    if ( SAT_MIN_ON(pid) && (total < pid->sat_min) )
        return pid->sat_min;
    if ( SAT_MAX_ON(pid) && (total > pid->sat_max) )
        return pid->sat_max;

	pid->error_previous = error;
	return total;
}




 #endif


 /*

#define PIDTYPE float


typedef struct
{

PIDTYPE SetPoint;       // 设定目标Desired value 
PIDTYPE Proportion;     // 比例常数Proportional Const 
PIDTYPE Integral;      // 积分常数Integral Const 
PIDTYPE Derivative;     // 微分常数Derivative Const 
PIDTYPE LastError;      // Error[-1] 
PIDTYPE PrevError;      // Error[-2] 
PIDTYPE SumError;       // Sums of Errors 

}PID;
//PID计算部
float PIDCalc( PID*p,PIDTYPE NextPoint ) 
{ 
	
  PIDTYPE dError=0, Error=0; 
  
  Error = p->SetPoint - NextPoint;           // 偏差 

 return (p->Proportion * Error ); 
 
} 

 	PID *PidPtr=&PID_Ctrl;

		PidPtr->Proportion = 0.83; // Set PID Coefficients   0.83 C 
		PidPtr->Integral = 0.0;   
		PidPtr->Derivative = 0.4;   //0.4


		PidPtr->SetPoint = 70; // Set PID Setpoint 70度
		PidPtr->LastError=0.0;     // Error[-1] 
		PidPtr->PrevError=0.0;    // Error[-2] 
		PidPtr->SumError=0.0;    // Sums of Errors 
	*/

