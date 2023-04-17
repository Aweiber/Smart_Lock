#include "include.h"
#include "nfcSpi.h"

#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    


/*
 * ��ֲ�������ʱ��Ҫ�޸����µĺ������ 
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
** ��������: SendRecv_Byte
** �������ܣ�һ�δ����ڲ��ĵ��ֽ�֡���������
** �������: �������Ϸ��ͳ�������
** �������: �������Ͻ��յ�������
** �� �� ֵ����
*********************************************************************************************************/
uint8_t SPI_SendByte (uint32_t ucData){
	 return SPI_NFC_SendByte(ucData);
}


/*********************************************************************************************************
** Function name:       spi_SetReg
** Descriptions:        SPIд����оƬ�Ĵ�������
** input parameters:    ucRegAddr���Ĵ�����ַ
**                      ucRegVal��Ҫд���ֵ
** output parameters:   ��
** Returned value:      TRUE
*********************************************************************************************************/
uint8_t spi_SetReg(uint8_t ucRegAddr, uint8_t ucRegVal)
{
    CD_EnNSS ();
    SPI_SendByte ( FLEN(8)  | (ucRegAddr<<1) );                         /* 8 λ��֡����                 */ 
      SPI_SendByte ( FLEN(8) | EOF_EN | ucRegVal );
    CD_DisNSS ();
    return (uint8_t)1;//return ture
}

/*********************************************************************************************************
** Function name:       spi_GetReg
** Descriptions:        SPI������оƬ�Ĵ�������
** input parameters:    ucRegAddr���Ĵ�����ַ
** output parameters:   ��
** Returned value:      Ŀ��Ĵ�����ֵ
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
** Descriptions:        SPI������оƬ�Ĵ�������
** input parameters:    ucRegAddr���Ĵ�����ַ
** output parameters:   ��
** Returned value:      Ŀ��Ĵ�����ֵ
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
** Descriptions:        SPI��FIFO�Ĵ�����ֵ
** input parameters:    sequence_length ���ݳ��� ucRegAddr���Ĵ�����ַ  *reg_value ����ָ��
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:        SPIдFIFO��ֵ
** input parameters:    sequence_length ���ݳ��� 
**                      ucRegAddr���Ĵ�����ַ  
**                      *reg_value ����ָ��
** output parameters:   ��
** Returned value:      ��
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



