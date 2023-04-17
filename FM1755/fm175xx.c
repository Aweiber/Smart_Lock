/**--------------File Info---------------------------------------------------------------------------------
** File name:           type_a.c
** Last modified Date:  2016-3-14
** Last Version:        V1.00
** Descriptions:        ISO/IEC144443A卡相关操作
**
**--------------------------------------------------------------------------------------------------------
*/

#include <stdint.h>
#include "fm175xx.h"
#include <string.h>    

#include "nfcSpi.h"
#include "delay.h"
#include "include.h"
#include "type_a.h"
#include "type_b.h"


#define MI_NOTAGERR 0xEE
#define MAXRLEN 64

uint8_t Nfc_Status=NO;
uint8_t CardID[8];//读卡ID


/*********************************************************************************************************
** Function name:       pcd_Init
** input parameters:    N/A
** output parameters:   N/A
** Returned value:      
*********************************************************************************************************/

void CD_CfgTPD(void){
	GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /*Configure GPIO pins : PBPin PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


void CD_SetTPD(void){
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
}


void CD_ClrTPD(void){
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
}

void FM175XX_HPD(unsigned char mode)//mode = 1 退出HPD模式 ，mode = 0 进入HPD模式
{
	GPIO_InitTypeDef GPIO_InitStruct;
	if(mode == 0x0)
		{
		delay_ms(1);//延时1ms,等待之前的操作结束
		CD_ClrTPD();//NPD = 0	进入HPD模式
#if 0
		GPIO_InitStruct.Pin = FLASH_SPI_SCK_PIN|FLASH_SPI_MOSI_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(FLASH_SPI_GPIO_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = FLASH_SPI_MISO_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(FLASH_SPI_GPIO_PORT, &GPIO_InitStruct);

		HAL_GPIO_WritePin(FLASH_SPI_GPIO_PORT, FLASH_SPI_SCK_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(FLASH_SPI_GPIO_PORT, FLASH_SPI_MISO_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(FLASH_SPI_GPIO_PORT, FLASH_SPI_MOSI_PIN, GPIO_PIN_RESET);		


		GPIO_InitStruct.Pin = FLASH_SPI_CS_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		HAL_GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStruct);
		HAL_GPIO_WritePin(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN, GPIO_PIN_SET);	//NSS = 1;			


#endif


       /*##-2- Configure peripheral GPIO ##########################################*/  
    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = NFC_SPI_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = SPI2_SCK_AF;
    HAL_GPIO_Init(SPI2_SCK_GPIO_PORT, &GPIO_InitStruct);

    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = NFC_SPI_MISO_PIN;
    GPIO_InitStruct.Alternate = SPI2_MISO_AF;
    HAL_GPIO_Init(SPI2_MISO_GPIO_PORT, &GPIO_InitStruct);

    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = NFC_SPI_MOSI_PIN;
    GPIO_InitStruct.Alternate = SPI2_MOSI_AF;
    HAL_GPIO_Init(SPI2_MOSI_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(NFC_SPI_GPIO_PORT, NFC_SPI_SCK_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(NFC_SPI_GPIO_PORT, NFC_SPI_MISO_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(NFC_SPI_GPIO_PORT, NFC_SPI_MOSI_PIN, GPIO_PIN_RESET); 	
		

	GPIO_InitStruct.Pin = NFC_SPI_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
  HAL_GPIO_Init(NFC_SPI_CS_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(NFC_SPI_CS_PORT, NFC_SPI_CS_PIN, GPIO_PIN_SET);



	}
	else
		{
		CD_SetTPD();//NPD = 1 退出HPD模式
		delay_ms(1);//延时1ms，等待FM175XX启动
			
		
		  GPIO_InitStruct.Pin 	  = NFC_SPI_SCK_PIN;
		  GPIO_InitStruct.Mode		= GPIO_MODE_AF_PP;
		  GPIO_InitStruct.Pull		= GPIO_PULLDOWN;
		  GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_HIGH;
		  GPIO_InitStruct.Alternate = SPI2_SCK_AF;
		  HAL_GPIO_Init(SPI2_SCK_GPIO_PORT, &GPIO_InitStruct);
		
		  /* SPI MISO GPIO pin configuration  */
		  GPIO_InitStruct.Pin = NFC_SPI_MISO_PIN;
		  GPIO_InitStruct.Alternate = SPI2_MISO_AF;
		  HAL_GPIO_Init(SPI2_MISO_GPIO_PORT, &GPIO_InitStruct);
		
		  /* SPI MOSI GPIO pin configuration  */
		  GPIO_InitStruct.Pin = NFC_SPI_MOSI_PIN;
		  GPIO_InitStruct.Alternate = SPI2_MOSI_AF;
		  HAL_GPIO_Init(SPI2_MOSI_GPIO_PORT, &GPIO_InitStruct);
		  HAL_GPIO_WritePin(NFC_SPI_GPIO_PORT, NFC_SPI_SCK_PIN, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(NFC_SPI_GPIO_PORT, NFC_SPI_MISO_PIN, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(NFC_SPI_GPIO_PORT, NFC_SPI_MOSI_PIN, GPIO_PIN_RESET);	  
			  
		
		  GPIO_InitStruct.Pin = NFC_SPI_CS_PIN;
		  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		  GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
		  HAL_GPIO_Init(NFC_SPI_CS_PORT, &GPIO_InitStruct);
		  HAL_GPIO_WritePin(NFC_SPI_CS_PORT, NFC_SPI_CS_PIN, GPIO_PIN_SET);
		
		}
	return;
}


/*********************************************************************************************************
** Function name:       MCU_TO_PCD_TEST
** Descriptions:        MCU与读卡芯片通信测试函数
**                      不同的读卡芯片的选取用于测试的寄存器不一样，需要更具具体芯片而确定目标寄存器
** input parameters:    N/A
** output parameters:   N/A
** Returned value:      TRUE---测试通过    FALSE----测试失败
*********************************************************************************************************/
uint8_t MCU_TO_PCD_TEST(void)
{
    volatile uint8_t ucRegVal;
    pcd_RST();                                                          /* 硬件复位                     */
    ucRegVal = spi_GetReg(ControlReg);
    spi_SetReg(ControlReg, 0x10);                                       /* 启动读写器模式               */
    ucRegVal = spi_GetReg(ControlReg);
    spi_SetReg(GsNReg, 0xF0 | 0x0F);                                    /* CWGsN = 0xF; ModGsN = 0x4    */
    ucRegVal = spi_GetReg(GsNReg);
    if(ucRegVal != 0xFF)                                                /* 验证接口正确                 */
        return FALSE;
    return TRUE;
}


/*********************************************************************************************************
** Function name:       pcd_RST
** Descriptions:        PCD控制PICC硬件复位函数
** input parameters:    N/A
** output parameters:   N/A
** Returned value:      寄存器数值
*********************************************************************************************************/
void pcd_RST(void)
{
	CD_SetTPD();
	//delay_ms(1);
	 delay_us(300);
	CD_ClrTPD();                                                        /* 复位管脚拉低                 */
	//delay_ms(1);
	 delay_us(500);
	CD_SetTPD();                                                        /* 再拉高                       */
	//delay_ms(1);
	delay_us(10*100);
}

void NRSTPD_CTRL(uint8_t  ctrltype)
{
    if(ctrltype == 0x0)
    CD_ClrTPD();
    else
    CD_SetTPD();
}

/*********************************************************************************************************
** Function name:       Read_Reg
** Descriptions:        读取寄存器        
** input parameters:    reg_add:寄存器数值
** output parameters:   N/A
** Returned value:      寄存器数值
*********************************************************************************************************/
uint8_t  Read_Reg(uint8_t  reg_add)
{
    uint8_t   reg_value;       
    reg_value=spi_GetReg(reg_add);
    return reg_value;
}

/*********************************************************************************************************
** Function name:       Read_Reg
** Descriptions:        读取寄存器        
** input parameters:    reg_add:寄存器数值
** output parameters:   N/A
** Returned value:      寄存器数值
*********************************************************************************************************/
uint8_t  GetReg(uint8_t  reg_add,uint8_t *regData)
{     
    *regData=spi_GetReg(reg_add);
    return *regData;
}

/*********************************************************************************************************
** Function name:       Read_Reg
** Descriptions:        读取寄存器        
** input parameters:    reg_add:寄存器数值
** output parameters:   N/A
** Returned value:      寄存器数值
*********************************************************************************************************/
uint8_t  SetReg(uint8_t  reg_add,uint8_t ucRegVal)
{     
    spi_SetReg(reg_add,ucRegVal);
    return 1;
}

/*********************************************************************************************************
** Function name:       Read_Reg_All
** Descriptions:        读取全部寄存器        
** input parameters:    reg_value:寄存器数值
** output parameters:   N/A
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  Read_Reg_All(uint8_t  *reg_value)
{
    uint8_t  i;
    for (i=0;i<64;i++)       
        *(reg_value+i) = spi_GetReg(i);
    return TRUE;
}


/*********************************************************************************************************
** Function name:       Write_Reg
** Descriptions:        写寄存器操作        
** input parameters:    reg_add:寄存器地址
**                      reg_value:寄存器数值
** output parameters:   N/A
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  Write_Reg(uint8_t  reg_add,uint8_t  reg_value)
{
    spi_SetReg(reg_add,reg_value);
    return TRUE;
}


/*********************************************************************************************************
** Function name:       Read_FIFO
** Descriptions:        读出FIFO的数据     
** input parameters:    length:读取数据长度
**                      *fifo_data:数据存放指针
** output parameters:   N/A
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
void Read_FIFO(uint8_t  length,uint8_t  *fifo_data)
{     
    SPIRead_Sequence(length,FIFODataReg,fifo_data);
    return;
}


/*********************************************************************************************************
** Function name:       Write_FIFO
** Descriptions:        写入FIFO     
** input parameters:    length:读取数据长度
**                      *fifo_data:数据存放指针
** output parameters:   N/A
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
void Write_FIFO(uint8_t  length,uint8_t  *fifo_data)
{
    SPIWrite_Sequence(length,FIFODataReg,fifo_data);
    return;
}


/*********************************************************************************************************
** Function name:       Clear_FIFO
** Descriptions:        清空FIFO          
** input parameters:   
** output parameters:   N/A
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  Clear_FIFO(void)
{
    Set_BitMask(FIFOLevelReg,0x80);                                     /* 清除FIFO缓冲                 */
    if( spi_GetReg(FIFOLevelReg) == 0 )
        return TRUE;
    else
        return FALSE;
}


/*********************************************************************************************************
** Function name:       Set_BitMask
** Descriptions:        置位寄存器操作    
** input parameters:    reg_add，寄存器地址
**                      mask，寄存器清除位
** output parameters:   N/A
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  Set_BitMask(uint8_t  reg_add,uint8_t  mask)
{
    uint8_t result;
    result=spi_SetReg(reg_add,Read_Reg(reg_add) | mask);                /* set bit mask                 */
    return result;
}

/*********************************************************************************************************
** Function name:       Clear_BitMask
** Descriptions:        清除位寄存器操作
** input parameters:    reg_add，寄存器地址
**                      mask，寄存器清除位
** output parameters:   N/A
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  Clear_BitMask(uint8_t  reg_add,uint8_t  mask)
{
    uint8_t   result;
    result=Write_Reg(reg_add,Read_Reg(reg_add) & ~mask);                /* clear bit mask               */
    return result;
}


/*********************************************************************************************************
** Function name:       Set_RF
** Descriptions:        设置射频输出
** input parameters:    mode，射频输出模式
**                      0，关闭输出
**                      1，仅打开TX1输出
**                      2，仅打开TX2输出
**                      3，TX1，TX2打开输出，TX2为反向输出
** output parameters:   N/A
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  Set_Rf(uint8_t  mode)
{
    uint8_t   result;
    if( (Read_Reg(TxControlReg)&0x03) == mode )
        return TRUE;
    if( mode == 0 )
        result = Clear_BitMask(TxControlReg,0x03);                      /* 关闭TX1，TX2输出             */
    if( mode== 1 )
        result = Clear_BitMask(TxControlReg,0x01);                      /* 仅打开TX1输出                */
    if( mode == 2)
        result = Clear_BitMask(TxControlReg,0x02);                      /* 仅打开TX2输出                */
    if (mode==3)
        result=Set_BitMask(TxControlReg,0x03);                          /* 打开TX1，TX2输出             */
//    Delay100us(2000);
    return result;
}
 
/*********************************************************************************************************
** Function name:       Pcd_Comm
** Descriptions:        读卡器通信 不利用IRQ管脚的情况
** input parameters:    Command:通信操作命令
**                      pInData:发送数据数组
**                      InLenByte:发送数据数组字节长度
**                      pOutData:接收数据数组
**                      pOutLenBit:接收数据的位长度
** output parameters:   N/A
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:       Pcd_Comm
** Descriptions:        读卡器通信 不利用IRQ管脚的情况
** input parameters:    Command:通信操作命令
**                      pInData:发送数据数组
**                      InLenByte:发送数据数组字节长度
**                      pOutData:接收数据数组
**                      pOutLenBit:接收数据的位长度
** output parameters:   N/A
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
unsigned char Pcd_Comm(    unsigned char Command, 
                         unsigned char *pInData, 
                         unsigned char InLenByte,
                         unsigned char *pOutData, 
                         unsigned int *pOutLenBit)
{
    uint8_t status  = FALSE;
    uint8_t irqEn   = 0x00;                                             /* 使能的中断                   */
    uint8_t waitFor = 0x00;                                             /* 等待的中断                   */
    uint8_t lastBits;
    uint8_t n,sendLen,sendPi,revLen,fifoLen,errorReg,collPos;
    uint32_t i;
	  sendPi = 0x00;
    Write_Reg(ComIrqReg, 0x7F);                                         /* 清楚IRQ标记                  */
    Write_Reg(TModeReg,0x80);                                           /* 设置TIMER自动启动            */
    switch (Command) {
    case MFAuthent:                                                     /* Mifare认证                   */
        irqEn   = 0x12;
        waitFor = 0x10;
        break;
    case Transceive:                                       /* 发送FIFO中的数据到天线，传输后激活接收电路*/
        irqEn   = 0x77;
        waitFor = 0x30;
        break;
    default:
        break;
    }
    
    Write_Reg(ComIEnReg, irqEn | 0x80);
   // Clear_BitMask(ComIrqReg, 0x80);
    Write_Reg(CommandReg, Idle);
    Set_BitMask(FIFOLevelReg, 0x80);
    sendLen = InLenByte>40?40:InLenByte;
    sendPi += sendLen;
    revLen = 0x00;
		InLenByte -= sendLen;
    Write_FIFO(sendLen,pInData);
    Write_Reg(CommandReg, Command);
    if (Command == Transceive) {
        Set_BitMask(BitFramingReg, 0x80);
    }

    i = 3000;                                              /* 根据时钟频率调整，操作M1卡最大等待时间25ms*/

    do {
        n = spi_GetReg(ComIrqReg);                       //
		    fifoLen = spi_GetReg(FIFOLevelReg);
		    if(( InLenByte >0 )&&( fifoLen <0x10 ))                            
	    	{
					  sendLen = InLenByte>30?30:InLenByte;
					  Write_FIFO(sendLen, &pInData[sendPi]);
			      sendPi += sendLen;
			      InLenByte -=sendLen;
		    }
		    if( ( InLenByte == 0)&&( fifoLen >0x28 ) )
		    {
					  Read_FIFO(fifoLen,&pOutData[revLen]);
			      revLen += fifoLen;
		    }
        i--;
    } while ((i != 0) && !(n & 0x03) && !(n & waitFor));            /* n&0x01!=1表示PCDsettimer时间未到 */
                                                                        /* n&waitFor!=1表示指令执行完成 */
    Clear_BitMask(BitFramingReg, 0x80);
    if (i != 0) {
			  errorReg = Read_Reg(ErrorReg);
        if(!(errorReg & 0x1B)) {
            status = TRUE;
            if (n & irqEn & 0x01) {
                status = MI_NOTAGERR;
            }
						
            if (Command == Transceive) {
                fifoLen = Read_Reg(FIFOLevelReg);
                lastBits = Read_Reg(ControlReg) & 0x07;
                if (lastBits) {
                    *pOutLenBit = lastBits;
									  if(fifoLen > 1)
											 *pOutLenBit += (fifoLen-1) * 8;
										if (fifoLen == 0) {
                       fifoLen = 1;
                    }
                }
								else {
                    *pOutLenBit = fifoLen * 8;
                }
                Read_FIFO(fifoLen,&pOutData[revLen]);
            }
        }
        else if(errorReg & 0x08)                                  /* 冲突                               */
        {
					  spi_SetReg(ErrorReg,~(0x08));                                         /* 清除接收中断 */
					  collPos = spi_GetReg(CollReg);                                        /* 获取冲突位置 */
		        collPos &= 0x1f;
		        *pOutLenBit = (collPos == 0 )?32:collPos;
					  fifoLen =*pOutLenBit/8 +(*pOutLenBit%8?1:0);
					  Read_FIFO(fifoLen,&pOutData[revLen]);
            status = Anticollision;
				}					
				else {
            status = FALSE;
        }
    }
    Clear_BitMask(BitFramingReg,0x80);//关闭发送
    return status;
}


