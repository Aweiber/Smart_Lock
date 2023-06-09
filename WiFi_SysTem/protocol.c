/**********************************Copyright (c)**********************************
**                       版权所有 (C), 2015-2020, 涂鸦科技
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    protocol.c
 * @author  涂鸦综合协议开发组
 * @version v1.0.7
 * @date    2020.11.9
 * @brief                
 *                       *******非常重要，一定要看哦！！！********
 *          1. 用户在此文件中实现数据下发/上报功能
 *          2. DP的ID/TYPE及数据处理函数都需要用户按照实际定义实现
 *          3. 当开始某些宏定义后需要用户实现代码的函数内部有#err提示,完成函数后请删除该#err
 */


#include "wifi.h"

/******************************************************************************
                                移植须知:
1:MCU必须在while中直接调用mcu_api.c内的wifi_uart_service()函数
2:程序正常初始化完成后,建议不进行关串口中断,如必须关中断,关中断时间必须短,关中断会引起串口数据包丢失
3:请勿在中断/定时器中断内调用上报函数
******************************************************************************/

         
/******************************************************************************
                              第一步:初始化
1:在需要使用到wifi相关文件的文件中include "wifi.h"
2:在MCU初始化中调用mcu_api.c文件中的wifi_protocol_init()函数
3:将MCU串口单字节发送函数填入protocol.c文件中uart_transmit_output函数内,并删除#error
4:在MCU串口接收函数中调用mcu_api.c文件内的uart_receive_input函数,并将接收到的字节作为参数传入
5:单片机进入while循环后调用mcu_api.c文件内的wifi_uart_service()函数
******************************************************************************/

/******************************************************************************
                        1:dp数据点序列类型对照表
          **此为自动生成代码,如在开发平台有相关修改请重新下载MCU_SDK**         
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_UNLOCK_PASSWORD, DP_TYPE_VALUE},
  {DPID_UNLOCK_TEMPORARY, DP_TYPE_VALUE},
  {DPID_UNLOCK_DYNAMIC, DP_TYPE_VALUE},
  {DPID_UNLOCK_CARD, DP_TYPE_VALUE},
  {DPID_ALARM_LOCK, DP_TYPE_ENUM},
  {DPID_UNLOCK_REQUEST, DP_TYPE_VALUE},
  {DPID_REPLY_UNLOCK_REQUEST, DP_TYPE_BOOL},
  {DPID_RESIDUAL_ELECTRICITY, DP_TYPE_VALUE},
  {DPID_UNLOCK_APP, DP_TYPE_VALUE},
  {DPID_HIJACK, DP_TYPE_BOOL},
  {DPID_CLOSED_OPENED, DP_TYPE_ENUM},
  {DPID_UNLOCK_FINGER_VEIN, DP_TYPE_VALUE},
  {DPID_UPDATE_ALL_PASSWORD, DP_TYPE_RAW},
  {DPID_UPDATE_ALL_CARD, DP_TYPE_RAW},
  {DPID_UPDATE_ALL_FIN_VEIN, DP_TYPE_RAW},
};



/******************************************************************************
                           2:串口单字节发送函数
请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
******************************************************************************/

/**
 * @brief  串口发送数据
 * @param[in] {value} 串口要发送的1字节数据
 * @return Null
 * @note   请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
 */
void uart_transmit_output(unsigned char value)
{
    HAL_UART_Transmit(&huart1, &value, 1, 50);
/*
    //示例:
    extern void Uart_PutChar(unsigned char value);
    Uart_PutChar(value);	                                //串口发送函数
*/
}
/******************************************************************************
                           第二步:实现具体用户函数
1:APP下发数据处理
2:数据上报处理
******************************************************************************/

/******************************************************************************
                            1:所有数据上报处理
当前函数处理全部数据上报(包括可下发/可上报和只上报)
  需要用户按照实际情况实现:
  1:需要实现可下发/可上报数据点上报
  2:需要实现只上报数据点上报
此函数为MCU内部必须调用
用户也可调用此函数实现全部数据上报
******************************************************************************/

//自动化生成数据上报函数

/**
 * @brief  系统所有dp点信息上传,实现APP和muc数据同步
 * @param  Null
 * @return Null
 * @note   此函数SDK内部需调用，MCU必须实现该函数内数据上报功能，包括只上报和可上报可下发型数据
 */
