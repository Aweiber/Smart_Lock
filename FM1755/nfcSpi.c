#include "include.h"
#include "nfcSpi.h"

#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    


/*
 * 移植本软件包时需要修改以下的函数或宏 
 */
#define     SSEL_EN         (0 << 16)
#define     SSEL_DIS        (1 << 16)
#define     EOT_EN          (1 << 20)
#define     EOT_DIS         (0 << 20)
#define     EOF_EN          (1 << 21)
#define     EOF_DIS         (0 << 21)
#define     RXIGNORE_EN     (1 << 22)
#define     RXIGNORE_DIS    (0 << 22)
#define     FLEN(n)         (((n) - 1) << 24)

#define TRUE  1
#define FALSE 0


/*********************************************************************************************************
** 函数名称: SendRecv_Byte
** 函数功能：一次传输内部的单字节帧发送与接收
** 输入参数: 数据线上发送出的数据
** 输出参数: 数据线上接收到的数据
** 返 回 值：无
*********************************************************************************************************/
uint8_t SPI_SendByte (uint32_t ucData){
	 return SPI_NFC_SendByte(ucData);
}


/*********************************************************************************************************
** Function name:       spi_SetReg
** Descriptions:        SPI写读卡芯片寄存器函数
** input parameters:    ucRegAddr：寄存器地址
**                      ucRegVal：要写入的值
** output parameters:   无
** Returned value:      TRUE
*********************************************************************************************************/
uint8_t spi_SetReg(uint8_t ucRegAddr, uint8_t ucRegVal)
{
    CD_EnNSS ();
    SPI_SendByte ( FLEN(8)  | (ucRegAddr<<1) );                         /* 8 位，帧结束                 */ 
      SPI_SendByte ( FLEN(8) | EOF_EN | ucRegVal );
    CD_DisNSS ();
    return (uint8_t)1;//return ture
}

/*********************************************************************************************************
** Function name:       spi_GetReg
** Descriptions:        SPI读读卡芯片寄存器函数
** input parameters:    ucRegAddr：寄存器地址
** output parameters:   无
** Returned value:      目标寄存器的值
*********************************************************************************************************/
uint8_t spi_GetReg(uint8_t ucRegAddr)
{
    unsigned char ucRegVal;
      CD_EnNSS();
    SPI_SendByte ( FLEN(8)  | 0x80|(ucRegAddr<<1) );
      ucRegVal = SPI_SendByte ( FLEN(8) | EOF_EN  | 0x00 );
      CD_DisNSS();
    return ucRegVal;
}


/*********************************************************************************************************
** Function name:       spi_GetReg
** Descriptions:        SPI读读卡芯片寄存器函数
** input parameters:    ucRegAddr：寄存器地址
** output parameters:   无
** Returned value:      目标寄存器的值
*********************************************************************************************************/
uint8_t spi_GetReg2(uint8_t ucRegAddr,uint8_t *p)
{
      CD_EnNSS ();
			SPI_SendByte ( FLEN(8)  | 0x80|(ucRegAddr<<1) );
      *p = SPI_SendByte ( FLEN(8) | EOF_EN  | 0x00 );
      CD_DisNSS ();
			return TRUE;
}

/*********************************************************************************************************
** Function name:       SPIRead_Sequence
** Descriptions:        SPI读FIFO寄存器的值
** input parameters:    sequence_length 数据长度 ucRegAddr：寄存器地址  *reg_value 数据指针
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void SPIRead_Sequence(unsigned char sequence_length,unsigned char ucRegAddr,unsigned char *reg_value)    
{
    uint8_t i;
    if (sequence_length==0)
    return;
    CD_EnNSS ();
    for(i=0;i<sequence_length;i++) {
       *(reg_value+i) = spi_GetReg(ucRegAddr);
    }
      CD_DisNSS ();
    return;

}

/*********************************************************************************************************
** Function name:       SPIWrite_Sequence
** Descriptions:        SPI写FIFO的值
** input parameters:    sequence_length 数据长度 
**                      ucRegAddr：寄存器地址  
**                      *reg_value 数据指针
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void SPIWrite_Sequence(unsigned char sequence_length,unsigned char ucRegAddr,unsigned char *reg_value)
{
    
    uint8_t i;
    if(sequence_length==0)
        return;
    for(i=0;i<sequence_length;i++) {
       spi_SetReg(ucRegAddr, *(reg_value+i));
    }
    CD_DisNSS ();
    return ;    
}



