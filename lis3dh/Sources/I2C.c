#include "I2C.h"
#include "time.h"
byte  ack;           /*应答标志位*/
//因为是50KHz的时钟，每个周期为20us

void DogHello(void){
    SRS=0X55;
    SRS=0XAA;   
}

/*********************延时函数xus***********************************/

void delay1us(void)
{
//Nop 0.18us
//_asm NOP;_asm NOP;_asm NOP;_asm NOP;_asm NOP;_asm NOP;
delayxus(1);
}
void delayxus(byte x)
{
//总线频率5.5296  T=0.18us  1uS=6*T
//(6*NOP+DECA+BNE)*N+LDA+RTS
	asm
		{
			LDA		x   ;2T
		DELAY_ASM:
			DECA	;1T
			NOP		;1T
			NOP
			BNE		DELAY_ASM;3T  
		}
}       
void delayx100us(int x) {
  for (;x--;x>0){
    if (x%1000==0) DogHello();
    delayxus(100); 
  }
}
void delayxms(int x){
    for (;x--;x>0){
        if (x%100==0) DogHello();
        delayx100us(10);   
    }
}

void SDA_HIGH(){
        SDA_3V3=1;
}
void SDA_LOW(){
        SDA_3V3=0;
}
void SCL_HIGH(){
        SCL_3V3=1;
}
void SCL_LOW(){
        SCL_3V3=0;
}

/*******************************************************************
                     起动总线函数               
函数原型: void  Start_I2c();  
功能:     启动I2C总线,即发送I2C起始条件.  
********************************************************************/
void Start_I2c()
{ /*发送起始条件的数据信号*/
  //if (is_3V3==1) SDA_3V3=1; else if( is_3V3==0) SDA_5V=1;
  SDA_HIGH();
  delayxus(5);//1
  //if (is_3V3==1) SCL_3V3=1; else if( is_3V3==0) SCL_5V=1;
  SCL_HIGH;
  delayxus(20);//5/*起始条件建立时间大于4.7us,延时*/
  /*发送起始信号*/
  //if (is_3V3==1)  SDA_3V3=0; else if( is_3V3==0) SDA_5V=0;
  SDA_LOW();
  delayxus(20);//5/* 起始条件锁定时间大于4μs*/
  /*钳住I2C总线，准备发送或接收数据 */
  //if (is_3V3==1)  SCL_3V3=0; else if( is_3V3==0) SCL_5V=0;
  SCL_LOW();
  delayxus(20);//5//delay10us();

}

/*******************************************************************
                      结束总线函数               
函数原型: void  Stop_I2c();  
功能:     结束I2C总线,即发送I2C结束条件.  
********************************************************************/
void Stop_I2c()
{
  /*发送结束条件的数据信号*/
  //if (is_3V3==1)  SDA_3V3=0; else if( is_3V3==0) SDA_5V=0;
  SDA_LOW();
  delayxus(5);//1    /*发送结束条件的时钟信号*/
  /*结束条件建立时间大于4μs*/
  //if (is_3V3==1)  SCL_3V3=1; else if( is_3V3==0) SCL_5V=1;
  SCL_HIGH();
  delayxus(20);//5//delay25us();
  /*发送I2C总线结束信号*/
  //if (is_3V3==1)  SDA_3V3=1; else if( is_3V3==0) SDA_5V=1;
  SDA_HIGH();
  delayxus(20);//5//delay25us();
}


