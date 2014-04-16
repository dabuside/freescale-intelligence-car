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

int i=0; 
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
  int dty[] = {90, 120, 150, 180, 210, 240, 210, 180, 150, 120, 90};
  
  byte key=0xff;    
  char  txtbuf[16]="";      
  byte pluser = 0; 
  Init_Dev();  
  LCD_Init();   
  //LCD_P14x16Str(10,0,"�������ܿƼ�"); 
  //LCD_P14x16Str(10,2,"��������"); 
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