void all_data_update(void)
{
//    #error "请在此处理可下发可上报数据及只上报数据示例,处理完成后删除该行"
    /* 
    //此代码为平台自动生成，请按照实际数据修改每个可下发可上报函数和只上报函数
    mcu_dp_value_update(DPID_UNLOCK_PASSWORD,当前普通密码解锁); //VALUE型数据上报;
    mcu_dp_value_update(DPID_UNLOCK_TEMPORARY,当前临时密码解锁); //VALUE型数据上报;
    mcu_dp_value_update(DPID_UNLOCK_DYNAMIC,当前动态密码解锁); //VALUE型数据上报;
    mcu_dp_value_update(DPID_UNLOCK_CARD,当前卡片解锁); //VALUE型数据上报;
    mcu_dp_enum_update(DPID_ALARM_LOCK,当前告警); //枚举型数据上报;
    mcu_dp_value_update(DPID_UNLOCK_REQUEST,当前远程开门请求倒计时); //VALUE型数据上报;
    mcu_dp_value_update(DPID_RESIDUAL_ELECTRICITY,当前剩余电量); //VALUE型数据上报;
    mcu_dp_value_update(DPID_UNLOCK_APP,当前App远程解锁	wifi门锁); //VALUE型数据上报;
    mcu_dp_bool_update(DPID_HIJACK,当前劫持告警); //BOOL型数据上报;
    mcu_dp_enum_update(DPID_CLOSED_OPENED,当前开合状态); //枚举型数据上报;
    mcu_dp_value_update(DPID_UNLOCK_FINGER_VEIN,当前指静脉解锁); //VALUE型数据上报;
    mcu_dp_raw_update(DPID_UPDATE_ALL_PASSWORD,当前同步所有密码编号指针,当前同步所有密码编号数据长度); //RAW型数据上报;
    mcu_dp_raw_update(DPID_UPDATE_ALL_CARD,当前同步所有卡编号指针,当前同步所有卡编号数据长度); //RAW型数据上报;
    mcu_dp_raw_update(DPID_UPDATE_ALL_FIN_VEIN,当前同步所有指静脉编号指针,当前同步所有指静脉编号数据长度); //RAW型数据上报;

    */
}


/******************************************************************************
                                WARNING!!!    
                            2:所有数据上报处理
自动化代码模板函数,具体请用户自行实现数据处理
******************************************************************************/
/*****************************************************************************
函数名称 : dp_download_reply_unlock_request_handle
功能描述 : 针对DPID_REPLY_UNLOCK_REQUEST的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 只下发类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_reply_unlock_request_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为BOOL
    unsigned char ret;
    //0:关/1:开
    unsigned char reply_unlock_request;
    
    reply_unlock_request = mcu_get_dp_download_bool(value,length);
    if(reply_unlock_request == 0) {			
			printf("远程开门拒绝开门\n");
			Lusers.APMData.test_error ++;
			if (Lusers.APMData.test_error >= TEST_ERROR_MAX)
			{
				printf("试错报警\n");
				Lusers.Application.Voice_Play(0x28);
				Lusers.Application.ONOFFLed(LED_OFF);
				Lusers.APMData.test_error = 0;
				Lusers.APMData.test_error_time = TEST_ERROR_TIME;
			}
			else
			{
				Lusers.Application.Voice_Play(0x25);
				Lusers.Application.OLED_Show(OLEDAPPOPENFAIL);
				Lusers.APMData.APP_Open_Time = 0;
				mcu_dp_value_update(DPID_UNLOCK_REQUEST, Lusers.APMData.APP_Open_Time, NOT_TIME_UPDATA);
				delay_ms(1000);
				Lusers.APMData.Show_Page = OLEDMAIN;
			}
      //开关关
    }else {
			printf("远程开门同意开门\n");
			Lusers.APMData.test_error = 0;
		  Lusers.Application.Voice_Play(0x26);
			Lusers.Application.OLED_Show(OLEDAPPOPENSUCCESS);
			Lusers.Application.ONOFFLed(LED_OFF);
			Lusers.APMData.APP_Open_Time = 0;
			mcu_dp_value_update(DPID_UNLOCK_REQUEST, Lusers.APMData.APP_Open_Time, NOT_TIME_UPDATA);
			Lusers.Application.Motor_Play(MOTOR_OPEN);
			delay_ms(1000);
			mcu_dp_enum_update(DPID_CLOSED_OPENED, 1);
			delay_ms(3000);
			Lusers.Application.Motor_Play(MOTOR_CLOSE);
			mcu_dp_enum_update(DPID_CLOSED_OPENED, 2);
			delay_ms(100);
			mcu_dp_value_update(DPID_UNLOCK_APP, 0, NEED_TIME_UPDATA);
			Lusers.Application.OLED_Show(OLEDCLEAR);
			Lusers.APMData.Show_Page = OLEDCLEAR;
        //开关开
    }
  
    //处理完DP数据后应有反馈
    ret = mcu_dp_bool_update(DPID_REPLY_UNLOCK_REQUEST,reply_unlock_request);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}





/******************************************************************************
                                WARNING!!!                     
以下函数用户请勿修改!!
******************************************************************************/
/**
 * @brief  dp下发处理函数
 * @param[in] {dpid} dpid 序号
 * @param[in] {value} dp数据缓冲区地址
 * @param[in] {length} dp数据长度
 * @return dp处理结果
 * -           0(ERROR): 失败
 * -           1(SUCCESS): 成功
 * @note   该函数用户不能修改
 */
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
    /*********************************
    当前函数处理可下发/可上报数据调用                    
    具体函数内需要实现下发数据处理
    完成用需要将处理结果反馈至APP端,否则APP会认为下发失败
    ***********************************/
    unsigned char ret;
    switch(dpid) {
        case DPID_REPLY_UNLOCK_REQUEST:
            //远程开门请求回复处理函数
				    if (Lusers.APMData.Show_Page == OLEDAPPOPENDOOR)
						{
              ret = dp_download_reply_unlock_request_handle(value,length);
						}
        break;


      default:
      break;
    }
    return ret;
}

