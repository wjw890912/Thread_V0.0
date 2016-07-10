#include <rtthread.h> 
#include<math.h>  
#include <stdio.h>
#include "NUC472_442.h"
#include "ds12864.h"


#ifdef BT_BOARD

#define SID_UP		 PF7=1
#define SID_DOWN	 PF7=0
#define SCK_UP		 PF6=1
#define SCK_DOWN	 PF6=0

#else

#define SID_UP		 PC8=1
#define SID_DOWN	 PC8=0
#define SCK_UP		 PC7=1
#define SCK_DOWN	 PC7=0

#endif

#define  clear   0x01  //����
#define  reset_DDRAM  0x02 //DDRAM��ַ��λ
#define  left_move  0x04 //�α�����
#define  right_move  0x06 //�α�����
#define  all_left_move 0x05 //������������
#define  all_right_move 0x07  //������������
#define  display_left_move  0x10  //��ʾ�α�����
#define  display_right_move  0x14  //��ʾ�α�����
#define  set_function1   0x30  //����ָ�����
#define  set_CGRAM    0x40  //�趨CGRAM��ַ
#define  set_DDRAM    0x80  //�趨DDRAM��ַ
#define  set_function2   0x34  //����ָ�����
#define  fanbai            0x04   //���׵�һ��(����ָ�)
#define  set_GDRAM    0x80  //�趨GDRAM��ַ(����ָ�)
#define  ON_G     0x36  //����ͼ��ʾ(����ָ�)
#define  set_function2   0x34  //�ػ�ͼ��ʾ(����ָ�)

/**************************************/
#define  uchar    unsigned char
#define  uint     unsigned int 
extern uint32_t LocalTime ;
 uchar  a[]={"QC12864"};      //����Ҫ��ʾ���ַ���

 



 void Init12864(void)
 {
 	#ifdef BT_BOARD

	GPIO_SetMode(PF, BIT6, GPIO_MODE_OUTPUT); //LED_CLK
  	GPIO_SetMode(PF, BIT7, GPIO_MODE_OUTPUT); //LCD_DAT

	#else
 	GPIO_SetMode(PC, BIT8, GPIO_MODE_OUTPUT); //LCD_DAT
  	GPIO_SetMode(PC, BIT7, GPIO_MODE_OUTPUT); //LED_CLK
 	#endif
 }
 





/**************************************/
//��ʱ����
/**************************************/
void Delay_nms(uchar n)
{
 	uint32_t trm=1000;//LocalTime;//get now systick times
		trm=n*trm;
	 do
	 {
	  //while(LocalTime -trm < 10);	//��ʱ
	 }while(trm--); 

}
/**************************************/
//���з���һ���ֽ�
/**************************************/
void LCD_sendbyte(uchar byte)
{
   uchar i;
   for(i=0;i<8;i++)
    {
   SCK_DOWN;               //����ʱ����

  // DrvSYS_Delay(1);//10US
   if(byte&0x80)
   {
   	SID_UP;
   
   }
   else
   {
   	SID_DOWN;

   }
   SCK_UP;               //�����ط�������
   byte=byte<<1;                  //����һλ
 }
}

/****************************************/
//дָ��
/****************************************/
void LCD_write_com(uchar com)
{
        
   LCD_sendbyte(0xf8);       //����5�������ġ�1��������һ������,11111,RW(0),RS(0),0
   LCD_sendbyte(0xf0&com);       //ȡ����λ�����ݷ����δ��ͣ�
                                 //ÿ���ֽڵ����ݱ����������ֽ�
               //����λ���ڵ�һ���ֽڵĸ���λ
   LCD_sendbyte(0xf0&(com<<4));        //����λ���ڵڶ����ֽڵĸ���λ 
           
  Delay_nms(1);             //���в�֧�ֶ����������ɼ��æ��������������ʱ���   
}

void LCD_write_com_clr(uchar com)
{
        
   LCD_sendbyte(0xf8);       //����5�������ġ�1��������һ������,11111,RW(0),RS(0),0
   LCD_sendbyte(0xf0&com);       //ȡ����λ�����ݷ����δ��ͣ�
                                 //ÿ���ֽڵ����ݱ����������ֽ�
               //����λ���ڵ�һ���ֽڵĸ���λ
   LCD_sendbyte(0xf0&(com<<4));        //����λ���ڵڶ����ֽڵĸ���λ 
           

}