/*********************************************************************************************************
** Function name:       Pcd_SetTimer
** Descriptions:        设置接收延时
** input parameters:    delaytime，延时时间（单位为毫秒）  
** output parameters:   N/A
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  Pcd_SetTimer(unsigned long delaytime)
{
    unsigned long  TimeReload;
    uint32_t  Prescaler;

    Prescaler=0;
    TimeReload=0;
    while(Prescaler<0xfff) {
        TimeReload = ((delaytime*(long)13560)-1)/(Prescaler*2+1);
        if( TimeReload<0xffff)
            break;
        Prescaler++;
    }
    TimeReload=TimeReload&0xFFFF;
    Set_BitMask(TModeReg,Prescaler>>8);
    Write_Reg(TPrescalerReg,Prescaler&0xFF);            
    Write_Reg(TReloadMSBReg,TimeReload>>8);
    Write_Reg(TReloadLSBReg,TimeReload&0xFF);
    return TRUE;
}

/*********************************************************************************************************
** Function name:       Pcd_ConfigISOType
** Descriptions:        配置ISO14443A/B协议
** input parameters:    type = 0：ISO14443A协议；
**                         type = 1，ISO14443B协议；   
** output parameters:   N/A
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  Pcd_ConfigISOType(uint8_t  type)
{
    if (type == 0)   {                                                  /* 配置为ISO14443_A             */
        Set_BitMask(ControlReg, 0x10);                                /* ControlReg 0x0C 设置reader模式 */
        Set_BitMask(TxAutoReg, 0x40);                                  /* TxASKReg 0x15 设置100%ASK有效 */
        Write_Reg(TxModeReg, 0x00);                /* TxModeReg 0x12 设置TX CRC无效，TX FRAMING =TYPE A */
        Write_Reg(RxModeReg, 0x00);                /* RxModeReg 0x13 设置RX CRC无效，RX FRAMING =TYPE A */
    }
    if (type == 1) {                                                   /* 配置为ISO14443_B              */
        Write_Reg(ControlReg,0x10);
        Write_Reg(TxModeReg,0x83);                                     /* BIT1~0 = 2'b11:ISO/IEC 14443B */
        Write_Reg(RxModeReg,0x83);                                     /* BIT1~0 = 2'b11:ISO/IEC 14443B */
        Write_Reg(TxAutoReg,0x00);
        Write_Reg(RxThresholdReg,0x55);
        Write_Reg(RFCfgReg,0x48);
        Write_Reg(TxBitPhaseReg,0x87);                                 /* 默认值                         */
        Write_Reg(GsNReg,0x83);                                        /* 0x83                           */
        Write_Reg(CWGsPReg,0x30);                                      /*  0x30                          */
        Write_Reg(GsNOffReg,0x38);
        Write_Reg(ModGsPReg,0x20);
    }
