#ifndef __BSP_SPINFC_H__
#define __BSP_SPINFC_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "include.h"

#define NFC_SPIx                                 SPI1
#define NFC_SPIx_RCC_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define NFC_SPIx_RCC_CLK_DISABLE()               __HAL_RCC_SPI1_CLK_DISABLE()

#define NFC_SPI_GPIO_ClK_ENABLE()                __HAL_RCC_GPIOA_CLK_ENABLE() 
#define NFC_SPI_GPIO_PORT                        GPIOA
#define NFC_SPI_SCK_PIN                          GPIO_PIN_5
#define NFC_SPI_MISO_PIN                         GPIO_PIN_6
#define NFC_SPI_MOSI_PIN                         GPIO_PIN_7
				
#define NFC_SPI_CS_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()    
#define NFC_SPI_CS_PORT                          GPIOA
#define NFC_SPI_CS_PIN                           GPIO_PIN_11

#define NFC_SPI_CS_ENABLE()                      HAL_GPIO_WritePin(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN, GPIO_PIN_RESET)
#define NFC_SPI_CS_DISABLE()                     HAL_GPIO_WritePin(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN, GPIO_PIN_SET)

#define NFC_SPI_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()

#define SPI2_MISO_GPIO_PORT              GPIOA
#define SPI2_MOSI_GPIO_PORT              GPIOA
#define SPI2_SCK_GPIO_PORT               GPIOA

#define SPI2_SCK_AF                      GPIO_AF0_SPI1
#define SPI2_MISO_AF                     GPIO_AF0_SPI1
#define SPI2_MOSI_AF                     GPIO_AF0_SPI1

/* 扩展变量 ------------------------------------------------------------------*/
extern SPI_HandleTypeDef hspinfc;

/* 函数声明 ------------------------------------------------------------------*/

uint8_t MX_SPINfc_Init(void);
uint8_t SPI_NFC_SendByte(uint8_t byte);
void SPI_WakeUp(void);
void nfc_interrupt_init(void);
#endif  /* __BSP_SPIFLASH_H__ */

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
