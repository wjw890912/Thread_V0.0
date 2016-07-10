


#include <stdio.h>
#include <string.h>
#include "ds18b20.h"  
#include "NUC472_442.h"



//500ms	 һ��
#define TEMP_CREATTRM_INTERVAL  /*(250*4)*2 */	(250*4)*2



#define Read18b20IO()       PA1




#define Delay_US(x)   Delay_us(x)
#define DS18B20_NUM   1    
#define MAXNUM        2  
void Set18b20IOout(void);
void Set18b20IOin(void);

static uint8_t  fac_us=0;//us��ʱ������
static uint16_t fac_ms=0;//ms��ʱ������
void Init_DS18B20_IO(void)
{
  /*
//���Լ��İ��ӵĵ����߿�
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);
  // Configure PD0 pins is 1-WRITE bus
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  */

  GPIO_SetMode(PA, BIT1, GPIO_MODE_QUASI); //ds18b20 IO

} 	
void Set18b20IOin(void)
{						 

 GPIO_SetMode(PA, BIT1, GPIO_MODE_INPUT); //ds18b20 IO
 /*
 GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);
  // Configure PD0 pins
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

*/
}  
 void Set18b20IOout(void)
{
   GPIO_SetMode(PA, BIT1,  GPIO_MODE_OUTPUT); //ds18b20 IO
  /*
   GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);
  // Configure PD0 pins 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

	  */
}            
//��ʱNms
//ע��Nms�ķ�Χ
//Nms<=0xffffff*8/SYSCLK
//��72M������,Nms<=1864 
 
//��ʼ���ӳٺ���
void Delay_init(uint8_t SYSCLK)
{


 SYS_UnlockReg(); 
CLK_EnableModuleClock(TMR2_MODULE);
CLK_SetModuleClock(TMR2_MODULE,CLK_CLKSEL1_TMR2SEL_HXT, 0);


 /*SysTick->CTRL&=0xfffffffb;//ѡ���ڲ�ʱ�� HCLK/8
 fac_us=SYSCLK/8;      
 fac_ms=(u16)fac_us*1000;
  */
 // TIMER_Open(TIMER2, TIMER_PERIODIC_MODE, 1000000);
  	//TIMER_Open(TIMER0, TIMER_PERIODIC_MODE,20000);//20
  //  TIMER_EnableInt(TIMER2);
//	NVIC_SetPriority(TMR0_IRQn, 3 ); //��ʱ�����ж����ȼ�Ӧ�ñȴ��ڵĵ�
  //  NVIC_EnableIRQ(TMR0_IRQn);
	// TIMER_Start(TIMER2);

  // CLK_SetModuleClock(TMR2_MODULE,CLK_CLKSEL1_TMR2SEL_HXT, 0);
   //TIMER_Open(TIMER2, TIMER_ONESHOT_MODE, 1000000);
  // TIMER_Start(TIMER2);
} 


  void delayus_soft(uint32_t t)
 {
 	   uint8_t i;
	   do
	   {
		 
	    for(i=2;i;i--)  __NOP();

 	   }while(t--);
 }


  void Delay_us(uint32_t Nus)
  {

	  
	delayus_soft(Nus);

  //TIMER_Delay(TIMER2, Nus);
 /* SysTick->LOAD=Nus*fac_us;       //ʱ�����      
 SysTick->CTRL|=0x01;            //��ʼ����    
 while(!(SysTick->CTRL&(1<<16)));//�ȴ�ʱ�䵽�� 
 SysTick->CTRL=0X00000000;       //�رռ�����
 SysTick->VAL=0X00000000;        //��ռ�����   
  */
  }   

  void Delay_ms(uint16_t nms)
  {
  TIMER_Delay(TIMER2,nms*1000);
  /* SysTick->LOAD=(u32)nms*fac_ms; //ʱ�����  
 SysTick->CTRL|=0x01;               //��ʼ����    
 while(!(SysTick->CTRL&(1<<16)));   //�ȴ�ʱ�䵽�� 
 SysTick->CTRL&=0XFFFFFFFE;         //�رռ�����
 SysTick->VAL=0X00000000;           //��ռ����� 
  	*/
  }