/*******************************************************************
                 字节数据发送函数               
函数原型: void  SendByte(uchar c);
功能:     将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
          此状态位进行操作.(不应答或非应答都使ack=0)     
          发送数据正常，ack=1; ack=0表示被控器无应答或损坏。
********************************************************************/
void  SendByte(unsigned char c)
{
	 unsigned char BitCnt;
	 unsigned char errCnt=100;
	 
	 for(BitCnt=0;BitCnt<8;BitCnt++)  /*要传送的数据长度为8位*/
     {
		if((c<<BitCnt)&0x80){
			/*判断发送位*/
			SDA_HIGH();
		}
		else  {
			SDA_LOW();
		}
		delayxus(5);//1
		/*置时钟线为高，通知被控器开始接收数据位*/
		SCL_HIGH();
		delayxus(20);//5 		/*保证时钟高电平周期大于4μs*/
		SCL_LOW();
     }
     DogHello();
     delayxus(20);//5//delay10us();
     /*8位发送完后释放数据线，准备接收应答位*/
     SDA_HIGH();
     delayxus(20);//5//delay10us();
	 SCL_HIGH();
     delayxus(20);//5;//delay10us();
    
     SDA_3V3_D=0;

    do
	{
		errCnt--;
		if(SDA_3V3==1) 
		{
			ack=0;  
		}
		else 
		{
			ack=1;        /*判断是否接收到应答信号*/
		}
	}
	while((errCnt!=0)&&(ack==0));
	 SDA_3V3_D=1;
 	 
	 SCL_LOW();
     delayxus(20);//5//delay10us();
}
/*******************************************************************
                 字节数据接收函数               
函数原型: uchar  RcvByte();
功能:     用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
          发完后请用应答函数应答从机。  
********************************************************************/ 
unsigned char  RcvByte()
{
    unsigned char retc;
    unsigned char BitCnt;

    retc=0; 
    /*置数据线为输入方式*/
    //if (is_3V3==1)  SDA_3V3=1; else if( is_3V3==0) SDA_5V=1;
    SDA_HIGH();

    
    SDA_3V3_D=0;  

    for(BitCnt=0;BitCnt<8;BitCnt++)
    {
        delay1us();           
        /*置时钟线为低，准备接收数据位*/
        //if (is_3V3==1)  SCL_3V3=0; else if( is_3V3==0) SCL_5V=0;
        SCL_LOW();
        delayxus(5);				/*时钟低电平周期大于4.7μs*/
        /*置时钟线为高使数据线上数据有效*/
        //if (is_3V3==1)  SCL_3V3=1; else if( is_3V3==0) SCL_5V=1;
        SCL_HIGH();
        delayxus(5);
        delayxus(5);
        retc=retc<<1;

        if(SDA_3V3==1)retc=retc+1;  /*读数据位,接收的数据位放入retc中 */

        delayxus(5);
        delayxus(5); 
    }

    SDA_3V3_D=1;  

    //if (is_3V3==1)  SCL_3V3=0; else if( is_3V3==0) SCL_5V=0;
    SCL_LOW();
    delayxus(5);
    delayxus(5);
    return(retc);
}

/********************************************************************
                     应答子函数
函数原型:  void Ack_I2c(bit a);
功能:      主控器进行应答信号(可以是应答或非应答信号，由位参数a决定)
********************************************************************/
void Ack_I2c(byte a)
{
  
  if(a==0){
  	/*在此发出应答或非应答信号 */
	//if (is_3V3==1)  SDA_3V3=0; else if( is_3V3==0) SDA_5V=0;
	SDA_LOW();
  }
  else {
	//if (is_3V3==1)  SDA_3V3=1; else if( is_3V3==0) SDA_5V=1;
	SDA_HIGH();
  }
  delay1us();
  delay1us();
  delay1us();      
  //if (is_3V3==1)  SCL_3V3=1; else if( is_3V3==0) SCL_5V=1;
  SCL_HIGH();
  delayxus(5);			   /*时钟低电平周期大于4μs*/
  /*清时钟线，钳住I2C总线以便继续接收*/
  //if (is_3V3==1)  SCL_3V3=0; else if( is_3V3==0) SCL_5V=0;
  SCL_LOW();

  delay1us();
  delay1us();    
}

/*******************************************************************
                 用户接口函数                                   
*******************************************************************/
/*******************************************************************
                 向无子地址器件发送字节数据函数               
功能:     从启动总线到发送地址，数据，结束总线的全过程,从器件地址sla.
          如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
byte ISendByte_NoSub(unsigned char sla,unsigned char c)
{
	unsigned char i;
	for(i=0;i<5;i++)
	{
		Start_I2c();               /*启动总线*/
	   	SendByte(sla);             /*发送器件地址*/
	   	if(ack==0)
	   	{
	   		Stop_I2c();                /*结束总线*/ 
	   		continue;
	   	}
	   	SendByte(c);               /*发送数据*/
	   	if(ack==0)
	   	{
	   		Stop_I2c();                /*结束总线*/ 
	   		continue;
	   	}
	   	//if(ack==0)return(0);
	   	Stop_I2c();                /*结束总线*/ 
	   	break;
	}
#ifdef FUNC_CAN_DIAG_EN
	if(i>=5)
	{
		canDTC[0]=1;
		return (0);
	}
	canDTC[0]=0;
#endif
	return(1);
}
/*******************************************************************
                  向有子地址器件发送字节数据函数               
功能:     从启动总线到发送地址，子地址,数据，结束总线的全过程,从器件
          地址sla，子地址suba，发送内容是s指向的内容，发送no个字节。
          如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
byte ISendByte(unsigned char sla,unsigned char suba,unsigned char c)
{
   //PTED_PTED4=~PTED_PTED4;
    //PTED_PTED5=~PTED_PTED5;
   Start_I2c();               /*启动总线*/
   SendByte(sla);             /*发送器件地址*/
//   if(ack==0)return(0);
   SendByte(suba);            /*发送器件子地址*/
