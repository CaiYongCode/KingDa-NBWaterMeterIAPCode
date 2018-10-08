#ifndef __BC95_H__
#define __BC95_H__
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include "include.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/
#define         RECV_BUFF_SIZE               256//���ջ�������С
#define         SEND_BUFF_SIZE               128//���ͻ�������С
#define         CONNECT_ERROR_NUM            3  //���Ӵ������
#define         SEND_ERROR_NUM               3  //���ʹ������

#define         BC95_POWER_UP_TIME           5//BC95�ϵ�ʱ�� ��λms
#define         BC95_R_TIMEROUT_TIME         5 //���ճ�ʱ ��λms

#define         BC95_S_TIMEROUT_TIME         6000 //�������ݳ�ʱ ��λms

/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/
struct Error_Connter_Str
{
  unsigned char Send;
  unsigned char Connect;
};
 

enum Start_Process_En   //��������
{
  BC95_POWER_DOWN,      //����
  BC95_POWER_UP,        //�ϵ�
  BC95_RECONNECT,       //����
  
  NRB,                  //����
  AT,                   //ͬ��������
  GETNBAND,             //��ѯƵ��
  SETNBAND,             //����Ƶ��
  GETCFUN,              //��ѯ�绰����
  SETCFUN,              //���õ绰����
  CIMI,                 //��ѯIMSI
  CGSN,                 //��ѯIMEI  
  CCID,                //��ѯCCID
  CSQ,                  //��ѯ�ź�ǿ��
  GETCGDCONT,           //��ѯAPN
  SETCGDCONT,           //����APN  
  GETCGATT,             //��ѯ���缤��״̬
  SETCGATT,             //��������
  CEREG,                //��ѯ����ע��״̬
  CCLK,                 //��ѯʵʱʱ��
  GETNCDP,              //��ѯCDP������
  SETNCDP,              //����CDP������
  NSMI,                 //���÷�����Ϣָʾ
  NNMI,                 //���ý�����Ϣָʾ
  NMGS,                 //������Ϣ
  NQMGR,                //��ѯ��Ϣ����
  NMGR,                 //������Ϣ
  BC95_CONNECT_ERROR,      //��������
};
  
struct BC95_Str//BC95 �ܽṹ��
{
  char R_Buffer[RECV_BUFF_SIZE];        //���ջ�����
  unsigned short Recv_Length;         //���ճ���
  enum Start_Process_En Start_Process;  //���ӽ���
  u8 Report_Bit;                        //����λ
  struct Error_Connter_Str Err_Conner; //�������
  unsigned char ErrorRecord;          //�����¼
  unsigned char Rssi;                 //�ź�ǿ��
  bool Incident_Pend;                  //�¼������־

  unsigned char Reconnect_Times;      //��������
  unsigned char FailTimes;            //ʧ�ܴ���
  unsigned char ICCID[20];
  unsigned char IMEI[15];
};
 
/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/
extern struct BC95_Str BC95;//BC95 �õļĴ���
/*********************************************************************************************************
����������
*********************************************************************************************************/
void BC95_Power_On(void); 
void BC95_Power_Off(void);
void BC95_Reset(void);
void BC95_Process(void);
void BC95_Data_Send(unsigned char *Data,unsigned short Len);

void BC95_Start(void);
void BC95_Recv_Timeout_CallBack(void);
void BC95_Delay_CallBack(void);

void Recv_Data_Process(unsigned char* buff);
void Send_Data_Process(void);
void ACK(u8 messageId,u8 errcode,u8 mid[4]);
void Report_All_Parameters(void);
void Report_HC_Flow(void);

unsigned char Report_History_Data(void);

/********************************************************************************/
#endif

/******************************************END********************************************************/