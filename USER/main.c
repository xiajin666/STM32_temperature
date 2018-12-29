#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "ds18b20.h" 
#include "exti.h"
#include "qjbl.h"
#include "text.h"
#include "image2lcd.h"
#include "wdg.h"
/************************************************
 ALIENTEK战舰STM32开发板实验30
 DS18B20数字温度传感器 实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
extern const u8 gImage_shiyanshi[];//图片数据(包含信息头),存储在image1.c里面.
extern const u8 gImage_image[];

 int main(void)
 {	 
	u8 i,y_d,t=0;	
	u16 color[4]={BLUE,YELLOW,BLACK,YELLOW};
	 
 	 
	short temperature;    	   

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为 115200
	LED_Init();		  		//初始化与LED连接的硬件接口
	LCD_Init();			   	//初始化LCD  
	EXTIX_Init();	
	KEY_Init();
	IWDG_Init(4,1250);    //与分频数为64,重载值为625,溢出时间为1s	
 	POINT_COLOR=BLUE;		//设置字体为红色 
	//LCD_ShowString(30,50,200,16,16,"WarShip STM32");	
	//LCD_ShowString(30,70,200,16,16,"DS18B20 TEST");	
	

	 for(y_d=255;y_d>=10;y_d=y_d-2)
	 {
		 LCD_Clear(WHITE);
		 image_display(40+10,y_d,(u8*)gImage_shiyanshi);
		 LCD_Show_Font(50+52,y_d+80,3);
		 LCD_Show_Font(74+52,y_d+80,4);
		 LCD_Show_Font(50+52,y_d+104,5);
		 LCD_Show_Font(74+52,y_d+104,6);
		 IWDG_Feed();
		 delay_ms(100);
	 }
	image_display(40+10,150,(u8*)gImage_image);
	 IWDG_Feed();
	 
	 for(i=0;i<=5;i++)
	 {
		 IWDG_Feed();
		 LCD_Show_Font(50+52,y_d+80,5);
		 LCD_Show_Font(74+52,y_d+80,6);
		 LCD_Show_Font(50+52,y_d+104,3);
		 LCD_Show_Font(74+52,y_d+104,4);
		 IWDG_Feed();
		 delay_ms(5000);
		 LCD_Show_Font(50+52,y_d+80,3);
		 LCD_Show_Font(74+52,y_d+80,4);
		 LCD_Show_Font(50+52,y_d+104,5);
		 LCD_Show_Font(74+52,y_d+104,6);
		 IWDG_Feed();
		 delay_ms(5000);
		 IWDG_Feed();
	 }

		
	//LCD_Show_Font(98,10,2);	 
	//在指定地址显示图片

	 /*
	while(1) 
	{		   
		if(i==0)
		{
			image_display(40,40,(u8*)gImage_image);//在指定地址显示图片
		}
		i++;
		if(i>10)i=0; 
		LED0=!LED0;					 
		delay_ms(200);	
	}											    

*/
	LCD_Clear(WHITE);
	image_display(40+10,y_d,(u8*)gImage_shiyanshi);
	LCD_Show_Font(50,275,3);
  LCD_Show_Font(74,275,4);
	POINT_COLOR=RED;
	LCD_ShowString(30,110,200,16,16,"2018/12/27");		
	
 	while(DS18B20_Init())	//DS18B20初始化	
	{
		LCD_ShowString(30,130,200,16,16,"DS18B20 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}								   
	LCD_ShowString(30,130,200,16,16,"DS18B20 OK");
	POINT_COLOR=BLUE;//设置字体为蓝色 
 	LCD_ShowString(30,150,200,16,16,"Temp:   . C");	 
	while(1)
	{	    	    
 		if(t%10==0)			//每100ms读取一次
		{									  
			temperature=DS18B20_Get_Temp();	
			LCD_Fill(200,10,220,300,WHITE);
			LCD_Fill(200,300-temperature%1000,220,300,RED);
			
			if(temperature/10>=wd_flag)   //温度标志
			{
				LCD_ShowString(30,90,200,16,16,"warning");
				GPIO_ResetBits(GPIOE,GPIO_Pin_5);	
				
			}
			else 
			{
				
				LCD_ShowString(30,90,200,16,16,"normal");
				GPIO_SetBits(GPIOE,GPIO_Pin_5);
				
			};
			LCD_ShowString(30,170,200,16,16,"SetTemp:   C");	
			LCD_ShowNum(30+40+32,170,wd_flag,2,16);
			if(wd_flag>30||wd_flag<0)wd_flag=19;
			if(temperature<0)
			{
				LCD_ShowChar(30+40,150,'-',16,0);			//显示负号
				temperature=-temperature;					//转为正数
			}else LCD_ShowChar(30+40,150,' ',16,0);			//去掉负号
			LCD_ShowNum(30+40+8,150,temperature/10,2,16);	//显示正数部分	    
   			LCD_ShowNum(30+40+32,150,temperature%10,1,16);	//显示小数部分 		   
		}				   
	 	delay_ms(10);
		IWDG_Feed();
		t++;
		if(t==20)
		{
			t=0;
			LED0=!LED0;
		}
	}
}

