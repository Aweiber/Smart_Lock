#include "finger.h"
#include "stm32f0xx_it.h"

uint8_t connect_statues = CONNECT_FAIL;
uint8_t Finger_Power_State = FINGER_POWER_OFF;
uint8_t Finger_Add_State_IT = 0;

uint8_t OpenDoorState = 0;

UART_HandleTypeDef huart2;

void Finger_Uart2_Init(void)
{
	huart2.Instance 	     = USART2;
	huart2.Init.BaudRate   = 57600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits   = UART_STOPBITS_1;
	huart2.Init.Parity	   = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	huart2.Init.Mode	     = UART_MODE_TX_RX;
	if(HAL_UART_DeInit(&huart2) != HAL_OK)
	{
	  Error_Handler();
	}
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
	
	HAL_UART_Receive_IT(&huart2, &finger_recv, 1);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_ERR);
}

void Finger_Send_CMD(uint8_t command, uint8_t lenth, uint8_t *Send_Buf_Host)
{
	uint8_t *p;
	p = Send_Buf_Host;
	
	uint8_t Cmd_Send_Bufer[24] = {0};
	uint8_t cnt = 0;
	uint16_t check_sum = 0;

	Cmd_Send_Bufer[0] 		= 0xBB;
	Cmd_Send_Bufer[1] 		= 0xAA;
	Cmd_Send_Bufer[2]   	= 0x00;
	Cmd_Send_Bufer[3]  	  = command;
	Cmd_Send_Bufer[4] 	  = 0x00;
	Cmd_Send_Bufer[5] 	  = lenth;

	for (int i = 6; i < 22; i++) 
  	{
 	    Cmd_Send_Bufer[i] = *p++;
  	}

	for(cnt=0; cnt < 22; cnt++){
		check_sum += Cmd_Send_Bufer[cnt];
	}
	Cmd_Send_Bufer[22] = (check_sum & 0xFF);
	Cmd_Send_Bufer[23] = (check_sum >> 8) & 0xFF;	
	
	HAL_UART_Transmit(&huart2, Cmd_Send_Bufer, sizeof(Cmd_Send_Bufer), 50);
}

//每收到一位数据就存入
#if 1
uint8_t finger_rx_bufer[255] = {0};
uint8_t finger_in_bufer[255] = {0}; 
uint8_t *finger_p_input = finger_in_bufer;
uint8_t *finger_p_output = finger_in_bufer;

void Finger_Input_Data(uint8_t finger_data)
{
	if (finger_p_input >= (finger_in_bufer + sizeof(finger_in_bufer)))
	{
		finger_p_input = finger_in_bufer;
	}
	*finger_p_input++ = finger_data;
}

uint8_t Finger_OutPut_Data(void)
{
	uint8_t value = 0;
	if (finger_p_input != finger_p_output)
	{
		if (finger_p_output >= (finger_in_bufer + sizeof(finger_in_bufer)))
		{
			finger_p_output = finger_in_bufer;
		}
		value = *finger_p_output++;
	}
	return value;
}

void fingerService(void)
{
	static uint8_t finger_index = 0;
	uint8_t finger_offset = 0;
	uint16_t check_sum = 0;
	while ((finger_p_output != finger_p_input) && (finger_index < sizeof(finger_rx_bufer)))
	{
		finger_rx_bufer[finger_index ++] = Finger_OutPut_Data();
	}
	if (finger_index < 24)
	{
		return;
	}
	while (((finger_index - finger_offset) >= 24) && (finger_index > finger_offset))
	{
		if (finger_rx_bufer[finger_offset+0] != 0xBB)
		{
			finger_offset ++;
			continue;
		}
		if (finger_rx_bufer[finger_offset+1] != 0xAA)
		{
			finger_offset ++;
			continue;			
		}
		for (uint8_t i = finger_offset; i < finger_offset + 22; i++)
		{
			check_sum += finger_rx_bufer[i];
		}
		if ((finger_rx_bufer[finger_offset + 22] != (check_sum & 0xFF)) || (finger_rx_bufer[finger_offset + 23] != ((check_sum >> 8) & 0xFF)))
		{
			finger_offset += 2;
			continue;
		}
		printf("处理一帧数据:");
		for (uint8_t i = finger_offset; i < finger_offset+24; i++)
	  {
			printf("0x%x ", finger_rx_bufer[i]);
	  }
		printf("\r\n");
		Finger_Recevie_Service(finger_rx_bufer, finger_offset);
		finger_offset += 24;
	}
	if (finger_index > finger_offset)
	{
		finger_index -= finger_offset;
		memcpy(finger_rx_bufer, finger_rx_bufer+finger_offset, finger_index);
	}
	else
	{
		finger_index = 0;
		finger_offset = 0;
	}
}
#endif

