/*********************************************************************************************************
//������
//���ߣ����ƮԾ    ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************************************/
#ifndef __IAP_H__
#define __IAP_H__

/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include "include.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/
/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/
enum Upgrade_Process
{
  WAIT,
  MESSAGE19 = 19,
  MESSAGE20 = 20,
  MESSAGE21 = 21,
  MESSAGE22 = 22,
  MESSAGE23 = 23,
  MESSAGE24 = 24,
  FINISH,
  FAIL,
};
struct Upgrade_Str//BC95 �ܽṹ��
{
  unsigned char Flag;   //������־
  bool Incident_Pend;//�¼������־
  enum Upgrade_Process Process;
  unsigned char Version[11];         //�����汾
  unsigned short PackageTotalNum;      //����������
  unsigned short PackageSize;           //��������Ƭ����
  unsigned char PackageNum;             //���������
  uint32_t ProgramAddr;                  //������д���ַ
  unsigned char Buffer[512];            //����������
  unsigned short Length;                //����������
};
/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/
extern struct Upgrade_Str Upgrade;
extern unsigned char APPValid;
/*********************************************************************************************************
����������
*********************************************************************************************************/
void Check_Run_APP(void);
void Run_APP(void);
void STM8_Interrupt_Vector_Table_Init(void);
uint32_t FLASH_ReadWord(uint32_t Address);
ErrorStatus FlashWrite (void *pbuff, unsigned short length);
void STM8_Interrupt_Vector_Table_Redirection(void);
void JumptoAPP(void);
void Upgrade_Process(unsigned char *str);
void SendUpgradeMessage19(void);
void SendUpgradeMessage20(unsigned char ResultCode);
void SendUpgradeMessage21(void);
void SendUpgradeMessage22(unsigned char ResultCode);
void SendUpgradeMessage23(void);
void SendUpgradeMessage24(void);
void Upgrade_TimeOut_CallBack(void);

void SendUpgradeMessage(void);
/********************************************************************************************************/
#endif

/******************************************END********************************************************/






