#include "flash.h"

flash_list_t flash_list;
open_list_t open_list;

void Flash_Init(void)
{
	flash_list.finger_user_num = 0;
	flash_list.pwd_user_num = 0;
	flash_list.nfc_user_num = 0;
	open_list.open_mode = 0;
	open_list.open_number = 0;
	memset(flash_list.finger_user_bufer, '\0', sizeof(flash_list.finger_user_bufer));
	memset(flash_list.pwd_user_bufer, '\0', sizeof(flash_list.pwd_user_bufer));
	memset(flash_list.pwd_user_pwd_bufer, '\0', sizeof(flash_list.pwd_user_pwd_bufer));
	memset(flash_list.nfc_user_bufer, '\0', sizeof(flash_list.nfc_user_bufer));
	memset(flash_list.nfc_user_nfc_bufer, '\0', sizeof(flash_list.nfc_user_nfc_bufer));
	memset(flash_list.time_bufer, '\0', sizeof(flash_list.time_bufer));
	memset(open_list.open_bufer, '\0', sizeof(open_list.open_bufer));
	flash_list.time_bufer[1] = 1;
	flash_list.time_bufer[2] = 1;
}

uint32_t Flash_Read_Word(uint32_t faddr)
{
	return *(__IO uint32_t *)faddr;
}

void Flash_ReadData(uint8_t ReadADDress, uint32_t *Rbufer, uint16_t NumTORead)
{
	uint32_t FlashADDress = 0;
	FlashADDress = ReadADDress * 1024 + 0x08000000;
	
	for (int i = 0; i < NumTORead; i++)
	{
		*(Rbufer + i) = Flash_Read_Word(FlashADDress + 4*i);
	}
}

void Flash_WriteData(uint8_t WriteADDress, uint32_t *Wbufer, uint16_t Size)
{
	uint32_t Page_Error = 0;
	uint32_t FlashADDress = WriteADDress * 1024 + 0x08000000;
	uint32_t Pdata = 0;
	
	FLASH_EraseInitTypeDef Flash;
	
	HAL_FLASH_Unlock();
	
	__disable_irq();
	
	Flash.TypeErase   = FLASH_TYPEERASE_PAGES;
	Flash.PageAddress = FlashADDress;
	if(WriteADDress == DATAFLASH)
	{
	  Flash.NbPages = 1;
	}
	else
	{
		Flash.NbPages = 2;
	}
	
	if (HAL_FLASHEx_Erase(&Flash, &Page_Error) == HAL_OK)
	{
		printf("擦除成功\n");
	}
	__enable_irq();
	for (int i = 0; i < Size; i++)
	{
		Pdata = *(Wbufer + i);
	  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FlashADDress + 4*i, Pdata); //FLash地址每次偏移4字节
	}
	
	
	HAL_FLASH_Lock();
}

#define FLASHREMAINDER   (sizeof(flash_list)%4 > 0 ? 1 : 0)
#define OPENREMAINDER    (sizeof(open_list)%4 > 0 ? 1 : 0) 	//三目运算必须加括号，否则会导致先乘除加减后再进行三目运算

void FLASH_ReadPlay(uint8_t FLASHState)
{
	if (FLASHState == DATAFLASH)
	{
		Flash_ReadData(DATA_FALSH_DRESS, (uint32_t *)&flash_list, sizeof(flash_list)/4 + FLASHREMAINDER);
	}
	else if (FLASHState == OPENFLASH)
	{
		Flash_ReadData(OPEN_DRESS, (uint32_t *)&open_list, sizeof(open_list)/4 + OPENREMAINDER);
	}
}

void FLASH_WritePlay(uint8_t FLASHState)
{
	if (FLASHState == DATAFLASH)
	{
		Flash_WriteData(DATA_FALSH_DRESS, (uint32_t *)&flash_list, sizeof(flash_list)/4 + FLASHREMAINDER);
	}
	else if (FLASHState == OPENFLASH)
	{
		Flash_WriteData(OPEN_DRESS, (uint32_t *)&open_list, sizeof(open_list)/4 + OPENREMAINDER);
	}
}

uint8_t *TUYA_WiFi_Zone_Page(uint8_t *inputbuf, uint8_t *len)
{
	uint8_t *p;
	uint8_t lenth;
	uint8_t ZonePage_Buf[26];   //定义26位数组，其中有13个分片，byte[0]代表分片位置，byte[1]代表分片内容
	memset(ZonePage_Buf, 0, sizeof(ZonePage_Buf));
	p = ZonePage_Buf;
	lenth = *len;
	*len = 0;
	for (uint8_t j = 0; j < lenth/8+1; j++)
	{
		for (uint8_t i = 0; i < ((lenth - 8*j)>=8?8:(lenth-8*j)); i++)
		{
			if (*(inputbuf+i+8*j) == 1)
			{
				if (i == 7)                     //不上传第0位编号，所有编码往后移一位
				{                               //模组及app上有第0位编码，锁具本地没有，锁具本地编码皆从01开始
					if ((ZonePage_Buf[*len] != 0) && (ZonePage_Buf[*len+1] != 0)) //如果当前位有数据就往后移一位
					{
						ZonePage_Buf[*len+2] = j+2;
						ZonePage_Buf[*len+3] = 1;
					}
					else   //没有数据就存入当前位                                         
					{
						ZonePage_Buf[*len] = j+2;
						ZonePage_Buf[*len+1] = 1;
					}
				}
				else
				{
				  ZonePage_Buf[*len] = j+1;
				  ZonePage_Buf[*len+1] |= (2<<i); //数据存入分片	
				}
			}
		}
		if (ZonePage_Buf[*len+1] != 0)    //该分片里有数据
		{
			*len = *len + 2;              //几个分片里有数据上报至模组的数据长度就为分片*2
		}
  }
	printf("pwd_lenth = %d\n", *len);
	if (*len == 0)
	{
		*len = 2;
	}
	return p;
}