//   if(ack==0)return(0);
   SendByte(c);               /*发送数据*/
   if(ack==0)return(0);
   Stop_I2c();                /*结束总线*/ 
   return(1);
}

 


/*******************************************************************
                    向有子地址器件发送多字节数据函数               
功能:     从启动总线到发送地址，子地址,数据，结束总线的全过程,从器件
          地址sla，子地址suba，发送内容是s指向的内容，发送no个字节。
          如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
byte ISendStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no)
{
   unsigned char i;

   Start_I2c();               /*启动总线*/
   SendByte(sla);             /*发送器件地址*/
   //if(ack==0)return(0);
   SendByte(suba);            /*发送器件子地址*/
   //if(ack==0)return(0);

   for(i=0;i<no;i++)
   {   
     SendByte(*s);            /*发送数据*/
     //if(ack==0)return(0);
     s++;
   } 
   Stop_I2c();                /*结束总线*/ 
   return(1);
}


/*******************************************************************
                    向无子地址器件发送多字节数据函数               
功能:     从启动总线到发送地址，子地址,数据，结束总线的全过程,从器件
          地址sla，发送内容是s指向的内容，发送no个字节。
          如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
byte ISendStr_NoSub(unsigned char sla,unsigned char *s,unsigned char no)
{
   unsigned char i;

   Start_I2c();               /*启动总线*/
   SendByte(sla);             /*发送器件地址*/
   //if(ack==0)return(0);

   for(i=0;i<no;i++)
   {   
     SendByte(*s);            /*发送数据*/
     //if(ack==0)return(0);
     s++;
   } 
   Stop_I2c();                /*结束总线*/ 
   return(1);
}

 

 

/*******************************************************************
                    向无子地址器件读字节数据函数               
功能:     从启动总线到发送地址，读数据，结束总线的全过程,从器件地
          址sla，返回值在c.
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
byte IRcvByte_NoSub(unsigned char sla,unsigned char *c)
{
   Start_I2c();                /*启动总线*/
   SendByte(sla);            /*发送器件地址*/
   //if(ack==0)return(0);
   *c=RcvByte();               /*读取数据*/
   Ack_I2c(1);                 /*发送非就答位*/
   Stop_I2c();                 /*结束总线*/ 
   return(1);
}

/*******************************************************************
                    向有子地址器件读字节数据函数               
功能:     从启动总线到发送地址，读数据，结束总线的全过程,从器件地
          址sla，返回值在c.
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
byte IRcvByte(unsigned char sla,char suba,unsigned char *c)
{
   Start_I2c();                /*启动总线*/
   SendByte(sla);            /*发送器件地址*/
//   if(ack==0)return(0);
   SendByte(suba);               /*发送器件子地址*/
//   if(ack==0)return(0);
   *c=RcvByte();               /*读取数据*/
   Ack_I2c(1);                 /*发送非就答位*/
   Stop_I2c();                 /*结束总线*/ 
   return(1);
}
 

/*******************************************************************
                    向有子地址器件读取多字节数据函数               
功能:     从启动总线到发送地址，子地址,读数据，结束总线的全过程,从器件
          地址sla，子地址suba，读出的内容放入s指向的存储区，读no个字节。
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
byte IRcvStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no)
{
   unsigned char i;

   Start_I2c();                  /*启动总线*/
   SendByte(sla);                /*发送器件地址*/
   if(ack==0)return(0);
   SendByte(suba);               /*发送器件子地址*/
   if(ack==0)return(0);

   Start_I2c();    /*重新启动总线*/
   SendByte(sla);
   if(ack==0)return(0);
   for(i=0;i<no-1;i++)
   {   
     *s=RcvByte();               /*发送数据*/
      Ack_I2c(0);                /*发送就答位*/  
     s++;
   } 
   *s=RcvByte();
   Ack_I2c(1);                   /*发送非应位*/
   Stop_I2c();                   /*结束总线*/ 
   return(1);
}


/*******************************************************************
                    向无子地址器件读取多字节数据函数               
功能:     从启动总线到发送地址,读数据,结束总线的全过程.
          从器件地址sla，读出的内容放入s指向的存储区，
          读no个字节。如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
byte IRcvStr_NoSub(unsigned char sla,unsigned char *s,unsigned char no)
{
   unsigned char i;

   Start_I2c();
   SendByte(sla);
   //if(ack==0)return(0);

   for(i=0;i<no-1;i++)
   {   
      *s=RcvByte();               /*发送数据*/
      Ack_I2c(0);                 /*发送就答位*/  
      s++;
   } 
   *s=RcvByte();
   Ack_I2c(1);                    /*发送非应位*/
   Stop_I2c();                    /*结束总线*/ 
   return(1);
}

