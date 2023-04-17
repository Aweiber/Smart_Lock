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

/**********一级菜单***********/
#define OLEDMENU1            12
#define OLEDMENU2            13
#define OLEDMENU3            14
#define OLEDMENU4            15
#define OLEDMENU5            16
#define OLEDMENU6            17

/*********二级手指菜单********/
#define OLEDFINGERMENU1      18
#define OLEDFINGERMENU2      19

/*********二级密码菜单********/
#define OLEDWORDMENU1        20
#define OLEDWORDMENU2        21

/*********二级卡片菜单********/
#define OLEDNFCMENU1         22
#define OLEDNFCMENU2         23

/*********三级手指菜单********/
#define OLEDADDFINGERMENU1   24
#define OLEDDELETEFINGERMENU 25

/*********三级密码菜单********/
#define OLEDADDPWDMENU1      26
#define OLEDDELETEFPWDMENU   27

/*********三级卡片菜单********/
#define OLEDADDNFCMENU1      28
#define OLEDDELETEFNFCMENU   29

#define OLEDNOTHAVEFINGER    30
#define OLEDNOTHAVEPWD       31
#define OLEDNOTHAVENFC       32

/*********二级系统菜单********/
#define OLEDSYSTEMMENU1      33
#define OLEDSYSTEMMENU2      34
#define OLEDSYSTEMMENU3      35
#define OLEDSYSTEMMENU4      36

/*********三级恢复出厂********/
#define OLEDFACTORYMENU      37
#define OLEDWAITFACTORY      38

/*******三级删除手指菜单******/
#define OLEDDELETEFINGERSUCC 39
#define OLEDDELETEFINGERFAIL 40

/**********输入密码***********/
#define OLEDINPUTPWD         41
#define OLEDADMININPUTPWD    42

/*********四级添加密码********/
#define OLEDINPUTADDPWD      43
#define OLEDINPUTADDPWDIN    44

/***********确认密码**********/
#define OLEDCONFIRMPWD       45
#define OLEDPWDNOTLENTH      46
#define OLEDCONFIRMPWDIN     47

/***********密码不同**********/
#define OLEDPWDDISHAVE       48
#define OLEDPWDISHAVE        49

/*********密码添加成功********/
#define OLEDADDPWDSUCCESS    50

/*********密码验证结果********/
#define OLEDPWDPASS          51
#define OLEDPWDNOTPASS       52

/*********密码删除结果********/
#define OLEDDELETEPWDSUCC    53

/*********设置时间界面********/
#define OLEDSETTIMEMENU      54
#define OLEDSETTIMESUCCESS   55

/*********设置音量界面********/
#define OLEDSETAUDIOMENU     56
#define OLEDSETAUDIOSUCCESS  57

/*********设置开门方式********/
#define OLEDSETOPENONLY      58
#define OLEDSETOPENCONMBO    59
#define OLEDSETOPENSUCCESS   60
#define OLEDOPENPLEASEFINGER 61

#define OLEDFACTORYFAIL      62

/*********用户开门记录********/
#define OLEDUSERRECORD       63
#define OLEDOPENRECORD       64

/*********登记用户查询********/
#define OLEDUSERREGISTER     65
/*********开门记录查询********/
#define OLEDOPENREGISTER     66

/*****指静脉密码卡片已满******/
#define OLEDFINGERFULLING    67
#define OLEDPWDFULLING       68
#define OLEDNFCFULLING       69

/*********没有开门记录********/
#define OLEDNOTHAVERECORD    70
#define OLEDFINDRECORD       71

/*********防撬系统锁定********/
#define OLEDSYSPICKPROOF     72

/*********锁具试错报警********/
#define OLEDTESTERROR        73

/*********四级添加卡片********/
#define OLEDADDNFCDOING      74
#define OLEDADDNFCFAIL       75
#define OLEDADDNFCSAME       76
#define OLEDADDNFCSUCCESS    77

/*********四级删除卡片********/
#define OLEDDELETENFCSUCCESS 78

/*********卡片验证通过********/
#define OLEDVNFCSUCCESS      79

/**********体验模式***********/
#define OLEDNOTHAVEADMIN     80

/**********远程配对***********/
#define OLEDAPPMENU1         81
#define OLEDAPPMENU2         82

#define OLEDAPPPAIR          83
#define OLEDAPPPAIRSUCCESS   84
#define OLEDAPPPAIRFAIL      85

/**********远程开门**********/
#define OLEDAPPOPENDOOR      86

#define OLEDAPPOPENSUCCESS   87
#define OLEDAPPOPENFAIL      88

/**********动态密码**********/
#define OLEDDYMANICPASS      89
#define OLEDDYMANICFAIL      90

/**********临时密码**********/
#define OLEDTEMPORARYWAIT    91
#define OLEDTEMPORARYPASS    92
#define OLEDTEMPORARYFAIL    93

/**********关闭远程***********/
#define OLEDCLOSEAPPPAIR     94


void OLED_Init(void);
void OLED_Clear(void);
void OLED_Show_16Char(unsigned char x, unsigned y, unsigned char Data);
void OLED_Show_Play(uint8_t show_data);
void FingerWN_User_Show(uint8_t show_data, uint8_t ShowUser);
#endif

