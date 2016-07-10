
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eefs_fileapi.h"
#include "eefs_filesys.h"
#include "common_types.h"
#include "eefstool_driver.h"
#include "eefstool_opts.h"
#include "eefs_macros.h"
CommandLineOptions_t EepromOptions;//ȫ�ֶ���
char                 eefs_filename[128];
char                 eefs_mountpoint[64] = "/eebank1";
char Dataimage[1024];// data image
void TestEEPROM_Filesystem()
{
  /*EEFS_InodeTable_t myeefsInodeTable;

  efile myfile;
  int len;
  char Eepromdata[20];
  myeefsInodeTable.FreeMemorySize=1024;
  myeefsInodeTable.FreeMemoryPointer=(uint8*)Dataimage;	//�׿ռ�ָ��
  EEFS_LibInit();//�������ȫ�������������
  EEFS_LibInitFS(&myeefsInodeTable,0) ;	//��0��ַ����EEPROM����Ϣ�����û�з��ش���				
 //����һ���ļ�����д�뵽EEPROM��
//���ش��ļ����������е�λ��
  myfile=EEFS_LibOpen(&myeefsInodeTable, "eeprom", O_CREAT, 1);//
   EEFS_LibFmode(&myeefsInodeTable, (EEFS_FREAD | EEFS_FWRITE));
  len=EEFS_LibWrite(myfile, "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", 10);
  len=EEFS_LibRead(myfile, Eepromdata, len);


 
 typedef struct
{
    uint32                              Crc;	 //CRCУ����
    uint32                              Magic;	 //��Կ��
    uint32                              Version;  //�汾
    uint32                              FreeMemoryOffset; //�洢ƫ��
    uint32                              FreeMemorySize;//�洢��С
    uint32                              NumberOfFiles; //�ļ�����
} EEFS_FileAllocationTableHeader_t;	 //������������EEPROM����Ϣ

typedef struct
{										 //�ļ�ͷƫ��
    uint32                              FileHeaderOffset;  
   										 //����ļ��ߴ�
    uint32                              MaxFileSize;     
} EEFS_FileAllocationTableEntry_t;

typedef struct
{
    EEFS_FileAllocationTableHeader_t    Header;	 //ͷ
    EEFS_FileAllocationTableEntry_t     File[EEFS_MAX_FILES];  //
} EEFS_FileAllocationTable_t; //�ļ���������

*/
	char                          UseData[100];
	int32 						  DataWritten;
	int32                         eefs_fd; 
	int32                         Status;
	EEFS_FileAllocationTable_t 	  EEFS_Base;
	EEFS_FileAllocationTable_t 	 *ptr=&EEFS_Base;



	EEFS_Base.Header.Crc=0;
	EEFS_Base.Header.Magic=EEFS_FILESYS_MAGIC;
	EEFS_Base.Header.Version=1;
	EEFS_Base.Header.FreeMemoryOffset=sizeof(EEFS_FileAllocationTable_t);
	EEFS_Base.Header.FreeMemorySize=1024-sizeof(EEFS_FileAllocationTable_t);	

									
	Status=sizeof(EEFS_FileAllocationTable_t); //debug used it display the number of struct .
	memcpy(&Dataimage[0],(char*)ptr,(sizeof(EEFS_FileAllocationTable_t)));				 

   #ifdef USE_EEPROM
   EepromOptions.EEFSMemoryAddress = 0;	  //used eeprom addr
   #endif


   #ifdef USE_RAM
   EepromOptions.EEFSMemoryAddress = (uint32)Dataimage;
   #endif
   strncpy(EepromOptions.Filename1,"myeeprom.txt", FILENAME_SIZE);

 Status = EEFS_InitFS("/EEDEV1", EepromOptions.EEFSMemoryAddress);  
    if ( Status == 0 )
    {
       Status = EEFS_Mount("/EEDEV1",eefs_mountpoint);//mount "/eebank1"disk on /EEDEV1 
       if ( Status != 0 )
       {
          printf("Error: Failed to mount EEPROM File System\n");
       }
    }
    else
    {
       printf("Error: Failed to initialize EEPROM FS\n");
    } 

	   strcpy(eefs_filename, eefs_mountpoint);
       strcat(eefs_filename,"/");
       strcat(eefs_filename,EepromOptions.Filename1);	
	   //EEDEV1��eefs_filename/myeeprom.txt
  eefs_fd = EEFS_Creat(eefs_filename, 0);
         if ( eefs_fd < 0 )
          {
             printf("Error calling EEFS_Creat on EEFS file: %s\n","myeeprom.txt");
          }
			 
 DataWritten = EEFS_Write(eefs_fd, "testwiter-file!write to EEPROM",sizeof("testwiter-file!write to EEPROM"));
          
			
	     EEFS_Close(eefs_fd);

       strcpy(eefs_filename, eefs_mountpoint);
       strcat(eefs_filename,"/");
       strcat(eefs_filename,EepromOptions.Filename1);
	   //EEDEV1��eefs_filename/myeeprom.txt

   eefs_fd = EEFS_Open(eefs_filename, 0);
	   if ( eefs_fd < 0 )
          {
             printf("Error opening EEFS file: %s\n",EepromOptions.Filename1);
          }
  DataWritten = EEFS_Read(eefs_fd, UseData, DataWritten);        
   			
         EEFS_Close(eefs_fd);
	

}