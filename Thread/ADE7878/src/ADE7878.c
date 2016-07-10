#include <stdio.h>
#include "NUC472_442.h"
#include "ADE7878_SPI.h"
#include "ADE7878.h"
#include <rtthread.h> 
 #define USE_0_55A_RANG


/******************************************************************************
**  Abstract:   
**		RESET ADE7878	
**  Parameters:	
**		none  
**  Returns:
**		none
*******************************************************************************/
void ADE7878Reset(void)
{
	#ifdef BT_BOARD
	GPIO_SetMode(PE, BIT9, GPIO_MODE_OUTPUT);// MCU_RESET PF11
	//SET_PIN14();   //
	PE9=1;
	#else
	GPIO_SetMode(PF, BIT11, GPIO_MODE_OUTPUT);// MCU_RESET PF11
	//SET_PIN14();   //
	PF11=1;
	#endif


	#ifdef BT_BOARD
	PD14=1;
	PD15=1;
	#else
	PB12=1;
	PB13=1;
	#endif

	Delay(200);


   	#ifdef BT_BOARD
	PE9=0;
	#else
	//CLR_PIN14();   //
	PF11=0;
	#endif




	#ifdef BT_BOARD
	PD14=0;
	#else
	PB12=0;
	#endif
	
	Delay(200);

	#ifdef BT_BOARD
	PE9=1;
	#else
	PF11=1;
	#endif
		
	#ifdef BT_BOARD
	PD14=1;
	#else
	PB12=1;	
	#endif
	//SET_PIN14();   //
	Delay(100);
	#ifdef BT_BOARD
	PD15=0;	
	#else
	PB13=0;	
	#endif 
	      	
}
/******************************************************************************
**  Abstract:   
**		choose PSM0 Mode   PM1 = 0; PM0 = 1	
**  Parameters:	
**		none  
**  Returns:
**		none
*******************************************************************************/
void ADE7878PSM0(void)
{
 

  

   	#ifdef BT_BOARD
    //PM1=0;PM0=1;	 PSM0��������ģ
   //PE11 -PM0 
   //PE10 -PM1
	GPIO_SetMode(PE, BIT11, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PE, BIT10, GPIO_MODE_OUTPUT);
	PE11=1;
	PE10=0;
	#else
	  //PM1=0;PM0=1;	 PSM0��������ģ
   //PB3 -PM0 
   //PB2 -PM1
   	GPIO_SetMode(PB, BIT3, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PB, BIT2, GPIO_MODE_OUTPUT);
	PB3=1;
	PB2=0;
	#endif




	Delay(1000);
/*	CLR_PIN15();  
	SET_PIN16();   //
	Delay(100);	   //wait for the ADE power up after PSM0 is chosen,	more than 40ms is needed 
*/

}
/******************************************************************************
**  Abstract:   
**		choose SPI interface,   toggle SS three times
**  Parameters:	
**		none  
**  Returns:
**		none
*******************************************************************************/
void ADE7878SPICfg(void)
{



	//PC12=1;
	SPIDelay();
	//PC12=0;
	//toggle once
	SPIDelay();
	//PC12=1;
	SPIDelay();
//	PC12=0;
//toggle twice
	SPIDelay();
//	PC12=1;
	SPIDelay();
//	PC12=0;
//triple triple
	SPIDelay();
//	PC12=1;
	SPIDelay();

	SPIWrite1Byte(0xEC01, 0x00);	 //SPI is the active serial port, any write to CONFIG2 register locks the port
	
	//The 2nd way to choose SPI mode is to execute three SPI write operations 
	//to a location in the address space that is not allocated to a specific ADE78xx register
	/*
	SPIWrite1Byte(0xEBFF, 0x00);								 
	SPI_read_32(0xEBFF);
	SPIWrite1Byte(0xEBFF, 0x00);
	SPI_read_32(0xEBFF);
	SPIWrite1Byte(0xEBFF, 0x00);
	SPI_read_32(0xEBFF);
	*/
}
/******************************************************************************
**  Abstract:   
**		ADE7878 configuration
**  Parameters:	
**		none  
**  Returns:
**		none
*******************************************************************************/
//#define USE_5_5A_RANG
void ADE7878Cfg(void)
{


	/*
	���ƣ�GAIN�Ĵ���(��ַ0xE60F)
	 Bits------Name------Function-------Value
	[15:9] 	����						 000 0000
	[8:6] 	PGA3[2:0]   ���ѹ����		 000
	[5:3] 	PGA2[2:0]	�����������	 000
	[2:0] 	PGA1[2:0]	���������		 000
						000: ���� = 1��
						001: ���� = 2��
						010: ���� = 4��
						011: ���� = 8��
						100: ���� = 16��
	Note:Ĭ��Ϊ1�����档��������ź�
	*/
	SPIWrite2Bytes(Gain,0x0004);			   //config the  Gain of the PGA , which is before the ADC of ADE7878
	SPIWrite2Bytes(CONFIG,0x0000);			  //
	SPIWrite2Bytes(HPFDIS,0x0000);			  //enabled high-pass filters
	
	SPIWrite4Bytes(AIGAIN,0x00000000);		  //calibration
	SPIWrite4Bytes(AVGAIN,0x00000000);
	SPIWrite4Bytes(BIGAIN,0x00000000);
	SPIWrite4Bytes(BVGAIN,0x00000000);
	SPIWrite4Bytes(CIGAIN,0x00000000);
	SPIWrite4Bytes(CVGAIN,0x00000000);
	SPIWrite4Bytes(NIGAIN,0x00000000);
 	SPIWrite4Bytes(AIRMSOS,0x00000000);
	SPIWrite4Bytes(AVRMSOS,0x00000000);
	SPIWrite4Bytes(BIRMSOS,0x00000000);
	SPIWrite4Bytes(BVRMSOS,0x00000000);
	SPIWrite4Bytes(CIRMSOS,0x00000000);
	SPIWrite4Bytes(CVRMSOS,0x00000000);
	SPIWrite4Bytes(NIRMSOS,0x00000000);
 	SPIWrite4Bytes(AWGAIN,0x00000000);
	SPIWrite4Bytes(AWATTOS,0x00000000);
	SPIWrite4Bytes(BWGAIN,0x00000000);
	SPIWrite4Bytes(BWATTOS,0x00000000);
	SPIWrite4Bytes(CWGAIN,0x00000000);
	SPIWrite4Bytes(CWATTOS,0x00000000);
	SPIWrite2Bytes(APHCAL,0x0000);
	SPIWrite2Bytes(BPHCAL,0x0000);
	SPIWrite2Bytes(CPHCAL,0x0000);

	SPIWrite2Bytes(CF1DEN,0x00A7);	  		  //configure the ENERGY-TO-FREQUENCY
	SPIWrite2Bytes(CF2DEN,0x00A7);
	SPIWrite2Bytes(CF3DEN,0x00A7);
	SPIWrite2Bytes(CFMODE,0x0E88);			  //CF1-total active power, disable; CF2-total reactvie power, disable; CF3- fundamental avtive power, disable

	SPIWrite2Bytes(ACCMODE,0x0000);
	SPIWrite2Bytes(COMPMODE,0x01FF);
	SPIWrite1Byte(MMODE,0x00);

	

	SPIWrite4Bytes(WTHR0,0x007BBE61);		  //PMAX = 33,516,139	  0.001WH/LSB	 //page 47 datasheet Rev. E										  
	SPIWrite4Bytes(WTHR1,0x00000017);
	SPIWrite4Bytes(VATHR0,0x007BBE61);		  //PMAX = 33,516,139
	SPIWrite4Bytes(VATHR1,0x00000017);
	SPIWrite4Bytes(VARTHR0,0x007BBE61);		   //PMAX = 33,516,139
	SPIWrite4Bytes(VARTHR1,0x00000017);										   


	
	
	SPIWrite1Byte(LCYCMODE,/*0x0F*/0x08);//phase A is selected for zero cross
	SPIWrite2Bytes(LINECYC,347);  //100pre	347

	SPIWrite4Bytes(MASK0,0x00000020);//line cycle interrupt enable
	SPIWrite4Bytes(MASK1,0x00000000);   



  	
   #if 1 /*�Ƿ�����������У׼����-start*/

	 
	 #ifdef  USE_5_5A_RANG
	 /*
	 				xWTHR ���ʷ�ֵ�ļ���


							WTHR�Ĵ���
				
					WTHR = (PMAX*fs*3600*10^n)/(Vfs*Ifs)
				
				
				 ���У�fS = 8 kHz����DSP���ڼ���˲ʱ���ʵ�Ƶ�ʡ�
				 THR����ʼ�մ��ڻ����PMAX�������ѹ�����������
				�����̷���ʱADE7878������й����ʣ�
				
					PMAX = 33,516,139
				
				 ���THRС��PMAX��Ӧ����VRMSREF��IRMSREF��
				
				
				PMAX = 33516139 = 0x1FF6A6B
				
				xWTHR=(PMAX*fs*3600*10^n)/(ufs*Ifs)
					=33516139*8000*3600*10^-3/ 5.5A*387.5V
					= 965264803200 /2131.5
					= 0x1AFE0CDA (Vfs=5.5A)
				����ã�
				THR0=0x00FE0CDA
				THR1=0x0000001A	

	 
	 
	 */

	SPIWrite4Bytes(WTHR0,0x00FE0CDA);
	SPIWrite4Bytes(WTHR1,0x0000001A);  

   #endif

   #ifdef USE_0_44A_RANG
	   /*
	 				xWTHR ���ʷ�ֵ�ļ���


							WTHR�Ĵ���
				
					WTHR = (PMAX*fs*3600*10^n)/(Vfs*Ifs)
				
				
				 ���У�fS = 8 kHz����DSP���ڼ���˲ʱ���ʵ�Ƶ�ʡ�
				 THR����ʼ�մ��ڻ����PMAX�������ѹ�����������
				�����̷���ʱADE7878������й����ʣ�
				
					PMAX = 33,516,139
				
				 ���THRС��PMAX��Ӧ����VRMSREF��IRMSREF��
				
				
				PMAX = 33516139 = 0x1FF6A6B
				
				xWTHR=(PMAX*fs*3600*10^n)/(ufs*Ifs)
					=33516139*8000*3600*10^-3/ 44A*485V[��������ƽ�ʵ�ʵ���ʱֹͣ]��387��
					= 965264803200/21340
					= 0x2B2320C(Ifs=44.1A)
				����ã�
				THR0=0x00D7B4C4
				THR1=0x00000002	
									  
	 				  
	 			 THR��ֵԽ�ӽ�PMAXֵ���ۼƵ����ٶ�Խ�죬��֮Խ����
				 Ӧ��ʹ�ñ�׼��У׼Ϊÿ���������ӵĵ��ܻ���һ�¼���
				 ����һ�ְ취���������壬����Ϊ��׼����ͬ���������
				 ���������ֵ�趨����֮�����ÿ���ʹADE7878�����������ڱ�׼�������CP�������У׼
				 
													   
	 */
	SPIWrite4Bytes(WTHR0,0x00B2320C);
	SPIWrite4Bytes(WTHR1,0x00000002);  
   #endif

   #ifdef USE_0_55A_RANG
	/*
	 				xWTHR ���ʷ�ֵ�ļ���


							WTHR�Ĵ���
				
					WTHR = (PMAX*fs*3600*10^n)/(Vfs*Ifs)
				
				
				 ���У�fS = 8 kHz����DSP���ڼ���˲ʱ���ʵ�Ƶ�ʡ�
				 THR����ʼ�մ��ڻ����PMAX�������ѹ�����������
				�����̷���ʱADE7878������й����ʣ�
				
					PMAX = 33,516,139
				
				 ���THRС��PMAX��Ӧ����VRMSREF��IRMSREF��
				
				
				PMAX = 33516139 = 0x1FF6A6B
													 330
				xWTHR=(PMAX*fs*3600*10^n)/(ufs*Ifs)
					=33516139*8000*3600*10^-3/ 55A*380V[��������ƽ�ʵ�ʵ���ʱֹͣ]��387��
					= 965264803200/20900
					= 0x2C0B9D6(Ifs=55A)
				����ã�
				THR0=0x00C0B9D6
				THR1=0x00000002	
									  
	 				  
	 			 THR��ֵԽ�ӽ�PMAXֵ���ۼƵ����ٶ�Խ�죬��֮Խ����
				 Ӧ��ʹ�ñ�׼��У׼Ϊÿ���������ӵĵ��ܻ���һ�¼���
				 ����һ�ְ취���������壬����Ϊ��׼����ͬ���������
				 ���������ֵ�趨����֮�����ÿ���ʹADE7878�����������ڱ�׼�������CP�������У׼
				 
													   
	 */
	SPIWrite4Bytes(WTHR0,0x00C0B9D6); 
	SPIWrite4Bytes(WTHR1,0x00000002);  

   #endif





	//SPIWrite4Bytes(WTHR0,0x007BBE61);		  //PMAX = 33,516,139	  0.001WH/LSB	 //page 47 datasheet Rev. E										  
	//SPIWrite4Bytes(WTHR1,0x00000017);
	SPIWrite4Bytes(VATHR0,0x007BBE61);		  //PMAX = 33,516,139
	SPIWrite4Bytes(VATHR1,0x00000017);

	SPIWrite4Bytes(VARTHR0,0x007BBE61);		   //PMAX = 33,516,139
	SPIWrite4Bytes(VARTHR1,0x00000017);										   


	
	
	SPIWrite1Byte(LCYCMODE,/*0x0F*/0x38);//phase A is selected for zero cross
	SPIWrite2Bytes(LINECYC,347);  //347���벨����preo		������Ƶ�ʣ���ʹ�����ۻ�ģʽ

	SPIWrite4Bytes(MASK0,0x00000020);//line cycle interrupt enable
	SPIWrite4Bytes(MASK1,0x0000ffff);   




	 ///////////////////////////////////////////////////////////////
	 //����A���ֵ��ѹ.
	SPIWrite4Bytes(PEAKCYC,16);//16��ѭ����ֵ���һ��
	SPIWrite1Byte(MMODE,0x04);//����A���ֵ��⡣					
	 
		   /*

			   ͨ���������ݲο�
		   VLEVEL =Vfs/Vn*491520
		          =387v/220V*491520
				  =0xD3174
		   
		   */

	  SPIWrite4Bytes(VLEVEL,0xD3174);//


		#ifdef  USE_5_5A_RANG
	 /*
	 				  xVGAIN�Ĵ���У׼����
	  ����A���ѹͨ�������棨����У׼VRMSֵ��ƫ�
	   1����A��ͨ��һ����׼��ѹVn��ʵ���������е�230V���ң�
	   2������ AVGAINֵΪ0������AVRMSOSΪ0��
	   3�����VRMSֵ������ƽ�������������ֵ�˲���
	   5��������ĵ�ѹ������������ѹ����ת��ΪVref=	230*10^4
	   6��Ȼ���ȡ��ǰ��VRMSֵ=2398439
	   7��GAIN = (Vref/VRMS -1 )= {(230*10^4/2398439)-1}
	   		   =(-0.041)
	   	       =((+0.041)*2^23)���ת��Ϊʮ������Ȼ��NOT�����+1
			   =0xFFFAC084
	   8���ٴβ���VRMS�Ĵ���ֵ��Ԥ��ֵ�ӽ���OK���У׼��
	   9����ʵ�ĵ�ѹ����xVRMS�Ĵ�����ֵ����10^4
	 */
 		SPIWrite4Bytes(AVGAIN,0xFFFAC084);//
	 /*			   xVRMSOS �Ĵ���У׼����
	   ����A���ѹͨ��������֮������У׼����Ĵ�����
	   1����A��ͨ��һ����׼��ѹVn=230V ��Ҫ��������С��ѹVmin=0V
	   2��A�����������׼��ѹ�����һ��VRMSn=230*10^4,��A������0VʱҲ���Ӧһ������ֵʵ��VRMSmin=0x2f8
	   3������˵������0V��ôVRMSһ����0�����ǲ����ǡ�������ҪУ׼��0.
	   4�����ݹ�ʽ�� xVRMSOS =[((VRMSmin^2*Vn^2)-(VRMSn^2*Vmin^2)) /(128*(Vmin^2-Vn^2))]
	   						 =(0x2f8^2*230^2) /128*(-230^2)
	   						 =���ת��Ϊʮ������Ȼ��NOT�����+1
	   						 =0xFFFFEE60
	 */
	SPIWrite4Bytes(AVRMSOS ,0xFFFFEE60);//
	  #endif

	  #ifdef  USE_0_44A_RANG

		 	 /*
	 				  xVGAIN�Ĵ���У׼����
	  ����A���ѹͨ�������棨����У׼VRMSֵ��ƫ�
	   1����A��ͨ��һ����׼��ѹVn��ʵ���������е�223V���ң�
	   2������ AVGAINֵΪ0������AVRMSOSΪ0��
	   3�����VRMSֵ������ƽ�������������ֵ�˲���
	   5��������ĵ�ѹ������������ѹ����ת��ΪVref=	223*10^4
	   6��Ȼ���ȡ��ǰ��VRMSֵ=2365068
	   7��GAIN = (Vref/VRMS -1 )= {(223*10^4/2365068)-1}
	   		   =(-0.057109563023135064192657462702975)
	   	       =((+0.057109563023135064192657462702975)*2^23)���ת��Ϊʮ������Ȼ��NOT�����+1
			   =0xFFF8B0A3
	   8���ٴβ���VRMS�Ĵ���ֵ��Ԥ��ֵ�ӽ���OK���У׼��
	   9����ʵ�ĵ�ѹ����xVRMS�Ĵ�����ֵ����10^4
	 */
 		SPIWrite4Bytes(AVGAIN,0xFFF8B0A3);//
		SPIWrite4Bytes(BVGAIN,0xFFF8B0A3);//
		SPIWrite4Bytes(CVGAIN,0xFFF8B0A3);//
	 /*			   xVRMSOS �Ĵ���У׼����
	   ����A���ѹͨ��������֮������У׼����Ĵ�����
	   1����A��ͨ��һ����׼��ѹVn=230V ��Ҫ��������С��ѹVmin=0V
	   2��A�����������׼��ѹ�����һ��VRMSn=230*10^4,��A������0VʱҲ���Ӧһ������ֵʵ��VRMSmin=0x2f8
	   3������˵������0V��ôVRMSһ����0�����ǲ����ǡ�������ҪУ׼��0.
	   4�����ݹ�ʽ�� xVRMSOS =[((VRMSmin^2*Vn^2)-(VRMSn^2*Vmin^2)) /(128*(Vmin^2-Vn^2))]
	   						 =(0x2f8^2*230^2) /128*(-230^2)
	   						 =���ת��Ϊʮ������Ȼ��NOT�����+1
	   						 =0xFFFFEE60
	 */
	SPIWrite4Bytes(AVRMSOS ,0xFFFFEE60);//
	SPIWrite4Bytes(BVRMSOS ,0xFFFFEE60);//
	SPIWrite4Bytes(CVRMSOS ,0xFFFFEE60);//

	#endif
	#ifdef USE_0_55A_RANG
	 	 /*
	 				  xVGAIN�Ĵ���У׼����
	  ����A���ѹͨ�������棨����У׼VRMSֵ��ƫ�
	   1����A��ͨ��һ����׼��ѹVn��ʵ���������е�223V���ң�
	   2������ AVGAINֵΪ0������AVRMSOSΪ0��
	   3�����VRMSֵ������ƽ�������������ֵ�˲���
	   5��������ĵ�ѹ������������ѹ����ת��ΪVref=	223*10^4
	   6��Ȼ���ȡ��ǰ��VRMSֵ=2365068
	   7��GAIN = (Vref/VRMS -1 )= {(223*10^4/2365068)-1}
	   		   =(-0.057109563023135064192657462702975)
	   	       =((+0.057109563023135064192657462702975)*2^23)���ת��Ϊʮ������Ȼ��NOT�����+1
			   =0xFFF8B0A3
	   8���ٴβ���VRMS�Ĵ���ֵ��Ԥ��ֵ�ӽ���OK���У׼��
	   9����ʵ�ĵ�ѹ����xVRMS�Ĵ�����ֵ����10^4
	 */
 		SPIWrite4Bytes(AVGAIN,0xFFF8B0A3);//
		SPIWrite4Bytes(BVGAIN,0xFFF8B0A3);//
		SPIWrite4Bytes(CVGAIN,0xFFF8B0A3);//
	 /*			   xVRMSOS �Ĵ���У׼����
	   ����A���ѹͨ��������֮������У׼����Ĵ�����
	   1����A��ͨ��һ����׼��ѹVn=230V ��Ҫ��������С��ѹVmin=0V
	   2��A�����������׼��ѹ�����һ��VRMSn=230*10^4,��A������0VʱҲ���Ӧһ������ֵʵ��VRMSmin=0x2f8
	   3������˵������0V��ôVRMSһ����0�����ǲ����ǡ�������ҪУ׼��0.
	   4�����ݹ�ʽ�� xVRMSOS =[((VRMSmin^2*Vn^2)-(VRMSn^2*Vmin^2)) /(128*(Vmin^2-Vn^2))]
	   						 =(0x2f8^2*230^2) /128*(-230^2)
	   						 =���ת��Ϊʮ������Ȼ��NOT�����+1
	   						 =0xFFFFEE60
	 */
	SPIWrite4Bytes(AVRMSOS ,0xFFFFEE60);//
	SPIWrite4Bytes(BVRMSOS ,0xFFFFEE60);//
    SPIWrite4Bytes(CVRMSOS ,0xFFFFEE60);//





   #endif

	  #ifdef  USE_5_5A_RANG
	 /*
	 				  xIGAIN�Ĵ���У׼����
	  ����A�����ͨ�������棨����У׼IRMSֵ��ƫ�
	   1����A��ͨ��һ����׼����In��ʵ����0.45A��
	   2������ AIGAINֵΪ0������AIRMSOSΪ0��PGA=0.
	   3�����IRMSֵ������ƽ�������������ֵ�˲���
	   5��������ĵ�ѹ������������ѹ����ת��ΪVref=	0.45*10^6 (450000)
	   6��Ȼ���ȡ��ǰ��IRMSֵ0x4556(17750)
	   7��Vref/VRMS=450000/17750= 25.3ԶԶ����+-1֮��ķ�Χ����Ȼ��Ҫ�����˷�����
	   8�������˷�Gain����= 16 Ҳ����˵GAIN����Ĵ�����ֵΪPGA1[2:0]=100b
	   9���ٴζ�ȡIRMSֵ 0x47544(292164)
	   10��GAIN = (Vref/VRMS -1 )= {(0.45*10^6/292164)-1}
	   		   =(0.5402)
	   	       =((+0.5402)*2^23)���ת��Ϊʮ������
			   =0x452648
	   8���ٴβ���IRMS�Ĵ���ֵ��Ԥ��ֵ�ӽ���OK���У׼��
	   9����ʵ�ĵ�ѹ����xIRMS�Ĵ�����ֵ����10^6
	 */
						 
	 	SPIWrite2Bytes(Gain,0x0004);  //PGA=16������
		SPIWrite4Bytes(AIGAIN ,0x452648);//
		


	   /*			   xIRMSOS �Ĵ���У׼����
	   ����A���ѹͨ��������֮������У׼����Ĵ�����
	   1����A��ͨ��һ����׼����In=0.45A ��Ҫ��������С����Imin=0A
	   2��A�����������׼���������һ��IRMSn=0.45*10^6,��A������0AʱҲ���Ӧһ������ֵʵ��IRMSmin=0xA20
	   3������˵������0V��ôIRMSһ����0�����ǲ����ǡ�������ҪУ׼��0.
	   4�����ݹ�ʽ�� xIRMSOS =[((IRMSmin^2*In^2)-(IRMSn^2*Imin^2)) /(128*(Imin^2-In^2))]
	   						 =(0xA20^2*0.45^2) /128*(-0.45^2)
	   						 =���ת��Ϊʮ������Ȼ��NOT�����+1
	   						 =0xFFFF32D0
	 */
	SPIWrite4Bytes(AIRMSOS  ,0XFFFF32D0);//

		#endif

		 #ifdef  USE_0_44A_RANG
/*
	        xIGAIN�Ĵ���У׼����
	  ����A�����ͨ�������棨����У׼IRMSֵ��ƫ�
	   1����A��ͨ��һ����׼����In��ʵ����0.42A��
	   2������ AIGAINֵΪ0������AIRMSOSΪ0��PGA=2.
	   3�����IRMSֵ������ƽ�������������ֵ�˲���
	   5��������ĵ�ѹ������������ѹ����ת��ΪVref=	0.44763*10^4 
	   6��Ȼ���ȡ��ǰ��IRMSֵ(37216) 
	   10��GAIN = (Vref/VRMS -1 )= {(0.44763*10^4/37216)-1}
	   		   =(-0.87972108770421324161650902837489)
	   	       =((0.87972108770421324161650902837489)*2^23)���ת��Ϊʮ������ NOT +1
			   = 0xFF8F654D
	   8���ٴβ���IRMS�Ĵ���ֵ��Ԥ��ֵ�ӽ���OK���У׼��
	   9����ʵ�ĵ�ѹ����xIRMS�Ĵ�����ֵ����10^4
*/		 
		SPIWrite2Bytes(Gain,0x0001);  //PGA=2������
		SPIWrite4Bytes(AIGAIN ,0xFF8F654D);//
		SPIWrite4Bytes(BIGAIN ,0xFF8F654D);//
		SPIWrite4Bytes(CIGAIN ,0xFF8F654D);//
		#endif


	   #ifdef USE_0_55A_RANG
	  /*
	        xIGAIN�Ĵ���У׼����
	  ����A�����ͨ�������棨����У׼IRMSֵ��ƫ�
	   1����A��ͨ��һ����׼����In��ʵ����0.0517A��
	   2������ AIGAINֵΪ0������AIRMSOSΪ0��PGA=16.
	   3�����IRMSֵ������ƽ�������������ֵ�˲���
	   5��������ĵ�ѹ������������ѹ����ת��ΪVref=	0.0518*10^4 
	   6��Ȼ���ȡ��ǰ��IRMSֵ(6738) 
	   10��GAIN = (Vref/VRMS -1 )= {(0.0518*10^4/6738)-1}
	   		   =(-0.92312258830513505491243692490353)
	   	       =((0.92312258830513505491243692490353)*8388608)���ת��Ϊʮ������ NOT +1
			   = 0xFF89D71F
	   8���ٴβ���IRMS�Ĵ���ֵ��Ԥ��ֵ�ӽ���OK���У׼��
	   9����ʵ�ĵ�ѹ����xIRMS�Ĵ�����ֵ����10^4
*/
		SPIWrite2Bytes(Gain,0x0004);  //PGA=16������
		SPIWrite4Bytes(AIGAIN ,0xFF89D71F);//
		SPIWrite4Bytes(BIGAIN ,0xFF89D71F);//
		SPIWrite4Bytes(CIGAIN ,0xFF89D71F);//
	
	   /*			   xIRMSOS �Ĵ���У׼����
	   ����A���ѹͨ��������֮������У׼����Ĵ�����
	   1����A��ͨ��һ����׼����In=0.05A ��Ҫ��������С����Imin=0A
	   2��A�����������׼���������һ��IRMSn=0.05*10^4,��A������0AʱҲ���Ӧһ������ֵʵ��IRMSmin=0x80
	   3������˵������0V��ôIRMSһ����0�����ǲ����ǡ�������ҪУ׼��0.
	   4�����ݹ�ʽ�� xIRMSOS =[((IRMSmin^2*In^2)-(IRMSn^2*Imin^2)) /(128*(Imin^2-In^2))]
	   						 =(0x80^2*0.05^2) /128*(-0.05^2)=16384* 0.0025/128*0.0025=40.96/0.32
	   						 =���ת��Ϊʮ������Ȼ��NOT�����+1
	   						 =0xFFFFFF80
	 */
	SPIWrite4Bytes(AIRMSOS  ,0xFFFFFF80);//
	SPIWrite4Bytes(BIRMSOS  ,0xFFFFFF80);//
	SPIWrite4Bytes(CIRMSOS  ,0xFFFFFF80);//




      #endif







	 #ifdef  USE_5_5A_RANG
	  /*
			ʹ��25Wʵ��Ϊ23.1W

		 AWGAIN= [��AWATTref/AWATT��-1]
		 	   =23.1/29.4 -1
			   =0.2142
			   =�������2^23��ת��Ϊ16���ƣ�ȡ����+1.
			   =0x0xFFE4927A
		  	ʹ��100Wʵ��Ϊ98.9W

		 AWGAIN= [��AWATTref/AWATT��-1]
		 	   =98.9/93.3 -1
			   =0.6
			   =�������2^23��ת��Ϊ16���ƣ�ȡ����+1.
			   =0x0xFFF85138

			   ��2�����ƽ��
			   =0xffee71d9
	  */
	SPIWrite4Bytes(AWGAIN  ,0xffee71d9);//

	 #endif
	  #ifdef  USE_0_44A_RANG

			/*
			
			 AWATTref = ��׼��Ķ���   101W
			 AWATT = ADE7878�ĵ�ǰ���� 120.5W

		 AWGAIN= [��AWATTref/AWATT��-1]
		 	   =101/120.5 -1
			   =-0.16182572614107883817427385892116
			   =�������2^23��ת��Ϊ16���ƣ�ȡ����+1.
			   =0xFFEB494C
		  	
	  */
	SPIWrite4Bytes(AWGAIN  ,0xFFEB494C);//
	SPIWrite4Bytes(BWGAIN  ,0xFFEB494C);//
	SPIWrite4Bytes(CWGAIN  ,0xFFEB494C);//

	 #endif
		  #ifdef USE_0_55A_RANG
		  	/*
			
			 AWATTref = ��׼��Ķ���   101W
			 AWATT = ADE7878�ĵ�ǰ���� 120.5W

		 AWGAIN= [��AWATTref/AWATT��-1]
		 	   =101/120.5 -1
			   =-0.16182572614107883817427385892116
			   =�������2^23��ת��Ϊ16���ƣ�ȡ����+1.
			   =0xFFEB494C
		  	
	  */
	SPIWrite4Bytes(AWGAIN  ,0xFFEB494C);//
	SPIWrite4Bytes(BWGAIN  ,0xFFEB494C);//
	SPIWrite4Bytes(CWGAIN  ,0xFFEB494C);//


		  #endif

				 /*
				 
				 	���봫������λУ׼

ͳһʹ�õ�ѹΪ�ο���׼����Ϊ����������е�ѹ�ɼ������õ�ѹ�������ɼ������ģ�����
�����ѹ����������һ������λ�������ͺ���ٶȣ�������Ҳ�ǻ������ɼ�����Ҳ����
�ͺ���ٶȵ����⣬��ô����˵��ѹ�������ͺ�30�֣���ʱ���õĵ���������Ҳ�ͺ�30�֣�
��������֮��������Ǿ���0�ȣ������ǲ���Ҫ�����ģ�������һ����ѹ�����������30��
��һ�����������������Ϊ60�ȣ������ͻ������λ��ͬ���������������ڲ��������Ը���
ʱƫ���С�����ǵ������Ǵ����Ը���ʱ�ͻ���ֺܴ��ƫ���ʵ�������ǲ����ܹ�������Ϊ1��
����Ҫִ����ǲ�����

 ref:p40
 ��λУ׼�Ĵ���(APHCAL��BPHCAL��CPHCAL)����10λ
�Ĵ�������Щ�Ĵ���������.374.0 s��+61.5 s��Χ�ڵ���
��ѹͨ���ź�·���е�ʱ����ǰ������PHCAL�Ĵ�����д
�븺ֵ��ʾʱ����ǰ������ֵ���ʾʱ���ӳ١�1 LSB�൱
��0.976 s��ʱ���ӳٻ�ʱ����ǰ(ʱ��Ƶ��Ϊ1.024 MHz)��
����������·Ƶ��Ϊ60 Hzʱ����λ�����ֱ��ʿɴﵽ0.0211��
(360�� �� 60 Hz/1.024 MHz)�����൱����60 Hzʱ��У����Χ
Ϊ.8.079�㵽+1.329�㡣50 HzʱУ����ΧΪ.6.732�㵽+1.107�㣬��
�ֱ���Ϊ0.0176�� (360�� �� 50 Hz/ 1.024 MHz)��
������λ���Ϊx�ȣ����������ѹΪ�ο��������ģ���
ôͨ����x������λ�ֱ���(60 HzʱΪ0.0211��/LSB����50 Hz
ʱΪ0.0176��/LSB)���ɼ������Ӧ��LSB���������λ��.383��
+63��Χ�ڣ�������Ч����������ȵ�ѹ��ǰ������Ϊ
��ֵ������PHCAL�Ĵ�����д�������ֵ����������ȵ�
ѹ�������Ϊ��ֵ�����������512��Ȼ����д��
xPHCAL
	 �����������ֲ��ϵ�������

	 ȥ���ѹ�������Ĳ�����֪����ƫ��Ϊ<=30'
	 ȥ������������Ĳ�����֪����ƫ����<=60'

		 UPHCAL-IPHCAL=30-60=(-30')
		 UPHCAL-IPHCAL<0 �������ͺ���Ҫ������
		 UPHCAL-IPHCAL>0 ����ѹ�ͺ���Ҫ������


���30�־�����Ҫ���������ġ�Ҫ��Ϊ���ڵ���ͨ��������һ��30�ȵ���ǰ���Բ������30�ȵ����

������λ���Ϊx�ȣ����������ѹΪ�ο��������ģ���
ôͨ����x������λ�ֱ���(60 HzʱΪ0.0211��/LSB����50 Hz
ʱΪ0.0176��/LSB)���ɼ������Ӧ��LSB���������λ��.383��
+63��Χ�ڣ�������Ч��				 
	
	���У�K=0.0176��/LSB	
		    xPHCAL = (UPHCAL-IPHCAL)/K
			��Ϊ  xPHCAL����Ϊ��-383��+63��Χ)��
		  �ѷ�ת��Ϊ�ȣ�30��ת��Ϊ��Ϊ0.5��
			    =-0.5��/ ��0.0176�㣩
				=-28.409090909090909090909090909091
			����Ϊ�ֲ���˵����Ϊ����ֵ�ķ�ʽ
			xPHCAL =|-28|=28
				   =28
												   
	
	 ��28д�뵽�Ĵ����У��ٴ��ý��ܵƻ��ߵƹܵ�cos!=1�ĸ���ȥ����
	 ���ֿ��������Ļ�ȡ�й�����OK�����˻������������λ������ɡ�
	 �����Ա���С���Բ���1W���豸��
	 20160301
	 bt wjw
				 
				 */

			 SPIWrite2Bytes(APHCAL,28);/*��λ�ǲ�*/  
			 SPIWrite2Bytes(BPHCAL,28);/*��λ�ǲ�*/ 
			 SPIWrite2Bytes(CPHCAL,28);/*��λ�ǲ�*/ 
		 /*
					  CFx�����������Ӽ���

		 		 MC=10000imp/KW.H
		 	CF1DEN = ��10^3/MC*10^n��[nΪ��������]

		  ע���ˣ���ط���10^n�η���ָ���ܵľ�ȷ�ȣ�
		  �ٸ�����
		  �����ľ�����10^-4�η���
		  �����ľ�����10^-4�η���
		  ��ô���ʵľ��Ⱦ���10^3*10^-4=10^-1�η�
		  ��ô���ܵ��ۻ����Ⱦ��ǹ��ʵľ��ȳ˻�10^-1*10^-1=10^-2
		  ��ô�����������ô���أ�

		  ������Ӧ����ȡ���ܵľ��������㡣��һ���ֲ���˵������
		 ref:������xWATTHR�Ĵ�����1 LSB�൱��10n wh����nΪ��������)
		  ��λ��W�����أ�

		  ����10^n�η�Ӧ�õ���ʵ�ʵĵ��ܾ��ȼ���
					10^n= 10^-2

		 ����֪ MC�����=10000
		 ����빫ʽ	  
		    	CF1DEN = ��10^3/MC*10^-2��
					   =10

		���� CF1DEN =10�� ��һ�ȵ��ת��Ӧ����10000��

		 */	
	//	SPIWrite2Bytes(COMPMODE,0x0001);//CF1�����������A����Ч	
		SPIWrite2Bytes(CFMODE,0x08c8);/*CF1����ʹ��CF1Ƶ��������й�������֮�ͳ���*/  
		SPIWrite2Bytes(CF1DEN,10);//CF1������ӡ�
	 











#endif	/*�Ƿ�����������У׼����-end*/





}
