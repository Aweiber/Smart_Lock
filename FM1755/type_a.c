/****************************************Copyright (c)****************************************************
**                            Guangzhou ZLGMCU Technology Co., LTD
**
**                                 http://www.zlgmcu.com
**
**      ������������Ƭ���Ƽ����޹�˾���ṩ�����з�������ּ��Э���ͻ����ٲ�Ʒ���з����ȣ��ڷ�����������ṩ
**  ���κγ����ĵ������Խ����������֧�ֵ����Ϻ���Ϣ���������ο����ͻ���Ȩ��ʹ�û����вο��޸ģ�����˾��
**  �ṩ�κε������ԡ��ɿ��Եȱ�֤�����ڿͻ�ʹ�ù��������κ�ԭ����ɵ��ر�ġ�żȻ�Ļ��ӵ���ʧ������˾��
**  �е��κ����Ρ�
**                                                                        ����������������Ƭ���Ƽ����޹�˾
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           type_a.c
** Last modified Date:  2016-3-14
** Last Version:        V1.00
** Descriptions:        ISO/IEC144443A����ز���
**
**--------------------------------------------------------------------------------------------------------
*/
/*
** ͷ�ļ�
*/

#include "include.h"
#include "string.h"
#include "fm175xx.h"
#include "type_a.h"

extern union Union_Card  Card_Number_Secret;
uint8_t Card_Operate_ID;


uint8_t Read_TyteA_Card (void)
{
    uint8_t k;
    uint8_t statues = TRUE;
    uint8_t picc_atqa[2];                                               /* ????????????     */
    uint8_t picc_uid[12];                                        /* ????UID??              */
    uint8_t picc_sak[3];                                                /* ????????             */
    memset(CardID,'\0',12);
		FM175X_SoftReset( );                                                /* FM175xx????              */
    Set_Rf( 3 );                                                        /* ?????                   */
    Pcd_ConfigISOType( 0 );                                             /* ISO14443??????         */
	  delay_ms(3);
    statues = TypeA_CardActive( picc_atqa,picc_uid,picc_sak );    /* ????                     */
    if ( statues == TRUE ) {
			memcpy(CardID,picc_uid,8);
				k=OK;
//			delay_ms(300);
    }
		else
		{
			k=NO;
		}
		Set_Rf( 0 );  
		SetReg(ComIEnReg, 0x80);                                    /* */
    SetReg(DivIEnReg, 0x80);
    SetReg(ComIrqReg,0x7f);                                    /* */ 	
    return k;		
}

uint8_t Add_TyteA_Card (void)
{
		return Read_TyteA_Card();
}



uint8_t TyteA_Comparation (void)
{
    uint8_t k;
    uint8_t statues = TRUE;
    uint8_t cardFlag = 35;
    uint8_t picc_atqa[2];                                               /* ????????????     */
    static uint8_t picc_uid[15];                                        /* ????UID??              */
    uint8_t picc_sak[3];                                                /* ????????             */
    FM175X_SoftReset( );                                                /* FM175xx????              */
    Set_Rf( 3 );                                                        /* ?????                   */
    Pcd_ConfigISOType( 0 );                                             /* ISO14443??????         */
	  delay_ms(3);
    statues = TypeA_CardActive( picc_atqa,picc_uid,picc_sak );    /* ????                     */
    if ( statues == TRUE ) {
					   //k=TypeA_Card_Comparation(picc_uid);
					   if(k<30){
							  cardFlag=k;
						    Card_Operate_ID=k;							 
             }							 
    }
		else
		{
			cardFlag=36;			
		}
		Set_Rf( 0 );  
  //  LED_RedOff();
		SetReg(ComIEnReg, 0x80);                                    /* */
    SetReg(DivIEnReg, 0x80);
    SetReg(ComIrqReg,0x7f);                                    /* */ 	
    return cardFlag;	
}





