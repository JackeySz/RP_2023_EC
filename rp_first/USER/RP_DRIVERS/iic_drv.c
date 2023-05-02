#include "iic_drv.h"


/*

bmi270�л�IIC:
CSB_Pin���ϵ�ʱ���ָߵ�ƽ��
SDO_Pin��IIC_CS��: �ӵص�ַ0x68 �ӵ��ַ0x69

*/
void IIC_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  __HAL_RCC_GPIOB_CLK_ENABLE();
	
  HAL_GPIO_WritePin(GPIOB,BMI_CS_Pin, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOB,IIC_CS_Pin, GPIO_PIN_SET);
	
  /*Configure GPIO pins : IIC_SCL_Pin IIC_CS_Pin IIC_SDA_Pin */
  GPIO_InitStruct.Pin = IIC_SCL_Pin|IIC_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = IIC_CS_Pin|BMI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
  HAL_GPIO_WritePin(GPIOB,BMI_CS_Pin, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOB,IIC_CS_Pin, GPIO_PIN_SET);	
}




//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(uint8_t txd)
{                        
	uint8_t t;   
	SDA_OUT(); 	    
	IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
	
	for(t=0;t<8;t++)
	{              
		IIC_SDA=(txd&0x80)>>7;
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
	}	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
	
	for(i=0;i<8;i++ )
	{
		IIC_SCL=0; 
		delay_us(2);
		IIC_SCL=1;
		receive<<=1;
		if(READ_SDA)receive++;   
		delay_us(1); 
	}					 
	
	if (!ack)
		IIC_NAck();//����nACK
	else
		IIC_Ack(); //����ACK   
	
	return receive;
}

void IIC_Read_Cicle(uint8_t reg,uint8_t *buff,uint8_t len)
{
	uint8_t i;
  IIC_Start(); 
	IIC_Send_Byte( (0x69<<1)|0  );
	IIC_Wait_Ack();		
  IIC_Send_Byte(reg);	
  IIC_Wait_Ack();
  IIC_Start();
	IIC_Send_Byte( (0x69<<1)|1 );
  IIC_Wait_Ack();		
	
	for(i = 0 ; i< len;i++)
	{
		*buff=IIC_Read_Byte(1);
		buff++;
	}
  IIC_Stop();			
		
}