Finger_Data finger_state;

void finger_for_delete(uint8_t *recevie_buf, uint8_t index)
{
	printf("收到  0x%x ", recevie_buf[index + 6]);
	if (recevie_buf[index + 6] == 0x00)
	{
		finger_state.Delete_Finger_State = DELETE_FINGER_SUCCESS;
	}
	else if (recevie_buf[index + 6] == 0x01)
	{
		finger_state.Delete_Finger_State = DELETE_FINGER_FAIL;
	}	
}

void finger_for_factory(uint8_t *recevie_buf, uint8_t index)
{
	if (recevie_buf[index + 6] == 0x00)
	{
		finger_state.Factory_Finger_State = FINGER_FACTORY_SUCCESS;
	}
	else if (recevie_buf[index + 6] == 0x01)
	{
		finger_state.Factory_Finger_State = FINGER_FACTORY_FAIL;
	}
}

void finger_for_add(uint8_t *recevie_buf, uint8_t index)
{
	printf("收到  0x%x \n", recevie_buf[index + 6]);
	if (recevie_buf[index + 6] == 0x20)
	{
		finger_state.Add_Finger_State = PLEASE_FINGER;
	}
	else if ((recevie_buf[index + 6] == 0x21) && (recevie_buf[index + 7] != 0x02))
	{
		finger_state.Add_Finger_State = PLEASE_FINGER_AGAING;
	}
	else if (recevie_buf[index + 6] == 0x00)
	{
		finger_state.Add_Finger_State = ADD_FINGER_SUCCESS;
	}
	else if ((recevie_buf[index + 6] == 0x01)&&(recevie_buf[index + 7] == 0x08))
	{
		finger_state.Add_Finger_State = ADD_FINGER_FAIL;
	}
	else if (recevie_buf[index + 6] == 0x01)
	{
		finger_state.Add_Finger_State = FINGER_FAIL;
	}	
}