/*********************************************************************************************************
** Function name:       TypeA_Request        
** Descriptions:        TypeA_Request��ƬѰ��    
** input parameters:    N/A
** output parameters:   pTagType[0],pTagType[1] =ATQA 
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
unsigned char TypeA_Request(unsigned char *pTagType)
{
    unsigned char  result,send_buff[1],rece_buff[2];
    unsigned int  rece_bitlen;  
    Clear_BitMask(TxModeReg,0x80);                                      /* �ر�TX CRC                   */
    Clear_BitMask(RxModeReg,0x80);                                      /* �ر�RX CRC                   */
    Set_BitMask(RxModeReg, 0x08);                                       /* �ر�λ����                   */
    Clear_BitMask(Status2Reg,0x08);                                     /* ������ܱ�־��ʹ������ͨ��   */
    Write_Reg(BitFramingReg,0x07);                                      /* ���һ�ֽڷ���7λ            */
    send_buff[0] = 0x26;                                                /* Ѱ������ 0x26                */                  
    Pcd_SetTimer(1);
    Clear_FIFO();
    result = Pcd_Comm(Transceive,send_buff,1,rece_buff,&rece_bitlen);
    if ((result == TRUE) && (rece_bitlen == 2*8)) {                     /* �������յ�2�ֽڷ�����Ϣ      */
        *pTagType     = rece_buff[0];
        *(pTagType+1) = rece_buff[1];
    }
    return result;
}

/*********************************************************************************************************
** Function name:       TypeA_WakeUp        
** Descriptions:        ��Ƭ����
** input parameters:    N/A
** output parameters:   pTagType[0],pTagType[1] =ATQA 
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
unsigned char TypeA_WakeUp(unsigned char *pTagType)
{
    unsigned char   result,send_buff[1],rece_buff[2];
    unsigned int   rece_bitlen;  
    Clear_BitMask(TxModeReg,0x80);                                      /* �ر�TX CRC                   */
    Clear_BitMask(RxModeReg,0x80);                                      /* �ر�RX CRC                   */
    Set_BitMask(RxModeReg, 0x08);                                       /* ������С��4bit������         */
    Clear_BitMask(Status2Reg,0x08);                                     /* ������ܱ�־��ʹ������ͨ��   */
    Write_Reg(BitFramingReg,0x07);                                      /* ���һ�ֽڷ���7λ            */
    send_buff[0] = 0x52;
    Pcd_SetTimer(1);
    Clear_FIFO();
    result = Pcd_Comm(Transceive,send_buff,1,rece_buff,&rece_bitlen);
    if ((result == TRUE) && (rece_bitlen == 2*8)) {
        *pTagType     = rece_buff[0];
        *(pTagType+1) = rece_buff[1];
    }
    return result;
}


/*********************************************************************************************************
** Function name:       RightMoveBit        
** Descriptions:        �ú���ʵ��һ���ֽڶ��ٸ�1
** input parameters:    bitNum: �ұ߶��ٸ�1
** output parameters:   
** Returned value:      
*********************************************************************************************************/
uint8_t RightMoveBit(uint8_t bitNum)
{
    uint8_t temp,i;
    temp = 0x00;
    for(i=0;i<bitNum;i++)
    {
      temp >>=1;
       temp += 0x80;
    }
    return temp;
}

