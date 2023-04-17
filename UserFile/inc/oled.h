#ifndef __oled_h__
#define __oled_h__

#include "include.h"

#define OLED_POWER_PORT GPIOC
#define OLED_POWER_PIN  GPIO_PIN_13

#define OLED_ON  HAL_GPIO_WritePin(OLED_POWER_PORT, OLED_POWER_PIN, GPIO_PIN_SET)
#define OLED_OFF HAL_GPIO_WritePin(OLED_POWER_PORT, OLED_POWER_PIN, GPIO_PIN_RESET)

#define OLEDCLEAR            0 
#define OLEDSTARTUP          1
#define OLEDVFAIL            2
#define OLEDVSUCCESS         3
#define OLEDMAIN             4
#define OLEDADDFINGER        5
#define OLEDADDFINGERAGING   6
#define OLEDADDFINGERSUCCESS 7
#define OLEDADDFINGERFAIL    8
#define OLEDFACTORYSUCCESS   9

#define OLEDLOWPOWER         10
#define OLEDADMIN            11

/**********һ���˵�***********/
#define OLEDMENU1            12
#define OLEDMENU2            13
#define OLEDMENU3            14
#define OLEDMENU4            15
#define OLEDMENU5            16
#define OLEDMENU6            17

/*********������ָ�˵�********/
#define OLEDFINGERMENU1      18
#define OLEDFINGERMENU2      19

/*********��������˵�********/
#define OLEDWORDMENU1        20
#define OLEDWORDMENU2        21

/*********������Ƭ�˵�********/
#define OLEDNFCMENU1         22
#define OLEDNFCMENU2         23

/*********������ָ�˵�********/
#define OLEDADDFINGERMENU1   24
#define OLEDDELETEFINGERMENU 25

/*********��������˵�********/
#define OLEDADDPWDMENU1      26
#define OLEDDELETEFPWDMENU   27

/*********������Ƭ�˵�********/
#define OLEDADDNFCMENU1      28
#define OLEDDELETEFNFCMENU   29

#define OLEDNOTHAVEFINGER    30
#define OLEDNOTHAVEPWD       31
#define OLEDNOTHAVENFC       32

/*********����ϵͳ�˵�********/
#define OLEDSYSTEMMENU1      33
#define OLEDSYSTEMMENU2      34
#define OLEDSYSTEMMENU3      35
#define OLEDSYSTEMMENU4      36

/*********�����ָ�����********/
#define OLEDFACTORYMENU      37
#define OLEDWAITFACTORY      38

/*******����ɾ����ָ�˵�******/
#define OLEDDELETEFINGERSUCC 39
#define OLEDDELETEFINGERFAIL 40

/**********��������***********/
#define OLEDINPUTPWD         41
#define OLEDADMININPUTPWD    42

/*********�ļ��������********/
#define OLEDINPUTADDPWD      43
#define OLEDINPUTADDPWDIN    44

/***********ȷ������**********/
#define OLEDCONFIRMPWD       45
#define OLEDPWDNOTLENTH      46
#define OLEDCONFIRMPWDIN     47

/***********���벻ͬ**********/
#define OLEDPWDDISHAVE       48
#define OLEDPWDISHAVE        49

/*********������ӳɹ�********/
#define OLEDADDPWDSUCCESS    50

/*********������֤���********/
#define OLEDPWDPASS          51
#define OLEDPWDNOTPASS       52

/*********����ɾ�����********/
#define OLEDDELETEPWDSUCC    53

/*********����ʱ�����********/
#define OLEDSETTIMEMENU      54
#define OLEDSETTIMESUCCESS   55

/*********������������********/
#define OLEDSETAUDIOMENU     56
#define OLEDSETAUDIOSUCCESS  57

/*********���ÿ��ŷ�ʽ********/
#define OLEDSETOPENONLY      58
#define OLEDSETOPENCONMBO    59
#define OLEDSETOPENSUCCESS   60
#define OLEDOPENPLEASEFINGER 61

#define OLEDFACTORYFAIL      62

/*********�û����ż�¼********/
#define OLEDUSERRECORD       63
#define OLEDOPENRECORD       64

/*********�Ǽ��û���ѯ********/
#define OLEDUSERREGISTER     65
/*********���ż�¼��ѯ********/
#define OLEDOPENREGISTER     66

/*****ָ�������뿨Ƭ����******/
#define OLEDFINGERFULLING    67
#define OLEDPWDFULLING       68
#define OLEDNFCFULLING       69

/*********û�п��ż�¼********/
#define OLEDNOTHAVERECORD    70
#define OLEDFINDRECORD       71

/*********����ϵͳ����********/
#define OLEDSYSPICKPROOF     72

/*********�����Դ���********/
#define OLEDTESTERROR        73

/*********�ļ���ӿ�Ƭ********/
#define OLEDADDNFCDOING      74
#define OLEDADDNFCFAIL       75
#define OLEDADDNFCSAME       76
#define OLEDADDNFCSUCCESS    77

/*********�ļ�ɾ����Ƭ********/
#define OLEDDELETENFCSUCCESS 78

/*********��Ƭ��֤ͨ��********/
#define OLEDVNFCSUCCESS      79

/**********����ģʽ***********/
#define OLEDNOTHAVEADMIN     80

/**********Զ�����***********/
#define OLEDAPPMENU1         81
#define OLEDAPPMENU2         82

#define OLEDAPPPAIR          83
#define OLEDAPPPAIRSUCCESS   84
#define OLEDAPPPAIRFAIL      85

/**********Զ�̿���**********/
#define OLEDAPPOPENDOOR      86

#define OLEDAPPOPENSUCCESS   87
#define OLEDAPPOPENFAIL      88

/**********��̬����**********/
#define OLEDDYMANICPASS      89
#define OLEDDYMANICFAIL      90

/**********��ʱ����**********/
#define OLEDTEMPORARYWAIT    91
#define OLEDTEMPORARYPASS    92
#define OLEDTEMPORARYFAIL    93

/**********�ر�Զ��***********/
#define OLEDCLOSEAPPPAIR     94


void OLED_Init(void);
void OLED_Clear(void);
void OLED_Show_16Char(unsigned char x, unsigned y, unsigned char Data);
void OLED_Show_Play(uint8_t show_data);
void FingerWN_User_Show(uint8_t show_data, uint8_t ShowUser);
#endif