void finger_for_vertiy(uint8_t *recevie_buf, uint8_t index)
{
	printf("验证结果指令 = 0x%x\n", recevie_buf[index + 6]);
	if ((recevie_buf[index + 6] == 0x20) || (recevie_buf[index + 6] == 0x21))
	{
		return;
	}
	Lusers.APMData.Pwd_Index = 0;
	if ((flash_list.finger_user_num == 0) && (flash_list.pwd_user_num == 0))
	{
		printf("没有管理员，体验模式\n");
		Lusers.Application.Voice_Play(0x0C);
		Lusers.Application.OLED_Show(OLEDNOTHAVEADMIN);
	  Lusers.Application.ONOFFLed(LED_OFF);
		Lusers.Application.Motor_Play(MOTOR_OPEN);
		delay_ms(3000);
		Lusers.Application.Motor_Play(MOTOR_CLOSE);
		Lusers.Application.OLED_Show(OLEDCLEAR);
		Lusers.APMData.Show_Page = OLEDCLEAR;
	}
	else
	{
		if(recevie_buf[index + 6] == 0x00)
		{
			printf("验证通过\n");
			if ((Lusers.APMData.Show_Page != OLEDADMIN) && (Lusers.APMData.Show_Page != OLEDADMININPUTPWD))
			{
				Lusers.APMData.test_error = 0;
				if ((open_list.open_mode != 1) || (OpenDoorState != 0))
				{
					OpenDoorState = 0;
					Lusers.APMData.Open_Finger_Userc = recevie_buf[index + 7];
					Lusers.Application.ONOFFLed(LED_OFF);
					Lusers.Application.Voice_Play(0x26);
					Lusers.Application.OLED_Show(OLEDVSUCCESS);
					
					Lusers.Application.GetTime();
					if (open_list.open_number >= 200)
					{
						open_list.open_number = 0;
					}
					open_list.open_bufer[open_list.open_number][0] = FINGEROPEN;
					open_list.open_bufer[open_list.open_number][1] = Lusers.APMData.Open_Finger_Userc;
					open_list.open_bufer[open_list.open_number][2] = hymd.Year;
					open_list.open_bufer[open_list.open_number][3] = hymd.Month;
					open_list.open_bufer[open_list.open_number][4] = hymd.Date;
					open_list.open_bufer[open_list.open_number][5] = hhms.Hours;
					open_list.open_bufer[open_list.open_number][6] = hhms.Minutes;
					open_list.open_bufer[open_list.open_number][7] = hhms.Seconds;
					open_list.open_number ++;
					Lusers.Application.FLASH_WPlay(OPENFLASH);
					printf("存入时间20%d-%d-%d", open_list.open_bufer[open_list.open_number - 1][2],open_list.open_bufer[open_list.open_number - 1][3],open_list.open_bufer[open_list.open_number - 1][4]);
					
          Lusers.Application.Motor_Play(MOTOR_OPEN);
					delay_ms(4000);
					mcu_dp_enum_update(DPID_CLOSED_OPENED, 1);
					delay_ms(1000);
					mcu_dp_enum_update(DPID_CLOSED_OPENED, 2);
					delay_ms(100);
          Lusers.Application.Motor_Play(MOTOR_CLOSE);
					Lusers.Application.OLED_Show(OLEDCLEAR);
          Lusers.APMData.Show_Page = OLEDCLEAR;
          mcu_dp_value_update(DPID_UNLOCK_FINGER_VEIN, Lusers.APMData.Open_Finger_Userc, NEED_TIME_UPDATA);
					if (Lusers.APMData.wifi_risi <= 5) //同步指静脉开门记录，最多同步十条记录
					{
						if (finger_state.finger_open_index >= 10)
						{
							finger_state.finger_open_index = 0;
						}
						finger_state.finger_open_buf[finger_state.finger_open_index ++] = Lusers.APMData.Open_Finger_Userc;
					}
				}
				else
				{
					OpenDoorState = 1;
					Lusers.APMData.Open_Finger_Userc = recevie_buf[index + 7];
					Lusers.Application.ONOFFLed(LED_OFF);
					Lusers.Application.Voice_Play(0x26);
					Lusers.Application.OLED_Show(OLEDVSUCCESS);
					delay_ms(1000);
					Lusers.Application.ONOFFLed(LED_ON);
					Lusers.Application.OLED_Show(OLEDINPUTPWD);
					Lusers.APMData.Show_Page = OLEDINPUTPWD;
				}
			}
			else
			{
				if(recevie_buf[index + 7] <= 0x05)
				{
					Lusers.APMData.Open_Finger_Userc = recevie_buf[index + 7];
					Lusers.Application.ONOFFLed(LED_OFF);
					Lusers.Application.Voice_Play(0x26);
					Lusers.Application.OLED_Show(OLEDVSUCCESS);
					LED1_ON;
					delay_ms(1000);
					Lusers.APMData.Show_Page = OLEDMENU1;
				}
				else
				{
					Lusers.Application.ONOFFLed(LED_OFF);
					Lusers.Application.Voice_Play(0x25);
					Lusers.Application.OLED_Show(OLEDVFAIL);
					delay_ms(1000);					
					Lusers.APMData.Show_Page = OLEDADMIN;				
				}
			}
	}
	else if(recevie_buf[index + 6] == 0x01)
	{
		printf("验证失败\n");
		Lusers.APMData.test_error ++;
		if (Lusers.APMData.test_error >= TEST_ERROR_MAX)
		{
			printf("试错报警");
			Lusers.Application.Voice_Play(0x28);
			Lusers.Application.ONOFFLed(LED_OFF);
			Lusers.APMData.test_error = 0;
			Lusers.APMData.test_error_time = TEST_ERROR_TIME;
			if (Lusers.APMData.wifi_risi >= 5)
			{
				mcu_dp_enum_update(DPID_ALARM_LOCK, 0);
			}
		}
		else
		{
			OpenDoorState = 0;
			Lusers.Application.ONOFFLed(LED_OFF);
			Lusers.Application.Voice_Play(0x25);
			Lusers.Application.OLED_Show(OLEDVFAIL);
			if (Lusers.APMData.Show_Page != OLEDADMIN)
			{
				delay_ms(1000);
				Lusers.Application.OLED_Show(OLEDMAIN);
				Lusers.APMData.Show_Page = OLEDMAIN;
			}
			else
			{
				delay_ms(1000);
				Lusers.APMData.Show_Page = OLEDADMIN;
			}
	   }
	 }
  }
}

