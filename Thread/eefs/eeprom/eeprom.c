
/*
***********************************************************************************







***********************************************************************************
*/

#include "common_types.h"
#include "eefs_fileapi.h"
#include "eefs_macros.h"
#include <string.h>
extern char Dataimage[1024];// data image



/*

	 the founction write some data to EEPROM 
	 Dest need must be make (void*) convetered to (int32) 
	 Src  it's the rom/ram appuse data area.
     note:

*/
void Write_EEPROM(void *Dest, void *Src, int Length)
{


	  memcpy(&Dataimage[(int32)Dest],(int8 *)Src,Length);


}
 /*
 	 the founction read some data from EEPROM 
	 Dest it's the rom/ram appuse data area.
	 Src  need must be make (void*) convetered to (int32)
     note:
 
 */
void Read_EEPROM(void *Dest, void *Src, int Length)
{


	 memcpy((int8 *)Dest,&Dataimage[(int32)Src],Length);
	 //


}