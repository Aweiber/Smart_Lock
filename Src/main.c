#include "main.h"
#include "include.h"
#include "stm32f0xx_it.h"

void SystemClock_Config(void);

//#define USART2_DEBUG
int fputc(int c, FILE *f)
{
#ifdef USART2_DEBUG
//	HAL_UART_Transmit(&huart1, (uint8_t *)&c, 1, 1000);
#endif
	return c;
}

/*
   define USER
*/

uint16_t GetPowerData = 0; 

APM32_Users Lusers;
/***������ʱ����ʱ��***/
void encrytime_func(uint8_t *timebuf, uint8_t *outbuf)
{
	uint8_t savebuf[6]={0};
	for(uint8_t i=0;i<5;i++)
	{
		savebuf[i] = timebuf[i];
	}
	savebuf[4]=timebuf[4]/10;
	for(uint8_t i=0;i<5;i++)
	{
		savebuf[5] = savebuf[5]^(savebuf[i]+i+0x41);
	}
	
	savebuf[5] = savebuf[5]>100?savebuf[5]%100:savebuf[5];
	
	for(uint8_t i=0;i<6;i++)
	{
		for(uint8_t j=0;j<6;j++)
		{
			 outbuf[i] = outbuf[i]^(savebuf[j]+((i+1)*3*(savebuf[j]%10)));
		}      
	}
	
	printf("ʱ�����Ϊ��");
	for(uint8_t i = 0;i<6;i++)
	{
		printf("%d,", outbuf[i]);
	}
	printf("\r\n");
	
}
/***���ܹ���Ա����***/
void Encry_TemPwd(uint8_t *savepwdbuf)
{
	uint8_t encry_pwdbuf[6] = {0};
	for(uint8_t i = 0; i < 6; i++)
	{
		encry_pwdbuf[i] = flash_list.pwd_user_pwd_bufer[0][i];
	}
	encry_pwdbuf[5] = encry_pwdbuf[5]>100?encry_pwdbuf[5]%100:encry_pwdbuf[5];
	for(uint8_t i=0;i<6;i++)
	{
		for(uint8_t j=0;j<6;j++)
		{
			 savepwdbuf[i] = savepwdbuf[i]^(encry_pwdbuf[j]+((i+1)*3*(encry_pwdbuf[j]%10)));
		}      
	}
	
	printf("���ܹ���Ա����Ϊ��");
	for(uint8_t i=0;i<6;i++)
	{
		printf("%d,", savepwdbuf[i]);
	}
	printf("\r\n");	
}