/*********************************************************************************************************
** Function name:       RightMoveBit        
** Descriptions:        �ú���ʵ��һ���ֽڶ��ٸ�1
** input parameters:    bitNum: ��߶��ٸ�1
** output parameters:   
** Returned value:      
*********************************************************************************************************/
uint8_t LeftMoveBit(uint8_t bitNum)
{
    uint8_t temp,i;
    temp = 0x00;
    for(i=0;i<bitNum;i++)
    {
      temp <<=1;
       temp += 0x01;
    }
    return temp;
}
/*********************************************************************************************************
** Function name:       CollSaveUID        
** Descriptions:        ͨ��ǰһ�γ�ͻλ�úͺ�һ�γ�ͻλ�ñ���UID
** input parameters:    preColl:ǰһ�γ�ͻλ��
                        nowColl:��һ�γ�ͻλ��
                        uid:����UIDָ��
** output parameters:   void
** Returned value:      
*********************************************************************************************************/
void CollSaveUID(uint8_t preColl,uint8_t nowColl,uint8_t *buff,uint8_t *uid)
{
    uint8_t i,j,preBytes,preBits,nowBytes,nowBits;
    uint8_t temp;
    preBytes = preColl/8;
    preBits = preColl%8;
    nowBytes = nowColl/8;
    nowBits = nowColl%8;
    
    if(preColl == 0) {                                                  /* ֮ǰû�г�ͻλ��ֱ�ӱ���     */
        for(i=0;i<nowBytes;i++)
             uid[i]=buff[i];  
        uid[nowBytes] = buff[nowBytes] & LeftMoveBit(nowBits);
    }
    else {                                                              /* ֮ǰ�г�ͻ                   */
        if(nowBytes > preBytes) {
             temp =  RightMoveBit(8-preBits);
            temp = buff[0] &temp;
            uid[preBytes] |= temp;
            if(nowBytes > (preBytes+1) ) {                              /* ���������ֽ�uid              */
                for(i=1,j=0;j<(nowBytes-preBytes-1);i++,j++)
                    uid[preBytes+i]=buff[i];  
            }
            if(nowBits) {                                               /* ����λUID                   */
                uid[nowBytes] = buff[nowBytes]&LeftMoveBit(nowBits);
            }
        }
        else
        {
            temp = buff[0] & LeftMoveBit(nowBits);
            uid[preBytes] |= temp;
        }
    }
}
/*********************************************************************************************************
** Function name:       Set_BitFraming        
** Descriptions:        �ú���ʵ�ֶ��յ��Ŀ�Ƭ�����кŵ��ж�
** input parameters:    bytes: ��֪��UID�ֽ���  
**                      bits: �������֪UIDbits        
**                      length: �Ӆ�����UID���ݳ��� 
** output parameters:   NVB:����ͻ����
** Returned value:      void
*********************************************************************************************************/
void TypeA_Set_BitFraming(uint8_t collBit,unsigned char *NVB)
{
    uint8_t bytes,bits;
    bytes = collBit/8;
    bits = collBit%8;
    switch(bytes) {
        case 0: 
            *NVB = 0x20;
            break;
        case 1:
            *NVB = 0x30;
            break;
        case 2:
            *NVB = 0x40;
            break;
        case 3:
            *NVB = 0x50;
            break;
        case 4:
            *NVB = 0x60;
            break;
        default:
            break;
    }    
    switch(bits) {
        case 0:
            Write_Reg(BitFramingReg,0x00);
            break;
        case 1:
            Write_Reg(BitFramingReg,0x11);
            *NVB = (*NVB | 0x01);
            break;
        case 2:
            Write_Reg(BitFramingReg,0x22);
            *NVB = (*NVB | 0x02);
            break;
        case 3:
            Write_Reg(BitFramingReg,0x33);
            *NVB = (*NVB | 0x03);
            break;
        case 4:
            Write_Reg(BitFramingReg,0x44);
            *NVB = (*NVB | 0x04);
            break;
        case 5:
            Write_Reg(BitFramingReg,0x55);
            *NVB = (*NVB | 0x05);
            break;
        case 6:
            Write_Reg(BitFramingReg,0x66);
            *NVB = (*NVB | 0x06);
            break;
        case 7:
            Write_Reg(BitFramingReg,0x77);
            *NVB = (*NVB | 0x07);
            break;
        default:
            break;
    }
}



