
#ifndef __DOORINOUT_H__
#define __DOORINOUT_H__

#ifdef __cplusplus
extern "C"
{
#endif


  void ConfigHWDoor(void);
 void rt_thread_entry_Door(void* parameter);
 void init_Door_thread(char priority,int tick);







#ifdef __cplusplus
}
#endif


#endif