/**
 * @brief  获取所有dp命令总和
 * @param[in] Null
 * @return 下发命令总和
 * @note   该函数用户不能修改
 */
unsigned char get_download_cmd_total(void)
{
    return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}



/******************************************************************************
                                WARNING!!!                     
此代码为SDK内部调用,请按照实际dp数据实现函数内部数据
******************************************************************************/
#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCU校对本地RTC时钟
 * @param[in] {time} 获取到的格林时间数据
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void mcu_write_rtctime(unsigned char time[])
{
//    #error "请自行完成RTC时钟写入代码,并删除该行"
    /*
    time[0]为是否获取时间成功标志，为 0 表示失败，为 1表示成功
    time[1] 为 年 份 , 0x00 表 示2000 年
    time[2]为月份，从 1 开始到12 结束
    time[3]为日期，从 1 开始到31 结束
    time[4]为时钟，从 0 开始到23 结束
    time[5]为分钟，从 0 开始到59 结束
    time[6]为秒钟，从 0 开始到59 结束
    time[7]为星期，从 1 开始到 7 结束，1代表星期一
   */
    if(time[0] == 1) {
			 printf("正确接收到wifi模块返回的本地时钟数据\n");
			 Lusers.APMData.Start_UPState = 1;
       MX_Set_Ymd(time[1], time[2], time[3]);
			 MX_Set_Hms(time[4], time[5], time[6]);
    }else {
       printf("没有接收到wifi模块返回的本地时钟数据\n");
			 Lusers.APMData.Start_UPState = 0;
    }
}
#endif

#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  MCU查询格林时间返回函数
 * @param[in] {gl_time} 返回的当前格林时间(从低到高7位，分别为年月日时分秒星期)
 * @return Null
 * @note   MCU主动调用 mcu_get_gelin_time 成功后,该函数内可获取格林时间
 */
void mcu_write_gltime(unsigned char gl_time[])
{
//	#error "请自行完成格林时间记录代码,并删除该行"
    /*
    gl_time[0]为是否获取时间成功标志，为 0 表示失败，为 1表示成功
    gl_time[1]为年份 , 0x00 表示 2000 年
    gl_time[2]为月份，从 1 开始到12 结束
    gl_time[3]为日期，从 1 开始到31 结束
    gl_time[4]为时钟，从 0 开始到23 结束
    gl_time[5]为分钟，从 0 开始到59 结束
    gl_time[6]为秒钟，从 0 开始到59 结束
    gl_time[7]为星期，从 1 开始到 7 结束，1代表星期一
   */
    if(gl_time[0] == 1) {
      //正确接收到wifi模块返回的本地时钟数据 
     
    }else {
      //获取本地时钟数据出错,有可能是当前wifi模块未联网
    }
}
#endif

