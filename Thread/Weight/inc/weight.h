 #ifndef __WEIGHT_H__
#define __WEIGHT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <rtthread.h>  
#ifndef	 BT_BOARD
//use weight sensor	is define it 
#define   USING_WEIGHT

#endif

 void ConfigSWTimr0(void);
 void ConfigHWADCHX711(void);
 void ConfigHWPwm0(void);
 void ConfigHW_IR_IRQ(void);
  void init_Weight_thread( char priority,int tick);







#ifdef __cplusplus
}
#endif


#endif




