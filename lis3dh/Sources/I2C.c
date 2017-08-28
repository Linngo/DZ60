#include "I2C.h"
#include "time.h"
byte  ack;           /*Ӧ���־λ*/
//��Ϊ��50KHz��ʱ�ӣ�ÿ������Ϊ20us

void DogHello(void){
    SRS=0X55;
    SRS=0XAA;   
}

/*********************��ʱ����xus***********************************/

void delay1us(void)
{
//Nop 0.18us
//_asm NOP;_asm NOP;_asm NOP;_asm NOP;_asm NOP;_asm NOP;
delayxus(1);
}
void delayxus(byte x)
{
//����Ƶ��5.5296  T=0.18us  1uS=6*T
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
                     �����ߺ���               
����ԭ��: void  Start_I2c();  
����:     ����I2C����,������I2C��ʼ����.  
********************************************************************/
void Start_I2c()
{ /*������ʼ�����������ź�*/
  //if (is_3V3==1) SDA_3V3=1; else if( is_3V3==0) SDA_5V=1;
  SDA_HIGH();
  delayxus(5);//1
  //if (is_3V3==1) SCL_3V3=1; else if( is_3V3==0) SCL_5V=1;
  SCL_HIGH;
  delayxus(20);//5/*��ʼ��������ʱ�����4.7us,��ʱ*/
  /*������ʼ�ź�*/
  //if (is_3V3==1)  SDA_3V3=0; else if( is_3V3==0) SDA_5V=0;
  SDA_LOW();
  delayxus(20);//5/* ��ʼ��������ʱ�����4��s*/
  /*ǯסI2C���ߣ�׼�����ͻ�������� */
  //if (is_3V3==1)  SCL_3V3=0; else if( is_3V3==0) SCL_5V=0;
  SCL_LOW();
  delayxus(20);//5//delay10us();

}

/*******************************************************************
                      �������ߺ���               
����ԭ��: void  Stop_I2c();  
����:     ����I2C����,������I2C��������.  
********************************************************************/
void Stop_I2c()
{
  /*���ͽ��������������ź�*/
  //if (is_3V3==1)  SDA_3V3=0; else if( is_3V3==0) SDA_5V=0;
  SDA_LOW();
  delayxus(5);//1    /*���ͽ���������ʱ���ź�*/
  /*������������ʱ�����4��s*/
  //if (is_3V3==1)  SCL_3V3=1; else if( is_3V3==0) SCL_5V=1;
  SCL_HIGH();
  delayxus(20);//5//delay25us();
  /*����I2C���߽����ź�*/
  //if (is_3V3==1)  SDA_3V3=1; else if( is_3V3==0) SDA_5V=1;
  SDA_HIGH();
  delayxus(20);//5//delay25us();
}


/*******************************************************************
                 �ֽ����ݷ��ͺ���               
����ԭ��: void  SendByte(uchar c);
����:     ������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������,�����ȴ�Ӧ��,����
          ��״̬λ���в���.(��Ӧ����Ӧ��ʹack=0)     
          ��������������ack=1; ack=0��ʾ��������Ӧ����𻵡�
********************************************************************/
void  SendByte(unsigned char c)
{
	 unsigned char BitCnt;
	 unsigned char errCnt=100;
	 
	 for(BitCnt=0;BitCnt<8;BitCnt++)  /*Ҫ���͵����ݳ���Ϊ8λ*/
     {
		if((c<<BitCnt)&0x80){
			/*�жϷ���λ*/
			SDA_HIGH();
		}
		else  {
			SDA_LOW();
		}
		delayxus(5);//1
		/*��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ*/
		SCL_HIGH();
		delayxus(20);//5 		/*��֤ʱ�Ӹߵ�ƽ���ڴ���4��s*/
		SCL_LOW();
     }
     DogHello();
     delayxus(20);//5//delay10us();
     /*8λ��������ͷ������ߣ�׼������Ӧ��λ*/
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
			ack=1;        /*�ж��Ƿ���յ�Ӧ���ź�*/
		}
	}
	while((errCnt!=0)&&(ack==0));
	 SDA_3V3_D=1;
 	 
	 SCL_LOW();
     delayxus(20);//5//delay10us();
}
/*******************************************************************
                 �ֽ����ݽ��պ���               
����ԭ��: uchar  RcvByte();
����:     �������մ���������������,���ж����ߴ���(����Ӧ���ź�)��
          ���������Ӧ����Ӧ��ӻ���  
********************************************************************/ 
unsigned char  RcvByte()
{
    unsigned char retc;
    unsigned char BitCnt;

    retc=0; 
    /*��������Ϊ���뷽ʽ*/
    //if (is_3V3==1)  SDA_3V3=1; else if( is_3V3==0) SDA_5V=1;
    SDA_HIGH();

    
    SDA_3V3_D=0;  

    for(BitCnt=0;BitCnt<8;BitCnt++)
    {
        delay1us();           
        /*��ʱ����Ϊ�ͣ�׼����������λ*/
        //if (is_3V3==1)  SCL_3V3=0; else if( is_3V3==0) SCL_5V=0;
        SCL_LOW();
        delayxus(5);				/*ʱ�ӵ͵�ƽ���ڴ���4.7��s*/
        /*��ʱ����Ϊ��ʹ��������������Ч*/
        //if (is_3V3==1)  SCL_3V3=1; else if( is_3V3==0) SCL_5V=1;
        SCL_HIGH();
        delayxus(5);
        delayxus(5);
        retc=retc<<1;

        if(SDA_3V3==1)retc=retc+1;  /*������λ,���յ�����λ����retc�� */

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
                     Ӧ���Ӻ���
����ԭ��:  void Ack_I2c(bit a);
����:      ����������Ӧ���ź�(������Ӧ����Ӧ���źţ���λ����a����)
********************************************************************/
void Ack_I2c(byte a)
{
  
  if(a==0){
  	/*�ڴ˷���Ӧ����Ӧ���ź� */
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
  delayxus(5);			   /*ʱ�ӵ͵�ƽ���ڴ���4��s*/
  /*��ʱ���ߣ�ǯסI2C�����Ա��������*/
  //if (is_3V3==1)  SCL_3V3=0; else if( is_3V3==0) SCL_5V=0;
  SCL_LOW();

  delay1us();
  delay1us();    
}

/*******************************************************************
                 �û��ӿں���                                   
*******************************************************************/
/*******************************************************************
                 �����ӵ�ַ���������ֽ����ݺ���               
����:     ���������ߵ����͵�ַ�����ݣ��������ߵ�ȫ����,��������ַsla.
          �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
byte ISendByte_NoSub(unsigned char sla,unsigned char c)
{
	unsigned char i;
	for(i=0;i<5;i++)
	{
		Start_I2c();               /*��������*/
	   	SendByte(sla);             /*����������ַ*/
	   	if(ack==0)
	   	{
	   		Stop_I2c();                /*��������*/ 
	   		continue;
	   	}
	   	SendByte(c);               /*��������*/
	   	if(ack==0)
	   	{
	   		Stop_I2c();                /*��������*/ 
	   		continue;
	   	}
	   	//if(ack==0)return(0);
	   	Stop_I2c();                /*��������*/ 
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
                  �����ӵ�ַ���������ֽ����ݺ���               
����:     ���������ߵ����͵�ַ���ӵ�ַ,���ݣ��������ߵ�ȫ����,������
          ��ַsla���ӵ�ַsuba������������sָ������ݣ�����no���ֽڡ�
          �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
byte ISendByte(unsigned char sla,unsigned char suba,unsigned char c)
{
   //PTED_PTED4=~PTED_PTED4;
    //PTED_PTED5=~PTED_PTED5;
   Start_I2c();               /*��������*/
   SendByte(sla);             /*����������ַ*/
//   if(ack==0)return(0);
   SendByte(suba);            /*���������ӵ�ַ*/
//   if(ack==0)return(0);
   SendByte(c);               /*��������*/
   if(ack==0)return(0);
   Stop_I2c();                /*��������*/ 
   return(1);
}

 


/*******************************************************************
                    �����ӵ�ַ�������Ͷ��ֽ����ݺ���               
����:     ���������ߵ����͵�ַ���ӵ�ַ,���ݣ��������ߵ�ȫ����,������
          ��ַsla���ӵ�ַsuba������������sָ������ݣ�����no���ֽڡ�
          �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
byte ISendStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no)
{
   unsigned char i;

   Start_I2c();               /*��������*/
   SendByte(sla);             /*����������ַ*/
   //if(ack==0)return(0);
   SendByte(suba);            /*���������ӵ�ַ*/
   //if(ack==0)return(0);

   for(i=0;i<no;i++)
   {   
     SendByte(*s);            /*��������*/
     //if(ack==0)return(0);
     s++;
   } 
   Stop_I2c();                /*��������*/ 
   return(1);
}


/*******************************************************************
                    �����ӵ�ַ�������Ͷ��ֽ����ݺ���               
����:     ���������ߵ����͵�ַ���ӵ�ַ,���ݣ��������ߵ�ȫ����,������
          ��ַsla������������sָ������ݣ�����no���ֽڡ�
          �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
byte ISendStr_NoSub(unsigned char sla,unsigned char *s,unsigned char no)
{
   unsigned char i;

   Start_I2c();               /*��������*/
   SendByte(sla);             /*����������ַ*/
   //if(ack==0)return(0);

   for(i=0;i<no;i++)
   {   
     SendByte(*s);            /*��������*/
     //if(ack==0)return(0);
     s++;
   } 
   Stop_I2c();                /*��������*/ 
   return(1);
}

 

 

/*******************************************************************
                    �����ӵ�ַ�������ֽ����ݺ���               
����:     ���������ߵ����͵�ַ�������ݣ��������ߵ�ȫ����,��������
          ַsla������ֵ��c.
           �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
byte IRcvByte_NoSub(unsigned char sla,unsigned char *c)
{
   Start_I2c();                /*��������*/
   SendByte(sla);            /*����������ַ*/
   //if(ack==0)return(0);
   *c=RcvByte();               /*��ȡ����*/
   Ack_I2c(1);                 /*���ͷǾʹ�λ*/
   Stop_I2c();                 /*��������*/ 
   return(1);
}