/*********************************************************************************************************
** Function name:       TypeA_Anticollision        
** Descriptions:        ��Ƭ����ͻ    
** input parameters:    selcode����Ƭѡ����� 0x93��0x95��0x97    
**                      picc_uid����ƬUID��
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
unsigned char TypeA_Anticollision(unsigned char selcode,unsigned char *uid)
{
    uint8_t send_buff[10];
    uint8_t rece_buff[5];
	  uint8_t result;
    uint8_t nBytes,preCollBit,nowCollBit;    
    uint32_t 	rece_bitlen;
    Clear_BitMask(TxModeReg,0x80);                                      /* �ر�TX CRC                   */
    Clear_BitMask(RxModeReg,0x80);                                      /* �ر�RX CRC                   */
    Clear_BitMask(Status2Reg,0x08);                                     /* �����֤��־��ʹ������ͨ��   */
    Write_Reg(BitFramingReg,0x00);                                      /* ���һ�ֽڷ���8λ            */
    Clear_BitMask(CollReg,0x80);                                        /* �������ͻλ                 */
    
    memset(uid,0x00,5);
    memset(rece_buff,0x00,5);
    preCollBit = 0;
	  nowCollBit = 0;
    result = Anticollision;
    while( result == Anticollision ) 
    {
        if(result == Anticollision) 
        {
            CollSaveUID(preCollBit,nowCollBit,&rece_buff[0],uid);
					  
            preCollBit = nowCollBit;
        }
        send_buff[0] = selcode;                                     /* ����ͻ����                   */
        TypeA_Set_BitFraming(preCollBit,&send_buff[1]);            /* ����NVB����                  */
        nBytes = preCollBit/8;                                      /* ���յ����ֽ���               */     
        if(preCollBit%8)
            nBytes++;
        memcpy(&send_buff[2],uid,nBytes);                         /* ����Ч�ֽڿ���������buff     */   
        Pcd_SetTimer(10);                                         /* ���ö�ʱ��                         */
        Clear_FIFO();
        result = Pcd_Comm(Transceive,send_buff,2+nBytes,rece_buff,&rece_bitlen);
//				uartPrintf(" Pcd_Comm=0x%x\r\n",result);
				nowCollBit = (uint8_t)(rece_bitlen&0xff);
//				uartPrintf(" nowCollBit=0x%x\r\n",nowCollBit);
//				uartPrintf(" rece_buff:0x%x 0x%x 0x%x 0x%x 0x%x\r\n",rece_buff[0],rece_buff[1],rece_buff[2],rece_buff[3],rece_buff[3]);
        if(result == TRUE) 
        {
            CollSaveUID(preCollBit,40,&rece_buff[0],uid);
        }
    }
//		uartPrintf(" UID:0x%x 0x%x 0x%x 0x%x\r\n",uid[0],uid[1],uid[2],uid[3]);
    if(uid[4] != (uid[0]^uid[1]^uid[2]^uid[3])) /* ���UIDУ��                 */
    {
        if(TRUE == result )
            result = FALSE;
    }
    return result;
}

/*********************************************************************************************************
** Function name:       TypeA_Select        
** Descriptions:        ѡ��Ƭ
** input parameters:    selcode����Ƭѡ����� 0x93��0x95��0x97    
**                      pSnr����ƬUID��
**                      pSak����Ƭѡ��Ӧ��
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
unsigned char TypeA_Select(unsigned char selcode,unsigned char *pSnr,unsigned char *pSak)
{
    unsigned char   result,i,send_buff[7],rece_buff[5];
    unsigned int   rece_bitlen;
    Write_Reg(BitFramingReg,0x00);
    Set_BitMask(TxModeReg,0x80);                                        /* ��TX CRC                   */
    Set_BitMask(RxModeReg,0x80);                                        /* �򿪽���RX ��CRCУ��         */
    Clear_BitMask(Status2Reg,0x08);                                     /* �����֤��־λ               */
    
    send_buff[0] = selcode;                                             /* select����                   */
    send_buff[1] = 0x70;                                                /* NVB                          */
    for (i=0; i<5; i++) {
        send_buff[i+2] = *(pSnr+i);
    }
    send_buff[6] = pSnr[0]^pSnr[1]^pSnr[2]^pSnr[3];
    Pcd_SetTimer(1);
    Clear_FIFO();
    result = Pcd_Comm(Transceive,send_buff,7,rece_buff,&rece_bitlen);
    if (result == TRUE) {
        *pSak=rece_buff[0]; 
    }
    return result;
}

