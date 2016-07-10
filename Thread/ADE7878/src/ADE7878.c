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
    //PM1=0;PM0=1;	 PSM0正常功耗模
   //PE11 -PM0 
   //PE10 -PM1
	GPIO_SetMode(PE, BIT11, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PE, BIT10, GPIO_MODE_OUTPUT);
	PE11=1;
	PE10=0;
	#else
	  //PM1=0;PM0=1;	 PSM0正常功耗模
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
	名称：GAIN寄存器(地址0xE60F)
	 Bits------Name------Function-------Value
	[15:9] 	保留						 000 0000
	[8:6] 	PGA3[2:0]   相电压增益		 000
	[5:3] 	PGA2[2:0]	零序电流增益	 000
	[2:0] 	PGA1[2:0]	相电流增益		 000
						000: 增益 = 1。
						001: 增益 = 2。
						010: 增益 = 4。
						011: 增益 = 8。
						100: 增益 = 16。
	Note:默认为1倍增益。如果输入信号
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



  	
   #if 1 /*是否启用老王的校准配置-start*/

	 
	 #ifdef  USE_5_5A_RANG
	 /*
	 				xWTHR 功率阀值的计算


							WTHR寄存器
				
					WTHR = (PMAX*fs*3600*10^n)/(Vfs*Ifs)
				
				
				 其中，fS = 8 kHz，即DSP用于计算瞬时功率的频率。
				 THR必须始终大于或等于PMAX——相电压和相电流具有
				满量程幅度时ADE7878计算的有功功率：
				
					PMAX = 33,516,139
				
				 如果THR小于PMAX，应调整VRMSREF或IRMSREF。
				
				
				PMAX = 33516139 = 0x1FF6A6B
				
				xWTHR=(PMAX*fs*3600*10^n)/(ufs*Ifs)
					=33516139*8000*3600*10^-3/ 5.5A*387.5V
					= 965264803200 /2131.5
					= 0x1AFE0CDA (Vfs=5.5A)
				整理得：
				THR0=0x00FE0CDA
				THR1=0x0000001A	

	 
	 
	 */

	SPIWrite4Bytes(WTHR0,0x00FE0CDA);
	SPIWrite4Bytes(WTHR1,0x0000001A);  

   #endif

   #ifdef USE_0_44A_RANG
	   /*
	 				xWTHR 功率阀值的计算


							WTHR寄存器
				
					WTHR = (PMAX*fs*3600*10^n)/(Vfs*Ifs)
				
				
				 其中，fS = 8 kHz，即DSP用于计算瞬时功率的频率。
				 THR必须始终大于或等于PMAX——相电压和相电流具有
				满量程幅度时ADE7878计算的有功功率：
				
					PMAX = 33,516,139
				
				 如果THR小于PMAX，应调整VRMSREF或IRMSREF。
				
				
				PMAX = 33516139 = 0x1FF6A6B
				
				xWTHR=(PMAX*fs*3600*10^n)/(ufs*Ifs)
					=33516139*8000*3600*10^-3/ 44A*485V[反复计算逼近实际电能时停止]（387）
					= 965264803200/21340
					= 0x2B2320C(Ifs=44.1A)
				整理得：
				THR0=0x00D7B4C4
				THR1=0x00000002	
									  
	 				  
	 			 THR的值越接近PMAX值其累计电能速度越快，反之越慢，
				 应该使用标准表校准为每分钟内增加的电能基本一致即可
				 还有一种办法就是用脉冲，设置为标准表相同的脉冲输出
				 这样当这个值设定好了之后正好可以使ADE7878输出的脉冲等于标准表输出的CP即可完成校准
				 
													   
	 */
	SPIWrite4Bytes(WTHR0,0x00B2320C);
	SPIWrite4Bytes(WTHR1,0x00000002);  
   #endif

   #ifdef USE_0_55A_RANG
	/*
	 				xWTHR 功率阀值的计算


							WTHR寄存器
				
					WTHR = (PMAX*fs*3600*10^n)/(Vfs*Ifs)
				
				
				 其中，fS = 8 kHz，即DSP用于计算瞬时功率的频率。
				 THR必须始终大于或等于PMAX——相电压和相电流具有
				满量程幅度时ADE7878计算的有功功率：
				
					PMAX = 33,516,139
				
				 如果THR小于PMAX，应调整VRMSREF或IRMSREF。
				
				
				PMAX = 33516139 = 0x1FF6A6B
													 330
				xWTHR=(PMAX*fs*3600*10^n)/(ufs*Ifs)
					=33516139*8000*3600*10^-3/ 55A*380V[反复计算逼近实际电能时停止]（387）
					= 965264803200/20900
					= 0x2C0B9D6(Ifs=55A)
				整理得：
				THR0=0x00C0B9D6
				THR1=0x00000002	
									  
	 				  
	 			 THR的值越接近PMAX值其累计电能速度越快，反之越慢，
				 应该使用标准表校准为每分钟内增加的电能基本一致即可
				 还有一种办法就是用脉冲，设置为标准表相同的脉冲输出
				 这样当这个值设定好了之后正好可以使ADE7878输出的脉冲等于标准表输出的CP即可完成校准
				 
													   
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
	SPIWrite2Bytes(LINECYC,347);  //347个半波周期preo		采样的频率，不使用线累积模式

	SPIWrite4Bytes(MASK0,0x00000020);//line cycle interrupt enable
	SPIWrite4Bytes(MASK1,0x0000ffff);   




	 ///////////////////////////////////////////////////////////////
	 //测量A相峰值电压.
	SPIWrite4Bytes(PEAKCYC,16);//16个循环峰值检测一次
	SPIWrite1Byte(MMODE,0x04);//开启A相峰值检测。					
	 
		   /*

			   通道计算数据参考
		   VLEVEL =Vfs/Vn*491520
		          =387v/220V*491520
				  =0xD3174
		   
		   */

	  SPIWrite4Bytes(VLEVEL,0xD3174);//


		#ifdef  USE_5_5A_RANG
	 /*
	 				  xVGAIN寄存器校准流程
	  配置A相电压通道的增益（用来校准VRMS值的偏差）
	   1、给A相通入一个标准电压Vn（实际是输入市电230V左右）
	   2、配置 AVGAIN值为0。配置AVRMSOS为0。
	   3、输出VRMS值（递推平均或者排序后中值滤波）
	   5、将输入的电压互感器采样电压进行转换为Vref=	230*10^4
	   6、然后读取当前的VRMS值=2398439
	   7、GAIN = (Vref/VRMS -1 )= {(230*10^4/2398439)-1}
	   		   =(-0.041)
	   	       =((+0.041)*2^23)结果转换为十六进制然后NOT，最后+1
			   =0xFFFAC084
	   8、再次测量VRMS寄存器值和预期值接近。OK完成校准。
	   9、真实的电压等于xVRMS寄存器的值除以10^4
	 */
 		SPIWrite4Bytes(AVGAIN,0xFFFAC084);//
	 /*			   xVRMSOS 寄存器校准流程
	   配置A相电压通道的增益之后再来校准这个寄存器。
	   1、给A相通入一个标准电压Vn=230V 需要测量的最小电压Vmin=0V
	   2、A相输入这个标准电压会产生一个VRMSn=230*10^4,当A相输入0V时也会对应一个噪声值实测VRMSmin=0x2f8
	   3、按理说输入是0V那么VRMS一定是0，但是并不是。所以需要校准到0.
	   4、根据公式： xVRMSOS =[((VRMSmin^2*Vn^2)-(VRMSn^2*Vmin^2)) /(128*(Vmin^2-Vn^2))]
	   						 =(0x2f8^2*230^2) /128*(-230^2)
	   						 =结果转换为十六进制然后NOT，最后+1
	   						 =0xFFFFEE60
	 */
	SPIWrite4Bytes(AVRMSOS ,0xFFFFEE60);//
	  #endif

	  #ifdef  USE_0_44A_RANG

		 	 /*
	 				  xVGAIN寄存器校准流程
	  配置A相电压通道的增益（用来校准VRMS值的偏差）
	   1、给A相通入一个标准电压Vn（实际是输入市电223V左右）
	   2、配置 AVGAIN值为0。配置AVRMSOS为0。
	   3、输出VRMS值（递推平均或者排序后中值滤波）
	   5、将输入的电压互感器采样电压进行转换为Vref=	223*10^4
	   6、然后读取当前的VRMS值=2365068
	   7、GAIN = (Vref/VRMS -1 )= {(223*10^4/2365068)-1}
	   		   =(-0.057109563023135064192657462702975)
	   	       =((+0.057109563023135064192657462702975)*2^23)结果转换为十六进制然后NOT，最后+1
			   =0xFFF8B0A3
	   8、再次测量VRMS寄存器值和预期值接近。OK完成校准。
	   9、真实的电压等于xVRMS寄存器的值除以10^4
	 */
 		SPIWrite4Bytes(AVGAIN,0xFFF8B0A3);//
		SPIWrite4Bytes(BVGAIN,0xFFF8B0A3);//
		SPIWrite4Bytes(CVGAIN,0xFFF8B0A3);//
	 /*			   xVRMSOS 寄存器校准流程
	   配置A相电压通道的增益之后再来校准这个寄存器。
	   1、给A相通入一个标准电压Vn=230V 需要测量的最小电压Vmin=0V
	   2、A相输入这个标准电压会产生一个VRMSn=230*10^4,当A相输入0V时也会对应一个噪声值实测VRMSmin=0x2f8
	   3、按理说输入是0V那么VRMS一定是0，但是并不是。所以需要校准到0.
	   4、根据公式： xVRMSOS =[((VRMSmin^2*Vn^2)-(VRMSn^2*Vmin^2)) /(128*(Vmin^2-Vn^2))]
	   						 =(0x2f8^2*230^2) /128*(-230^2)
	   						 =结果转换为十六进制然后NOT，最后+1
	   						 =0xFFFFEE60
	 */
	SPIWrite4Bytes(AVRMSOS ,0xFFFFEE60);//
	SPIWrite4Bytes(BVRMSOS ,0xFFFFEE60);//
	SPIWrite4Bytes(CVRMSOS ,0xFFFFEE60);//

	#endif
	#ifdef USE_0_55A_RANG
	 	 /*
	 				  xVGAIN寄存器校准流程
	  配置A相电压通道的增益（用来校准VRMS值的偏差）
	   1、给A相通入一个标准电压Vn（实际是输入市电223V左右）
	   2、配置 AVGAIN值为0。配置AVRMSOS为0。
	   3、输出VRMS值（递推平均或者排序后中值滤波）
	   5、将输入的电压互感器采样电压进行转换为Vref=	223*10^4
	   6、然后读取当前的VRMS值=2365068
	   7、GAIN = (Vref/VRMS -1 )= {(223*10^4/2365068)-1}
	   		   =(-0.057109563023135064192657462702975)
	   	       =((+0.057109563023135064192657462702975)*2^23)结果转换为十六进制然后NOT，最后+1
			   =0xFFF8B0A3
	   8、再次测量VRMS寄存器值和预期值接近。OK完成校准。
	   9、真实的电压等于xVRMS寄存器的值除以10^4
	 */
 		SPIWrite4Bytes(AVGAIN,0xFFF8B0A3);//
		SPIWrite4Bytes(BVGAIN,0xFFF8B0A3);//
		SPIWrite4Bytes(CVGAIN,0xFFF8B0A3);//
	 /*			   xVRMSOS 寄存器校准流程
	   配置A相电压通道的增益之后再来校准这个寄存器。
	   1、给A相通入一个标准电压Vn=230V 需要测量的最小电压Vmin=0V
	   2、A相输入这个标准电压会产生一个VRMSn=230*10^4,当A相输入0V时也会对应一个噪声值实测VRMSmin=0x2f8
	   3、按理说输入是0V那么VRMS一定是0，但是并不是。所以需要校准到0.
	   4、根据公式： xVRMSOS =[((VRMSmin^2*Vn^2)-(VRMSn^2*Vmin^2)) /(128*(Vmin^2-Vn^2))]
	   						 =(0x2f8^2*230^2) /128*(-230^2)
	   						 =结果转换为十六进制然后NOT，最后+1
	   						 =0xFFFFEE60
	 */
	SPIWrite4Bytes(AVRMSOS ,0xFFFFEE60);//
	SPIWrite4Bytes(BVRMSOS ,0xFFFFEE60);//
    SPIWrite4Bytes(CVRMSOS ,0xFFFFEE60);//





   #endif

	  #ifdef  USE_5_5A_RANG
	 /*
	 				  xIGAIN寄存器校准流程
	  配置A相电流通道的增益（用来校准IRMS值的偏差）
	   1、给A相通入一个标准电流In（实际是0.45A）
	   2、配置 AIGAIN值为0。配置AIRMSOS为0。PGA=0.
	   3、输出IRMS值（递推平均或者排序后中值滤波）
	   5、将输入的电压互感器采样电压进行转换为Vref=	0.45*10^6 (450000)
	   6、然后读取当前的IRMS值0x4556(17750)
	   7、Vref/VRMS=450000/17750= 25.3远远大于+-1之间的范围，显然需要配置运放增益
	   8、设置运放Gain增益= 16 也就是说GAIN增益寄存器的值为PGA1[2:0]=100b
	   9、再次读取IRMS值 0x47544(292164)
	   10、GAIN = (Vref/VRMS -1 )= {(0.45*10^6/292164)-1}
	   		   =(0.5402)
	   	       =((+0.5402)*2^23)结果转换为十六进制
			   =0x452648
	   8、再次测量IRMS寄存器值和预期值接近。OK完成校准。
	   9、真实的电压等于xIRMS寄存器的值除以10^6
	 */
						 
	 	SPIWrite2Bytes(Gain,0x0004);  //PGA=16倍增益
		SPIWrite4Bytes(AIGAIN ,0x452648);//
		


	   /*			   xIRMSOS 寄存器校准流程
	   配置A相电压通道的增益之后再来校准这个寄存器。
	   1、给A相通入一个标准电流In=0.45A 需要测量的最小电流Imin=0A
	   2、A相输入这个标准电流会产生一个IRMSn=0.45*10^6,当A相输入0A时也会对应一个噪声值实测IRMSmin=0xA20
	   3、按理说输入是0V那么IRMS一定是0，但是并不是。所以需要校准到0.
	   4、根据公式： xIRMSOS =[((IRMSmin^2*In^2)-(IRMSn^2*Imin^2)) /(128*(Imin^2-In^2))]
	   						 =(0xA20^2*0.45^2) /128*(-0.45^2)
	   						 =结果转换为十六进制然后NOT，最后+1
	   						 =0xFFFF32D0
	 */
	SPIWrite4Bytes(AIRMSOS  ,0XFFFF32D0);//

		#endif

		 #ifdef  USE_0_44A_RANG