//    Delay100us(30);
    return TRUE;
}


/*********************************************************************************************************
** Function name:       FM175X_SoftReset
** Descriptions:        FM175xx软件复位
** input parameters:    N/A       
** output parameters:   N/A
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t   FM175X_SoftReset(void)
{    
    Write_Reg(CommandReg,SoftReset);
    return Set_BitMask(ControlReg,0x10);                                /* 17520初始值配置              */
}

/*********************************************************************************************************
** Function name:       FM175X_HardReset
** Descriptions:        FM175xx硬件复位
** input parameters:    N/A       
** output parameters:   N/A
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  FM175X_HardReset(void)
{    
    CD_ClrTPD();
    delay_ms(10);
    CD_SetTPD();
    delay_ms(10);
    return TRUE;
}

    
/*********************************************************************************************************
** Function name:       FM175X_SoftPowerdown
** Descriptions:        硬件低功耗操作
** input parameters:    N/A       
** output parameters:   N/A
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  FM175X_SoftPowerdown(void)
{
    if(Read_Reg(CommandReg)&0x10) {
        Clear_BitMask(CommandReg,0x10);                                 /* 退出低功耗模式               */
        return FALSE;
    }
    else
    Set_BitMask(CommandReg,0x10);                                       /* 进入低功耗模式               */
    return TRUE;
}

