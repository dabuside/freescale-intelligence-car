/********************************************
�������ܳ��ۺϿ���ƽ̨ 
Designed by Chiu Sir
E-mail:chiusir@163.com
����汾:V1.1
������:2009��12��16��
�����Ϣ�ο����е�ַ:
��վ:  http://www.lqist.cn
�Ա���:http://shop36265907.taobao.com
------------------------------------
�����������Ƴ������: 
�Ȳ���MC33886ģ�飬ֱ����������
1.��������
  1.1 UP     ����              
  1.2 DOWN   ����
  1.3 CANCEL ֹͣ
  1.4 OK     ֹͣ
  1.5 LEFT   ��ת
  1.6 RIGHT  ��ת
  1.7 F1���������ת
  1.8 F2���������ת
2.���Թ���
  2.1(����VR6)ȷ�϶����ѹΪ7V����;
  2.2(����VR3)����ͷ�����ͺŷֱ����Ϊ10V(CMOS)/12V;
  2.3���϶�������Ҽ����Ե�������ת
  2.4���ϵ��MC33886����ģ��:
     F1:�����תD11��;
        UP:��ѹ����;
        DOWM:��ѹ����;
     F2:�����תD10��;
        UP:��ѹ����;
        DOWM:��ѹ����;  
  
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
//====================�жϺ���================================== 

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
  LCD_P14x16Str(10,0,"�������ܿƼ�"); 
  //LCD_P14x16Str(10,2,"��������"); 
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
    
    
    fenwei=ReadATD(2) *200/17;//�ο���ѹ5V, 500/255    
    gewei=fenwei/100;
    fenwei=fenwei%100;    
    sprintf(txtbuf,"���:%d.%02dV",gewei,fenwei);
    LCD_Print(44,2,txtbuf);
    putstr(txtbuf);
    
    if((PWMDTY7 ==25)&&(PWMDTY6 ==25)) 
      LCD_P14x16Str(10,2,"ͣ��");
    else{       
      if(Brake_Flg) LCD_P14x16Str(10,2,"����"); 
      else  LCD_P14x16Str(10,2,"ǰ��");    
    }
    
    Dly_ms(40);
  }//for(;;)
} //main