/***У����ʱ����***/
void Verity_TemPwd(uint8_t *inputtimebuf, uint8_t *encryadminpwd, uint8_t *savebuf)
{
	uint8_t tensbuf[6] = {0};
	for(uint8_t i=0;i<6;i++)
	{
		savebuf[i] = (inputtimebuf[i]^encryadminpwd[i])%10;
		tensbuf[i] = (inputtimebuf[i]^encryadminpwd[i])/10;
	}
	for(uint8_t i=0;i<6;i++)
	{
		savebuf[6] = savebuf[6]^tensbuf[i];
	}
	savebuf[6] = savebuf[6]%10;
	for(uint8_t i=0;i<7;i++)
	{
		savebuf[i] += 0x30;
	}
	
	printf("������ʱ����Ϊ��");
	for(uint8_t i=0;i<7;i++)
	{
		printf("%d,", savebuf[i]-0x30);
	}
	printf("\r\n");
}
/***�ȶ���ʱ����***/
uint8_t Compare_TemPwd(uint8_t *local_pwd, uint8_t *farpwd)
{
	for(uint8_t i=0;i<7;i++)
	{
		if(local_pwd[i] == farpwd[i])
		{
			if(i==6)
			{
				return TRUE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	return FALSE;
}

/***���������㷨�����ܺ�����Ϊ��λ������λΪ����λ������λΪ����λ��ǰ��λΪ����ԭλ***/
void Encryption_PWD(uint8_t *inputbufer, uint8_t *savebufer, uint8_t Pwd_Index)
{
	memset(savebufer, 0, 6);
	for(uint8_t i = 0; i < 4; i++)
	{
		savebufer[i] = inputbufer[i];
	}
	savebufer[4] = Pwd_Index;
	for(uint8_t i = 0; i < Pwd_Index; i++)
	{
		savebufer[5] ^= (inputbufer[i] + 'A' + i);
	}
	
	printf("���ܺ�����Ϊ��");
	for (uint8_t j = 0; j < 6; j++)
	{
		printf("0x%x  ", savebufer[j]);
	}
	printf("\r\n");
}

/***��������ȶԣ�֧����λ����***/
uint8_t Compare_PWD(uint8_t *inputbufer)
{
	uint8_t Pwd_Tmp_Bufer[10] = {0};
	uint8_t Pwd_Encryp_Bufer[6] = {0};
	
	for (uint8_t i = 0; i < 10; i++)
	{
		if (flash_list.pwd_user_bufer[i] == 1)
		{
			for (uint8_t j = 0; j < Lusers.APMData.Pwd_Index; j++)
			{
				if (flash_list.pwd_user_pwd_bufer[i][0] == Lusers.APMData.Pwd_Bufer[j])
				{
					 if (flash_list.pwd_user_pwd_bufer[i][1] == Lusers.APMData.Pwd_Bufer[j + 1])
					 {
						 if (flash_list.pwd_user_pwd_bufer[i][2] == Lusers.APMData.Pwd_Bufer[j + 2])
						 {
							 if (flash_list.pwd_user_pwd_bufer[i][3] == Lusers.APMData.Pwd_Bufer[j + 3])
							 {
								 printf("ǰ��λ��ͬ��������������м���\n");
								 printf("pwd_list.user_pwd_bufer[i][4] = 0x%x\n", flash_list.pwd_user_pwd_bufer[i][4]);
								 for (uint8_t k = 0; k < flash_list.pwd_user_pwd_bufer[i][4]; k++)
								 {
									 Pwd_Tmp_Bufer[k] = inputbufer[j + k];
									 printf("0x%x ",Pwd_Tmp_Bufer[k]);
								 }
								 printf("\r\n");
								 Encryption_PWD(Pwd_Tmp_Bufer, Pwd_Encryp_Bufer, flash_list.pwd_user_pwd_bufer[i][4]);
								 for (uint8_t m = 0; m < 6; m++)
								 {
									 if (flash_list.pwd_user_pwd_bufer[i][m] == Pwd_Encryp_Bufer[m])
									 {
										 if (m == 5)
										 {
											 return i+1;
										 }
									 }
									 else
									 {
										 break;
									 }
								 }
							 }							 
						 }						 
					 }
				}
			}
		}
	}
	return 0;
}

/***������룬�����ظ�����***/
uint8_t Find_SamePWD(uint8_t *InputBufer)
{
	for (uint8_t j = 0; j < 10; j++)
	{
		if (flash_list.pwd_user_bufer[j] == 1)
		{
			for (uint8_t k = 0; k < 6; k++)
			{
				if (flash_list.pwd_user_pwd_bufer[j][k] == InputBufer[k])
				{
					if (k == 5)
					{
						printf("�����ظ�\n");
						return 1;
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	return 0;
}


/***���ܿ����㷨����������ǰ3λ�����һλΪУ��λ***/
void Encryption_NFC(uint8_t *inputbufer, uint8_t *savebufer)
{
	memset(savebufer, 0, 4);
	for(uint8_t i = 0; i < 3; i++)
	{
		savebufer[i] = inputbufer[i];
	}
	for (uint8_t j = 0; j < 6; j++)
  {
		savebufer[3] ^= (inputbufer[j] + 'A' + j);
  }

	printf("���ܺ󿨺�Ϊ��");
	for (uint8_t j = 0; j < 4; j++)
	{
		printf("0x%x  ", savebufer[j]);
	}
	printf("\r\n");	
}

/**********�Ա���ͬ����**********/
uint8_t Find_SameNFC(uint8_t *inputbufer)
{
	for (uint8_t i = 0; i < 80; i++)
	{
		if (flash_list.nfc_user_bufer[i] == 1)
		{
			for (uint8_t j = 0; j < 4; j++)
			{
				if (flash_list.nfc_user_nfc_bufer[i][j] == inputbufer[j])
				{
					if (j == 3)
					{
						printf("�����ظ�\n");
						return 1;
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	return 0;
}

/**********�Աȿ��ſ���**********/
uint8_t Compare_NFC(uint8_t *inputbufer)
{
	for(uint8_t i = 0; i < 80; i++)
	{
		if (flash_list.nfc_user_bufer[i] == 1)
		{
			for (uint8_t j = 0; j < 4; j++)
			{
				if(flash_list.nfc_user_nfc_bufer[i][j] == inputbufer[j])
				{
					if (j == 3)
					{
						return i+1;  //��Ƭ��֤ͨ��
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	return 0;
}

void Initialization_Layer(void)
{
	Lusers.Initialization.Finger_Uart     = Finger_Uart2_Init;
	Lusers.Initialization.Delay           = delay_init;
	Lusers.Initialization.Led             = LED_Init;
	Lusers.Initialization.Finger_PowerPin = Finger_Power_Init;
	Lusers.Initialization.Finger_InterPin = Finger_Inter_Init;
	Lusers.Initialization.Voice_Pin       = WTN6_Gpio_Init;
	Lusers.Initialization.Oled            = OLED_Init;
	Lusers.Initialization.Power_Adc       = MX_ADC_Init;
	Lusers.Initialization.Motor           = Motor_Init;
	Lusers.Initialization.Touch           = Touch_Init;
	Lusers.Initialization.Flash           = Flash_Init;
	Lusers.Initialization.Rtc             = MX_RTC_Init;
	Lusers.Initialization.Tim3            = MX_TIM3_Init;
	Lusers.Initialization.Wifi            = WIFI_Init;
	
	Lusers.Initialization.Finger_Uart();
	Lusers.Initialization.Delay();
	Lusers.Initialization.Led();
	Lusers.Initialization.Finger_PowerPin();
	Lusers.Initialization.Finger_InterPin();
	Lusers.Initialization.Voice_Pin();
	Lusers.Initialization.Oled();
	Lusers.Initialization.Power_Adc();
	Lusers.Initialization.Motor();
	Lusers.Initialization.Touch();
	Lusers.Initialization.Rtc();
	Lusers.Initialization.Tim3();
	Lusers.Initialization.Wifi();
}

void Application_Layer(void)
{
	Lusers.Application.USDelay        = delay_us;
	Lusers.Application.MSDelay        = delay_ms;
	Lusers.Application.ONOFFLed       = LED_OnOff;
	Lusers.Application.ONOFFFinger    = Finger_Power_OnOff;
  Lusers.Application.Finger_Play    = Finger_Show_Play;
	Lusers.Application.Voice_Play     = WTN6_Play;
	Lusers.Application.OLED_Show      = OLED_Show_Play;
	Lusers.Application.Get_Power      = Get_Power_Value;
	Lusers.Application.Motor_Play     = Motor_OpenClose;
	Lusers.Application.FLASH_RPlay    = FLASH_ReadPlay;
	Lusers.Application.FLASH_WPlay    = FLASH_WritePlay;
	Lusers.Application.GetTime        = RTC_GET_Time;
}

void DATA_Layer_Init(void)
{
  Lusers.APMData.Inter_Data        = NOT_INTER;
  Lusers.APMData.Show_Page         = OLEDMAIN;
	Lusers.APMData.Finger_Userc      = 0;
	Lusers.APMData.Pwd_Userc         = 0;
	Lusers.APMData.Nfc_Userc         = 0;
	Lusers.APMData.Open_Finger_Userc = 0;
	Lusers.APMData.Open_Pwd_Userc    = 0;
	Lusers.APMData.Pwd_Index         = 0;
	Lusers.APMData.Input_PwdIndex    = 0;
	Lusers.APMData.Confirm_PwdIndex  = 0;
	Lusers.APMData.Audio_Index       = 2;
	Lusers.APMData.time_index        = 0;
	Lusers.APMData.Open_Record_Userc = 0;
	Lusers.APMData.OLEDReturn        = 0;
	Lusers.APMData.pick_state        = 0;
	Lusers.APMData.test_error        = 0;
	Lusers.APMData.test_error_time   = 0;
	Lusers.APMData.Indoor_time       = 0;
	Lusers.APMData.wifi_risi         = 0;
	Lusers.APMData.Start_UPState     = 0;
	Lusers.APMData.APP_Open_Time     = 0;
	Lusers.APMData.Low_Power_toWiFi  = 0;
	Lusers.APMData.Pick_toWiFi       = 0;
	Lusers.APMData.UPData_Open_Record = 0;
	finger_state.finger_open_index   = 0;
	Lusers.APMData.Temparory_pwd_state = 1;
	memset(finger_state.finger_open_buf, 0, sizeof(finger_state.finger_open_buf));
	memset(Lusers.APMData.Pwd_Bufer, 0, sizeof(Lusers.APMData.Pwd_Bufer));
	memset(Lusers.APMData.Input_PwdBufer, 0, sizeof(Lusers.APMData.Input_PwdBufer));
	memset(Lusers.APMData.Confirm_PwdBufer, 0, sizeof(Lusers.APMData.Confirm_PwdBufer));
	memset(Lusers.APMData.time_bufer, 0, sizeof(Lusers.APMData.time_bufer));
	memset(Lusers.APMData.Temparoary_Pwd_Bufer, 0, sizeof(Lusers.APMData.Temparoary_Pwd_Bufer));
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
	__HAL_RCC_PWR_CLK_ENABLE();
	
  Initialization_Layer();//��ʼ������
  Application_Layer();   //��ʼ�û�����
	DATA_Layer_Init();     //��ʼ�û�����
	Sys_Pick_Proof();      //���˳�ʼ��
	INDoor_Reset_Init();   //���ڸ�λ��ʼ��
	
	MX_SPINfc_Init();//NFC
	printf("Nfc_Status = %d\n", Nfc_Status);

        Lpcd_Set_Mode(0);//�˳�LPCD  
        Lpcd_Get_IRQ(&Lpcd.Irq);		
        Lpcd_IRQ_Event();
	
  Lusers.Initialization.Flash();
	Lusers.Application.FLASH_RPlay(DATAFLASH);
	Lusers.Application.FLASH_RPlay(OPENFLASH);
	
	printf("flash_list.finger_user_num = %d\n", flash_list.finger_user_num);
	printf("flash_list.pwd_user_num = %d\n", flash_list.pwd_user_num);
	printf("���ŷ�ʽmode = %d\n", open_list.open_mode);
	MX_Set_Ymd(flash_list.time_bufer[0], flash_list.time_bufer[1], flash_list.time_bufer[2]);
	MX_Set_Hms(flash_list.time_bufer[3], flash_list.time_bufer[4], flash_list.time_bufer[5]);
	
  Lusers.Application.OLED_Show(OLEDSTARTUP);
  Lusers.Application.MSDelay(1500);
	
  Lusers.Application.ONOFFFinger(FINGER_POWER_ON);
  Lusers.Application.MSDelay(200);
  Lusers.Application.Finger_Play(FINGER_CONNECT, 0);
  Lusers.Application.MSDelay(100);
  Lusers.Application.Finger_Play(FINGER_GETINFO, 0);
  
  Lusers.Application.Motor_Play(MOTOR_CLOSE);
	
  GetPowerData = Lusers.Application.Get_Power();
  Lusers.Application.OLED_Show(Lusers.APMData.Show_Page);

  while (1)
  {
		if ((Lusers.APMData.pick_state == 1) && (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == 0))
		{
			if (VOICE_BUSY_STATE)
			{
			  Lusers.Application.Voice_Play(0x28);
			}
			if ((Lusers.APMData.wifi_risi >= 5) && (Lusers.APMData.Pick_toWiFi == 0))
			{
				Lusers.APMData.Pick_toWiFi = 1;
				mcu_dp_enum_update(DPID_ALARM_LOCK, 8);
			}
			Lusers.Application.OLED_Show(OLEDSYSPICKPROOF);
		  Lusers.APMData.OLEDReturn = 0;
			Lusers.Application.ONOFFLed(LED_ON);
			Lusers.Application.MSDelay(300);
			Lusers.Application.ONOFFLed(LED_OFF);
			Lusers.Application.MSDelay(300);
			wifi_uart_service();
			
			if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12))
			{
				printf("�˳���������\n");
				WTN6_Send_Data(0xFE);
				Lusers.APMData.pick_state = 0;
				Lusers.APMData.Show_Page = OLEDMAIN;
			}
			continue;
		}
		if (Lusers.APMData.test_error_time)
		{
			Lusers.APMData.Show_Page = OLEDTESTERROR;
			Lusers.Application.OLED_Show(OLEDTESTERROR);
			Lusers.APMData.OLEDReturn = 0;
			Lusers.Application.MSDelay(200);
			continue;
		}
		
		if ((HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0) && (Lusers.APMData.Show_Page == OLEDMAIN))
		{
			if (Lusers.APMData.Indoor_time == 0)
			{
				Lusers.APMData.Indoor_time = 5;
			}
			else if (Lusers.APMData.Indoor_time == 1)
			{
				LED1_ON;
				LED2_OFF;
				Lusers.APMData.Show_Page = OLEDFACTORYMENU;
			}
		}
		
		if ((Lusers.APMData.Show_Page == OLEDMAIN) && (Lusers.APMData.wifi_risi >= 5) && (Lusers.APMData.Start_UPState == 0))
		{
			printf("���ߵ�һ���ϵ磬������wifi��ȡ����ʱ��\n");
			mcu_get_system_time();
			Lusers.Application.MSDelay(100);
		}
		else if ((Lusers.APMData.Show_Page == OLEDTEMPORARYWAIT) && (Lusers.APMData.wifi_risi >= 5) && (Lusers.APMData.Temparory_pwd_state == 0))
		{
			printf("�����������磬������ʱ����\n");
			mcu_get_mul_temp_pass();
			Lusers.Application.MSDelay(200);
		}
		else if ((GetPowerData <= 2230) && (Lusers.APMData.wifi_risi >= 5) && (Lusers.APMData.Low_Power_toWiFi == 0))
		{
			printf("�͵����澯\n");
			Lusers.APMData.Low_Power_toWiFi = 1;
			mcu_dp_enum_update(DPID_ALARM_LOCK, 10);
			Lusers.Application.MSDelay(200);
		}
		
    if (Nfc_Status == OK)
		{
			if ((Lpcd_Card_Event() == OK) && (Lusers.APMData.Show_Page == OLEDMAIN))
			{
				uint8_t save_nfc_bufer[4] = {0};
				Encryption_NFC(CardID, save_nfc_bufer);
				Lusers.APMData.Open_Nfc_Userc = Compare_NFC(save_nfc_bufer);
				if ((Lusers.APMData.Open_Nfc_Userc == 0) || (open_list.open_mode == 1))
				{
					printf("������֤ʧ��\n");
					Lusers.APMData.test_error ++;
					if (Lusers.APMData.test_error >= TEST_ERROR_MAX)
					{
						printf("�Դ���");
						Lusers.Application.Voice_Play(0x28);
						Lusers.Application.ONOFFLed(LED_OFF);
						Lusers.APMData.test_error = 0;
						Lusers.APMData.test_error_time = TEST_ERROR_TIME;
						if (Lusers.APMData.wifi_risi >= 5)
						{
							mcu_dp_enum_update(DPID_ALARM_LOCK, 2);
						}							
					}
					else
					{
						Lusers.Application.Voice_Play(0x25);
						Lusers.Application.OLED_Show(OLEDVFAIL);
						Lusers.Application.ONOFFLed(LED_OFF);
						Lusers.Application.MSDelay(1000);
						Lusers.APMData.Show_Page = OLEDMAIN;
					}
				}
				else
				{
					printf("������֤ͨ��\n");
					Lusers.APMData.test_error = 0;
					Lusers.Application.Voice_Play(0x26);
					Lusers.Application.OLED_Show(OLEDVNFCSUCCESS);
					Lusers.Application.ONOFFLed(LED_OFF);
					Lusers.Application.GetTime();
					if (open_list.open_number >= 200)
					{
						open_list.open_number = 0;
					}
					open_list.open_bufer[open_list.open_number][0] = NFCOPEN;
					open_list.open_bufer[open_list.open_number][1] = Lusers.APMData.Open_Nfc_Userc;
					open_list.open_bufer[open_list.open_number][2] = hymd.Year;
					open_list.open_bufer[open_list.open_number][3] = hymd.Month;
					open_list.open_bufer[open_list.open_number][4] = hymd.Date;
					open_list.open_bufer[open_list.open_number][5] = hhms.Hours;
					open_list.open_bufer[open_list.open_number][6] = hhms.Minutes;
					open_list.open_bufer[open_list.open_number][7] = hhms.Seconds;
					open_list.open_number ++;
					Lusers.Application.FLASH_WPlay(OPENFLASH);
					
          Lusers.Application.Motor_Play(MOTOR_OPEN);
          Lusers.Application.MSDelay(4000);
					mcu_dp_enum_update(DPID_CLOSED_OPENED, 1);
					Lusers.Application.MSDelay(1000);
					mcu_dp_enum_update(DPID_CLOSED_OPENED, 2);
					Lusers.Application.MSDelay(100);
          Lusers.Application.Motor_Play(MOTOR_CLOSE);
					Lusers.Application.OLED_Show(OLEDCLEAR);
          Lusers.APMData.Show_Page = OLEDCLEAR;
          mcu_dp_value_update(DPID_UNLOCK_CARD, Lusers.APMData.Open_Nfc_Userc, NEED_TIME_UPDATA);					
				}
			}
		}
		
		if (Key_Data != 13)
		{
			printf("KEY = %d\n",Key_Data);
			Lusers.Application.Voice_Play(0x10);
			if (Lusers.APMData.Show_Page == OLEDCLEAR)
			{
				Lusers.APMData.Show_Page = OLEDMAIN;
			}
			if ((Key_Data != TOUCH_GOING) && (Key_Data != TOUCH_BACK))
			{
				if ((Lusers.APMData.Show_Page == OLEDMAIN) || (Lusers.APMData.Show_Page == OLEDADMIN))
				{
					Lusers.APMData.Pwd_Index = 0;
					memset(Lusers.APMData.Pwd_Bufer, 0, sizeof(Lusers.APMData.Pwd_Bufer));
				}
				if ((Lusers.APMData.Show_Page == OLEDMAIN) || (Lusers.APMData.Show_Page == OLEDINPUTPWD))
				{
					printf("��������������\n");
					Lusers.Application.ONOFFLed(LED_ON);
					Lusers.APMData.Pwd_Bufer[Lusers.APMData.Pwd_Index ++] = Key_Data + 0x30;
					printf("Lusers.APMData.Pwd_Index = %d\n",Lusers.APMData.Pwd_Index);
					if (Lusers.APMData.Pwd_Index >= 30)
					{
						printf("��������\n");
						Lusers.Application.Voice_Play(0x23);
						Lusers.APMData.Pwd_Index = 0;
						memset(Lusers.APMData.Pwd_Bufer, 0, sizeof(Lusers.APMData.Pwd_Bufer));
					}
					Lusers.APMData.Show_Page = OLEDINPUTPWD;
				}
				else if ((Lusers.APMData.Show_Page == OLEDADMIN) || (Lusers.APMData.Show_Page == OLEDADMININPUTPWD))
				{
					printf("����Ա��������\n");
					Lusers.APMData.Pwd_Bufer[Lusers.APMData.Pwd_Index ++] = Key_Data + 0x30;
					printf("Lusers.APMData.Pwd_Index = %d\n",Lusers.APMData.Pwd_Index);
					if (Lusers.APMData.Pwd_Index >= 30)
					{
						printf("��������\n");
						Lusers.Application.Voice_Play(0x23);
						Lusers.APMData.Pwd_Index = 0;
						memset(Lusers.APMData.Pwd_Bufer, 0, sizeof(Lusers.APMData.Pwd_Bufer));
					}
					Lusers.APMData.Show_Page = OLEDADMININPUTPWD;
				}
				else if ((Lusers.APMData.Show_Page == OLEDINPUTADDPWD) || (Lusers.APMData.Show_Page == OLEDINPUTADDPWDIN))
				{
					printf("���������������\n");
					Lusers.APMData.Input_PwdBufer[Lusers.APMData.Input_PwdIndex ++] = Key_Data + 0x30;
					printf("Lusers.APMData.Input_PwdBufer[Lusers.APMData.Input_PwdIndex ++] = 0x%x\n", Key_Data + 0x30);
					printf("Lusers.APMData.Input_PwdIndex = %d\n",Lusers.APMData.Input_PwdIndex);
					if (Lusers.APMData.Input_PwdIndex >= 11)
					{
						printf("��������\n");
						Lusers.Application.Voice_Play(0x23);
						Lusers.APMData.Input_PwdIndex = 0;
						memset(Lusers.APMData.Input_PwdBufer, 0, sizeof(Lusers.APMData.Input_PwdBufer));
					}
					Lusers.APMData.Show_Page = OLEDINPUTADDPWDIN;
				}
				else if ((Lusers.APMData.Show_Page == OLEDCONFIRMPWD) || (Lusers.APMData.Show_Page == OLEDCONFIRMPWDIN))
				{
					printf("�������ȷ������\n");
					Lusers.APMData.Confirm_PwdBufer[Lusers.APMData.Confirm_PwdIndex ++] = Key_Data + 0x30;
					printf("Lusers.APMData.Confirm_PwdBufer[Lusers.APMData.Confirm_PwdIndex ++] = 0x%x\n", Key_Data + 0x30);
					printf("Lusers.APMData.Confirm_PwdIndex = %d\n",Lusers.APMData.Confirm_PwdIndex);
					if (Lusers.APMData.Confirm_PwdIndex >= 11)
					{
						printf("��������\n");
						Lusers.Application.Voice_Play(0x23);
						Lusers.APMData.Confirm_PwdIndex = 0;
						memset(Lusers.APMData.Confirm_PwdBufer, 0, sizeof(Lusers.APMData.Confirm_PwdBufer));
					}
					Lusers.APMData.Show_Page = OLEDCONFIRMPWDIN;
				}
        else if (Lusers.APMData.Show_Page == OLEDSETTIMEMENU)
				{
					printf("����ʱ������ʱ��\n");
					Lusers.APMData.time_bufer[Lusers.APMData.time_index ++] = Key_Data;
					
					if (Lusers.APMData.time_index >= 12)
					{
						Lusers.APMData.time_index = 0;
					}
				}					
		  }
			if (Key_Data == TOUCH_GOING)  //���� #
			{
				if (Lusers.APMData.Show_Page == OLEDMAIN)
				{
					if ((flash_list.finger_user_num > 0) || (flash_list.pwd_user_num > 0))
					{
						if ((flash_list.finger_user_num != 255) && (flash_list.pwd_user_num != 255))
						{
							Lusers.Application.Voice_Play(0x05);			
							Lusers.APMData.Show_Page = OLEDADMIN;
						}
						else
						{
							LED1_ON;
							LED2_OFF;							
							Lusers.APMData.Show_Page = OLEDFACTORYMENU;
						}
					}
					else
					{
						LED1_ON;
						LED2_OFF;
						Lusers.Application.Voice_Play(0x0C);
						Lusers.APMData.Show_Page = OLEDMENU1;
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDMENU1)
				{
					Lusers.APMData.Show_Page = OLEDFINGERMENU1;
				}
				else if (Lusers.APMData.Show_Page == OLEDMENU2)
				{
					Lusers.APMData.Show_Page = OLEDWORDMENU1;
				}
				else if (Lusers.APMData.Show_Page == OLEDMENU3)
				{
					Lusers.APMData.Show_Page = OLEDNFCMENU1;
				}
				else if (Lusers.APMData.Show_Page == OLEDFINGERMENU1) //�����ָ
				{
					Lusers.APMData.Finger_Userc = 0;
					for (uint8_t i = 0; i < 100; i++)
					{
						if (flash_list.finger_user_bufer[Lusers.APMData.Finger_Userc] == 0)
						{
							Lusers.APMData.Show_Page = OLEDADDFINGERMENU1;
							break;
						}
						else
						{
							Lusers.APMData.Finger_Userc ++;
						}
					}
					if (Lusers.APMData.Show_Page != OLEDADDFINGERMENU1)
					{
						printf("ָ��������\n");
						Lusers.Application.Voice_Play(0x0B);
						Lusers.Application.OLED_Show(OLEDFINGERFULLING);
						Lusers.Application.MSDelay(1000);
						Lusers.APMData.Show_Page = OLEDFINGERMENU1;
					}				
				}
				else if (Lusers.APMData.Show_Page == OLEDFINGERMENU2) //ɾ����ָ
				{
					if (flash_list.finger_user_num > 1)
					{
						Lusers.APMData.Finger_Userc = 1;
						for (uint8_t i = 1; i < 100; i++)
						{
							if (flash_list.finger_user_bufer[Lusers.APMData.Finger_Userc] == 1)
							{
								Lusers.APMData.Show_Page = OLEDDELETEFINGERMENU;
								break;
							}
							else
							{
								Lusers.APMData.Finger_Userc ++;
							}
						}
				  }
					else
					{
						printf("û��ָ��������\n");
						Lusers.APMData.Show_Page = OLEDNOTHAVEFINGER;
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDWORDMENU1) //�������
				{
					Lusers.APMData.Pwd_Userc = 0;
					for (uint8_t i = 0; i < 10; i++)
					{
						if (flash_list.pwd_user_bufer[Lusers.APMData.Pwd_Userc] == 0)
						{
							Lusers.APMData.Show_Page = OLEDADDPWDMENU1;
							break;
						}
						else
						{
							Lusers.APMData.Pwd_Userc ++;
						}
					}
					if (Lusers.APMData.Show_Page != OLEDADDPWDMENU1)
					{
						printf("��������\n");
						Lusers.Application.Voice_Play(0x14);
						Lusers.Application.OLED_Show(OLEDPWDFULLING);
						Lusers.Application.MSDelay(1000);
						Lusers.APMData.Show_Page = OLEDWORDMENU1;
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDWORDMENU2) //ɾ������
				{
					if (flash_list.pwd_user_num > 1)
					{
						Lusers.APMData.Pwd_Userc = 1;
						for (uint8_t i = 1; i < 10; i++)
						{
							if (flash_list.pwd_user_bufer[Lusers.APMData.Pwd_Userc] == 1)
							{
								Lusers.APMData.Show_Page = OLEDDELETEFPWDMENU;
								break;
							}
							else
							{
								Lusers.APMData.Pwd_Userc ++;
							}
						}
				  }
					else
					{
						printf("û����������\n");
						Lusers.APMData.Show_Page = OLEDNOTHAVEPWD;
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDNFCMENU1)
				{
					Lusers.APMData.Nfc_Userc = 0;
					for (uint8_t i = 0; i < 80; i++)
					{
						if (flash_list.nfc_user_bufer[Lusers.APMData.Nfc_Userc] == 0)
						{
							Lusers.APMData.Show_Page = OLEDADDNFCMENU1;
							break;
						}
						else
						{
							Lusers.APMData.Nfc_Userc ++;
						}
					}
					if (Lusers.APMData.Show_Page != OLEDADDNFCMENU1)
					{
						printf("��Ƭ����\n");
						Lusers.Application.OLED_Show(OLEDNFCFULLING);
						Lusers.Application.MSDelay(1000);
						Lusers.APMData.Show_Page = OLEDNFCMENU1;						
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDNFCMENU2)
				{
					if (flash_list.nfc_user_num >= 1)
					{
						Lusers.APMData.Nfc_Userc = 0;
						for (uint8_t i = 0; i < 80; i++)
						{
							if (flash_list.nfc_user_bufer[Lusers.APMData.Nfc_Userc] == 1)
							{
								Lusers.APMData.Show_Page = OLEDDELETEFNFCMENU;
								break;
							}
							else
							{
								Lusers.APMData.Nfc_Userc ++;
							}
						}
				  }
					else
					{
						printf("û�п�Ƭ����\n");
						Lusers.APMData.Show_Page = OLEDNOTHAVENFC;
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDADDFINGERMENU1)
				{
					uint8_t please_finger_time = 0;
					Lusers.Application.Finger_Play(FINGER_ADD, Lusers.APMData.Finger_Userc + 1);
					if (finger_state.Add_Finger_State == PLEASE_FINGER)
					{
						Lusers.Application.Voice_Play(0x19); //�����ָ
						Lusers.Application.OLED_Show(OLEDADDFINGER);
						do
						{
							if (please_finger_time >= 5)
							{
								printf("������Ӵ������ʧ��\n");
								Lusers.Application.Voice_Play(0x01); //���ʧ��
								Lusers.Application.OLED_Show(OLEDADDFINGERFAIL);
								delay_ms(1000);
								Lusers.APMData.Show_Page = OLEDFINGERMENU1;				
								break;
							}
							if (finger_state.Add_Finger_State == PLEASE_FINGER_AGAING)
							{
								printf("����¼һ��\n");
								Lusers.Application.Voice_Play(0x1B); //����¼һ��
								Lusers.Application.OLED_Show(OLEDADDFINGERAGING);
								please_finger_time ++;
								finger_state.Add_Finger_State = 6;
							}
							else if (finger_state.Add_Finger_State == ADD_FINGER_SUCCESS)
							{
								printf("��ӳɹ�\n");
								Lusers.Application.Voice_Play(0x03); //��ӳɹ�
								Lusers.Application.Finger_Play(FINGER_BREAK, 0);
								Lusers.Application.OLED_Show(OLEDADDFINGERSUCCESS);
								flash_list.finger_user_num ++;
								flash_list.finger_user_bufer[Lusers.APMData.Finger_Userc] = 1;
								Lusers.Application.FLASH_WPlay(DATAFLASH);
								uint8_t finger_lenth[1] = {sizeof(flash_list.finger_user_bufer)};
								uint8_t *fingerzonepage = TUYA_WiFi_Zone_Page(flash_list.finger_user_bufer, finger_lenth);
								mcu_dp_raw_update(DPID_UPDATE_ALL_FIN_VEIN, fingerzonepage, finger_lenth[0]);
								delay_ms(1000);
								Lusers.APMData.Show_Page = OLEDFINGERMENU1;
								break;
							}
							else if ((finger_state.Add_Finger_State == FINGER_FAIL) || (finger_state.Add_Finger_State == ADD_FINGER_FAIL))
							{
								printf("���ʧ��\n");
								Lusers.Application.Voice_Play(0x01); //���ʧ��
								Lusers.Application.Finger_Play(FINGER_BREAK, 0);
								Lusers.Application.OLED_Show(OLEDADDFINGERFAIL);
								delay_ms(1000);
								Lusers.APMData.Show_Page = OLEDFINGERMENU1;				
								break;
							}
							if (Key_Data == TOUCH_BACK)
							{
								printf("���˷��ؼ�\n");
								Lusers.Application.Voice_Play(0x10);
								Lusers.Application.Finger_Play(FINGER_BREAK, 0);
								delay_ms(200);
								break;
							}
							else if (Lusers.APMData.OLEDReturn >= 10)
							{
								printf("��ʱ�˳������ָ\n");
								Lusers.Application.Finger_Play(FINGER_BREAK, 0);
								delay_ms(200);
								break;				
							}
							fingerService();
							Lusers.Application.MSDelay(200);
						}while(Lusers.APMData.OLEDReturn < 10);

					}
					else if(finger_state.Add_Finger_State == ADD_FINGER_FAIL)
					{
						printf("�ռ������������ʧ��\n");
						Lusers.Application.Voice_Play(0x01); //���ʧ��
						flash_list.finger_user_num ++;
						flash_list.finger_user_bufer[Lusers.APMData.Finger_Userc] = 1;
						Lusers.Application.Finger_Play(FINGER_BREAK, 0);
						Lusers.Application.OLED_Show(OLEDADDFINGERFAIL);
						delay_ms(1000);
						Lusers.APMData.Show_Page = OLEDFINGERMENU1;
					}
					else
					{
						Lusers.Application.Voice_Play(0x01); //���ʧ��
						Lusers.Application.Finger_Play(FINGER_BREAK, 0);
						Lusers.Application.OLED_Show(OLEDADDFINGERFAIL);
						delay_ms(1000);
						Lusers.APMData.Show_Page = OLEDFINGERMENU1;						
					}
					Finger_Add_State_IT = 0;					
				}
				else if (Lusers.APMData.Show_Page == OLEDDELETEFINGERMENU)
				{
					Lusers.Application.Finger_Play(FINGER_DELETE, Lusers.APMData.Finger_Userc + 1);
					Lusers.APMData.OLEDReturn = 0;
					do
					{
						if (finger_state.Delete_Finger_State == DELETE_FINGER_SUCCESS)
						{
							printf("ɾ���ɹ�\n");
							Lusers.Application.Voice_Play(0x08);
							Lusers.Application.OLED_Show(OLEDDELETEFINGERSUCC);
							flash_list.finger_user_num --;
							flash_list.finger_user_bufer[Lusers.APMData.Finger_Userc] = 0;
							Lusers.Application.FLASH_WPlay(DATAFLASH);
							
							uint8_t finger_lenth[1] = {sizeof(flash_list.finger_user_bufer)};
							uint8_t *fingerzonepage = TUYA_WiFi_Zone_Page(flash_list.finger_user_bufer, finger_lenth);
							mcu_dp_raw_update(DPID_UPDATE_ALL_FIN_VEIN, fingerzonepage, finger_lenth[0]);
								
							Lusers.Application.MSDelay(1000);
							Lusers.APMData.Show_Page = OLEDFINGERMENU2;
							break;
						}
						else if (finger_state.Delete_Finger_State == DELETE_FINGER_FAIL)
						{
							printf("ɾ��ʧ��\n");
							Lusers.Application.Voice_Play(0x0A);
							Lusers.Application.OLED_Show(OLEDDELETEFINGERFAIL);
							Lusers.Application.MSDelay(1000);
							Lusers.APMData.Show_Page = OLEDFINGERMENU2;
							break;
						}
						else if (Key_Data == TOUCH_BACK)
						{
							printf("���˷��ؼ�\n");
							Lusers.Application.Voice_Play(0x10);
							Lusers.Application.Finger_Play(FINGER_BREAK, 0);
							Lusers.Application.MSDelay(200);
							break;
						}
						else if (Lusers.APMData.OLEDReturn >= 5)
						{
							printf("��ʱ�˳�ɾ����ָ\n");
							Lusers.Application.Finger_Play(FINGER_BREAK, 0);
							Lusers.Application.MSDelay(200);
							break;				
						}
						fingerService();
						Lusers.Application.MSDelay(200);
					}while(Lusers.APMData.OLEDReturn < 5);
				}
				else if (Lusers.APMData.Show_Page == OLEDADDPWDMENU1) //��������һ�������������
				{
					Lusers.Application.Voice_Play(0x0E);
					Lusers.APMData.Input_PwdIndex = 0;
					Lusers.APMData.Confirm_PwdIndex = 0;
					memset(Lusers.APMData.Input_PwdBufer, 0, sizeof(Lusers.APMData.Input_PwdBufer));
					memset(Lusers.APMData.Confirm_PwdBufer, 0, sizeof(Lusers.APMData.Confirm_PwdBufer));
					Lusers.Application.ONOFFLed(LED_ON);
					Lusers.APMData.Show_Page = OLEDINPUTADDPWD;
				}
				else if (Lusers.APMData.Show_Page == OLEDINPUTADDPWDIN) //�������ڶ���ȷ���������
				{
					if (Lusers.APMData.Input_PwdIndex >= 6)
					{
						Lusers.Application.Voice_Play(0x07);
						Lusers.APMData.Show_Page = OLEDCONFIRMPWD;
					}
					else                                                  // ���Ȳ�����λ
					{
						Lusers.Application.Voice_Play(0x17);
						Lusers.Application.OLED_Show(OLEDPWDNOTLENTH);
						Lusers.Application.MSDelay(1000);
						Lusers.APMData.Input_PwdIndex = 0;
						memset(Lusers.APMData.Input_PwdBufer, 0, sizeof(Lusers.APMData.Input_PwdBufer));
						Lusers.APMData.Show_Page = OLEDINPUTADDPWD;
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDCONFIRMPWDIN)
				{
					uint8_t SavePWD_Bufer[6] = {0};
					if (Lusers.APMData.Input_PwdIndex == Lusers.APMData.Confirm_PwdIndex)
					{
						if (strcmp((const char *)Lusers.APMData.Input_PwdBufer, (const char *)Lusers.APMData.Confirm_PwdBufer) == 0)
						{
							printf("������ͬ\n");
							uint8_t pwd_ret = 0;
							Encryption_PWD(Lusers.APMData.Confirm_PwdBufer, SavePWD_Bufer, Lusers.APMData.Confirm_PwdIndex);
							pwd_ret = Find_SamePWD(SavePWD_Bufer);
							if (pwd_ret == 0)
							{
								flash_list.pwd_user_num ++;
								flash_list.pwd_user_bufer[Lusers.APMData.Pwd_Userc] = 1;
								
								printf("��������Ϊ ��");
								for (uint8_t i = 0; i < 6; i++)
								{
									flash_list.pwd_user_pwd_bufer[Lusers.APMData.Pwd_Userc][i] = SavePWD_Bufer[i];
									printf("0x%x  ", flash_list.pwd_user_pwd_bufer[Lusers.APMData.Pwd_Userc][i]);
								}
								printf("\r\n");
								Lusers.Application.FLASH_WPlay(DATAFLASH);
								Lusers.Application.Voice_Play(0x03);
								Lusers.Application.OLED_Show(OLEDADDPWDSUCCESS);
								Lusers.Application.ONOFFLed(LED_OFF);
								
								uint8_t pwd_lenth[1] = {sizeof(flash_list.pwd_user_bufer)};
								uint8_t *pwdzonepage = TUYA_WiFi_Zone_Page(flash_list.pwd_user_bufer, pwd_lenth);
								mcu_dp_raw_update(DPID_UPDATE_ALL_PASSWORD, pwdzonepage, pwd_lenth[0]);
								
								Lusers.Application.MSDelay(1000);
								LED1_ON;
								Lusers.APMData.Show_Page = OLEDWORDMENU1;
						  }
							else
							{
								printf("�����ظ�\n");
								Lusers.Application.Voice_Play(0x0F);
								Lusers.Application.OLED_Show(OLEDPWDISHAVE);
								Lusers.Application.ONOFFLed(LED_OFF);
								Lusers.Application.MSDelay(1000);
								LED1_ON;
								Lusers.APMData.Show_Page = OLEDADDPWDMENU1;								
							}
						}
						else
						{
							printf("���벻ͬ\n");
							Lusers.Application.Voice_Play(0x0F);
							Lusers.Application.OLED_Show(OLEDPWDDISHAVE);
							Lusers.Application.ONOFFLed(LED_OFF);
							Lusers.Application.MSDelay(1000);
							LED1_ON;
							Lusers.APMData.Show_Page = OLEDADDPWDMENU1;
						}
				  }
					else
					{
						printf("����ȷ�����볤�Ȳ�ͬ\n");
						Lusers.Application.Voice_Play(0x0F);
						Lusers.Application.OLED_Show(OLEDPWDDISHAVE);
						Lusers.Application.ONOFFLed(LED_OFF);
						Lusers.Application.MSDelay(1000);
						LED1_ON;
						Lusers.APMData.Show_Page = OLEDADDPWDMENU1;
					}
					LED2_OFF;
				}
				else if (Lusers.APMData.Show_Page == OLEDDELETEFPWDMENU) //ɾ������
				{
					Lusers.Application.Voice_Play(0x08);
					Lusers.Application.OLED_Show(OLEDDELETEPWDSUCC);
					flash_list.pwd_user_num --;
					flash_list.pwd_user_bufer[Lusers.APMData.Pwd_Userc] = 0;
					memset(flash_list.pwd_user_pwd_bufer[Lusers.APMData.Pwd_Userc], 0, 6);
					Lusers.Application.FLASH_WPlay(DATAFLASH);
					
					uint8_t pwd_lenth[1] = {sizeof(flash_list.pwd_user_bufer)};
					uint8_t *zonepage = TUYA_WiFi_Zone_Page(flash_list.pwd_user_bufer, pwd_lenth);
					mcu_dp_raw_update(DPID_UPDATE_ALL_PASSWORD, zonepage, pwd_lenth[0]);
					
					Lusers.Application.MSDelay(1000);
					Lusers.APMData.Show_Page = OLEDWORDMENU2;
				}
				else if (Lusers.APMData.Show_Page == OLEDADDNFCMENU1) //���NFC������
				{
					if (Nfc_Status == OK)
					{
						Lusers.Application.Voice_Play(0x15);
						Lusers.Application.OLED_Show(OLEDADDNFCDOING);
						while(1)
						{
							if (Lpcd_Card_Event() == OK)
							{
								uint8_t nfc_save_bufer[4] = {0};
								Encryption_NFC(CardID, nfc_save_bufer);
								if (Find_SameNFC(nfc_save_bufer) == 1)
								{
									printf("����ظ�\n");
									Lusers.Application.Voice_Play(0x01);
									Lusers.Application.OLED_Show(OLEDADDNFCSAME);
									Lusers.Application.MSDelay(1000);
									Lusers.APMData.Show_Page = OLEDADDNFCMENU1;
									break;
								}
								else
								{
									printf("��ӳɹ�\n");
									Lusers.Application.Voice_Play(0x03);
									Lusers.Application.OLED_Show(OLEDADDNFCSUCCESS);
									flash_list.nfc_user_num ++;
									flash_list.nfc_user_bufer[Lusers.APMData.Nfc_Userc] = 1;
									for (uint8_t i = 0; i < 4; i++)
									{
										flash_list.nfc_user_nfc_bufer[Lusers.APMData.Nfc_Userc][i] = nfc_save_bufer[i];									
									}
									Lusers.Application.FLASH_WPlay(DATAFLASH);
									
									uint8_t nfc_lenth[1] = {sizeof(flash_list.nfc_user_bufer)};
									uint8_t *nfczonepage = TUYA_WiFi_Zone_Page(flash_list.nfc_user_bufer, nfc_lenth);
									mcu_dp_raw_update(DPID_UPDATE_ALL_CARD, nfczonepage, nfc_lenth[0]);
									
									Lusers.Application.MSDelay(1000);
									Lusers.APMData.Show_Page = OLEDNFCMENU1;
									break;									
								}
							}
							else if (Key_Data == 0x0A)
							{
								printf("���˷��ؼ�\n");
								Lusers.Application.Voice_Play(0x10);
								Lusers.Application.MSDelay(100);
								Lusers.Application.Voice_Play(0x01);
								Lusers.Application.OLED_Show(OLEDADDNFCFAIL);
								Lusers.Application.MSDelay(1000);
								Lusers.APMData.Show_Page = OLEDADDNFCMENU1;
                break;								
							}
							else if (Lusers.APMData.OLEDReturn >= 10)
							{
								printf("��ʱ�˳�\n");
								Lusers.APMData.Show_Page = OLEDADDNFCMENU1;
								break;
							}
							Lusers.Application.MSDelay(10);
						}
				  }
					else
					{
						Lusers.Application.Voice_Play(0x01);
						Lusers.Application.OLED_Show(OLEDADDNFCFAIL);
            Lusers.Application.MSDelay(1000);
            Lusers.APMData.Show_Page = OLEDNFCMENU1;						
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDDELETEFNFCMENU) //ɾ����Ƭ
				{
					Lusers.Application.Voice_Play(0x08);
					Lusers.Application.OLED_Show(OLEDDELETENFCSUCCESS);
					
					flash_list.nfc_user_num --;
					flash_list.nfc_user_bufer[Lusers.APMData.Nfc_Userc] = 0;
					memset(flash_list.nfc_user_nfc_bufer[Lusers.APMData.Nfc_Userc], 0, 4);
					Lusers.Application.FLASH_WPlay(DATAFLASH);
					
					uint8_t nfc_lenth[1] = {sizeof(flash_list.nfc_user_bufer)};
					uint8_t *nfczonepage = TUYA_WiFi_Zone_Page(flash_list.nfc_user_bufer, nfc_lenth);
					mcu_dp_raw_update(DPID_UPDATE_ALL_CARD, nfczonepage, nfc_lenth[0]);		
					
					Lusers.Application.MSDelay(1000);
					Lusers.APMData.Show_Page = OLEDNFCMENU2;
				}
				else if (Lusers.APMData.Show_Page == OLEDMENU4)
				{
					Lusers.APMData.Show_Page = OLEDSYSTEMMENU1;
				}
				else if (Lusers.APMData.Show_Page == OLEDSYSTEMMENU1)  //����ʱ��
				{
					Lusers.Application.ONOFFLed(LED_ON);
					Lusers.APMData.time_bufer[0] = hymd.Year/10;
					Lusers.APMData.time_bufer[1] = hymd.Year%10;
					Lusers.APMData.time_bufer[2] = hymd.Month/10;
					Lusers.APMData.time_bufer[3] = hymd.Month%10;
					Lusers.APMData.time_bufer[4] = hymd.Date/10;
					Lusers.APMData.time_bufer[5] = hymd.Date%10;
					Lusers.APMData.time_bufer[6] = hhms.Hours/10;
					Lusers.APMData.time_bufer[7] = hhms.Hours%10;
					Lusers.APMData.time_bufer[8] = hhms.Minutes/10;
					Lusers.APMData.time_bufer[9] = hhms.Minutes%10;
					Lusers.APMData.time_bufer[10] = hhms.Seconds/10;
					Lusers.APMData.time_bufer[11] = hhms.Seconds%10;
					Lusers.APMData.Show_Page = OLEDSETTIMEMENU;
				}
				else if (Lusers.APMData.Show_Page == OLEDSETTIMEMENU)
				{
					Lusers.Application.Voice_Play(0x1E);
					Lusers.Application.OLED_Show(OLEDSETTIMESUCCESS);
					hymd.Year  = Lusers.APMData.time_bufer[0]*10 + Lusers.APMData.time_bufer[1];
					hymd.Month = Lusers.APMData.time_bufer[2]*10 + Lusers.APMData.time_bufer[3];
					hymd.Date  = Lusers.APMData.time_bufer[4]*10 + Lusers.APMData.time_bufer[5];
					hhms.Hours = Lusers.APMData.time_bufer[6]*10 + Lusers.APMData.time_bufer[7];
					hhms.Minutes = Lusers.APMData.time_bufer[8]*10 + Lusers.APMData.time_bufer[9];
					hhms.Seconds = Lusers.APMData.time_bufer[10]*10 + Lusers.APMData.time_bufer[11];
					MX_Set_Ymd(hymd.Year, hymd.Month, hymd.Date);
					MX_Set_Hms(hhms.Hours, hhms.Minutes, hhms.Seconds);
					Lusers.Application.ONOFFLed(LED_OFF);
					Lusers.Application.MSDelay(1000);
					LED1_ON;
					Lusers.APMData.Show_Page = OLEDSYSTEMMENU1;
				}
				else if (Lusers.APMData.Show_Page == OLEDSYSTEMMENU2)
				{
					Lusers.APMData.Show_Page = OLEDSETAUDIOMENU;
				}
				else if (Lusers.APMData.Show_Page == OLEDSETAUDIOMENU) //��������	
				{
					if (Lusers.APMData.Audio_Index == 0)
					{
						WTN6_Send_Data(0xE0);
					}
					else if (Lusers.APMData.Audio_Index == 1)
					{
						WTN6_Send_Data(0xEA);
					}
					else if (Lusers.APMData.Audio_Index == 2)
					{
						WTN6_Send_Data(0xEF);
					}
					Lusers.Application.Voice_Play(0x1E);
					Lusers.Application.OLED_Show(OLEDSETAUDIOSUCCESS);
					Lusers.Application.ONOFFLed(LED_OFF);
					Lusers.Application.MSDelay(1000);
					LED1_ON;
					Lusers.APMData.Show_Page = OLEDSYSTEMMENU2;				
				}
				else if (Lusers.APMData.Show_Page == OLEDSYSTEMMENU3) //���ÿ��ŷ�ʽ 
				{
					Lusers.APMData.Show_Page = OLEDSETOPENONLY;
				}
				else if (Lusers.APMData.Show_Page == OLEDSETOPENONLY)
				{
					open_list.open_mode = 0;
					Lusers.Application.Voice_Play(0x1E);
					Lusers.Application.OLED_Show(OLEDSETOPENSUCCESS);
					Lusers.Application.ONOFFLed(LED_OFF);
					Lusers.Application.FLASH_WPlay(OPENFLASH);
					Lusers.Application.MSDelay(1000);
					LED1_ON;
					Lusers.APMData.Show_Page = OLEDSYSTEMMENU3;		
				}
				else if (Lusers.APMData.Show_Page == OLEDSETOPENCONMBO)
				{
					open_list.open_mode = 1;
					Lusers.Application.Voice_Play(0x1E);
					Lusers.Application.OLED_Show(OLEDSETOPENSUCCESS);
					Lusers.Application.ONOFFLed(LED_OFF);
					Lusers.Application.FLASH_WPlay(OPENFLASH);
					Lusers.Application.MSDelay(1000);
					LED1_ON;
					Lusers.APMData.Show_Page = OLEDSYSTEMMENU3;
				}
				else if (Lusers.APMData.Show_Page == OLEDSYSTEMMENU4) //�ָ���������
				{
					Lusers.Application.Voice_Play(0x16);
					Lusers.APMData.Show_Page = OLEDFACTORYMENU;
				}
				else if (Lusers.APMData.Show_Page == OLEDFACTORYMENU)
				{
					Lusers.Application.Voice_Play(0x1C);
					Lusers.Application.OLED_Show(OLEDWAITFACTORY);
					Lusers.Application.MSDelay(1000);
					Lusers.Application.Finger_Play(FINGER_FACTORY, 0);
					do
					{
						if (finger_state.Factory_Finger_State == FINGER_FACTORY_SUCCESS)
						{
							printf("�ָ������ɹ�\n");
							Lusers.Application.ONOFFLed(LED_OFF);
							Lusers.Application.ONOFFFinger(FINGER_POWER_OFF);
							Lusers.Initialization.Flash();
							Lusers.Application.FLASH_WPlay(DATAFLASH);
							Lusers.Application.FLASH_WPlay(OPENFLASH);
							Lusers.Application.Voice_Play(0x09); //�ָ������ɹ�
							Lusers.Application.OLED_Show(OLEDFACTORYSUCCESS);
							mcu_reset_wifi();
							delay_ms(2000);
							wifi_uart_service();
							goto facsleep;
//							NVIC_SystemReset();
						}
						else
						{
							printf("�ָ�����ʧ��\n");
							Lusers.Application.Voice_Play(0x0A); //�ָ�����ʧ��
							Lusers.Application.OLED_Show(OLEDFACTORYFAIL);
							delay_ms(1000);
							Lusers.APMData.Show_Page = OLEDSYSTEMMENU4;
							break;
						}
					}while(Lusers.APMData.OLEDReturn < 10);
				}
				else if (Lusers.APMData.Show_Page == OLEDMENU5) //���ż�¼���û���Ϣ��ѯ
				{
					Lusers.APMData.Show_Page = OLEDUSERRECORD;
				}
				else if (Lusers.APMData.Show_Page == OLEDUSERRECORD)
				{
					Lusers.APMData.Show_Page = OLEDUSERREGISTER;
				}
				else if (Lusers.APMData.Show_Page == OLEDOPENRECORD)
				{
					if (open_list.open_number == 0)
					{
						Lusers.APMData.Open_Record_Userc = 1;
					}
					else
					{
					  Lusers.APMData.Open_Record_Userc = open_list.open_number;
					}
					if (open_list.open_bufer[Lusers.APMData.Open_Record_Userc - 1][0] != 0)
					{
						printf("���ڴ˿��ż�¼\n");
						Lusers.APMData.Show_Page = OLEDFINDRECORD;
					}
					else
					{
						Lusers.APMData.Show_Page = OLEDNOTHAVERECORD;
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDINPUTPWD)
				{
					Lusers.APMData.Open_Pwd_Userc = 0;
					if (Lusers.APMData.Pwd_Index >= 6)
					{
						printf("������Ч����\n");
						Lusers.APMData.Open_Pwd_Userc = Compare_PWD(Lusers.APMData.Pwd_Bufer);
						if (Lusers.APMData.Open_Pwd_Userc == 0)
						{
							if ((Lusers.APMData.Pwd_Index == 8) && (Lusers.APMData.wifi_risi >= 1))
							{
								printf("����̬����У��\n");
								uint8_t Dynamic_time[6] = {0};
								Lusers.Application.GetTime();
								Dynamic_time[0] = hymd.Year;
								Dynamic_time[1] = hymd.Month;
								Dynamic_time[2] = hymd.Date;
								Dynamic_time[3] = hhms.Hours;
								Dynamic_time[4] = hhms.Minutes;
								Dynamic_time[5] = hhms.Seconds;
								BeiJTime_To_GelinUTC(Dynamic_time);
								dynamic_pass_check(Dynamic_time, Lusers.APMData.Pwd_Bufer, 0, 0, NULL);
							}
							else if (Lusers.APMData.Pwd_Index == 7)
							{
								if(flash_list.pwd_user_bufer[0] != 0)
								{
									printf("������ʱ����У��\n");
									uint8_t encryadminpwdbuf[6] = {0};
									uint8_t intimebuf[6] = {0};
									uint8_t encrytime[6] = {0};
									uint8_t gettemppwd[7] = {0};
									uint8_t temppwd_result = FALSE;
									Lusers.Application.Voice_Play(0x1C);
									Lusers.APMData.Show_Page = OLEDTEMPORARYWAIT;
									Lusers.Application.OLED_Show(Lusers.APMData.Show_Page);
									Lusers.Application.GetTime();
									intimebuf[0] = hymd.Year;
									intimebuf[1] = hymd.Month;
									intimebuf[2] = hymd.Date;
									intimebuf[3] = hhms.Hours;
									intimebuf[4] = hhms.Minutes;
									intimebuf[5] = hhms.Seconds;
									Lusers.Application.MSDelay(1000);
									Encry_TemPwd(encryadminpwdbuf);
									encrytime_func(intimebuf, encrytime);
									Verity_TemPwd(encrytime, encryadminpwdbuf, gettemppwd);
									temppwd_result = Compare_TemPwd(gettemppwd, Lusers.APMData.Pwd_Bufer);
									if (temppwd_result == TRUE)
									{
										printf("��ʱ������֤ͨ��\n");
										Lusers.Application.Voice_Play(0x26);
										Lusers.Application.ONOFFLed(LED_OFF);
										Lusers.Application.OLED_Show(OLEDTEMPORARYPASS);
										Lusers.Application.Motor_Play(MOTOR_OPEN);
										Lusers.Application.MSDelay(2000);
										Lusers.Application.MSDelay(3000);
										Lusers.Application.Motor_Play(MOTOR_CLOSE);
										Lusers.Application.MSDelay(100);
										Lusers.Application.OLED_Show(OLEDCLEAR);
										Lusers.APMData.Show_Page = OLEDCLEAR;									
									}
									else if(Lusers.APMData.wifi_risi >= 1)
									{
										printf("������ʱ����У��\n");
										memcpy(Lusers.APMData.Temparoary_Pwd_Bufer, Lusers.APMData.Pwd_Bufer, 7);
										Lusers.APMData.Temparory_pwd_state = 0;
										Lusers.APMData.Show_Page = OLEDTEMPORARYWAIT;
									}
									else
									{
										printf("��ʱ������֤ʧ��");
										Lusers.Application.ONOFFLed(LED_OFF);
										Lusers.Application.Voice_Play(0x25);
										Lusers.Application.OLED_Show(OLEDTEMPORARYFAIL);
										Lusers.Application.MSDelay(1000);
										Lusers.APMData.Show_Page = OLEDMAIN;
									}									
								}
								else if(Lusers.APMData.wifi_risi >= 1)
								{
									printf("������ʱ����У��\n");
									memcpy(Lusers.APMData.Temparoary_Pwd_Bufer, Lusers.APMData.Pwd_Bufer, 7);
									Lusers.Application.Voice_Play(0x1C);
									Lusers.APMData.Temparory_pwd_state = 0;
									Lusers.APMData.Show_Page = OLEDTEMPORARYWAIT;
								}
								else
								{
									printf("��ʱ������֤ʧ��");
									Lusers.Application.ONOFFLed(LED_OFF);
									Lusers.Application.Voice_Play(0x25);
									Lusers.Application.OLED_Show(OLEDTEMPORARYFAIL);
									Lusers.Application.MSDelay(1000);
									Lusers.APMData.Show_Page = OLEDMAIN;									
								}
							}
							else
							{
								printf("������֤ʧ��\n");
								Lusers.APMData.test_error ++;
								if (Lusers.APMData.test_error >= TEST_ERROR_MAX)
								{
									printf("�Դ���\n");
									Lusers.Application.Voice_Play(0x28);
									Lusers.Application.ONOFFLed(LED_OFF);
									Lusers.APMData.test_error = 0;
									Lusers.APMData.test_error_time = TEST_ERROR_TIME;
									if (Lusers.APMData.wifi_risi >= 5)
									{
										mcu_dp_enum_update(DPID_ALARM_LOCK, 1);
									}
								}
								else
								{
									OpenDoorState = 0;
									Lusers.Application.ONOFFLed(LED_OFF);
									Lusers.Application.Voice_Play(0x25);
									Lusers.Application.OLED_Show(OLEDVFAIL);
									Lusers.Application.MSDelay(1000);
									Lusers.APMData.Show_Page = OLEDMAIN;
								}
						  }							
						}
						else
						{
							printf("������֤ͨ��\n");
							Lusers.APMData.test_error = 0;
							if ((open_list.open_mode != 1) || (OpenDoorState != 0))
							{
								OpenDoorState = 0;
								Lusers.Application.Voice_Play(0x26);
								Lusers.Application.ONOFFLed(LED_OFF);
								Lusers.Application.OLED_Show(OLEDPWDPASS);
								
								Lusers.Application.GetTime();
								if (open_list.open_number >= 200)
								{
									open_list.open_number = 0;
								}
								open_list.open_bufer[open_list.open_number][0] = PWDOPEN;
								open_list.open_bufer[open_list.open_number][1] = Lusers.APMData.Open_Pwd_Userc;
								open_list.open_bufer[open_list.open_number][2] = hymd.Year;
								open_list.open_bufer[open_list.open_number][3] = hymd.Month;
								open_list.open_bufer[open_list.open_number][4] = hymd.Date;
								open_list.open_bufer[open_list.open_number][5] = hhms.Hours;
								open_list.open_bufer[open_list.open_number][6] = hhms.Minutes;
								open_list.open_bufer[open_list.open_number][7] = hhms.Seconds;
								open_list.open_number ++;
								Lusers.Application.FLASH_WPlay(OPENFLASH);									
								Lusers.Application.Motor_Play(MOTOR_OPEN);
								Lusers.Application.MSDelay(2000);
								mcu_dp_enum_update(DPID_CLOSED_OPENED, 1);
								Lusers.Application.MSDelay(3000);
								mcu_dp_enum_update(DPID_CLOSED_OPENED, 2);
								Lusers.Application.Motor_Play(MOTOR_CLOSE);
								Lusers.Application.MSDelay(100);
								Lusers.Application.OLED_Show(OLEDCLEAR);
								Lusers.APMData.Show_Page = OLEDCLEAR;
								mcu_dp_value_update(DPID_UNLOCK_PASSWORD, Lusers.APMData.Open_Pwd_Userc, NEED_TIME_UPDATA);
							}
              else
							{
								OpenDoorState = 1;
								Lusers.Application.Voice_Play(0x26);
								Lusers.Application.ONOFFLed(LED_OFF);
								Lusers.Application.OLED_Show(OLEDPWDPASS);
								Lusers.Application.MSDelay(1000);
								Lusers.Application.OLED_Show(OLEDOPENPLEASEFINGER);
								Lusers.APMData.Show_Page = OLEDOPENPLEASEFINGER;						
							}								
						}
					}
					else
					{
						if ((Lusers.APMData.Pwd_Index == 2)&&(Lusers.APMData.Pwd_Bufer[0] == 0x38)&&(Lusers.APMData.Pwd_Bufer[1] == 0x38))
						{
							printf("APPԶ�̿����ϱ�\n");
							Lusers.Application.Voice_Play(0x1C);
							Lusers.APMData.APP_Open_Time = 60;
							Lusers.APMData.Show_Page = OLEDAPPOPENDOOR;
						}
						else
						{
							printf("������Ч����\n");
							Lusers.APMData.test_error ++;
							if (Lusers.APMData.test_error >= TEST_ERROR_MAX)
							{
								printf("�Դ���");
								Lusers.Application.Voice_Play(0x28);
								Lusers.Application.ONOFFLed(LED_OFF);
								Lusers.APMData.test_error = 0;
								Lusers.APMData.test_error_time = TEST_ERROR_TIME;
							}
							else
							{							
								OpenDoorState = 0;
								Lusers.Application.ONOFFLed(LED_OFF);
								Lusers.Application.Voice_Play(0x25);
								Lusers.Application.OLED_Show(OLEDVFAIL);
								Lusers.Application.MSDelay(1000);
								Lusers.APMData.Show_Page = OLEDMAIN;
							}
					  }
					}
					Lusers.APMData.Pwd_Index = 0;
					memset(Lusers.APMData.Pwd_Bufer, 0 , sizeof(Lusers.APMData.Pwd_Bufer));
				}
				else if (Lusers.APMData.Show_Page == OLEDADMININPUTPWD)
				{
					Lusers.APMData.Open_Pwd_Userc = 0;
					if (Lusers.APMData.Pwd_Index >= 6)
					{
						printf("������Ч����\n");
						Lusers.APMData.Open_Pwd_Userc = Compare_PWD(Lusers.APMData.Pwd_Bufer);
						if ((Lusers.APMData.Open_Pwd_Userc == 0) || (Lusers.APMData.Open_Pwd_Userc > 2))
						{
							printf("������֤ʧ��\n");
							Lusers.APMData.test_error ++;
							if (Lusers.APMData.test_error >= TEST_ERROR_MAX)
							{
								printf("�Դ���");
								Lusers.Application.Voice_Play(0x28);
								Lusers.Application.ONOFFLed(LED_OFF);
								Lusers.APMData.test_error = 0;
								Lusers.APMData.test_error_time = TEST_ERROR_TIME;
							}
							else
							{
								Lusers.Application.ONOFFLed(LED_OFF);
								Lusers.Application.Voice_Play(0x25);
								Lusers.Application.OLED_Show(OLEDVFAIL);
								Lusers.Application.MSDelay(1000);
								Lusers.APMData.Show_Page = OLEDADMIN;
							}							
						}
						else
						{
							printf("������֤ͨ��\n");
							Lusers.APMData.test_error = 0;
							Lusers.Application.Voice_Play(0x26);
							Lusers.Application.ONOFFLed(LED_OFF);
							Lusers.Application.OLED_Show(OLEDPWDPASS);
							Lusers.Application.MSDelay(1000);
							LED1_ON;
							Lusers.APMData.Show_Page = OLEDMENU1;							
						}
					}
					else
					{
						printf("������Ч����\n");
						Lusers.APMData.test_error ++;
						if (Lusers.APMData.test_error >= TEST_ERROR_MAX)
						{
							printf("�Դ���");
							Lusers.Application.Voice_Play(0x28);
							Lusers.Application.ONOFFLed(LED_OFF);
							Lusers.APMData.test_error = 0;
							Lusers.APMData.test_error_time = TEST_ERROR_TIME;
						}
						else
						{
							Lusers.Application.ONOFFLed(LED_OFF);
							Lusers.Application.Voice_Play(0x25);
							Lusers.Application.OLED_Show(OLEDVFAIL);
							Lusers.Application.MSDelay(1000);
							Lusers.APMData.Show_Page = OLEDADMIN;							
						}
					}
					Lusers.APMData.Pwd_Index = 0;
					memset(Lusers.APMData.Pwd_Bufer, 0 , sizeof(Lusers.APMData.Pwd_Bufer));
				}
				else if (Lusers.APMData.Show_Page == OLEDMENU6) //Զ�����ý���
				{
					Lusers.APMData.Show_Page = OLEDAPPMENU1;
				}
				else if (Lusers.APMData.Show_Page == OLEDAPPMENU1) //Զ����Խ���
				{
					Lusers.APMData.Show_Page = OLEDAPPPAIR;
					Lusers.APMData.wifi_risi = 0;
					Lusers.Application.Voice_Play(0x1C);
					Lusers.Application.ONOFFLed(LED_OFF);
					mcu_set_wifi_mode(0);
				}
				else if (Lusers.APMData.Show_Page == OLEDAPPMENU2) //�ر�Զ�̽���
				{
					Lusers.APMData.wifi_risi = 0;
					Lusers.Application.Voice_Play(0x06);					
					Lusers.Application.OLED_Show(OLEDCLOSEAPPPAIR);
					Lusers.Application.ONOFFLed(LED_OFF);
          mcu_reset_wifi();
					Lusers.Application.MSDelay(1000);
					Lusers.APMData.Show_Page = OLEDAPPMENU2;
					LED1_ON;
				} 
			}
			else if (Key_Data == TOUCH_BACK) // ���� *
			{
				if (Lusers.APMData.Show_Page == OLEDMAIN)
				{
					Lusers.APMData.Show_Page = OLEDCLEAR;
				}
				else if (Lusers.APMData.Show_Page == OLEDADMIN)
				{
					Lusers.APMData.Show_Page = OLEDMAIN;
				}
				else if ((Lusers.APMData.Show_Page == OLEDFINGERMENU1) || (Lusers.APMData.Show_Page == OLEDFINGERMENU2))
				{
					Lusers.APMData.Show_Page = OLEDMENU1;
				}
				else if ((Lusers.APMData.Show_Page == OLEDWORDMENU1) || (Lusers.APMData.Show_Page == OLEDWORDMENU2))
				{
					Lusers.APMData.Show_Page = OLEDMENU2;
				}
				else if ((Lusers.APMData.Show_Page == OLEDNFCMENU1) || (Lusers.APMData.Show_Page == OLEDNFCMENU2))
				{
					Lusers.APMData.Show_Page = OLEDMENU3;
				}
				else if (Lusers.APMData.Show_Page == OLEDADDFINGERMENU1)
				{
					Lusers.APMData.Show_Page = OLEDFINGERMENU1;
				}
				else if (Lusers.APMData.Show_Page == OLEDADDPWDMENU1)
				{
					Lusers.APMData.Show_Page = OLEDWORDMENU1;
				}
				else if ((Lusers.APMData.Show_Page == OLEDINPUTADDPWD) || (Lusers.APMData.Show_Page == OLEDCONFIRMPWD))
				{
					LED2_OFF;
					Lusers.APMData.Show_Page = OLEDADDPWDMENU1;
				}				
				else if (Lusers.APMData.Show_Page == OLEDADDNFCMENU1)
				{
					Lusers.APMData.Show_Page = OLEDNFCMENU1;
				}
				else if ((Lusers.APMData.Show_Page == OLEDDELETEFINGERMENU) || (Lusers.APMData.Show_Page == OLEDNOTHAVEFINGER))
				{
					Lusers.APMData.Show_Page = OLEDFINGERMENU2;
				}
				else if ((Lusers.APMData.Show_Page == OLEDDELETEFPWDMENU) || (Lusers.APMData.Show_Page == OLEDNOTHAVEPWD))
				{
					Lusers.APMData.Show_Page = OLEDWORDMENU2;
				}
				else if ((Lusers.APMData.Show_Page == OLEDDELETEFNFCMENU) || (Lusers.APMData.Show_Page == OLEDNOTHAVENFC))
				{
					Lusers.APMData.Show_Page = OLEDNFCMENU2;
				}
				else if ((Lusers.APMData.Show_Page == OLEDSYSTEMMENU1) || (Lusers.APMData.Show_Page == OLEDSYSTEMMENU2) || (Lusers.APMData.Show_Page == OLEDSYSTEMMENU3) || (Lusers.APMData.Show_Page == OLEDSYSTEMMENU4))
				{
					Lusers.APMData.Show_Page = OLEDMENU4;
				}
				else if ((Lusers.APMData.Show_Page == OLEDUSERRECORD) || (Lusers.APMData.Show_Page == OLEDOPENRECORD))
				{
					Lusers.APMData.Show_Page = OLEDMENU5;
				}
				else if (Lusers.APMData.Show_Page == OLEDUSERREGISTER)
				{
					Lusers.APMData.Show_Page = OLEDUSERRECORD;
				}
				else if ((Lusers.APMData.Show_Page == OLEDFINDRECORD) || (Lusers.APMData.Show_Page == OLEDNOTHAVERECORD))
				{
					Lusers.APMData.Show_Page = OLEDOPENRECORD;
				}
				else if ((Lusers.APMData.Show_Page == OLEDAPPMENU1) || (Lusers.APMData.Show_Page == OLEDAPPMENU2))
				{
					Lusers.APMData.Show_Page = OLEDMENU6;
				}
				else if (Lusers.APMData.Show_Page == OLEDAPPPAIR)
				{
					LED1_ON;
					Lusers.APMData.Show_Page = OLEDAPPMENU1;
				}
				else if (Lusers.APMData.Show_Page == OLEDAPPOPENDOOR)
				{
					Lusers.APMData.APP_Open_Time = 0;
					mcu_dp_value_update(DPID_UNLOCK_REQUEST, Lusers.APMData.APP_Open_Time, NOT_TIME_UPDATA);
					Lusers.APMData.Show_Page = OLEDMAIN;
				}
				else if (Lusers.APMData.Show_Page == OLEDSETTIMEMENU)
				{
					if (Lusers.APMData.time_index == 0)
					{
						LED2_OFF;
						Lusers.APMData.Show_Page = OLEDSYSTEMMENU1;
					}
					else
					{
					  Lusers.APMData.time_index --;
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDSETAUDIOMENU)
				{
					Lusers.APMData.Show_Page = OLEDSYSTEMMENU2;
				}
				else if ((Lusers.APMData.Show_Page == OLEDSETOPENCONMBO) || (Lusers.APMData.Show_Page == OLEDSETOPENONLY))
				{
					Lusers.APMData.Show_Page = OLEDSYSTEMMENU3;
				}
				else if (Lusers.APMData.Show_Page == OLEDFACTORYMENU)
				{
					Lusers.APMData.Show_Page = OLEDSYSTEMMENU4;
				}
				else if (Lusers.APMData.Show_Page == OLEDOPENPLEASEFINGER)
				{
					OpenDoorState = 0;
					Lusers.APMData.Show_Page = OLEDMAIN;
				}
			  else if (Lusers.APMData.Show_Page == OLEDINPUTPWD) //������������
				{
					if (Lusers.APMData.Pwd_Index <= 0)
					{
						OpenDoorState = 0;
						Lusers.APMData.Show_Page = OLEDMAIN;
					}
					else
					{
				 	  Lusers.APMData.Pwd_Index --;
						if (Lusers.APMData.Pwd_Index < 10)
						{
					    OLED_Show_16Char(24 + 8*Lusers.APMData.Pwd_Index, 6, 0x40);
						}
					}
				}
			  else if (Lusers.APMData.Show_Page == OLEDADMININPUTPWD) //��֤����Ա��������
				{
					if (Lusers.APMData.Pwd_Index <= 0)
					{
						Lusers.APMData.Show_Page = OLEDADMIN;
					}
					else
					{
				 	  Lusers.APMData.Pwd_Index --;
						if (Lusers.APMData.Pwd_Index < 10)
						{
					    OLED_Show_16Char(24 + 8*Lusers.APMData.Pwd_Index, 6, 0x40);
						}
					}
				}
			  else if (Lusers.APMData.Show_Page == OLEDINPUTADDPWDIN) //���������������
				{
					if (Lusers.APMData.Input_PwdIndex <= 0)
					{
						LED2_OFF;
						Lusers.APMData.Show_Page = OLEDADDPWDMENU1;
					}
					else
					{
				 	  Lusers.APMData.Input_PwdIndex --;
						if (Lusers.APMData.Input_PwdIndex < 10)
						{
					    OLED_Show_16Char(24 + 8*Lusers.APMData.Input_PwdIndex, 6, 0x40);
						}
					}
				}
			  else if (Lusers.APMData.Show_Page == OLEDCONFIRMPWDIN) //�������ȷ������
				{
					if (Lusers.APMData.Confirm_PwdIndex <= 0)
					{
						LED2_OFF;
						Lusers.APMData.Show_Page = OLEDADDPWDMENU1;
					}
					else
					{
				 	  Lusers.APMData.Confirm_PwdIndex --;
						if (Lusers.APMData.Confirm_PwdIndex < 10)
						{
					    OLED_Show_16Char(24 + 8*Lusers.APMData.Confirm_PwdIndex, 6, 0x40);
						}
					}
				}
				else if (Lusers.APMData.Show_Page != OLEDCLEAR)
				{
					Lusers.APMData.Show_Page = OLEDMAIN;
				}
			}
			else if (Key_Data == TOUCH_UP) // ���� �Ϸ�
			{
				if (Lusers.APMData.Show_Page == OLEDMENU1)
				{
					Lusers.APMData.Show_Page = OLEDMENU6;
				}
				else if (Lusers.APMData.Show_Page == OLEDMENU2)
				{
					Lusers.APMData.Show_Page = OLEDMENU1;
				}
				else if (Lusers.APMData.Show_Page == OLEDMENU3)
				{
					Lusers.APMData.Show_Page = OLEDMENU2;
				}
				else if (Lusers.APMData.Show_Page == OLEDMENU4)
				{
					Lusers.APMData.Show_Page = OLEDMENU3;
				}
				else if (Lusers.APMData.Show_Page == OLEDMENU5)
				{
					Lusers.APMData.Show_Page = OLEDMENU4;
				}
				else if (Lusers.APMData.Show_Page == OLEDMENU6)
				{
					Lusers.APMData.Show_Page = OLEDMENU5;
				}
				else if (Lusers.APMData.Show_Page == OLEDFINGERMENU2)
				{
					Lusers.APMData.Show_Page = OLEDFINGERMENU1;
				}
				else if (Lusers.APMData.Show_Page == OLEDWORDMENU2)
				{
					Lusers.APMData.Show_Page = OLEDWORDMENU1;
				}
				else if (Lusers.APMData.Show_Page == OLEDNFCMENU2)
				{
					Lusers.APMData.Show_Page = OLEDNFCMENU1;
				}
				else if (Lusers.APMData.Show_Page == OLEDSYSTEMMENU1)
				{
					Lusers.APMData.Show_Page = OLEDSYSTEMMENU4;
				}
				else if (Lusers.APMData.Show_Page == OLEDSYSTEMMENU2)
				{
					Lusers.APMData.Show_Page = OLEDSYSTEMMENU1;
				}
				else if (Lusers.APMData.Show_Page == OLEDSYSTEMMENU3)
				{
					Lusers.APMData.Show_Page = OLEDSYSTEMMENU2;
				}
				else if (Lusers.APMData.Show_Page == OLEDSYSTEMMENU4)
				{
					Lusers.APMData.Show_Page = OLEDSYSTEMMENU3;
				}
				else if (Lusers.APMData.Show_Page == OLEDOPENRECORD)
				{
					Lusers.APMData.Show_Page = OLEDUSERRECORD;
				}
				else if (Lusers.APMData.Show_Page == OLEDAPPMENU2)
				{
					Lusers.APMData.Show_Page = OLEDAPPMENU1;
				}
				else if (Lusers.APMData.Show_Page == OLEDSETAUDIOMENU)
				{
					if (Lusers.APMData.Audio_Index <= 0)
					{
						Lusers.APMData.Audio_Index = 2;
					}
					else
					{
					  Lusers.APMData.Audio_Index --;
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDSETOPENCONMBO)
				{
					Lusers.APMData.Show_Page = OLEDSETOPENONLY;
				}
				else if (Lusers.APMData.Show_Page == OLEDADDFINGERMENU1)
				{
					if (Lusers.APMData.Finger_Userc <= 0) //��userΪ0���ڽ����Լ�1ʱ�޷�����ֵΪ127����˵�Ϊ0ʱ�ظ�ֵ
					{
						Lusers.APMData.Finger_Userc = 100;
					}					
					Lusers.APMData.Finger_Userc --;
					for (uint8_t i = 0; i < 100; i++)
					{
						if (flash_list.finger_user_bufer[Lusers.APMData.Finger_Userc] == 0)
						{
							break;
						}
						else
						{
							if (Lusers.APMData.Finger_Userc <= 0)
							{
								Lusers.APMData.Finger_Userc = 100;
							}							
							Lusers.APMData.Finger_Userc --;
						}							
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDDELETEFINGERMENU)
				{
					if (Lusers.APMData.Finger_Userc <= 1) //��userΪ0���ڽ����Լ�1ʱ�޷�����ֵΪ127����˵�Ϊ0ʱ�ظ�ֵ
					{
						Lusers.APMData.Finger_Userc = 100;
					}					
					Lusers.APMData.Finger_Userc --;
					for (uint8_t i = 1; i < 100; i++)
					{
						if (flash_list.finger_user_bufer[Lusers.APMData.Finger_Userc] == 1)
						{
							break;
						}
						else
						{
							if (Lusers.APMData.Finger_Userc <= 1)
							{
								Lusers.APMData.Finger_Userc = 100;
							}							
							Lusers.APMData.Finger_Userc --;
						}							
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDADDPWDMENU1)
				{
					if (Lusers.APMData.Pwd_Userc <= 0) //��userΪ0���ڽ����Լ�1ʱ�޷�����ֵΪ127����˵�Ϊ0ʱ�ظ�ֵ
					{
						Lusers.APMData.Pwd_Userc = 10;
					}
					Lusers.APMData.Pwd_Userc --;
					for (uint8_t i = 0; i < 10; i++)
					{
						if (flash_list.pwd_user_bufer[Lusers.APMData.Pwd_Userc] == 0)
						{
							break;
						}
						else
						{
							if (Lusers.APMData.Pwd_Userc <= 0)
							{
								Lusers.APMData.Pwd_Userc = 10;
							}
							Lusers.APMData.Pwd_Userc --;
						}							
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDDELETEFPWDMENU)
				{
					if (Lusers.APMData.Pwd_Userc <= 1) //��userΪ0���ڽ����Լ�1ʱ�޷�����ֵΪ127����˵�Ϊ0ʱ�ظ�ֵ
					{
						Lusers.APMData.Pwd_Userc = 10;
					}
					Lusers.APMData.Pwd_Userc --;
					for (uint8_t i = 1; i < 10; i++)
					{
						if (flash_list.pwd_user_bufer[Lusers.APMData.Pwd_Userc] == 1)
						{
							break;
						}
						else
						{
							if (Lusers.APMData.Pwd_Userc <= 1)
							{
								Lusers.APMData.Pwd_Userc = 10;
							}
							Lusers.APMData.Pwd_Userc --;
						}							
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDADDNFCMENU1)
				{
					if (Lusers.APMData.Nfc_Userc <= 0) //��userΪ0���ڽ����Լ�1ʱ�޷�����ֵΪ127����˵�Ϊ0ʱ�ظ�ֵ
					{
						Lusers.APMData.Nfc_Userc = 80;
					}					
					Lusers.APMData.Nfc_Userc --;
					for (uint8_t i = 0; i < 80; i++)
					{
						if (flash_list.nfc_user_bufer[Lusers.APMData.Nfc_Userc] == 0)
						{
							break;
						}
						else
						{
							if (Lusers.APMData.Nfc_Userc <= 0)
							{
								Lusers.APMData.Nfc_Userc = 80;
							}							
							Lusers.APMData.Nfc_Userc --;
						}							
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDDELETEFNFCMENU)
				{
					if (Lusers.APMData.Nfc_Userc <= 0) //��userΪ0���ڽ����Լ�1ʱ�޷�����ֵΪ127����˵�Ϊ0ʱ�ظ�ֵ
					{
						Lusers.APMData.Nfc_Userc = 80;
					}					
					Lusers.APMData.Nfc_Userc --;
					for (uint8_t i = 0; i < 80; i++)
					{
						if (flash_list.nfc_user_bufer[Lusers.APMData.Nfc_Userc] == 1)
						{
							break;
						}
						else
						{
							if (Lusers.APMData.Nfc_Userc <= 0)
							{
								Lusers.APMData.Nfc_Userc = 80;
							}							
							Lusers.APMData.Nfc_Userc --;
						}							
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDFINDRECORD)
				{
					if (Lusers.APMData.Open_Record_Userc <= 1)
					{
						if (open_list.open_bufer[199][0] != 0)
						{
							Lusers.APMData.Open_Record_Userc = 200;
						}
						else
						{
							Lusers.APMData.Open_Record_Userc = open_list.open_number;
						}
					}
					else 
					{
						Lusers.APMData.Open_Record_Userc --;
					}
				}
			}
			else if (Key_Data == TOUCH_DOWN)  //�����·�
			{
				if (Lusers.APMData.Show_Page == OLEDMENU1)
				{
					Lusers.APMData.Show_Page = OLEDMENU2;
				}
				else if (Lusers.APMData.Show_Page == OLEDMENU2)
				{
					Lusers.APMData.Show_Page = OLEDMENU3;
				}
				else if (Lusers.APMData.Show_Page == OLEDMENU3)
				{
					Lusers.APMData.Show_Page = OLEDMENU4;
				}
				else if (Lusers.APMData.Show_Page == OLEDMENU4)
				{
					Lusers.APMData.Show_Page = OLEDMENU5;
				}
				else if (Lusers.APMData.Show_Page == OLEDMENU5)
				{
					Lusers.APMData.Show_Page = OLEDMENU6;
				}
				else if (Lusers.APMData.Show_Page == OLEDMENU6)
				{
					Lusers.APMData.Show_Page = OLEDMENU1;
				}
				else if (Lusers.APMData.Show_Page == OLEDFINGERMENU1)
				{
					Lusers.APMData.Show_Page = OLEDFINGERMENU2;
				}
				else if (Lusers.APMData.Show_Page == OLEDWORDMENU1)
				{
					Lusers.APMData.Show_Page = OLEDWORDMENU2;
				}
				else if (Lusers.APMData.Show_Page == OLEDNFCMENU1)
				{
					Lusers.APMData.Show_Page = OLEDNFCMENU2;
				}
				else if (Lusers.APMData.Show_Page == OLEDSYSTEMMENU4)
				{
					Lusers.APMData.Show_Page = OLEDSYSTEMMENU1;
				}
				else if (Lusers.APMData.Show_Page == OLEDSYSTEMMENU3)
				{
					Lusers.APMData.Show_Page = OLEDSYSTEMMENU4;
				}
				else if (Lusers.APMData.Show_Page == OLEDSYSTEMMENU2)
				{
					Lusers.APMData.Show_Page = OLEDSYSTEMMENU3;
				}
				else if (Lusers.APMData.Show_Page == OLEDSYSTEMMENU1)
				{
					Lusers.APMData.Show_Page = OLEDSYSTEMMENU2;
				}
				else if (Lusers.APMData.Show_Page == OLEDUSERRECORD)
				{
					Lusers.APMData.Show_Page = OLEDOPENRECORD;
				}
				else if (Lusers.APMData.Show_Page == OLEDAPPMENU1)
				{
					Lusers.APMData.Show_Page = OLEDAPPMENU2;
				}
				else if (Lusers.APMData.Show_Page == OLEDSETAUDIOMENU)
				{
					if (Lusers.APMData.Audio_Index >= 2)
					{
						Lusers.APMData.Audio_Index = 0;
					}
					else
					{
					  Lusers.APMData.Audio_Index ++;
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDSETOPENONLY)
				{
					Lusers.APMData.Show_Page = OLEDSETOPENCONMBO;
				}
				else if (Lusers.APMData.Show_Page == OLEDADDFINGERMENU1)
				{
					Lusers.APMData.Finger_Userc ++;
					if (Lusers.APMData.Finger_Userc > 99)
					{
						Lusers.APMData.Finger_Userc = 0;
					}
					for (uint8_t i = 0; i < 100; i++)
					{
						if (flash_list.finger_user_bufer[Lusers.APMData.Finger_Userc] == 0)
						{
							break;
						}
						else
						{
							Lusers.APMData.Finger_Userc ++;
							if (Lusers.APMData.Finger_Userc > 99)
							{
								Lusers.APMData.Finger_Userc = 0;
							}
						}							
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDDELETEFINGERMENU)
				{
					Lusers.APMData.Finger_Userc ++;
					if (Lusers.APMData.Finger_Userc > 99)
					{
						Lusers.APMData.Finger_Userc = 1;
					}
					for (uint8_t i = 1; i < 100; i++)
					{
						if (flash_list.finger_user_bufer[Lusers.APMData.Finger_Userc] == 1)
						{
							break;
						}
						else
						{
							Lusers.APMData.Finger_Userc ++;
							if (Lusers.APMData.Finger_Userc > 99)
							{
								Lusers.APMData.Finger_Userc = 1;
							}
						}							
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDADDPWDMENU1)
				{
					Lusers.APMData.Pwd_Userc ++;
					if (Lusers.APMData.Pwd_Userc > 9)
					{
						Lusers.APMData.Pwd_Userc = 0;
					}
					for (uint8_t i = 0; i < 10; i++)
					{
						if (flash_list.pwd_user_bufer[Lusers.APMData.Pwd_Userc] == 0)
						{
							break;
						}
						else
						{
							Lusers.APMData.Pwd_Userc ++;
							if (Lusers.APMData.Pwd_Userc > 9)
							{
								Lusers.APMData.Pwd_Userc = 0;
							}
						}							
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDDELETEFPWDMENU)
				{
					Lusers.APMData.Pwd_Userc ++;
					if (Lusers.APMData.Pwd_Userc > 9)
					{
						Lusers.APMData.Pwd_Userc = 1;
					}
					for (uint8_t i = 1; i < 10; i++)
					{
						if (flash_list.pwd_user_bufer[Lusers.APMData.Pwd_Userc] == 1)
						{
							break;
						}
						else
						{
							Lusers.APMData.Pwd_Userc ++;
							if (Lusers.APMData.Pwd_Userc > 9)
							{
								Lusers.APMData.Pwd_Userc = 1;
							}
						}							
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDADDNFCMENU1)
				{
					Lusers.APMData.Nfc_Userc ++;
					if (Lusers.APMData.Nfc_Userc > 79)
					{
						Lusers.APMData.Nfc_Userc = 0;
					}
					for (uint8_t i = 0; i < 80; i++)
					{
						if (flash_list.nfc_user_bufer[Lusers.APMData.Nfc_Userc] == 0)
						{
							break;
						}
						else
						{
							Lusers.APMData.Nfc_Userc ++;
							if (Lusers.APMData.Nfc_Userc > 79)
							{
								Lusers.APMData.Nfc_Userc = 0;
							}
						}							
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDDELETEFNFCMENU)
				{
					Lusers.APMData.Nfc_Userc ++;
					if (Lusers.APMData.Nfc_Userc > 79)
					{
						Lusers.APMData.Nfc_Userc = 0;
					}
					for (uint8_t i = 0; i < 80; i++)
					{
						if (flash_list.nfc_user_bufer[Lusers.APMData.Nfc_Userc] == 1)
						{
							break;
						}
						else
						{
							Lusers.APMData.Nfc_Userc ++;
							if (Lusers.APMData.Nfc_Userc > 79)
							{
								Lusers.APMData.Nfc_Userc = 0;
							}
						}							
					}
				}
				else if (Lusers.APMData.Show_Page == OLEDFINDRECORD)
				{
					if (open_list.open_bufer[Lusers.APMData.Open_Record_Userc][0] != 0)
					{
						printf("��һ���������п�������\n");
						if (Lusers.APMData.Open_Record_Userc >= 200)
						{
							Lusers.APMData.Open_Record_Userc = 1;
						}
						else
						{
						  Lusers.APMData.Open_Record_Userc ++;
						}
					}
					else 
					{
						Lusers.APMData.Open_Record_Userc = 1;
					}
				}
			}			
			Key_Data = 13; //�����갴�����ݺ��λ
		}

		if (Lusers.APMData.Show_Page != OLEDCLEAR)
		{
			if ((Lusers.APMData.Show_Page == OLEDMAIN) && (Lusers.APMData.OLEDReturn > 5)) //������5���������
			{
				printf("������ͣ���������룬�Զ���������״̬\n");
				Lusers.APMData.OLEDReturn = 0;			
				Lusers.APMData.Show_Page = OLEDCLEAR;
			}
			else if ((Lusers.APMData.Show_Page == OLEDLOWPOWER) && (Lusers.APMData.OLEDReturn > 3))
			{
				Lusers.APMData.OLEDReturn = 0;
				Lusers.APMData.Show_Page = OLEDMAIN;				
			}
			else if ((Lusers.APMData.Show_Page != OLEDMAIN) && (Lusers.APMData.Show_Page != OLEDLOWPOWER) && (Lusers.APMData.Show_Page != OLEDAPPPAIR) && (Lusers.APMData.Show_Page != OLEDAPPOPENDOOR) && (Lusers.APMData.OLEDReturn > 10)) //��������ʮ�������������
			{
				Lusers.APMData.OLEDReturn = 0;
				Lusers.APMData.Show_Page = OLEDMAIN;
				printf("����ʮ�룬�����������棬����һЩ��ʱ����\n");
				Lusers.APMData.Pwd_Index = 0;
				memset(Lusers.APMData.Pwd_Bufer, 0, sizeof(Lusers.APMData.Pwd_Bufer));
				OpenDoorState = 0;
			}
			else if ((Lusers.APMData.Show_Page == OLEDAPPPAIR) && (Lusers.APMData.OLEDReturn > 180))
			{
				Lusers.Application.Voice_Play(0x01); //���ʧ��
				Lusers.Application.OLED_Show(OLEDAPPPAIRFAIL);
				Lusers.Application.MSDelay(1000);
				Lusers.APMData.Show_Page = OLEDMAIN;
			}
			else if ((Lusers.APMData.Show_Page == OLEDAPPOPENDOOR) && (Lusers.APMData.OLEDReturn > 60))
			{
				Lusers.APMData.APP_Open_Time = 0;
				Lusers.APMData.OLEDReturn = 0;
				Lusers.APMData.Show_Page = OLEDMAIN;
			}
			else if ((Lusers.APMData.Show_Page == OLEDLOWPOWER) && (Lusers.APMData.OLEDReturn > 3))
			{
				Lusers.APMData.OLEDReturn = 0;
				Lusers.APMData.Show_Page = OLEDMAIN;
			}
	  }
		fingerService();      //ָ����������
		wifi_uart_service();  //WIFI���ݴ��������
	  Lusers.Application.OLED_Show(Lusers.APMData.Show_Page);
		Lusers.Application.MSDelay(10);
/************************��������״̬******************************/
		
		if (Lusers.APMData.Show_Page == OLEDCLEAR)
		{
			OLED_Clear();
			Lusers.APMData.OLEDReturn = 0;
			Lusers.Application.ONOFFLed(LED_OFF);
			if (Lusers.APMData.wifi_risi == 0)
			{
				printf("����WIFIģ��\n");
				WIFI_POWER_OFF;
			}
			Lusers.Application.MSDelay(300);
			WIFI_POWER_ON;
			Lusers.Application.ONOFFFinger(FINGER_POWER_OFF);
			Lusers.APMData.Inter_Data = NOT_INTER;
/*******************************ѭ���������Ƿ�����жϻ���*******************************/
			waitsleep: if (Lusers.APMData.OLEDReturn <= 3)
			{
				printf("����ǰѭ������Ƿ���������\n");
				Lusers.Application.MSDelay(300);
				if (Lusers.APMData.Inter_Data != NOT_INTER)
				{
					Lusers.Application.ONOFFFinger(FINGER_POWER_ON);
					if (Lusers.APMData.Inter_Data == FINGER_INTER)
					{
						Lusers.Application.MSDelay(200);					
						Lusers.Application.Finger_Play(FINGER_VERITY, 0);
					}
					Lusers.APMData.Show_Page = OLEDMAIN;
					continue;
				}
				wifi_uart_service();
				goto waitsleep;
			}
			
			Lusers.APMData.Inter_Data = NOT_INTER;
			Lusers.APMData.OLEDReturn = 0;	
			
			waitloop: if ((Lusers.APMData.wifi_risi == 1) && (Lusers.APMData.OLEDReturn < 10))
			{
				printf("wifiģ��Ӳ�������������ȴ�10sʹ������\n");
				Lusers.Application.MSDelay(300);
				if (Lusers.APMData.Inter_Data != NOT_INTER)
				{
					Lusers.Application.ONOFFFinger(FINGER_POWER_ON);
					if (Lusers.APMData.Inter_Data == FINGER_INTER)
					{
						printf("����ָ������ָ֤��12\n");
						Lusers.Application.MSDelay(200);					
						Lusers.Application.Finger_Play(FINGER_VERITY, 0);
					}
					Lusers.APMData.Show_Page = OLEDMAIN;
					continue;
				}
				wifi_uart_service();
				goto waitloop;
			}

			facsleep: OLED_OFF;
      Lusers.Application.GetTime();
			flash_list.time_bufer[0] = hymd.Year;
			flash_list.time_bufer[1] = hymd.Month;
			flash_list.time_bufer[2] = hymd.Date;
			flash_list.time_bufer[3] = hhms.Hours;
			flash_list.time_bufer[4] = hhms.Minutes;
			flash_list.time_bufer[5] = hhms.Seconds;
			Lusers.Application.FLASH_WPlay(DATAFLASH);//����ʱ��
			
			printf("Lusers.APMData.wifi_risi = %d\n", Lusers.APMData.wifi_risi);
			if (Lusers.APMData.wifi_risi >= 5)
			{
				printf("�ϱ���ǰ�ٷֱȵ��� %d%%\n", 100*(GetPowerData - 2120)/420);
				mcu_dp_value_update(DPID_RESIDUAL_ELECTRICITY, (100*(GetPowerData - 2120)/420>=100)?100:100*(GetPowerData - 2120)/420, NOT_TIME_UPDATA);
				Lusers.Application.MSDelay(200);
				for (uint8_t i = 0; i < finger_state.finger_open_index; i++)
				{
					printf("�ϱ���ָ���ż�¼\n");
					mcu_dp_value_update(DPID_UNLOCK_FINGER_VEIN, finger_state.finger_open_buf[i], NEED_TIME_UPDATA);
					Lusers.Application.MSDelay(200);
				}
			  finger_state.finger_open_index = 0;
				memset(finger_state.finger_open_buf, 0, sizeof(finger_state.finger_open_buf));
        wifi_uart_service();
			}
			
			printf("��������ģʽ\n");
			Lusers.APMData.OLEDReturn = 0;
			Lusers.APMData.wifi_risi = 0;
			WIFI_POWER_OFF; 
			
			Finger_Inter_IT = 0;
			Nfc_Status = NO;
			Lpcd_Set_Mode(1);
      Touch_DeInit();
			MX_ADC_DeInit();

			HAL_I2C_DeInit(&hi2c1);
			HAL_TIM_Base_DeInit(&htim3);
			HAL_ADC_DeInit(&hadc);
		
			HAL_TIM_Base_Stop_IT(&htim3);
			HAL_TIM_Base_MspDeInit(&htim3);
			HAL_ADC_MspDeInit(&hadc);

			HAL_UART_DeInit(&huart1);
			HAL_UART_DeInit(&huart2);
			
			Low_Power_Gpio();
//			HAL_DeInit();
      __HAL_RCC_PWR_CLK_ENABLE();
			__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
			HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);
/************************��������**********************************/
      HAL_Init();
			SystemClock_Config();
			__HAL_RCC_PWR_CLK_ENABLE();

			Initialization_Layer();
			Application_Layer();
			printf("����ԴΪ��%d\n", Lusers.APMData.Inter_Data);
			
		  Lusers.Application.ONOFFLed(LED_ON);
			Lusers.Application.ONOFFFinger(FINGER_POWER_ON);

      __HAL_RCC_SPI1_CLK_ENABLE();
			Lpcd_Set_Mode(0);//�˳�LPCD 			
			SPI_WakeUp();
			Lpcd_Get_IRQ(&Lpcd.Irq);
			Lpcd_IRQ_Event();
			printf("nfc init ok!\n");

			HAL_ADC_MspInit(&hadc);
			HAL_I2C_MspInit(&hi2c1);
			HAL_TIM_Base_MspInit(&htim3);	

			GetPowerData = Lusers.Application.Get_Power();
			if (GetPowerData <= 2230)
			{
				Lusers.APMData.Low_Power_toWiFi = 0;
				Lusers.APMData.Show_Page = OLEDLOWPOWER;
			}
			else
			{
				Lusers.APMData.Show_Page = OLEDMAIN;
			}
			Lusers.Application.OLED_Show(Lusers.APMData.Show_Page);	
			
			Lusers.Application.MSDelay(100);
			printf("ָ������������\n");	
			Lusers.Application.Finger_Play(FINGER_CONNECT, 0);
			if (Finger_Inter_IT == 1)
			{
				Lusers.Application.MSDelay(200);
				fingerService();
				printf("ָ�������ѷ�����֤\n");
				Lusers.Application.Finger_Play(FINGER_VERITY, 0);
			}
			Key_Data = 13;
			Lusers.Application.MSDelay(100);  //����ǰ�������ֵ����ʱ200ms����ʾ������
	  }
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14
                              |RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