/*********************************************************************************************************
** Function name:       TypeA_Halt        
** Descriptions:        ��Ƭ˯��
** input parameters:    AnticollisionFlag ������֤��־ 
**                      AnticollisionFlag = 0 û����֤��ʹ������ͨ�ţ���Ҫ�����֤��־
**                      AnticollisionFlag = 1 ������֤����ͨ����ʹ������ͨ�ţ�����Ҫ�����֤��־
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
unsigned char TypeA_Halt(unsigned char AnticollisionFlag)
{
    unsigned char   result,send_buff[2],rece_buff[1];
    unsigned int   rece_bitlen;
    send_buff[0] = 0x50;
    send_buff[1] = 0x00;
    Write_Reg(BitFramingReg,0x00);                                      /* �����һ�ֽ�8λ            */
    Set_BitMask(TxModeReg,0x80);                                        /* ��TX CRC                   */
    Set_BitMask(RxModeReg,0x80);                                        /* ��RX CRC                   */
    if( !AnticollisionFlag ) {
        Clear_BitMask(Status2Reg,0x08);
    }
    Pcd_SetTimer(100);
    Clear_FIFO();
    result = Pcd_Comm(Transmit,send_buff,2,rece_buff,&rece_bitlen);
    return result;
}

/*********************************************************************************************************
** Function name:       TypeA_CardActive        
** Descriptions:        ����ʵ��Ѱ������ͻ��ѡ��
** input parameters:    pTagType: ��Ƭ���� ATQA
**                      pSnr: ��ƬUID
**                      pSak: ��ƬӦ������ SAK
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
unsigned char TypeA_CardActive(unsigned char *pTagType,unsigned char *pSnr,unsigned char *pSak)
{
    unsigned char   result;
    result=TypeA_Request(pTagType);                                     /* Ѱ�� Standard                */
//	  uartPrintf( "TypeA_Request = 0x%x\r\n",result);
        if (result!=TRUE) {
        return FALSE;
    }

    if( (pTagType[0]&0xC0) == 0x00 ) {                                  /* M1��,ID��ֻ��4λ             */
        result=TypeA_Anticollision(0x93,pSnr);
        if (result==FALSE) {
            return FALSE;
        }
        result=TypeA_Select(0x93,pSnr,pSak);                            /* ѡ��UID                      */
        if (result == FALSE) {
            return FALSE;
        }    
    }
        
    if( (pTagType[0]&0xC0) == 0x40 )  {                                 /* ID����7λ                    */
        result=TypeA_Anticollision(0x93,pSnr);
//			 uartPrintf( "TypeA_Anticollision = 0x%x\r\n",result);
        if (result==FALSE) {
            return FALSE;
        }
        result=TypeA_Select(0x93,pSnr,pSak);
//				uartPrintf( "TypeA_Select = 0x%x\r\n",result);
        if (result==FALSE) {
            return FALSE;
        }
        result=TypeA_Anticollision(0x95,pSnr+5);
//				uartPrintf( "TypeA_Anticollision = 0x%x\r\n",result);
        if (result==FALSE) {
            return FALSE;
        }
        result=TypeA_Select(0x95,pSnr+5,pSak+1);
//				uartPrintf( "TypeA_Select = 0x%x\r\n",result);
        if (result==FALSE) {
            return FALSE;
        }
    }
		
    if( (pTagType[0]&0xC0) == 0x80 )  {                                 /* ID����10λ                   */
        result=TypeA_Anticollision(0x93,pSnr);
        if (result==FALSE) {
            return FALSE;
        }
        result=TypeA_Select(0x93,pSnr,pSak);
        if (result==FALSE) {
             return FALSE;
        }
        result=TypeA_Anticollision(0x95,pSnr+5);
        if (result==FALSE) {
            return FALSE;
        }
        result=TypeA_Select(0x95,pSnr+5,pSak+1);
        if (result==FALSE) {
            return FALSE;
        }
        result=TypeA_Anticollision(0x97,pSnr+10);
        if (result==FALSE) {
            return FALSE;
        }
        result=TypeA_Select(0x97,pSnr+10,pSak+2);
        if (result==FALSE) {
            return FALSE;
        }
    }
    return result;
}