/*
	        xIGAIN寄存器校准流程
	  配置A相电流通道的增益（用来校准IRMS值的偏差）
	   1、给A相通入一个标准电流In（实际是0.42A）
	   2、配置 AIGAIN值为0。配置AIRMSOS为0。PGA=2.
	   3、输出IRMS值（递推平均或者排序后中值滤波）
	   5、将输入的电压互感器采样电压进行转换为Vref=	0.44763*10^4 
	   6、然后读取当前的IRMS值(37216) 
	   10、GAIN = (Vref/VRMS -1 )= {(0.44763*10^4/37216)-1}
	   		   =(-0.87972108770421324161650902837489)
	   	       =((0.87972108770421324161650902837489)*2^23)结果转换为十六进制 NOT +1
			   = 0xFF8F654D
	   8、再次测量IRMS寄存器值和预期值接近。OK完成校准。
	   9、真实的电压等于xIRMS寄存器的值除以10^4
*/		 
		SPIWrite2Bytes(Gain,0x0001);  //PGA=2倍增益
		SPIWrite4Bytes(AIGAIN ,0xFF8F654D);//
		SPIWrite4Bytes(BIGAIN ,0xFF8F654D);//
		SPIWrite4Bytes(CIGAIN ,0xFF8F654D);//
		#endif


	   #ifdef USE_0_55A_RANG
	  /*
	        xIGAIN寄存器校准流程
	  配置A相电流通道的增益（用来校准IRMS值的偏差）
	   1、给A相通入一个标准电流In（实际是0.0517A）
	   2、配置 AIGAIN值为0。配置AIRMSOS为0。PGA=16.
	   3、输出IRMS值（递推平均或者排序后中值滤波）
	   5、将输入的电压互感器采样电压进行转换为Vref=	0.0518*10^4 
	   6、然后读取当前的IRMS值(6738) 
	   10、GAIN = (Vref/VRMS -1 )= {(0.0518*10^4/6738)-1}
	   		   =(-0.92312258830513505491243692490353)
	   	       =((0.92312258830513505491243692490353)*8388608)结果转换为十六进制 NOT +1
			   = 0xFF89D71F
	   8、再次测量IRMS寄存器值和预期值接近。OK完成校准。
	   9、真实的电压等于xIRMS寄存器的值除以10^4
*/
		SPIWrite2Bytes(Gain,0x0004);  //PGA=16倍增益
		SPIWrite4Bytes(AIGAIN ,0xFF89D71F);//
		SPIWrite4Bytes(BIGAIN ,0xFF89D71F);//
		SPIWrite4Bytes(CIGAIN ,0xFF89D71F);//
	
	   /*			   xIRMSOS 寄存器校准流程
	   配置A相电压通道的增益之后再来校准这个寄存器。
	   1、给A相通入一个标准电流In=0.05A 需要测量的最小电流Imin=0A
	   2、A相输入这个标准电流会产生一个IRMSn=0.05*10^4,当A相输入0A时也会对应一个噪声值实测IRMSmin=0x80
	   3、按理说输入是0V那么IRMS一定是0，但是并不是。所以需要校准到0.
	   4、根据公式： xIRMSOS =[((IRMSmin^2*In^2)-(IRMSn^2*Imin^2)) /(128*(Imin^2-In^2))]
	   						 =(0x80^2*0.05^2) /128*(-0.05^2)=16384* 0.0025/128*0.0025=40.96/0.32
	   						 =结果转换为十六进制然后NOT，最后+1
	   						 =0xFFFFFF80
	 */
	SPIWrite4Bytes(AIRMSOS  ,0xFFFFFF80);//
	SPIWrite4Bytes(BIRMSOS  ,0xFFFFFF80);//
	SPIWrite4Bytes(CIRMSOS  ,0xFFFFFF80);//




      #endif







	 #ifdef  USE_5_5A_RANG
	  /*
			使用25W实测为23.1W

		 AWGAIN= [（AWATTref/AWATT）-1]
		 	   =23.1/29.4 -1
			   =0.2142
			   =结果乘以2^23后转换为16进制，取反后+1.
			   =0x0xFFE4927A
		  	使用100W实测为98.9W

		 AWGAIN= [（AWATTref/AWATT）-1]
		 	   =98.9/93.3 -1
			   =0.6
			   =结果乘以2^23后转换为16进制，取反后+1.
			   =0x0xFFF85138

			   将2者求和平均
			   =0xffee71d9
	  */
	SPIWrite4Bytes(AWGAIN  ,0xffee71d9);//

	 #endif
	  #ifdef  USE_0_44A_RANG

			/*
			
			 AWATTref = 标准表的读数   101W
			 AWATT = ADE7878的当前读数 120.5W

		 AWGAIN= [（AWATTref/AWATT）-1]
		 	   =101/120.5 -1
			   =-0.16182572614107883817427385892116
			   =结果乘以2^23后转换为16进制，取反后+1.
			   =0xFFEB494C
		  	
	  */
	SPIWrite4Bytes(AWGAIN  ,0xFFEB494C);//
	SPIWrite4Bytes(BWGAIN  ,0xFFEB494C);//
	SPIWrite4Bytes(CWGAIN  ,0xFFEB494C);//

	 #endif
		  #ifdef USE_0_55A_RANG
		  	/*
			
			 AWATTref = 标准表的读数   101W
			 AWATT = ADE7878的当前读数 120.5W

		 AWGAIN= [（AWATTref/AWATT）-1]
		 	   =101/120.5 -1
			   =-0.16182572614107883817427385892116
			   =结果乘以2^23后转换为16进制，取反后+1.
			   =0xFFEB494C
		  	
	  */
	SPIWrite4Bytes(AWGAIN  ,0xFFEB494C);//
	SPIWrite4Bytes(BWGAIN  ,0xFFEB494C);//
	SPIWrite4Bytes(CWGAIN  ,0xFFEB494C);//


		  #endif

				 /*
				 
				 	输入传感器相位校准

统一使用电压为参考基准，因为在这个方案中电压采集的是用电压传感器采集进来的，所以
这个电压传感器会有一定的相位误差，比如滞后多少度，而电流也是互感器采集来的也会有
滞后多少度的问题，那么比如说电压传感器滞后30分，此时采用的电流传感器也滞后30分，
这样他们之间的相对相角就是0度，这样是不需要补偿的，问题是一个电压互感器相角是30度
另一个电流互感器的相角为60度，这样就会出现相位不同的情况。这种情况在测量纯阻性负载
时偏差很小，但是当测量非纯阻性负载时就会出现很大的偏差。现实生活中是不可能功率因数为1的
所以要执行相角补偿。

 ref:p40
 相位校准寄存器(APHCAL、BPHCAL和CPHCAL)都是10位
寄存器，这些寄存器可以在.374.0 s到+61.5 s范围内调整
电压通道信号路径中的时间提前量。向PHCAL寄存器中写
入负值表示时间提前，而正值则表示时间延迟。1 LSB相当
于0.976 s的时间延迟或时间提前(时钟频率为1.024 MHz)。
这样，当线路频率为60 Hz时，相位补偿分辨率可达到0.0211°
(360° × 60 Hz/1.024 MHz)。这相当于在60 Hz时总校正范围
为.8.079°到+1.329°。50 Hz时校正范围为.6.732°到+1.107°，而
分辨率为0.0176° (360° × 50 Hz/ 1.024 MHz)。
假设相位误差为x度，且是以相电压为参考来测量的，那
么通过将x除以相位分辨率(60 Hz时为0.0211°/LSB，而50 Hz
时为0.0176°/LSB)即可计算出相应的LSB。结果必须位于.383到
+63范围内，否则无效。如果电流比电压超前，则结果为
负值，会向PHCAL寄存器中写入其绝对值。如果电流比电
压落后，则结果为正值，将结果加上512，然后再写入
xPHCAL
	 以上是引用手册上的描述。

	 去查电压互感器的参数可知他的偏移为<=30'
	 去查电流互感器的参数可知他的偏移是<=60'

		 UPHCAL-IPHCAL=30-60=(-30')
		 UPHCAL-IPHCAL<0 ：电流滞后需要负补偿
		 UPHCAL-IPHCAL>0 ：电压滞后需要正补偿


这个30分就是需要补偿回来的。要人为的在电流通道中引入一个30度的提前量以补偿这个30度的相角

假设相位误差为x度，且是以相电压为参考来测量的，那
么通过将x除以相位分辨率(60 Hz时为0.0211°/LSB，而50 Hz
时为0.0176°/LSB)即可计算出相应的LSB。结果必须位于.383到
+63范围内，否则无效。				 
	
	则有：K=0.0176°/LSB	
		    xPHCAL = (UPHCAL-IPHCAL)/K
			因为  xPHCAL不能为（-383到+63范围)外
		  把分转换为度，30‘转换为度为0.5度
			    =-0.5°/ （0.0176°）
				=-28.409090909090909090909090909091
			又因为手册上说负数为绝对值的方式
			xPHCAL =|-28|=28
				   =28
												   
	
	 将28写入到寄存器中，再次用节能灯或者灯管灯cos!=1的负载去测试
	 发现可以正常的获取有功功率OK，至此互感器引入的相位补偿完成。
	 经过对比最小可以测量1W的设备。
	 20160301
	 bt wjw
				 
				 */

			 SPIWrite2Bytes(APHCAL,28);/*相位角差*/  
			 SPIWrite2Bytes(BPHCAL,28);/*相位角差*/ 
			 SPIWrite2Bytes(CPHCAL,28);/*相位角差*/ 
		 /*
					  CFx引脚脉冲因子计算

		 		 MC=10000imp/KW.H
		 	CF1DEN = （10^3/MC*10^n）[n为正负整数]

		  注意了，这地方的10^n次方是指电能的精确度，
		  举个例子
		  电流的精度是10^-4次方；
		  电流的精度是10^-4次方；
		  那么功率的精度就是10^3*10^-4=10^-1次方
		  那么电能的累积精度就是功率的精度乘积10^-1*10^-1=10^-2
		  那么这个脉冲数怎么算呢？

		  脉冲数应该是取电能的精度来计算。这一点手册上说的明白
		 ref:（假设xWATTHR寄存器的1 LSB相当于10n wh其中n为正负整数)
		  单位是W（瓦特）

		  所以10^n次方应该等于实际的电能精度即：
					10^n= 10^-2

		 再已知 MC电表常数=10000
		 则代入公式	  
		    	CF1DEN = （10^3/MC*10^-2）
					   =10

		所以 CF1DEN =10。 走一度电的转数应该是10000次

		 */	
	//	SPIWrite2Bytes(COMPMODE,0x0001);//CF1脉冲仅仅对于A相有效	
		SPIWrite2Bytes(CFMODE,0x08c8);/*CF1脉冲使能CF1频率与基波有功功比率之和成正*/  
		SPIWrite2Bytes(CF1DEN,10);//CF1脉冲分子。
	 











#endif	/*是否启用老王的校准配置-end*/





}
