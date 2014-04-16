/********************************************
龙丘智能车综合开发平台 
Designed by Chiu Sir
E-mail:chiusir@163.com
软件版本:V1.1
最后更新:2009年4月18日
相关信息参考下列地址：
博客：  http://longqiu.21ic.org
淘宝店：http://shop36265907.taobao.com
********************************************/ 
#include "derivative.h"
#include <stdio.h>
#include "LQ12864.h"
#include "LQfun.h"
//#include "printp.h"

//extern U32 rst_time;
extern word SpdNum;
extern byte MaxSPD; 
//----------------------------------------------------- 
void Dly_ms(int ms)
{
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
     //for(jj=0;jj<1335;jj++);  //16MHz--1ms
     //for(jj=0;jj<4006;jj++);  //48MHz--1ms 
     for(jj=0;jj<5341;jj++);    //64MHz--1ms  
} 
//-----------------------------------------------------  

//----------------------------------------------------- 
//======================================================
void BeepHz(word hz)
{
  word tmhz=0;
  tmhz=12500/hz;
  PWME_PWME3 = 1;
  if((hz<200)&&(hz>49))
  {    
    PWMPER3 =tmhz*10;         
    PWMDTY3 =tmhz*9; 
  }  
  Dly_ms(40);
  PWME_PWME3 = 0;
  PWMPER3 =125;         
  PWMDTY3 =100; 
}
void Beep(void)
{   
  PWME_PWME3 = 1;
  PWMPER3 =125;         
  PWMDTY3 =100;   
  Dly_ms(10);
  PWME_PWME3 = 0;  
} 
//----------------------------------------------------- 
byte Scan_Key(void)
{  
  Dly_ms(10);
       if(PTIH_PTIH7==0) return 'o'; // OK
  else if(PTIH_PTIH5==0) return 'd'; // DWON 
  else if(PTIH_PTIH6==0) return 'c'; // ESC
  else if(PTIH_PTIH4==0) return 'r'; // RIGHT
  else if(PTIH_PTIH3==0) return 'l'; // LEFT
  else if(PTIH_PTIH2==0) return 'u'; // UP
  else if(PTIH_PTIH1==0) return 'b'; // F2
  else if(PTIH_PTIH0==0) return 'a'; // F1
  else return 0xff;
}

//----------------------------------------------------------
//刹车
void Brake(word ms)
{        
   PWMDTY7=25;
   PWMDTY6=4;
   ms=(ms>10)?10:ms;
   Dly_ms(ms*10+20);//刹车时间为多少比较合适   
   PWMDTY6=25;
   PWMDTY7=3;
           
}
byte ReadATD(byte ch)
{
  byte ad=0;  
  while(!ATD0STAT0_SCF);
  switch(ch)
  {
    default:
    case 0:
      ad= ATD0DR0L;
    break;
    case 1:
      ad= ATD0DR1L;
    break;
    case 2:
      ad= ATD0DR2L;
    break;
    case 3:
      ad= ATD0DR3L;
    break;
    case 4:
      ad= ATD0DR4L;
    break;
    case 5:
      ad= ATD0DR5L;
    break;
    case 6:
      ad= ATD0DR6L;
    break;
    case 7:
      ad= ATD0DR7L;
    break;   
  }
  return ad;
}