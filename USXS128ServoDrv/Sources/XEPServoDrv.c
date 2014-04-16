/********************************************
龙丘智能车综合开发平台 
Designed by Chiu Sir
E-mail:chiusir@163.com
软件版本:V1.1
最后更新:2009年12年16日
相关信息参考下列地址:
网站:  http://www.lqist.cn
淘宝店:http://shop36265907.taobao.com
------------------------------------
电机及舵机控制程序操作: 
先插上MC33886模块，直流电机，舵机
1.按键功能
  1.1 UP     加速              
  1.2 DOWN   减速
  1.3 CANCEL 停止
  1.4 OK     停止
  1.5 LEFT   左转
  1.6 RIGHT  右转
  1.7 F1驱动电机正转
  1.8 F2驱动电机反转
2.测试过程
  2.1(调整VR6)确认舵机电压为7V以下;
  2.2(调整VR3)摄像头根据型号分别调整为10V(CMOS)/12V;
  2.3插上舵机，左右键可以调整左右转
  2.4插上电机MC33886驱动模块:
     F1:电机正转D11亮;
        UP:电压升高;
        DOWM:电压降低;
     F2:电机反转D10亮;
        UP:电压升高;
        DOWM:电压降低;  
  
*****************************************************************************************/
#include <hidef.h>           /* common defines and macros */
#include "derivative.h"
#include <stdio.h>
#include "LQprintp.h" 
#include "LQinit.h" 
#include "LQ12864.h"
#include "LQfun.h" 

 
char  tmch[16],RCV_CH=0xff;   
volatile byte  Brake_Flg=0;
//====================中断函数================================== 

#pragma CODE_SEG __NEAR_SEG NON_BANKED 
void interrupt 20 SCI0_ISR(void)
{       
  SCI0CR2_RIE=0;
  RCV_CH=uart_getkey();
  SCI0CR2_RIE = 1;
  PORTB_PB4=~PORTB_PB4;    
} 

//====================main()==================================
#pragma CODE_SEG DEFAULT     
void main(void)    
{
  int fenwei=0,gewei=0;  
  byte key=0xff;    
  char  txtbuf[16]="";      
    
  Init_Dev();  
  LCD_Init();   
  LCD_P14x16Str(10,0,"龙丘智能科技"); 
  //LCD_P14x16Str(10,2,"龙丘智能"); 
  putstr("\nWWW.BJUT.EDU.CN\n");
  Beep();
  EnableInterrupts;   
  for(;;)
  {     
    if((key=Scan_Key())!=0xff){
      RCV_CH=key;
      Beep();
      PORTB_PB0=~PORTB_PB0; 
    }    
    switch(RCV_CH)
    {       
      case KEY_F1:    
         Brake_Flg=0;        
      break;
      case KEY_F2:    
         Brake_Flg=1;        
      break;
      case KEY_UP:
        if(Brake_Flg){          
           if(PWMDTY6 >1) PWMDTY6--;                   
        }else{        
           if(PWMDTY7 >1) PWMDTY7--;  
        }        
      break;
      case KEY_DOWN:    
        if(Brake_Flg){          
           if(PWMDTY6 <24)  PWMDTY6++;                   
        }else{        
           if(PWMDTY7 <24) PWMDTY7++;  
        }   
      break;
      case KEY_LEFT:  
        if(PWMDTY01<2200) 
          PWMDTY01+=20;
      break;
      case KEY_RIGHT:    
        if(PWMDTY01>800) 
          PWMDTY01-=20; 
      break;
      case KEY_OK:    
           PWMDTY6 =25;
           PWMDTY7 =25;
           PWMDTY01=2200;    
      break;
      case KEY_ESC:    
           PWMDTY6 =25;
           PWMDTY7 =25;
           PWMDTY01=800;       
      break;                 
      default:
        PORTB_PB4=~PORTB_PB4;              
      break;
            
    }//switch(RCV_CH)
    RCV_CH=0XFF;
    sprintf(txtbuf,"DTY6:%d ",PWMDTY6);     
    LCD_P8x16Str(4,4,txtbuf);     
    sprintf(txtbuf,"DTY7:%d ",PWMDTY7);     
    LCD_P8x16Str(64,4,txtbuf);
    sprintf(txtbuf,"DTY01:%d ",PWMDTY01);     
    LCD_P8x16Str(4,6,txtbuf); 
    printp("\n PWMDTY6: %d0  ",PWMDTY6);
    printp("\n PWMDTY7: %d0  \n",PWMDTY7);
    printp("\n PWMDTY01: %d0  \n",PWMDTY01); 
    
    
    fenwei=ReadATD(2) *200/17;//参考电压5V, 500/255    
    gewei=fenwei/100;
    fenwei=fenwei%100;    
    sprintf(txtbuf,"电池:%d.%02dV",gewei,fenwei);
    LCD_Print(44,2,txtbuf);
    putstr(txtbuf);
    
    if((PWMDTY7 ==25)&&(PWMDTY6 ==25)) 
      LCD_P14x16Str(10,2,"停车");
    else{       
      if(Brake_Flg) LCD_P14x16Str(10,2,"倒车"); 
      else  LCD_P14x16Str(10,2,"前进");    
    }
    
    Dly_ms(40);
  }//for(;;)
} //main