void Finger_Recevie_Service(uint8_t *recevie_buf, uint8_t index)
{
	uint8_t finger_cmd = 0;
	finger_cmd = recevie_buf[index + 3];
		switch (finger_cmd)
		{
			case 0x01:
			  printf("连接成功\n");
			  connect_statues = CONNECT_SUCCESS;
			  break;
			case 0x11:
				finger_for_delete(recevie_buf, index);
				break;
			case 0x12:
				finger_for_factory(recevie_buf, index);
				break;
			case 0x14:
				printf("查询指静脉用户数量\n");
			    flash_list.finger_user_num = recevie_buf[index + 7];
			    printf("指静脉用户数量为 ：%d\n", flash_list.finger_user_num);
				break;
			case 0x16:
				finger_for_add(recevie_buf, index);
				break;
			case 0x17:
			  finger_for_vertiy(recevie_buf, index);
				break;
			case 0x19:
				printf("中断当前指令成功\n");
				break;
			default :
			break;
		}
}

void Finger_Inter_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();	
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin   = FINTER_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(FINTER_PORT, &GPIO_InitStruct);
	
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 2 , 1);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void Finger_Power_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = FPOWER_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(FPOWER_PORT, &GPIO_InitStruct);	
}

void Finger_Power_OnOff(uint8_t State)
{
	if ((State == FINGER_POWER_ON) && (Finger_Power_State == FINGER_POWER_OFF))
	{
		printf("开启指静脉电源\n");
		HAL_GPIO_WritePin(FPOWER_PORT, FPOWER_PIN, GPIO_PIN_SET);
		Finger_Power_State = FINGER_POWER_ON; 
	}
	else if ((State == FINGER_POWER_OFF) && (Finger_Power_State == FINGER_POWER_ON))
	{
		printf("关闭指静脉电源\n");
		HAL_GPIO_WritePin(FPOWER_PORT, FPOWER_PIN, GPIO_PIN_RESET);
		connect_statues = CONNECT_FAIL;
		Finger_Power_State = FINGER_POWER_OFF;
	}
}

void Finger_Send_Connect(void)
{
	if ((connect_statues == CONNECT_FAIL)&&(Finger_Power_State == FINGER_POWER_ON))
	{
		printf("发送连接指令\n");
		uint8_t Bufer_Host[16] = {0};
		for (int i = 0; i < 8; i++)
		{
			Bufer_Host[i] = 0x30;
		}	
		Finger_Send_CMD(0x01, 0x08, Bufer_Host);		
  }	
}

void Finger_Send_1vN(void)
{
	uint8_t Bufer_Host[16] = {0};
	Finger_Send_CMD(0x17, 0x04, Bufer_Host);
}

void Finger_Send_Add_Figner(uint8_t UserID)
{
	finger_state.Add_Finger_State = 6;
	Finger_Add_State_IT = 1;
	uint8_t Bufer_Host[16] = {0};
	Bufer_Host[0]  = UserID;
	Bufer_Host[5]  = 0x03;
	Bufer_Host[10] = 0x05;
	Finger_Send_CMD(0x16, 0x04, Bufer_Host);
	delay_ms(300);
	fingerService();
}

void Finger_Factory(void)
{
	finger_state.Factory_Finger_State = 3;
	uint8_t Bufer_Host[16] = {0};
	Finger_Send_CMD(0x12, 0x00, Bufer_Host);
  delay_ms(300);
	fingerService();
}

void Finger_Break_Cmd(void)
{
	uint8_t Bufer_Host[16] = {0};
	Finger_Send_CMD(0x19, 0x00, Bufer_Host);
}

void Finger_Delete(uint8_t UserID)
{
	finger_state.Delete_Finger_State = 2;
	uint8_t Bufer_Host[16] = {0};
	Bufer_Host[0] = UserID;
	Finger_Send_CMD(0x11, 0x04, Bufer_Host);
	delay_ms(300);
	fingerService();
}

void finger_get_info(void)
{
	uint8_t Bufer_Host[16] = {0};
	Finger_Send_CMD(0x14, 0x00, Bufer_Host);
}

void Finger_Show_Play(uint8_t finger_data, uint8_t UserID)
{
	if (finger_data == FINGER_CONNECT)
	{
		Finger_Send_Connect();
	}
	else if (finger_data == FINGER_VERITY)
	{
		Finger_Send_1vN();
	}
	else if (finger_data == FINGER_ADD)
	{
		Finger_Send_Add_Figner(UserID);
	}
	else if (finger_data == FINGER_FACTORY)
	{
		Finger_Factory();
	}
	else if (finger_data == FINGER_BREAK)
	{
		Finger_Break_Cmd();
	}
	else if (finger_data == FINGER_DELETE)
	{
		Finger_Delete(UserID);
	}
	else if (finger_data == FINGER_GETINFO)
	{
		finger_get_info();
	}	
}
