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

int i=0; 
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
  int dty[] = {90, 120, 150, 180, 210, 240, 210, 180, 150, 120, 90};
  
  byte key=0xff;    
  char  txtbuf[16]="";      
  byte pluser = 0; 
  Init_Dev();  
  LCD_Init();   
  //LCD_P14x16Str(10,0,"龙丘智能科技"); 
  //LCD_P14x16Str(10,2,"龙丘智能"); 
  putstr("\nWWW.BJUT.EDU.CN\n");
  EnableInterrupts;   
  //for(PWMPER23 = 3320;PWMPER23 <= 3340;PWMPER23 += 10)
  //{
  PWMPER23 = 3333; 
    //for(i = 50;i <= 150;i += 1)    
      //{
    for(;;){
      
    for(i = 0; i < sizeof(dty)/sizeof(int); ++i) {
    
    
    //i = 60 +90    ;
    PWMDTY23 = dty[i];      
    RCV_CH=0XFF;
    sprintf(txtbuf,"PER23:%d ",PWMPER23);     
    LCD_P8x16Str(4,4,txtbuf);     
    //sprintf(txtbuf,"DTY7:%d ",PWMDTY7);     
    //LCD_P8x16Str(64,4,txtbuf);
    sprintf(txtbuf,"DTY23:%d ",PWMDTY23);     
    LCD_P8x16Str(4,6,txtbuf); 
    printp("\n PWMPER23: %d0  ",PWMPER23);
    //printp("\n PWMDTY7: %d0  \n",PWMDTY7);
    printp("\n PWMDTY23: %d0  \n",PWMDTY23);
      //PWMDTY23 = i;

      Dly_ms(500);
    }
   }
      //LCD_P14x16Str(10,0,"kkk"); 
      //}
  //}
}