


#ifndef __DS12864_H
#define __DS12864_H

void LCD_init(void);
void LCD_draw_clr(void);//����
void LCD_Setaddress(unsigned char x,unsigned char y);//������ʾ��ַ
void lcd_main(void);
void Display_Task(void);//ˢ��
void Set_Vrms2Display(int Vrms);
void Set_Irms2Display(int Irms);
void Set_Watt2Display(int Watt);
void Set_Watthr2Display(int Watthr);
void LCD_write_com_clr(unsigned char com);
#endif