void Write18b20IO(u8 flage) 
{

	
	  if(flage)
	  {
	   PA1=1;
	  }else
	  { 
	   PA1=0;
	  }	 
	 
}
u8 DS18B20_Reset(void)  
{  
    u8 i = 0;  
      
    Set18b20IOout();        //�����˿��������ģʽ  
    Write18b20IO(1);  
    Delay_US(1);  
    Write18b20IO(0);        //��������480us~240us  
    Delay_US(500);          //>480US��ʱ  
    Write18b20IO(1);  
    Delay_US(2);            //��λ���  
    Set18b20IOin();         //�����˿ڸ�������ģʽ  	  0
    while(Read18b20IO())    //�ȴ��͵�ƽӦ���ź�  
    {  
        i ++;  
        Delay_US(1);  
        if(i > 100)  
        {  
           // uart_printf("DS18B20 error!\r\n");  
            return FALSE;   //�ȴ���ʱ,��ʼ��ʧ��,����FALSE;  
        }  
    }  
   // Delay_US(250);          //�����ظ��ź�  
   Delay_US(410);          //�����ظ��ź� 
    return TRUE;            //��⵽DS18B20,���ҳ�ʼ���ɹ�  
}  
__inline u8 DS18B20_ReadBit(void)  
{  
    u8 data = 0;  
   
    Set18b20IOout();    //�����˿��������ģʽ  
    Write18b20IO(0);    //��������10-15us  
    Delay_US(12);  
   Write18b20IO(1);    //�ͷ�����  
    Set18b20IOin();     //�����˿ڸ�������ģʽ  
    Delay_US(10);  
    if(Read18b20IO())   //��ȡ����,��ȡ���Լ��ʱ40-45us  
       data = 0x01;
	
    Delay_US(40); 
    return data;  
}  
u8 DS18B20_ReadData(void)  
{ 
    u8 i,data = 0;  
     __disable_irq();//  �൱�� CPSID I   
    for(i = 0;i < 8;i ++)  
    {  
  
        data >>= 1;  
        if(DS18B20_ReadBit())  
            data |= 0x80;  
    }
	__enable_irq();//   �൱�� CPSIE I   
    return data;  
}  
  
void DS18B20_WriteBit(u8 bit)  
{  
 
    Set18b20IOout();    //�����˿��������ģʽ  
    Write18b20IO(0);        //��������10-15us  
    Delay_US(12);  
    Write18b20IO(bit & 0x01);//   //д������λ,����20-45us  
    Delay_US(30);  
    Write18b20IO(1);        //�ͷ�����  
    Delay_US(5); 
   
}  
  void DS18B20_WriteData(u8 data)  
{  
    u8 i;  
   	__disable_irq();//  �൱�� CPSID I   
    for(i = 0;i < 8;i ++)  
    {  
        DS18B20_WriteBit(data);  
        data >>= 1;  
    } 
	__enable_irq();//   �൱�� CPSIE I  
}  
s16 DS18B20_ReadTemper(void)  
{  
    u8 th, tl;  
    s16 data;  
      
    if(DS18B20_Reset() == FALSE)      
    {  
        return 0xffff;  //���ش���  
    }  
  
    DS18B20_WriteData(0xcc);    //���������к�  
    DS18B20_WriteData(0x44);    //�����¶�ת��  
    DS18B20_Reset();  
    DS18B20_WriteData(0xcc);    //���������к�  
    DS18B20_WriteData(0xbe);    //��ȡ�¶�  
    tl = DS18B20_ReadData();    //��ȡ�Ͱ�λ  
    th = DS18B20_ReadData();    //��ȡ�߰�λ  
    data = th;  
    data <<= 8;  
    data |= tl;  
    data *= 6.25;               //�¶�ֵ����100������ȷ��2λС��  
      
    return data;  
}  
u8 DS18B20_Read2Bit(void)  
{  
    u8 i,data = 0;  
      
    for(i = 0;i < 2;i ++)  
    {  
  
        data <<= 1;  
        if(DS18B20_ReadBit())  
            data |= 1;  
    }  
    return data;  
}    
u8 DS18B20_SearchROM(u8 (*pID)[8],u8 Num)  
{   
    unsigned char k,l,chongtuwei,m,n;  
    unsigned char zhan[(MAXNUM-1)];  
    unsigned char ss[64];  
    unsigned char s=0; 
	u8 num = 0; 
    l=0;  
      
    do  
    {  
	 
        DS18B20_Reset();  
        DS18B20_WriteData(0xf0);      
        for(m=0;m<8;m++)  
        {  
            s=0;  
            for(n=0;n<8;n++)  
            {  
                k=DS18B20_Read2Bit();//����λ����  
                k=k&0x03;  
                s>>=1;  
                if(k==0x01)//01����������Ϊ0 д0 ��λΪ0��������Ӧ  
                {             
                    DS18B20_WriteBit (0);  
                    ss[(m*8+n)]=0;  
                }  
                else if(k==0x02)//����������Ϊ1 д1 ��λΪ1��������Ӧ  
                {  
                    s=s|0x80;  
                    DS18B20_WriteBit (1);  
                    ss[(m*8+n)]=1;  
                }  
                else if(k==0x00)//����������Ϊ00 �г�ͻλ �жϳ�ͻλ   
                {               //�����ͻλ����ջ��д0 С��ջ��д��ǰ���� ����ջ��д1  
                    chongtuwei=m*8+n+1;                   
                    if(chongtuwei>zhan[l])  
                    {                         
                        DS18B20_WriteBit (0);  
                        ss[(m*8+n)]=0;                                                
                        zhan[++l]=chongtuwei;                         
                    }  
                    else if(chongtuwei<zhan[l])  
                    {  
                        s=s|((ss[(m*8+n)]&0x01)<<7);  
                        DS18B20_WriteBit (ss[(m*8+n)]);  
                    }  
                    else if(chongtuwei==zhan[l])  
                    {  
                        s=s|0x80;  
                        DS18B20_WriteBit (1);  
                        ss[(m*8+n)]=1;  
                        l=l-1;  
                    }
				  
                }  
                else  
                {  
                    return num; //�������,//�����������ĸ���  
                }  
            }  
            pID[num][m]=s;        
        }  
        num=num+1;  
    }  
    while(zhan[l]!=0&&(num<MAXNUM));   
      
    return num;     //�����������ĸ���  
}  
  s16 DS18B20_ReadDesignateTemper( u8 pID[8])  
{  
    u8 th, tl;  
    s16 data;  
      
    if(DS18B20_Reset() == FALSE)      
    {  
        return 0xffff;              //���ش���  
    }  
  		
    DS18B20_WriteData(0xcc);        //���������к� 
    DS18B20_WriteData(0x44);        //�����¶�ת�� 

    DS18B20_Reset();  
    DS18B20_WriteData(0x55);        //�������к�ƥ������  
    for(data = 0;data < 8;data ++)   //����8byte�����к�     
    {  
       DS18B20_WriteData(pID[data]);  
    }  	
    Delay_US(10); 
    DS18B20_WriteData(0xbe);    //��ȡ�¶�  
    tl = DS18B20_ReadData();    //��ȡ�Ͱ�λ  
    th = DS18B20_ReadData();    //��ȡ�߰�λ  

    data = th;  
    data <<= 8;  
    data |= tl;  
    data *= 6.25;               //�¶�ֵ����100������ȷ��2λС��  
      
    return data;  
} 

