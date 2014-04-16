/********************************************
�������ܳ��ۺϿ���ƽ̨ 
Designed by Chiu Sir
E-mail:chiusir@163.com
����汾:V1.1
������:2009��4��18��
�����Ϣ�ο����е�ַ��
���ͣ�  http://longqiu.21ic.org
�Ա��꣺http://shop36265907.taobao.com
********************************************/ 
#include "derivative.h"

//-----------------------------------------------------
static void SCI_Init(void) 
{
    SCI0CR2=0x2c; //enable Receive Full Interrupt,RX enable,Tx enable      
    SCI0BDH=0x01;
    SCI0BDL=0xa0; //SCI0BDL=busclk/(16*SCI0BDL)
                  //busclk 8MHz,  9600bps,SCI0BDL=0x340
                  //busclk 24MHz, 9600bps,SCI0BDL=0x9C
}                 //busclk 32MHz, 9600bps,SCI0BDL=0xD0
                  //busclk 64MHz, 9600bps,SCI0BD=0x1a0
//----------------------------------------------------- 
static void Port_Init(void)
{  
    DDRA = 0xff;  //LCD1100,PA0--4,PA67 D1D2
    PORTA= 0x00;   
       
    DDRB = 0xff;  //LED  PTB0--7,
    PORTB= 0xff;  //LEDs on  
   
    DDRE = 0xFF;  //MOTOR CONTROL
    PORTE= 0x00;  //P      
    
    DDRH = 0x00; // PORTH input
    PTIH = 0X00; // KEY,PH0--5
    PERH = 0xff; // PORTH pull up  
    PPSH = 0x00; // Port H Polarity Select Register-falling edge
    PIEH = 0x00; // PORTH interrut disable,            
    
    
    DDRM = 0XFF; // motor2������ģ��MC33886�ĳ�ʼ���ӳ���
    PTIM = 0X01; // EN:PM0=0,operating run;
    PERM = 0xff; // EN:PM0=1,full run;        

}
//----------------------------------------------------- 
static void PWM_Init(void)
{     
  //pwm0.pwm1
    PWME = 0;
    PWMCTL_CON23=1;		  //0��1���ϳ�16λPWM��
    PWMCAE=0;			//ѡ�����ģʽΪ��������ģʽ
    PWMCNT23 = 0;				//���������㣻
    PWMPOL_PPOL3=1;			//������ߵ�ƽ��������DTYʱ����ת��ƽ
    PWMPRCLK = 0X50;    //clockA����Ƶ,clockA=busclock=32MHz;CLK B 32��Ƶ:1Mhz   
    PWMSCLA = 0x10;     //��clock SA ����2*PWMSCLA=32��Ƶ��pwm clock=clockA/32=1MHz;     
    PWMCLK_PCLK3 = 1;   //ѡ��clock SA��ʱ��Դ
    PWMPER23 = 50000;   //����20ms�� 50Hz;(����ʹ�õķ�Χ��50-200hz)
    PWMDTY23 =  0;   //�ߵ�ƽʱ��Ϊ1.5ms;  
    PWME_PWME3 = 1;
    
    //SA,A for ch2367
    //SB,B for ch0145
    
    //PWMSCLB=0X05;     //clk SB=clk B/(2*pwmsclb):75KHZ
    PWMSCLB=0X04;       //clk SB=clk B/(2*pwmsclb)=125KHZ 
    /*   
    //pwm3; 
    PWMCNT3 = 0;
    PWMCAE_CAE3=0;      //ѡ�����ģʽΪ��������ģʽ
    PWMPOL_PPOL3=0;                      
    PWMPER3 =125;       //����������Ƶ��Ϊ1KHZ 
    PWMDTY3 =100;       //duty circle=(PWMPER3-PWMDTY3)/PWMPER3=20%
    PWMCLK_PCLK3 = 1;                   
    PWME_PWME3 = 0;
       */
    //PWM6,IN1�����������
    PWMCNT6 = 0;
    PWMCAE_CAE6=0;      //ѡ�����ģʽΪ��������ģʽ
    PWMPOL_PPOL6=0;                      
    PWMPER6 =25;        //��������CLOCK��Լ125K--5.000KHz,75K--3K=clk B/PWMPER2
    PWMDTY6 =25;        //duty circle=(PWMPER2-PWMDTY2)/PWMPER2=0
    PWMCLK_PCLK6 = 1;   //1 = Clock SB is the clock source for PWM channel 2.                
    PWME_PWME6 = 1;     //�����ֹ����PWM2����ߵ�ƽ����ζ��MC33886ȫ������
    
    //PWM7,IN2�����������
    PWMCNT7 = 0;   
    PWMCAE_CAE7=0;      
    PWMPOL_PPOL7=0;                      
    PWMPER7 =25;      
    PWMDTY7 =25;       
    PWMCLK_PCLK7 = 1;   
    PWME_PWME7 = 1;     
}
void AD_Init(void) 
{  
  ATD0CTL1=0x00;   //7:1-�ⲿ����,65:00-8λ����,4:�ŵ�,3210:ch
  ATD0CTL2=0x40;   //��ֹ�ⲿ����, �жϽ�ֹ    
  ATD0CTL3=0xC0;   //�Ҷ����޷���,ÿ��ת��8������, No FIFO, Freezeģʽ�¼���ת    
  ATD0CTL4=0x01;   //765:����ʱ��Ϊ4��ADʱ������,ATDClock=[BusClock*0.5]/[PRS+1]
  ATD0CTL5=0x30;   //6:0����ͨ����ֹ,5:1����ת�� ,4:1��ͨ����������
  ATD0DIEN=0x00;   //��ֹ�������� 
} 

// PLL��ʼ���ӳ���   BUS Clock=32M
void SetBusCLK_32M(void)
{   
    CLKSEL=0X00;				// disengage PLL to system
    PLLCTL_PLLON=1;			// turn on PLL
    SYNR =0x40 | 0x03;  // pllclock=2*osc*(1+SYNR)/(1+REFDV)=64MHz;                      
    REFDV=0x80 | 0x01; 
    POSTDIV=0x00;  
    _asm(nop);          // BUS CLOCK=32M
    _asm(nop);
    while(!(CRGFLG_LOCK==1));	  //when pll is steady ,then use it;
    CLKSEL_PLLSEL =1;		        //engage PLL to system; 
}

//-----------------------------------------------------
#pragma CODE_SEG DEFAULT
void Init_Dev(void)
{
    SetBusCLK_32M();
    Port_Init();
    SCI_Init();
    PWM_Init();
    AD_Init();    
}

//----------------------------------------------------- 