/******************************************/
//д����
/******************************************/
void LCD_write_dat(uchar dat)
{
            
   LCD_sendbyte(0xfa);        //����5�������ġ�1��������һ������,11111,RW(0),RS(1),0
   LCD_sendbyte(0xf0&dat);       //ȡ����λ�����ݷ����δ��ͣ�
                                 //ÿ���ֽڵ����ݱ����������ֽ�
               //����λ���ڵ�һ���ֽڵĸ���λ
   LCD_sendbyte(0xf0&(dat<<4));        //����λ���ڵڶ����ֽ�
           
   Delay_nms(10);
}
/********************************************/
//LCD��ʼ��
/********************************************/
void LCD_init(void)
{  
  Init12864();     
 LCD_write_com(0x30);             //ѡ�����ָ�   
 LCD_write_com(0x0c);       //����ʾ�����α꣬������
 LCD_write_com(0x01);       //�����ʾ��Ļ����DDRAMλַ����������Ϊ00H
 Delay_nms(5);         //��������ʱ��ϳ�1.6ms ��˼Ӵ���ʱ
 LCD_write_com(0x02);       //��DDRAMλַ��λ,�˴�ò���������ظ�
 LCD_write_com(0x06);       //�趨������ƣ�������ʾ���ƶ�
}
/*************************************************/
//��ʾ�ַ���
/*************************************************/
void print(uchar *s)
{ 
    while(*s!='\0')
    {  
       LCD_write_dat(*s);
       s++;     
    }
}
/***************************************************/
//������ʾ��ַ
/***************************************************/
void LCD_Setaddress(uchar x,uchar y)
{              //��ַ�ӵ�1�е�1�п�ʼ����0��ʼ
  uchar addr;
  switch(x)
  {
   case 1: addr=0x80+y-1;
           break;
   case 2: addr=0x90+y-1;
           break;
   case 3: addr=0x88+y-1;
           break;
   case 4: addr=0x98+y-1;
           break;
   default : break;
  }
   LCD_write_com(addr);    //�ַ���ʾ��ʼ��ַ
}
/*****************************************************/
//���ַ�����ʾ�ڹ̶�λ��
/*****************************************************/
void LCD_Putstring( uchar x, uchar y, uchar *pData )
{
 LCD_Setaddress(x,y);
 while( *pData != '\0' )
 {
   LCD_write_dat( *pData++ );
 }
}

/*---------------------------------------------------------------------------------------------------------------------- */
//����ͼ������������Ļ�ϴ�ϡ��ļ����㣬�������ں����������
void LCD_draw_point(uchar x, uchar y) 
 {
   uchar x_byte, x_bit;         //�ں��������һ���ֽڣ���һ��λ
   uchar y_byte, y_bit;    //�����������һ���ֽڣ���һ��λ
   x_byte=x/16;                   //���������һ���ֽ�(��ַ)
                                      //ע��һ����ַ��16λ��
   x_bit=x%16;                    //(ȡģ)���������һ��λ
   y_byte=y/32;                    //y��û���ĸ��ֽ����˵��
                             //����ֻ��ȷ�������ϰ���(32��Ϊһ��)�����°���
                                     //0:�ϰ��� 1:�°���
   y_bit=y%32;                    //y_bitȷ�������ڵڼ���
   LCD_write_com(0x34);         //����չָ�
  
 LCD_write_com(0x80+y_bit);       //��ֱ��ַ(��)   ò����˵���������෴
   LCD_write_com(0x80+x_byte+8*y_byte);  //��дˮƽ���꣨�£�   ò����˵���������෴    ???????
 
                                      //������������ֲ� 
                                               //�°�����ˮƽ������ʼ��ַΪ0x88
                                     //(+8*y_byte)��������ȷ�����ϰ��������°���
   if(x_bit<8)                              //���x_bitλ��С��8
   {
       LCD_write_dat(0x01<<(7-x_bit));    //д���ֽڡ���Ϊ�����Ǵ������ҵ�
                                                //��GDRAM��λ���󣬵�λ����
    LCD_write_dat(0x00);                   //���ֽ�ȫ����0
   }
   else
   {
       LCD_write_dat(0x00);                   //���ֽ�ȫ����0
       LCD_write_dat(0x01<<(15-x_bit));
   } 
   LCD_write_com(0x36);                     //�򿪻�ͼ��ʾ
   LCD_write_com(0x30);               //�ص�����ָ�
 }

/************************************/
 //����ͼ  һ�δ�ˮƽһ��     ���Ա���ϵ�����
 //x��ʾ������׵�ַ��y��ʾ�������ֵ��Ҳ���Ǳ�ʾ�ڶ�����
//����һ�������е������������Ĵ���
//������飬��Ĭ������Ϊһ�����ļ�¼��128�ֽڣ�ֻҪ��������������y���Ͱѵ�y�е���ȫ�����
/************************************/
void LCD_draw_word(uchar *x, uchar y) 
 {
  uchar i,j,k,m,n,count=0;
   uchar hdat, ldat;         
   uchar y_byte, y_bit;    //�����������һ���ֽڣ���һ��λ
    uchar   a[16];
   LCD_write_com(0x34);         //����չָ�
    y_byte=y/32;                    //y��û���ĸ��ֽ����˵��
    y_bit=y%32;                    //y_bitȷ�������ڵڼ���
   for(j=0;j<8;j++)
 {
  hdat=0, ldat=0;     //�˴������Ǻܱ�Ҫ��
  n=j*16;
  for(k=n;k<n+16;k++)
   {
   if(x[k]==y)
      {
    a[count]=k;
    count++;
    }
    } 
 for(m=0;m<count;m++)
   {
    i=a[m]-n;
       if(i<8)                              //���x_bitλ��С��8
          hdat=hdat|(0x01<<(7-i));    //д���ֽڡ���Ϊ�����Ǵ������ҵ�
       else
        ldat=ldat|(0x01<<(15-i));
      }
   
   LCD_write_com(0x80+y_bit);       //��ֱ��ַ(��)   ò����˵���������෴
     LCD_write_com(0x80+j+8*y_byte);  //ˮƽ���꣨�£�   ò����˵���������෴   
   LCD_write_dat(hdat);
    LCD_write_dat(ldat);
  }
   LCD_write_com(0x36);                     //�򿪻�ͼ��ʾ
   LCD_write_com(0x30);               //�ص�����ָ�
 }
/**********************************************************/
//��ͼ�γ���
/**********************************************************/
void LCD_draw_clr(void)
{ 
uchar i,j;
     LCD_write_com(0x34);      //8Bit����ָ�,��ʹ��36HҲҪд����
    LCD_write_com(0x36);      //��ͼON,����ָ�����36H���ܿ���ͼ 
     for(i=0;i<32;i++)            //12864ʵ��Ϊ256x32
     {
           LCD_write_com(0x80+i);      //��λ��    ò����˵���������෴ ���ϣ�
           LCD_write_com(0x80);      //��λ��     ò����˵���������෴    ���£�
           for(j=0;j<32;j++)            //256/8=32 byte
     LCD_write_dat(0);
  }
  LCD_write_com(0x30);     //������ָ�
}

/*----------------------------------------------------------------------------------------------------------------------*/
/******************************************************/
//�����Ҳ��Ĳ���
/******************************************************/
void print_sinx(void)
{
 uchar i;
 uchar y_sin[128];   //������Ļ��Ҫ������Ҳ���������
   uchar  v[128]={32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,};
 float y;
 for(i=0;i<128;i++)
 {
  y=31*sin(0.09*i)+0.5;   //�˴�ϵ��Ϊ31�ȽϺ�
   y_sin[i]=32-y;
 }
  for(i=0;i<64;i++)     
 LCD_draw_word(y_sin, i);   //��ͼ  һ��һ�л�
  LCD_draw_word(v, 32);
  
}

/******************************************************/
//������
//���ڹۿ���ʾЧ��
/******************************************************/
void lcd_main(void)
{ 

 LCD_init();  
 LCD_Setaddress(2,3);
 print("����ʹ��");   
 LCD_Putstring(3,3,a);
// LCD_write_dat(0x35);  
// LCD_draw_clr();
 print_sinx(); 
}

static char VRMS_ds12864[22]="��ѹ :000.00";
static char IRMS_ds12864[22]="���� :111.11";
static char WATT_ds12864[22]="���� :222.22";
static char WATTHR_ds12864[22]="���� :222.22";
#define Start 6
#define End 10
#define Start_len 2
#define End_len 1
/*
The task must keep some "ms" updata once again and again ..

   			100-500MS
*/
void Display_Task(void)
{
   
   LCD_Setaddress(1,2);
   print(VRMS_ds12864); 
   LCD_Setaddress(2,2);
   print(IRMS_ds12864); 
   LCD_Setaddress(3,2);
   print(WATT_ds12864);
   LCD_Setaddress(4,2);
   print(WATTHR_ds12864);

}



void Set_Vrms2Display(int Vrms)
{
	  static char d=0,b=0;
  d=sprintf(&VRMS_ds12864[Start],"%d",Vrms);  	
   if(d!=b)
   {	 b=d;
   		LCD_write_com(1); 
   }

}
void Set_Irms2Display(int Irms)
{	 static char d=0,b=0;
   d=sprintf(&IRMS_ds12864[Start],"%d",Irms);
   if(d!=b)
   {	 b=d;
   		LCD_write_com(1); 
   }
}
void Set_Watt2Display(int Watt)
{	 static char d=0,b=0;

   d=sprintf(&WATT_ds12864[Start],"%d",Watt);
   if(d!=b)
   {	 b=d;
   		LCD_write_com(1); 
   }
}

void Set_Watthr2Display(int Watthr)
{	 static char d=0,b=0;

   d=sprintf(&WATTHR_ds12864[Start],"%d",Watthr);
   if(d!=b)
   {	 b=d;
   		LCD_write_com(1); 
   }
}

	