/*******************************************************************
                    �����ӵ�ַ�������ֽ����ݺ���               
����:     ���������ߵ����͵�ַ�������ݣ��������ߵ�ȫ����,��������
          ַsla������ֵ��c.
           �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
byte IRcvByte(unsigned char sla,char suba,unsigned char *c)
{
   Start_I2c();                /*��������*/
   SendByte(sla);            /*����������ַ*/
//   if(ack==0)return(0);
   SendByte(suba);               /*���������ӵ�ַ*/
//   if(ack==0)return(0);
   *c=RcvByte();               /*��ȡ����*/
   Ack_I2c(1);                 /*���ͷǾʹ�λ*/
   Stop_I2c();                 /*��������*/ 
   return(1);
}
 

/*******************************************************************
                    �����ӵ�ַ������ȡ���ֽ����ݺ���               
����:     ���������ߵ����͵�ַ���ӵ�ַ,�����ݣ��������ߵ�ȫ����,������
          ��ַsla���ӵ�ַsuba�����������ݷ���sָ��Ĵ洢������no���ֽڡ�
           �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
byte IRcvStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no)
{
   unsigned char i;

   Start_I2c();                  /*��������*/
   SendByte(sla);                /*����������ַ*/
   if(ack==0)return(0);
   SendByte(suba);               /*���������ӵ�ַ*/
   if(ack==0)return(0);

   Start_I2c();    /*������������*/
   SendByte(sla);
   if(ack==0)return(0);
   for(i=0;i<no-1;i++)
   {   
     *s=RcvByte();               /*��������*/
      Ack_I2c(0);                /*���;ʹ�λ*/  
     s++;
   } 
   *s=RcvByte();
   Ack_I2c(1);                   /*���ͷ�Ӧλ*/
   Stop_I2c();                   /*��������*/ 
   return(1);
}


/*******************************************************************
                    �����ӵ�ַ������ȡ���ֽ����ݺ���               
����:     ���������ߵ����͵�ַ,������,�������ߵ�ȫ����.
          ��������ַsla�����������ݷ���sָ��Ĵ洢����
          ��no���ֽڡ��������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
byte IRcvStr_NoSub(unsigned char sla,unsigned char *s,unsigned char no)
{
   unsigned char i;

   Start_I2c();
   SendByte(sla);
   //if(ack==0)return(0);

   for(i=0;i<no-1;i++)
   {   
      *s=RcvByte();               /*��������*/
      Ack_I2c(0);                 /*���;ʹ�λ*/  
      s++;
   } 
   *s=RcvByte();
   Ack_I2c(1);                    /*���ͷ�Ӧλ*/
   Stop_I2c();                    /*��������*/ 
   return(1);
}

