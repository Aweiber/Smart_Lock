#ifndef __nfcSpi_h
#include "include.h"

void SPIWrite_Sequence(unsigned char sequence_length,unsigned char ucRegAddr,unsigned char *reg_value);
void SPIRead_Sequence(unsigned char sequence_length,unsigned char ucRegAddr,unsigned char *reg_value) ;
uint8_t spi_GetReg2(uint8_t ucRegAddr,uint8_t *p);
uint8_t spi_GetReg(uint8_t ucRegAddr);
uint8_t spi_SetReg(uint8_t ucRegAddr, uint8_t ucRegVal);
uint8_t SPI_SendByte (uint32_t ucData);
void  nfcSpiInit (void);


#define  CD_EnNSS()  			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET)
#define CD_DisNSS()     	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET)


#endif