/*********************************************************************************************************
** Function name:       FM175X_HardPowerdown
** Descriptions:        硬件低功耗操作
** input parameters:    N/A       
** output parameters:   N/A
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  FM175X_HardPowerdown(void)
{    
    //NPD=~NPD;
    //if(NPD==1)                                                          /* 进入低功耗模式               */
    return TRUE;                                
//    else
    //return FALSE;                                                     /* 退出低功耗模式               */
}

/*********************************************************************************************************
** Function name:       Read_Ext_Reg
** Descriptions:        读取扩展寄存器
** input parameters:    reg_add，寄存器地址；         
** output parameters:   reg_value，寄存器数值
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  Read_Ext_Reg(uint8_t  reg_add)
{
    Write_Reg(0x0F,0x80+reg_add);
    return Read_Reg(0x0F);
}

/*********************************************************************************************************
** Function name:       Write_Ext_Reg
** Descriptions:        写入扩展寄存器
** input parameters:    reg_add，寄存器地址；
**                      reg_value，寄存器数值
** output parameters:   
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  Write_Ext_Reg(uint8_t  reg_add,uint8_t  reg_value)
{
    Write_Reg(0x0F,0x40+reg_add);
    return (Write_Reg(0x0F,0xC0+reg_value));
}

uint8_t  FM175XX_Initial(void)
{
    uint8_t  regdata,res;
    
    regdata = 0x20;         //WaterLevel，收到一半数据时候，起中断
    res = spi_SetReg(WaterLevelReg,regdata);
    if(res != TRUE)
    return FALSE;
    
    return TRUE;
}