extern uint16_t  usSRegHoldBuf[100];
extern int OWFirst(void);
extern int  OWSearch(void);
extern unsigned char* GetRomAddr(void);
uint32_t time_run=0;
uint8_t  temp[33];
uint8_t ID_Buff[DS18B20_NUM][8];//={{0x28,0xb0,0x4a,0x24,0x07,0x00,0x00,0x49},{0x28,0x84,0x17,0x24,0x07,0x00,0x00,0x01}};  
void DS1820main(void)  
{   
static uint8_t fistsech=0;
    int16_t temp1,temp2;
    //u8 buff[16];  
  //  u8 i,j,num=DS18B20_NUM;  
	 	
		if(fistsech==0)
		{
		Delay_init(72);//72M
		Init_DS18B20_IO();

	
		if(1==OWFirst())	//��һ��������ROM����
		{
		  memcpy(&ID_Buff[0][0],GetRomAddr(),8);//����ID���ݵ�IDbuff
		  
  		 for(temp1=1;temp1<DS18B20_NUM;temp1++)
		  {
		    
			if( OWSearch()==1)
			{
			 memcpy(&ID_Buff[temp1][0],GetRomAddr(),8);//����ID���ݵ�IDbuff
			}
			else
			{
			break;//û������
			} 

		  }
		}
		else{
		
		return;
		}


		temp[0]= '\t'; 

		temp[1]='T';
		temp[2]='i';
		temp[3]='m';
		temp[4]='e';
		temp[5]=':';
		temp[11]= '\t'; 

		temp[12]='T';
		temp[13]='e';
		temp[14]='m';
		temp[15]='p';
		temp[16]=':';

		temp[23]='\t';
		temp[20]='.' ;

		temp[27]='.' ;
		temp[30]= '\t';

		temp[31]= '\r';
		temp[32]= '\n';

		fistsech=1;
		}

		

  		//��ȡ2��ָ��ID���¶ȴ�������ֵ
		// tempA���ڶ���û�л��ģ�tempB��������һ��:-)
        temp1 = DS18B20_ReadDesignateTemper(ID_Buff[0]);  
   	//	temp2 = DS18B20_ReadDesignateTemper(ID_Buff[1]);
	  
		usSRegHoldBuf[23]= temp1;
		usSRegHoldBuf[24]= temp2;
		temp[6]=time_run/10000+0x30;
		temp[7]=time_run%10000/1000+0x30;
		temp[8]=time_run%10000%1000/100+0x30;
		temp[9]=time_run%10000%1000%100/10+0x30;
		temp[10]=time_run%10000%1000%100%10+0x30;
	
		
		
		temp[17]= temp1/10000+0x30;
		temp[18]= temp1%10000/1000+0x30;
		temp[19]= temp1%10000%1000/100+0x30;
	
		temp[21]= temp1%10000%1000%100/10+0x30;
		temp[22]= temp1%10000%1000%100%10+0x30;

		

		temp[24]=temp2/10000+0x30;
		temp[25]= temp2%10000/1000+0x30;
		temp[26]= temp2%10000%1000/100+0x30;
	
		temp[28]= temp2%10000%1000%100/10+0x30;
		temp[29]= temp2%10000%1000%100%10+0x30;
		
		


}	
uint32_t TempCreatTrm=0;
uint16_t Day=0,Count_mail=0;
uint8_t Assic[6]={0};
char mailsentbox[18];
void TemperatureThread(uint32_t Time)
{
	
   			 
if (Time - TempCreatTrm >= TEMP_CREATTRM_INTERVAL)
  {


    TempCreatTrm =  Time;

	DS1820main();//�ɼ�һ���¶����ݴ���TEMPA��B���Ѿ�ת��ΪASCII��ֱ��д���ļ�ϵͳ�м��ɡ���


  }

}

