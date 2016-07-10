

/*


*/
#include <rtthread.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "NUC472_442.h"
#include "ADE7878_SPI.h"
#include "ADE7878.h"
#include "ade7878_Int.h" 
#include"Usart.h"
#include "ADE7878_Measure.h"
#include "ds12864.h"
extern uint32_t LocalTime ;
unsigned int  IRQStautsRead0 = 0;
unsigned int  IRQStautsRead1 = 0;
unsigned char IRQFlag = 0;
int  IRMSRead[20];					  
char IRMSIndex = 0;
int  VRMSRead[20];					  
char VRMSIndex = 0;
int PhaseAEnergy = 0;		 
int PhaseAIRMS   = 0;
int PhaseAVRMS   = 0;
int PhaseAPeroid = 0;
int PhaseAAngle  = 0;

int ADE7878_main(void)
{	  
	  
	  
	ADE7878Reset();				  //Reset ADE7878
	ADE7878PSM0();			      //setting ADE7878 to normal power mode
	ADE7878SPICfg();		      //choose SPI mode

	//********clear the IRQ1 interrupt, by writing the STATUS1 register**********************
	//*The ADE7878 signals the end of the transition period by triggering the IRQ1 interrupt pin low 
	//*and setting Bit 15 (RSTDONE) in the STATUS1 register to 1. 
	//*This bit is 0 during the transition period and becomes 1 when the transition ends.
	//***************************************************************************************
	IRQStautsRead0 = SPIRead4Bytes(STATUS1);
	SPIWrite4Bytes(STATUS1,IRQStautsRead0);
	if((IRQStautsRead0&BIT15)==BIT15)
	{
	  IRQStautsRead0 ='k'; // is ok
	}
	
		ADE7878Cfg();				  //config the ADE7878 that it is very important for read Data.
		SPIWrite2Bytes(Run,0x0001);	  //run ADE7878 DSP
		Delay(1000);				  //Delay to wait the Rms settling
	 
}
  extern uint16_t  usSRegHoldBuf[100];
   int DisplayTrm;
   char Pash_poll=0;//A\B\C
   extern void PollADE7878(uint16_t pashe);
 void PowerMeasurment(void)
 {
	   char sfIrms[20];
	   char sfVrms[20];
	   char sfWatt[20];
	   static char f,i;
	   char*p;
	   char HZ;
	   uint32_t Arg[14];

 		   	
		  
		if (1==IRQFlag)  		  //waiting for a interrupt
		{	  f=~f;
		usSRegHoldBuf[21]=f;
			 #ifdef BT_BOARD
			 PD14=f;
			 #else
		     PB13=f;
			 #endif
			IRQFlag = 0;		  
			IRQStautsRead0 = SPIRead4Bytes(STATUS0); 		 //read the interrupt status
			SPIDelay();			
			SPIWrite4Bytes(STATUS0,IRQStautsRead0);        //write the same STATUSx content back to clear the flag and reset the IRQ line
			SPIDelay();  
//PhaseAAngle  = SPIRead2Bytes(ANGLE0);//A��ģ�������ѹ��������ǵ��������Ҫ
/*PhaseAEnergy*/		 
/*PhaseAIRMS*/  
/*PhaseAVRMS*/  
				
				Arg[0] = SPIRead4Bytes(AVRMS);	 //A��ѹ��Чֵ
			    Arg[1] = SPIRead4Bytes(AIRMS);	 //A������Чֵ
				Arg[2] = SPIRead4Bytes(AWATT);	 //A˲ʱ����ֵ
				Arg[3] = SPIRead4Bytes(AWATTHR);//A�����ۻ�ֵ
			
				Arg[4] = SPIRead4Bytes(BVRMS);	 //B��ѹ��Чֵ
			    Arg[5] = SPIRead4Bytes(BIRMS);	 //B������Чֵ
				Arg[6] = SPIRead4Bytes(BWATT);	 //B˲ʱ����ֵ
				Arg[7] = SPIRead4Bytes(BWATTHR);//B�����ۻ�ֵ
			
		
				Arg[8] = SPIRead4Bytes(CVRMS);	 //C��ѹ��Чֵ
			    Arg[9] = SPIRead4Bytes(CIRMS);	 //C������Чֵ
				Arg[10]= SPIRead4Bytes(CWATT); //C˲ʱ����ֵ
				Arg[11]= SPIRead4Bytes(CWATTHR);//C�����ۻ�ֵ
				  
				/*
				������period�Ĵ�������ͨ����ʽ������·���ں�Ƶ�ʣ�
				TL=period[15:0]+1/256*10^3 (sec)
				fL=256*10^3/period[15:0]+1  (HZ)
				*/
			    Arg[12] = SPIRead2Bytes(PERIODADE);//50HZӦ��Ϊperiod�Ĵ���ֵ��ԼΪ5120 (256 kHz/50 Hz)����60 Hz 4267 (256 kHz/60 Hz)
				Arg[12] = 256000/(Arg[12]+1); //Ƶ�ʣ�HZ��
				Arg[13] = SPIRead4Bytes(VPEAK);//��ֵ��ѹ
			{  			  
				float Vrms;
			    uint16_t l1,l2,l3;
			    Vrms = Arg[1];	
				Set_Vrms2Display(Arg[0]); //A-��ѹ
				Set_Irms2Display(Arg[1]); //A-����
				Set_Watt2Display(Arg[2]); //A-����
				Set_Watthr2Display(Arg[3]);//A-����			 
			    l2=sprintf(sfVrms,"%f",Vrms); 

				usSRegHoldBuf[0]=Arg[0]/1000;  //A��ѹ��Чֵ  10-1V
				usSRegHoldBuf[1]=Arg[1];  //A������Чֵ  10-4A
				usSRegHoldBuf[2]=Arg[2];  //A˲ʱ����ֵ		  10-1w
				usSRegHoldBuf[3]=Arg[3]/100;  ////A�����ۻ�ֵ W.H
				usSRegHoldBuf[4]=Arg[4]/1000;  //B��ѹ��Чֵ  10-1V
				usSRegHoldBuf[5]=Arg[5];  //B������Чֵ  10-4A
				usSRegHoldBuf[6]=Arg[6];  //B˲ʱ����ֵ		  10-1w
				usSRegHoldBuf[7]=Arg[7]/100;  ////B�����ۻ�ֵ W.H
				usSRegHoldBuf[8]=Arg[8]/1000;  //C��ѹ��Чֵ  10-1V
				usSRegHoldBuf[9]=Arg[9];  //C������Чֵ  10-4A
				usSRegHoldBuf[10]=Arg[10];  //C˲ʱ����ֵ	  10-1w
				usSRegHoldBuf[11]=Arg[11]/100;  ////C�����ۻ�ֵ W.H
				usSRegHoldBuf[12]=Arg[12];  ////Ƶ�ʣ�HZ��
				usSRegHoldBuf[13]=Arg[13];  //////��ֵ��ѹ
			
			 }

		 
			 
			
				
		}
		if(LocalTime - DisplayTrm>1000)
		{
			
		  								   
		  DisplayTrm=LocalTime;
		  Display_Task();//refresh
		}
		
		
					
	
 
 
 }