#ifdef WIFI_TEST_ENABLE
/**
 * @brief  wifi功能测试反馈
 * @param[in] {result} wifi功能测试结果
 * @ref       0: 失败
 * @ref       1: 成功
 * @param[in] {rssi} 测试成功表示wifi信号强度/测试失败表示错误类型
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void wifi_test_result(unsigned char result,unsigned char rssi)
{
//    #error "请自行实现wifi功能测试成功/失败代码,完成后请删除该行"
    if(result == 0) {
        //测试失败
        if(rssi == 0x00) {
            //未扫描到名称为tuya_mdev_test路由器,请检查
        }else if(rssi == 0x01) {
            //模块未授权
        }
    }else {
        //测试成功
        //rssi为信号强度(0-100, 0信号最差，100信号最强)
    }
}
#endif

/**
 * @brief  MCU请求wifi固件升级返回函数
 * @param[in] {status} 校验标志
 * @return Null
 * @note   MCU主动调用 wifi_update_request 函数完成后该函数内可获取升级当前状态
 */
void wifi_update_handle(unsigned char status)
{ 
//    #error "请自行完成wifi模块升级状态返回代码,并删除该行"

    switch (status) {
        case 0: {
            //开始检查固件更新
            break;
        }

        case 1: {
            //已经是最新固件
            break;
        }

        case 2: {
            //正在更新固件
            break;
        }

        case 3: {
            //固件更新成功
            break;
        }

        case 4: {
            //固件更新失败
            break;
        }

        default:
        break;
    }
}

#ifdef SUPPORT_MCU_FIRM_UPDATE
/**
 * @brief  MCU请求mcu固件升级
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在 mcu_update_handle 函数内可获取升级当前状态
 */
void mcu_update_request(void)
{ 
    wifi_uart_write_frame(MCU_UG_REQ_CMD, 0);
}

/**
 * @brief  MCU请求mcu固件升级返回函数
 * @param[in] {status} 校验标志
 * @return Null
 * @note   MCU主动调用 mcu_update_request 函数完成后该函数内可获取升级当前状态
 */
void mcu_update_handle(unsigned char status)
{ 
//    #error "请自行完成mcu升级状态处理代码,并删除该行"

    switch (status) {
        case 0: {
            //开始检查固件更新
            break;
        }

        case 1: {
            //已经是最新固件
            break;
        }

        case 2: {
            //正在更新固件
            break;
        }

        case 3: {
            //固件更新成功
            break;
        }

        case 4: {
            //固件更新失败
            break;
        }

        default:
        break;
    }
}

/**
 * @brief  MCU进入固件升级模式
 * @param[in] {value} 固件缓冲区
 * @param[in] {position} 当前数据包在于固件位置
 * @param[in] {length} 当前固件包长度(固件包长度为0时,表示固件包发送完成)
 * @return Null
 * @note   MCU需要自行实现该功能
 */
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
//    #error "请自行完成MCU固件升级代码,完成后请删除该行"
    if(length == 0) {
        //固件数据发送完成
    }else {
        //固件数据处理
    }
    
    return SUCCESS;
}
#endif

#ifdef REPORTED_MCU_SN_ENABLE
/**
 * @brief  MCU上报SN
 * @param[in] {result} 状态结果
 * @return Null
 * @note   MCU需要先自行调用mcu_sn_updata函数后，在此函数对接收的结果进行处理
 */
void mcu_sn_updata_result(unsigned char result)
{
//    #error "请自行完成MCU上报SN结果处理代码,并删除该行"
    if(0 == result) {
        //上报成功
    }else {
        //上报失败
    }
}
#endif

#ifdef WIFI_RESET_NOTICE_ENABLE
/**
 * @brief  模块重置状态通知
 * @param[in] {result} 状态结果
 * @return Null
 * @note   MCU需要先自行调用mcu_sn_updata函数后，在此函数对接收的结果进行处理
 */
void wifi_reset_notice(unsigned char result)
{
//    #error "请自行完成模块重置状态通知处理代码,并删除该行"
    
    switch(result) {
        case 0x00:
            //模块本地重置
        break;
        
        case 0x01:
            //APP远程重置
        break;
        
        case 0x02:
            //APP恢复出厂重置
        break;
        
        case 0x03:
            //本地数据清除，但设备不离网
        break;
        
        default:break;
    }
    wifi_uart_write_frame(WIFI_RESET_NOTICE_CMD, 0);
}
#endif

/*****************************************************************************
获取当前WIFI信号强度，最高100，最低为0.查询当前wifi强度
*****************************************************************************/
void Get_Wifi_Risi(unsigned char result, unsigned char wifirisi)
{
	if (result == SUCCESS)
	{
		printf("当前信号强度为 %d\n", wifirisi);
		Lusers.APMData.wifi_risi = wifirisi;
	}
	else
	{
		printf("未连上路由器\n");
		Lusers.APMData.wifi_risi = 0;
	}
}

void Send_Get_Wifi_Risi(void)
{
	wifi_uart_write_frame(ROUTE_RSSI_CMD, 0x0002);
}




