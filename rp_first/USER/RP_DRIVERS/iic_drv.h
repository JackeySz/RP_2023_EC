#ifndef __IIC_DRV_H
#define __IIC_DRV_H

#include "tick_drv.h"
#include "gpio_drv.h"
#include "type.h"
//IO�ڷ�������
#define SDA_IN() 	  {GPIOB->MODER&=~((uint32_t)3<<(15*2));GPIOB->MODER|=(uint32_t)0<<(15*2);}	//PB15����ģʽ
#define SDA_OUT() 	{GPIOB->MODER&=~((uint32_t)3<<(15*2));GPIOB->MODER|=(uint32_t)1<<(15*2);}	//PB15����ģʽ
//IO�ڲ�������
#define IIC_SCL 	PBout(13)
#define IIC_SDA 	PBout(15)
#define READ_SDA 	PBin(15)			

//IIC���в�������
void IIC_Init(void);
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t IIC_Read_Byte(uint8_t ack);//IIC��ȡһ���ֽ�
uint8_t IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�
void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	  
void IIC_Read_Cicle(uint8_t reg,uint8_t *buff,uint8_t len);


#endif

