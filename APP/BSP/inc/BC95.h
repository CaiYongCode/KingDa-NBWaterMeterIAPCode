#ifndef __BC95_H__
#define __BC95_H__
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include "include.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/
#define         RECV_BUFF_SIZE                512//���ջ�������С
#define         SEND_BUFF_SIZE               128//���ͻ�������С
#define         CONNECT_ERROR_NUM            3  //���Ӵ������
#define         SEND_ERROR_NUM               3  //���ʹ������

#define         BC95_POWER_UP_TIME           5//BC95�ϵ�ʱ�� ��λms
#define         BC95_R_TIMEROUT_TIME         3 //���ճ�ʱ ��λms

#define         BC95_S_TIMEROUT_TIME         6000 //�������ݳ�ʱ ��λms

/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/
//struct Network_Par_Str
//{
//  unsigned char IP[4];   //����IP
//  unsigned short Port;   //���Ӷ˿�
//};
//
//union Network_Par_Uni
//{
//  struct Network_Par_Str Str;
//  unsigned char Byte[6];
//};

struct Error_Connter_Str
{
  unsigned char Send;
  unsigned char Connect;
};
 

enum Start_Process_En   //��������
{
  BC95_RECONNECT,        //����
  BC95_POWER_DOWN,      //����
  BC95_POWER_UP,        //�ϵ�
  
  NRB,                  //����
  AT,                   //ͬ��������
  CIMI,                 //��ѯIMSI
  CGSN,                 //��ѯIMEI  
  
  GETNBAND,             //��ѯƵ��
  SETNBAND,             //����Ƶ��
  GETCFUN,              //��ѯ�绰����
  SETCFUN,              //���õ绰����

//  GETCGDCONT,           //��ѯAPN
//  SETCGDCONT,           //����APN
  GETNCDP,              //��ѯCDP������
  SETNCDP,              //����CDP������
  
  GETCGATT,             //��ѯ���缤��״̬
  SETCGATT,             //��������
  CEREG,                //��ѯ����ע��״̬
  CSQ,                  //��ѯ�ź�ǿ��
  NSMI,                 //���÷�����Ϣָʾ
  NNMI,                 //���ý�����Ϣָʾ
  NQMGR,                //��ѯ��Ϣ����
  NMGR,                 //������Ϣ
  NMGS,                 //������Ϣ
  BC95_CONNECT_ERROR,      //��������
};

enum BC95_State_Eum
{
  NO_State              = 0x00,                                                 //�޽��
  BC95_OK               = 0X01,                                                 //��������
  BC95_NG               = 0X02,                                                 //����ʧ��
  NO_Card               = 0X03                                                  //�޿�
};


enum Send_Process_En//��������
{
  WAIT_CONNECT,//�ȴ�����
  SEND_READY,//���͵ȴ�
  
  SEND_AII_PARA,//�������в���
  SEND_HC_FLOW,//������ʷ�ۻ�����
  SEND_VOL_ALARM,//���͵�ѹ����
  SEND_MAG_ALARM,//���ʹű���  

  SEND_WAIT_OK,//�ȴ����ͳɹ�
  SEND_ERROR,//����ʧ��
  SEND_FINISH,//�������
};

struct Alarm_EN         //���ͱ�־λ
{
  unsigned char Vol_Alarm:1;    //���͵�ѹ����
  unsigned char Mag_Alarm:1;   //���ʹű���  
};
  
struct BC95_Str//BC95 �ܽṹ��
{
  char R_Buffer[RECV_BUFF_SIZE]; //���ջ�����
  char S_Buffer[SEND_BUFF_SIZE]; //���ͻ�����
  unsigned short Recv_Length; //���ͳ���
  unsigned short Send_Length; //���ͳ���
//  unsigned char Data_Buffer[RECV_BUFF_SIZE-20];//���ݻ���
  unsigned short Data_Length;//���ݳ���
  unsigned char R_Channel;//���յ��ŵ�
  unsigned char S_Channel;//���͵��ŵ�
  bool Data_Pend;//���ݹ���
  bool Channel_Connect[6];//�ŵ����ӱ�־
  bool Channel_Connect_ERROR[6];//�ŵ�����ʧ�ܱ�־
  unsigned char Connect_Channel_Num;//�����е��ŵ�
  enum Start_Process_En Start_Process;//���ӽ���
  enum Send_Process_En Send_Process;//���ͽ���
  struct Alarm_EN  Alarm;         //�澯
  u8 Report_Bit;                          //����λ
  struct Error_Connter_Str Err_Conner;//�������
  unsigned char ErrorRecord;    //�����¼
  unsigned char Rssi;//�ź�ǿ��
  bool Incident_Pend;//�¼������־
  enum BC95_State_Eum State;//״̬
  bool ZIPSTATUS_Pend;//���ڲ�ѯ״̬
  unsigned char Reconnect_Times;        //��������
  unsigned short Run_Time;         //����ʱ��
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
void BC95_Start_Timeout_CallBalk(void);
void BC95_Delay_CallBalk(void);

void Recv_Data_Process(void);
void Send_Data_Process(void);
void ACK(u8 messageId,u8 errcode,u8 mid[4]);
void Report_All_Parameters(void);
void Report_HC_Flow(void);

/********************************************************************************/
#endif

/******************************************END********************************************************/