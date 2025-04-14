#include "stm32f10x.h"                  // Device header
#include "IIC.h"
#include "MPU6050.h"
#include "MPU6050_REG.h"
#include "Delay.h"

//1表示接收到应答，2表示未接收到应答
u8 MPU_SendByte(u8 reg,u8 data) 				 
{ 
    IIC_Start(); 
	IIC_SendByte((MPU_ADDR<<1)|0);//发送器件地址+写命令	
	if(!IIC_ReceiveAck())	//等待应答
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_SendByte(reg);	//写寄存器地址
    IIC_ReceiveAck();		//等待应答 
	IIC_SendByte(data);//发送数据
	if(!IIC_ReceiveAck())	//等待ACK
	{
		IIC_Stop();
		return 1;
	}	 
    IIC_Stop();	 
	return 0;
}

u8 MPU_Send_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    IIC_Start(); 
	IIC_SendByte((addr<<1)|0);//发送器件地址+写命令	
	if(!IIC_ReceiveAck())	//等待应答
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_SendByte(reg);	//写寄存器地址
    IIC_ReceiveAck();		//等待应答
	for(i=0;i<len;i++)
	{
		IIC_SendByte(buf[i]);	//发送数据
		if(!IIC_ReceiveAck())		//等待ACK
		{
			IIC_Stop();	 
			return 1;		 
		}		
	}    
    IIC_Stop();	 
	return 0;	
} 

u8 MPU_ReadByte(u8 reg)
{
	u8 res;
    IIC_Start(); 
	IIC_SendByte((MPU_ADDR<<1)|0);//发送器件地址+写命令	
	IIC_ReceiveAck();		//等待应答 
    IIC_SendByte(reg);	//写寄存器地址
    IIC_ReceiveAck();		//等待应答
	
    IIC_Start();
	IIC_SendByte((MPU_ADDR<<1)|1);//发送器件地址+读命令	
    IIC_ReceiveAck();		//等待应答 
	res=IIC_ReceiveByte();//读取数据,发送nACK 
	IIC_SendAck(0);
    IIC_Stop();			//产生一个停止条件 
	return res;		
}

u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	IIC_Start(); 
	IIC_SendByte((addr<<1)|0);//发送器件地址+写命令	
	if(!IIC_ReceiveByte())	//等待应答
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_SendByte(reg);	//写寄存器地址
    IIC_ReceiveByte();		//等待应答
    IIC_Start();
	IIC_SendByte((addr<<1)|1);//发送器件地址+读命令	
    IIC_ReceiveByte();		//等待应答 
	while(len)
	{
		if(len==1){*buf=IIC_ReceiveByte();IIC_SendAck(0);}//读数据,发送nACK 
		else {*buf=IIC_ReceiveByte();IIC_SendAck(1);}		//读数据,发送ACK  
		len--;
		buf++; 
	}    
    IIC_Stop();	//产生一个停止条件 
	return 0;	
}

u8 MPU6050_Init(void)
{
	
    IIC_Init();
    u8 res; 
	MPU_SendByte(MPU_PWR_MGMT1_REG,0X80);	//复位MPU6050
    Delay_ms(100);
	MPU_SendByte(MPU_PWR_MGMT1_REG,0X00);	//唤醒MPU6050 
	MPU_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(0);					//加速度传感器,±2g
	MPU_Set_Rate(200);						//设置采样率50Hz
	MPU_SendByte(MPU_INT_EN_REG,0X00);	//关闭所有中断
	MPU_SendByte(MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
	MPU_SendByte(MPU_FIFO_EN_REG,0X00);	//关闭FIFO
	MPU_SendByte(MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	res=MPU_ReadByte(MPU_DEVICE_ID_REG); 
	if(res==MPU_ADDR)//器件ID正确
	{
		MPU_SendByte(MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
		MPU_SendByte(MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
		MPU_Set_Rate(100);						//设置采样率为50Hz
 	}else return 1;
	return 0;

}	

//设置MPU6050陀螺仪传感器满量程范围

u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_SendByte(MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}
//设置MPU6050加速度传感器满量程范围

u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_SendByte(MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}
//设置MPU6050的数字低通滤波器

u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_SendByte(MPU_CFG_REG,data);//设置数字低通滤波器  
}
//设置MPU6050的采样率(假定Fs=1KHz)

u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_SendByte(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

//得到温度值

short MPU_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;;
}
//得到陀螺仪值(原始值)

u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
//得到加速度值(原始值)

u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}

void MPU6050_EXTI_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
	
	NVIC_InitTypeDef NVICDEF;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVICDEF.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVICDEF.NVIC_IRQChannelCmd=ENABLE;
	NVICDEF.NVIC_IRQChannelPreemptionPriority=0;
	NVICDEF.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVICDEF);
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line5;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
}

