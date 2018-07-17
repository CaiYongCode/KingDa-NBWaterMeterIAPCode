/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "include.h"
/*********************************************************************************
����������
*********************************************************************************/

/*********************************************************************************
��������������
*********************************************************************************/
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
/*********************************************************************************
˽�б���������
*********************************************************************************/ 
struct BC95_Str BC95;            //BC95 �õļĴ���
/*********************************************************************************
���Ա���������
*********************************************************************************/
/*********************************************************************************
�ڲ�����������
*********************************************************************************/

/*********************************************************************************
���ܴ��붨����
*********************************************************************************/
/*********************************************************************************
 Function:      //void BC95_Power_On(void)
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void BC95_Power_On(void)        //BC95�ϵ�
{
  RCC_Configuration();
  USART2_Configuration();       //��ʼ������2
   
  
  GPIO_SetBits(GPIOE,GPIO_Pin_2);       //VBAT����        3.1-4.2V������ֵ3.6V
  GPIO_SetBits(GPIOE,GPIO_Pin_1);      //��λ������
  
  BC95.Start_Process = BC95_POWER_UP; 
  
  Create_Timer(ONCE,1,
                     BC95_Reset,0,PROCESS);//���鶨ʱ����ʱ�ص�
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void BC95_Power_Off(void)        //BC95�ϵ�
{
  GPIO_ResetBits(GPIOE,GPIO_Pin_2); 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void BC95_Reset(void)
{
  GPIO_ResetBits(GPIOE,GPIO_Pin_1);     //��λ������
  
  Create_Timer(ONCE,40,
                     BC95_Start,0,PROCESS);//���鶨ʱ����ʱ�ص�
}

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void BC95_Start(void)        //BC95��λ
{
  BC95.Start_Process = AT; //ָ����һ������
  BC95.Incident_Pend = TRUE; //�¼���־����
  BC95.Err_Conner.Connect = SEND_ERROR_NUM;     //���ӳ�ʱ����
}

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void BC95_Process(void)                         //BC95������
{
  char *str = NULL;
  //�����Ҫ�ϱ���Ϣ������BC95
  if(BC95.Report_Bit != 0)
  {
    if(BC95.Start_Process == BC95_RECONNECT)
    {
      if(BC95.Reconnect_Times >= 3)  //������������˯��
      {
        BC95.Reconnect_Times = 0;
        
        BC95.Start_Process = BC95_POWER_DOWN;
        BC95_Power_Off();
        Device_Status = SLEEP_MODE;
      }
      else     //��������
      {
        Device_Status = RUN_MODE;
        BC95_Power_On();
      }
    }
  }
  
  if(BC95.Incident_Pend != FALSE) //����Ƿ����¼�����
  {
    BC95.Incident_Pend = FALSE; //����¼�����
    switch(BC95.Start_Process)
    {
    case AT:                  //ͬ��������
      {
        BC95_Data_Send("AT\r\n",4);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Start_Timeout_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�
      }
      break;
    case GETNBAND:               //��ѯƵ��
      {
        
        BC95_Data_Send("AT+NBAND?\r\n",11);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Start_Timeout_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�
      }
      break;
    case GETCFUN:                //��ѯ�绰����
      {
        BC95_Data_Send("AT+CFUN?\r\n",10);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Start_Timeout_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�
      } 
      break;
    case CGSN:                 //��ѯIMEI      
      {
        BC95_Data_Send("AT+CGSN=1\r\n",11);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Start_Timeout_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�
      }
      break;  
     case CIMI:                 //��ѯIMSI
      {
//        BC95_Data_Send("AT+CIMI\r\n",9); 
        BC95_Data_Send("AT+NCCID\r\n",10); 
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Start_Timeout_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�
      }
      break;  
    case CSQ:                //��ѯ�绰����
      {
        BC95_Data_Send("AT+CSQ\r\n",8);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Start_Timeout_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�
      } 
      break;
    case GETCGATT:                //��ѯ���缤��״̬    
      {
        BC95_Data_Send("AT+CGATT?\r\n",11);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Start_Timeout_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�
      }
      break;    
    case CEREG:                //��ѯ����ע��״̬     
      {
        BC95_Data_Send("AT+CEREG?\r\n",11);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Start_Timeout_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�
      }
      break;
    case GETNCDP:                 //��ѯCDP������
      {
        BC95_Data_Send("AT+NCDP?\r\n",10);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Start_Timeout_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�
      }
      break;
    case SETNCDP:                 //����CDP������ 
      {
        BC95_Data_Send("AT+NCDP=180.101.147.115,5683\r\n",30);   
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Start_Timeout_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�
      }
      break; 
    case NSMI:                 //���÷�����Ϣָʾ
      {
        BC95_Data_Send("AT+NSMI=1\r\n",11);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Start_Timeout_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�
      }
      break;
//    case NNMI:                 //���ý�����Ϣָʾ
//      {
//        BC95_Data_Send("AT+NNMI=1\r\n",11);
//        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
//                     BC95_Start_Timeout_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�
//      }
//      break;
    case NMGS:                 //������Ϣ     
      {       
        if(BC95.Report_Bit != 0)   //����Ϣ����
        {   
          Send_Data_Process();
          Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Start_Timeout_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�
        }
      }
      break;
    case NQMGR:                 //��ѯ��Ϣ���ջ���
      {
        BC95_Data_Send("AT+NQMGR\r\n",10);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Start_Timeout_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�
      }
      break;
    case NMGR:                 //������Ϣ
      {
        BC95_Data_Send("AT+NMGR\r\n",9);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Start_Timeout_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�
      }
      break;
    case BC95_CONNECT_ERROR:      //����ʧ��
      BC95_Power_Off();
      BC95.Start_Process = BC95_RECONNECT;
      BC95.Reconnect_Times++;
      break;
    case BC95_POWER_DOWN:       //���ͽ��������ֱ�ӹص�
      BC95_Power_Off();
      Device_Status = SLEEP_MODE;       //˯��
      break;
    default:
      break;
    }
  }
  if(Uart2.Receive_Pend == TRUE)//�ж�������
  {
    memset(BC95.R_Buffer,'\0',RECV_BUFF_SIZE);//�建��
    BC95.Recv_Length = Uart2_Receive((unsigned char *)BC95.R_Buffer);//��������

    switch(BC95.Start_Process)
    {
    case AT:            //ͬ��������
      {
        if(strstr(BC95.R_Buffer,"OK") != NULL)
        {         
          BC95.Start_Process = GETNBAND;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
        }
      }
      break;
    case GETNBAND:               //��ѯƵ��
      {       
        if(strstr(BC95.R_Buffer,"+NBAND:5") != NULL)    //֧��Ƶ��5
        {
          BC95.Start_Process = GETCFUN;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
        }
      }
      break;
    case GETCFUN:                //��ѯ�绰����
      {
        if(strstr(BC95.R_Buffer,"+CFUN:1") != NULL)     //ȫ����
        {         
          BC95.Start_Process = CGSN;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
        }
      } 
      break;
    case CGSN:         // ��ѯIMEI
      {
        str = strstr(BC95.R_Buffer,"+CGSN");
        if( str != NULL)//��ȡ��IMEI
        {
          memcpy(BC95.IMEI,&str[6],15);
          BC95.Start_Process = CIMI;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
        }
      }
      break;
     case CIMI:          //��ѯIMSI
      {
//        if( strstr(BC95.R_Buffer,"OK") != NULL)
        str = strstr(BC95.R_Buffer,"+NCCID");
        if( str != NULL)
        {
          memcpy(BC95.ICCID,&str[7],20);
          BC95.Start_Process = CSQ;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
        }
      }
      break;
    case CSQ:           //��ѯ�ź�ǿ��
      {
        if(strstr(BC95.R_Buffer,"+CSQ") != NULL)
        {
          BC95.Rssi =0;//�����ź�ǿ��
          BC95.Rssi += BC95.R_Buffer[7]-0x30;
          BC95.Rssi *=10;
          BC95.Rssi += BC95.R_Buffer[8]-0x30;
          BC95.Rssi %= 100;
          if(BC95.Rssi < 99)
          {
            BC95.Start_Process = GETCGATT;
            BC95.Incident_Pend = TRUE;//��ǹ���
            Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
          }
        }
      }
      break;
    case GETCGATT:       //��ѯ���缤��״̬
      {
        if( strstr(BC95.R_Buffer,"+CGATT:1") != NULL)//���缤��
        {        
          BC95.Start_Process = CEREG;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
        }       
      }
      break;
    case CEREG:       //��ѯ����ע��״̬
      {
        if( strstr(BC95.R_Buffer,"+CEREG:0,1") != NULL)//����ע��
        {        
          BC95.Start_Process = GETNCDP;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
        }
      }
      break;   
    case  GETNCDP:                 //��ѯCDP������
      {
        if( strstr(BC95.R_Buffer,"+NCDP:180.101.147.115,5683") != NULL)//��ȡ��NCDP
        {        
          BC95.Start_Process = NSMI;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
        }
        else
        {        
          BC95.Start_Process = SETNCDP;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
        }
      }
      break;
    case  SETNCDP:                 //����CDP������ 
      {
        if(strstr(BC95.R_Buffer,"OK") != NULL)
        {         
          BC95.Start_Process = GETNCDP;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
        }  
      }
      break;
    case NSMI:                 //���÷�����Ϣָʾ
      {
        if(strstr(BC95.R_Buffer,"OK") != NULL)
        {         
          BC95.Start_Process = NMGS;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
        }  
      }
      break;
//    case  NNMI:                 //���ý�����Ϣָʾ 
//      {
//        if(strstr(BC95.R_Buffer,"OK") != NULL)
//        {         
//          BC95.Start_Process = NMGS;
//          BC95.Incident_Pend = TRUE;//��ǹ���
//          Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
//        }  
//      }
//      break;
    case NMGS:        //������Ϣ
      {      
        if( strstr(BC95.R_Buffer,"+NSMI:SENT") != NULL)//�ѷ���
        { 
          BC95.Start_Process = NQMGR;
          Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
          Create_Timer(ONCE,5,
                     BC95_Delay_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�,������Ϣ��ȴ�3s��ѯ��Ϣ���ջ���
        }
      }
      break;
    case NQMGR:        //��ѯ��Ϣ���ջ���
      {
        if(strstr(BC95.R_Buffer,"ERROR") != NULL)       //���ش���
        {
          
        }
        else if(strstr(BC95.R_Buffer,"BUFFERED=0") != NULL)  //������Ϣ����Ϊ��
        { 
          if(BC95.Report_Bit == 0)      //û����Ϣ�ϱ�,��ػ�
          {          
            BC95.Start_Process = BC95_POWER_DOWN;
            Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�   
            Create_Timer(ONCE,2,
                     BC95_Delay_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�,�ȴ�2s�ػ�
          }
          else  //������Ϣ
          {
            BC95.Start_Process = NMGS;
            BC95.Incident_Pend = TRUE;//��ǹ���
            Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
          }
        }
        else                                            //�����ȡ��Ϣ
        {
          BC95.Start_Process = NMGR;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
        }
      }
      break;
    case NMGR:        //������Ϣ
      {
        if(strstr(BC95.R_Buffer,"ERROR") != NULL)       //���ش���
        {
          
        }
        else if(strstr(BC95.R_Buffer,"AAAA0000") != NULL)  //�ϱ�ȫ����������Ӧ
        {      
          BC95.Alarm.Mag_Alarm = 0;
          BC95.Report_Bit = 2;
          BC95.Start_Process = NMGS;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
        }
        else if(strstr(BC95.R_Buffer,"AAAA0001") != NULL)     //�ϱ���ʷ��������Ӧ
        {
          BC95.Report_Bit = 0;
          BC95.Start_Process = NQMGR;
          Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
          Create_Timer(ONCE,2,
                     BC95_Delay_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�,�ȴ�1s��ѯ��Ϣ���ջ���
        }
        else   //ƽ̨��Ϣ
        {
          Recv_Data_Process();   
          BC95.Start_Process = NQMGR;
          Delete_Timer(BC95_Start_Timeout_CallBalk);//ɾ����ʱ�ص�
          Create_Timer(ONCE,2,
                     BC95_Delay_CallBalk,0,PROCESS);//���鶨ʱ����ʱ�ص�,�ȴ�1s��ѯ��Ϣ���ջ���
        }
      }
      break;
    case BC95_CONNECT_ERROR:
      //����쳣����
      break;
    default:
      break;
    }
  }
  
}
   
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void BC95_Data_Send(unsigned char *Data,unsigned short Len)
{    
  memset(BC95.R_Buffer,'\0',RECV_BUFF_SIZE);//����ջ�����
  Uart2_Send((unsigned char*)Data,Len);
}

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void BC95_Start_Timeout_CallBalk(void)//������ʱ�ط�
{
  if(BC95.Err_Conner.Connect != 0)//�жϴ����Ƿ�ʱ
  {
    BC95.Incident_Pend = TRUE;
    BC95.Err_Conner.Connect--;
  }
  else
  {
    BC95.Incident_Pend = TRUE;
    BC95.Start_Process = BC95_CONNECT_ERROR;//��������
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void BC95_Delay_CallBalk(void)
{
  BC95.Incident_Pend = TRUE;//��ǹ���
}
/*********************************************************************************
 Function:      //
 Description:   //������Ϣ����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void Recv_Data_Process(void)
{
  u16 i = 0;
  u8 month = 0;
  u8 messageId = 0;
  u16 messageLen = 0;
  u8 mid[4] = {0};           //�������к�
  u8 valueH = 0,valueL = 0;
  RTC_TimeTypeDef  RTC_Time;
  RTC_DateTypeDef  RTC_Date;
  union flow_union Flow;
  
  for(i = 0;i < BC95.Recv_Length;i++)
  {
    BC95.R_Buffer[i] = ASCLL_to_Int(BC95.R_Buffer[i]);
  }
  i = 0;
  while(BC95.R_Buffer[2+i] != ',')
  {
    messageLen = messageLen*10+BC95.R_Buffer[2+i];
    i++;
  }
  
  
  messageId = BC95.R_Buffer[2+i+1]*16+BC95.R_Buffer[2+i+2];
  mid[0] = BC95.R_Buffer[2+i+3];
  mid[1] = BC95.R_Buffer[2+i+4];
  mid[2] = BC95.R_Buffer[2+i+5];
  mid[3] = BC95.R_Buffer[2+i+6];
  
  switch(messageId)
  {
    case 0x02:            //���ò���
      {//45,020004303030303138303530303030303132303138303532323136333033300102D00140000000000000000030
        //���
        valueH = BC95.R_Buffer[2+i+7]*0x10+BC95.R_Buffer[2+i+8];
        valueL = BC95.R_Buffer[2+i+9]*0x10+BC95.R_Buffer[2+i+10];
        Meter_Number[0] = ASCLL_to_Int(valueH)*0x10+ASCLL_to_Int(valueL);
        valueH = BC95.R_Buffer[2+i+11]*0x10+BC95.R_Buffer[2+i+12];
        valueL = BC95.R_Buffer[2+i+13]*0x10+BC95.R_Buffer[2+i+14];
        Meter_Number[1] = ASCLL_to_Int(valueH)*0x10+ASCLL_to_Int(valueL);
        valueH = BC95.R_Buffer[2+i+15]*0x10+BC95.R_Buffer[2+i+16];
        valueL = BC95.R_Buffer[2+i+17]*0x10+BC95.R_Buffer[2+i+18];
        Meter_Number[2] = ASCLL_to_Int(valueH)*0x10+ASCLL_to_Int(valueL);
        valueH = BC95.R_Buffer[2+i+19]*0x10+BC95.R_Buffer[2+i+20];
        valueL = BC95.R_Buffer[2+i+21]*0x10+BC95.R_Buffer[2+i+22];
        Meter_Number[3] = ASCLL_to_Int(valueH)*0x10+ASCLL_to_Int(valueL);
        valueH = BC95.R_Buffer[2+i+23]*0x10+BC95.R_Buffer[2+i+24];
        valueL = BC95.R_Buffer[2+i+25]*0x10+BC95.R_Buffer[2+i+26];
        Meter_Number[4] = ASCLL_to_Int(valueH)*0x10+ASCLL_to_Int(valueL);
        valueH = BC95.R_Buffer[2+i+27]*0x10+BC95.R_Buffer[2+i+28];
        valueL = BC95.R_Buffer[2+i+29]*0x10+BC95.R_Buffer[2+i+30];
        Meter_Number[5] = ASCLL_to_Int(valueH)*0x10+ASCLL_to_Int(valueL);
        valueH = BC95.R_Buffer[2+i+31]*0x10+BC95.R_Buffer[2+i+32];
        valueL = BC95.R_Buffer[2+i+33]*0x10+BC95.R_Buffer[2+i+34];
        Meter_Number[6] = ASCLL_to_Int(valueH)*0x10+ASCLL_to_Int(valueL);
        Save_Meter_Number();
        //ʱ��       
        RTC_Date.RTC_Year = BC95.R_Buffer[2+i+40]*10+BC95.R_Buffer[2+i+42];
        RTC_Date.RTC_Month = BC95.R_Buffer[2+i+44]*10+BC95.R_Buffer[2+i+46];
        RTC_Date.RTC_Date = BC95.R_Buffer[2+i+48]*10+BC95.R_Buffer[2+i+50];
        RTC_Date.RTC_WeekDay = RTC_Weekday_Monday;
        
        RTC_Time.RTC_Hours   = BC95.R_Buffer[2+i+52]*10+BC95.R_Buffer[2+i+54];
        RTC_Time.RTC_Minutes = BC95.R_Buffer[2+i+56]*10+BC95.R_Buffer[2+i+58];
        RTC_Time.RTC_Seconds = BC95.R_Buffer[2+i+60]*10+BC95.R_Buffer[2+i+62];
        
        RTC_SetDate(RTC_Format_BIN, &RTC_Date);
        RTC_SetTime(RTC_Format_BIN, &RTC_Time);
        //��������
        Settle_Date = BC95.R_Buffer[2+i+63]*0x10+BC95.R_Buffer[2+i+64];   //���ý�������
        Save_Settle_Date();       
        //�ϱ�����
         Report_Cycle = BC95.R_Buffer[2+i+65]*0x1000+BC95.R_Buffer[2+i+66]*0x100+BC95.R_Buffer[2+i+67]*0x10+BC95.R_Buffer[2+i+68]; //�����ϱ�����
        Save_Report_Cycle();
        //�澯��ѹ
        BAT_Alarm_Vol = BC95.R_Buffer[2+i+69]*0x1000+BC95.R_Buffer[2+i+70]*0x100+BC95.R_Buffer[2+i+71]*0x10+BC95.R_Buffer[2+i+72];
        Save_BAT_Alarm_Value();
        
        //0x03��Ӧ
        ACK(0x03,0x00,mid);
      }
      break;
    case 0x04:           //У׼ˮ��
      {//8,040003010000007B 
        month = BC95.R_Buffer[2+i+7]*0x10+BC95.R_Buffer[2+i+8];
        if(month == 0)  //У׼��ǰ�ۻ�ˮ��
        {
          Cal.Water_Data.flow8[0] = BC95.R_Buffer[2+i+9]*0x10+BC95.R_Buffer[2+i+10];
          Cal.Water_Data.flow8[1] = BC95.R_Buffer[2+i+11]*0x10+BC95.R_Buffer[2+i+12];
          Cal.Water_Data.flow8[2] = BC95.R_Buffer[2+i+13]*0x10+BC95.R_Buffer[2+i+14];
          Cal.Water_Data.flow8[3] = BC95.R_Buffer[2+i+15]*0x10+BC95.R_Buffer[2+i+16];
          Save_Add_Flow(ADD_FLOW_ADD,&Cal.Water_Data);
        }
        else if(month <= 13)  //У׼�������ۻ�ˮ��
        {
          Flow.flow8[0] = BC95.R_Buffer[2+i+9]*0x10+BC95.R_Buffer[2+i+10];
          Flow.flow8[1] = BC95.R_Buffer[2+i+11]*0x10+BC95.R_Buffer[2+i+12];
          Flow.flow8[2] = BC95.R_Buffer[2+i+13]*0x10+BC95.R_Buffer[2+i+14];
          Flow.flow8[3] = BC95.R_Buffer[2+i+15]*0x10+BC95.R_Buffer[2+i+16];
          Save_Add_Flow(SDCF1_ADDR+4*(month-1),&Flow);
        }
        //0x05��Ӧ
        ACK(0x05,0x00,mid);
      } 
      break;
    default:
      break;
  }
}
/*********************************************************************************
 Function:      //
 Description:   //������Ϣ����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void Send_Data_Process(void)
{ 
   switch(BC95.Report_Bit)
  {
    case 1:            //����ȫ������
      {
        Report_All_Parameters();
      }
      break;
    case 2:            //������ʷ�ۻ�����
      {
        Report_HC_Flow();
      }
      break;
    default:
      break;
  }
}
/*********************************************************************************
 Function:      //
 Description:   //������Ӧ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void ACK(u8 messageId,u8 errcode,u8 mid[4])
{
  uint8_t data[64] = "AT+NMGS=4,00000000\r\n";
  
  data[10] = Int_to_ASCLL(messageId/0x10);
  data[11] = Int_to_ASCLL(messageId%0x10);
  data[12] = Int_to_ASCLL(errcode/0x10);
  data[13] = Int_to_ASCLL(errcode%0x10);
  data[14] = Int_to_ASCLL(mid[0]);
  data[15] = Int_to_ASCLL(mid[1]);
  data[16] = Int_to_ASCLL(mid[2]);
  data[17] = Int_to_ASCLL(mid[3]);
  
  BC95_Data_Send(data,20);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
//uint8_t data[320] = "AT+NMGS=147,00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\
//0000000000000030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030\r\n";
void Report_All_Parameters(void)
{
  uint8_t data[320] = "AT+NMGS=76,00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\r\n";
  union flow_union flow;     //�������ۻ�����
  uint8_t year,month,date,hour,minute,second;
  uint8_t valueH = 0,valueL = 0;
  
  //��ȡ��һ���ۼ�����
  Read_ACUM_Flow(SDCF1_ADDR,&flow);
  //�ɼ��¶�
  GPIO_Init(GPIOA, GPIO_Pin_4,  GPIO_Mode_Out_PP_High_Fast);         // ��������
  GPIO_Init(GPIOA, GPIO_Pin_5,  GPIO_Mode_In_FL_No_IT);      // ��������ADC����
  Read_Temp();
  GPIO_Init(GPIOA, GPIO_Pin_4,  GPIO_Mode_Out_PP_Low_Slow); 
  //��ȡʱ��   
  RTC_GetDate(RTC_Format_BCD, &RTC_DateStr);
  RTC_GetTime(RTC_Format_BCD, &RTC_TimeStr);
  year = BCD_to_Int((unsigned char)RTC_DateStr.RTC_Year);
  month = BCD_to_Int((unsigned char)RTC_DateStr.RTC_Month);
  date = BCD_to_Int((unsigned char)RTC_DateStr.RTC_Date);
  hour = BCD_to_Int((unsigned char)RTC_TimeStr.RTC_Hours);
  minute = BCD_to_Int((unsigned char)RTC_TimeStr.RTC_Minutes);
  second = BCD_to_Int((unsigned char)RTC_TimeStr.RTC_Seconds);
  
  //��ǰ�ۻ�����
  data[13] = Int_to_ASCLL(Cal.Water_Data.flow32/0x10000000);
  data[14] = Int_to_ASCLL(Cal.Water_Data.flow32%0x10000000/0x1000000);
  data[15] = Int_to_ASCLL(Cal.Water_Data.flow32%0x1000000/0x100000);
  data[16] = Int_to_ASCLL(Cal.Water_Data.flow32%0x100000/0x10000);
  data[17] = Int_to_ASCLL(Cal.Water_Data.flow32%0x10000/0x1000);
  data[18] = Int_to_ASCLL(Cal.Water_Data.flow32%0x1000/0x100);
  data[19] = Int_to_ASCLL(Cal.Water_Data.flow32%0x100/0x10);
  data[20] = Int_to_ASCLL(Cal.Water_Data.flow32%0x10);

  //�������ۻ�����
  data[21] = Int_to_ASCLL(flow.flow32/0x10000000);
  data[22] = Int_to_ASCLL(flow.flow32%0x10000000/0x1000000);
  data[23] = Int_to_ASCLL(flow.flow32%0x1000000/0x100000);
  data[24] = Int_to_ASCLL(flow.flow32%0x100000/0x10000);
  data[25] = Int_to_ASCLL(flow.flow32%0x10000/0x1000);
  data[26] = Int_to_ASCLL(flow.flow32%0x1000/0x100);
  data[27] = Int_to_ASCLL(flow.flow32%0x100/0x10);
  data[28] = Int_to_ASCLL(flow.flow32%0x10);
  
  //��ص�ѹ
  data[29] = Int_to_ASCLL(BAT_Vol/0x1000);
  data[30] = Int_to_ASCLL(BAT_Vol%0x1000/0x100);
  data[31] = Int_to_ASCLL(BAT_Vol%0x100/0x10);
  data[32] = Int_to_ASCLL(BAT_Vol%0x10);
  
  //�ź�ǿ��
  data[33] = Int_to_ASCLL(BC95.Rssi/0x10);
  data[34] = Int_to_ASCLL(BC95.Rssi%0x10);
  //�¶� 
  data[35] = Int_to_ASCLL((u8)Temp/0x10);
  data[36] = Int_to_ASCLL((u8)Temp%0x10);
  
  //��
  data[39] = Int_to_ASCLL(year/0x10);
  data[40] = Int_to_ASCLL(year%0x10); 
  //��
  data[41] = Int_to_ASCLL(month/0x10);
  data[42] = Int_to_ASCLL(month%0x10);
  //��
  data[43] = Int_to_ASCLL(date/0x10);
  data[44] = Int_to_ASCLL(date%0x10);
  //ʱ
  data[45] = Int_to_ASCLL(hour/0x10);
  data[46] = Int_to_ASCLL(hour%0x10);
  //��
  data[47] = Int_to_ASCLL(minute/0x10);
  data[48] = Int_to_ASCLL(minute%0x10);
  //��
  data[49] = Int_to_ASCLL(second/0x10);
  data[50] = Int_to_ASCLL(second%0x10);
  //���
  valueH = Int_to_ASCLL(Meter_Number[0]/0x10);
  valueL = Int_to_ASCLL(Meter_Number[0]%0x10);
  data[51] = Int_to_ASCLL(valueH/0x10);
  data[52] = Int_to_ASCLL(valueH%0x10);
  data[53] = Int_to_ASCLL(valueL/0x10);
  data[54] = Int_to_ASCLL(valueL%0x10);
  valueH = Int_to_ASCLL(Meter_Number[1]/0x10);
  valueL = Int_to_ASCLL(Meter_Number[1]%0x10);
  data[55] = Int_to_ASCLL(valueH/0x10);
  data[56] = Int_to_ASCLL(valueH%0x10);
  data[57] = Int_to_ASCLL(valueL/0x10);
  data[58] = Int_to_ASCLL(valueL%0x10);
  valueH = Int_to_ASCLL(Meter_Number[2]/0x10);
  valueL = Int_to_ASCLL(Meter_Number[2]%0x10);
  data[59] = Int_to_ASCLL(valueH/0x10);
  data[60] = Int_to_ASCLL(valueH%0x10);
  data[61] = Int_to_ASCLL(valueL/0x10);
  data[62] = Int_to_ASCLL(valueL%0x10);
  valueH = Int_to_ASCLL(Meter_Number[3]/0x10);
  valueL = Int_to_ASCLL(Meter_Number[3]%0x10);
  data[63] = Int_to_ASCLL(valueH/0x10);
  data[64] = Int_to_ASCLL(valueH%0x10);
  data[65] = Int_to_ASCLL(valueL/0x10);
  data[66] = Int_to_ASCLL(valueL%0x10);
  valueH = Int_to_ASCLL(Meter_Number[4]/0x10);
  valueL = Int_to_ASCLL(Meter_Number[4]%0x10);
  data[67] = Int_to_ASCLL(valueH/0x10);
  data[68] = Int_to_ASCLL(valueH%0x10);
  data[69] = Int_to_ASCLL(valueL/0x10);
  data[70] = Int_to_ASCLL(valueL%0x10);
  valueH = Int_to_ASCLL(Meter_Number[5]/0x10);
  valueL = Int_to_ASCLL(Meter_Number[5]%0x10);
  data[71] = Int_to_ASCLL(valueH/0x10);
  data[72] = Int_to_ASCLL(valueH%0x10);
  data[73] = Int_to_ASCLL(valueL/0x10);
  data[74] = Int_to_ASCLL(valueL%0x10);
  valueH = Int_to_ASCLL(Meter_Number[6]/0x10);
  valueL = Int_to_ASCLL(Meter_Number[6]%0x10);
  data[75] = Int_to_ASCLL(valueH/0x10);
  data[76] = Int_to_ASCLL(valueH%0x10);
  data[77] = Int_to_ASCLL(valueL/0x10);
  data[78] = Int_to_ASCLL(valueL%0x10);
   //��������
  data[79] = Int_to_ASCLL(Settle_Date/0x10);
  data[80] = Int_to_ASCLL(Settle_Date%0x10);
  //�ϱ�����
  data[81] = Int_to_ASCLL(Report_Cycle/0x1000);
  data[82] = Int_to_ASCLL(Report_Cycle%0x1000/0x100);
  data[83] = Int_to_ASCLL(Report_Cycle%0x100/0x10);
  data[84] = Int_to_ASCLL(Report_Cycle%0x10);
  //�澯��ѹ
  data[85] = Int_to_ASCLL(BAT_Alarm_Vol/0x1000);
  data[86] = Int_to_ASCLL(BAT_Alarm_Vol%0x1000/0x100);
  data[87] = Int_to_ASCLL(BAT_Alarm_Vol%0x100/0x10);
  data[88] = Int_to_ASCLL(BAT_Alarm_Vol%0x10);
  //����״̬
  data[89] = 0x30;
  data[90] = 0x30;
  //����״̬
  data[91] = Int_to_ASCLL(BC95.Alarm.Mag_Alarm/0x10);
  data[92] = Int_to_ASCLL(BC95.Alarm.Mag_Alarm%0x10);
  //IMEI
  data[93] = Int_to_ASCLL(BC95.IMEI[0]/0x10);
  data[94] = Int_to_ASCLL(BC95.IMEI[0]%0x10);
  data[95] = Int_to_ASCLL(BC95.IMEI[1]/0x10);
  data[96] = Int_to_ASCLL(BC95.IMEI[1]%0x10);
  data[97] = Int_to_ASCLL(BC95.IMEI[2]/0x10);
  data[98] = Int_to_ASCLL(BC95.IMEI[2]%0x10);
  data[99] = Int_to_ASCLL(BC95.IMEI[3]/0x10);
  data[100] = Int_to_ASCLL(BC95.IMEI[3]%0x10);
  data[101] = Int_to_ASCLL(BC95.IMEI[4]/0x10);
  data[102] = Int_to_ASCLL(BC95.IMEI[4]%0x10);
  data[103] = Int_to_ASCLL(BC95.IMEI[5]/0x10);
  data[104] = Int_to_ASCLL(BC95.IMEI[5]%0x10);
  data[105] = Int_to_ASCLL(BC95.IMEI[6]/0x10);
  data[106] = Int_to_ASCLL(BC95.IMEI[6]%0x10);
  data[107] = Int_to_ASCLL(BC95.IMEI[7]/0x10);
  data[108] = Int_to_ASCLL(BC95.IMEI[7]%0x10);
  data[109] = Int_to_ASCLL(BC95.IMEI[8]/0x10);
  data[110] = Int_to_ASCLL(BC95.IMEI[8]%0x10);
  data[111] = Int_to_ASCLL(BC95.IMEI[9]/0x10);
  data[112] = Int_to_ASCLL(BC95.IMEI[9]%0x10);
  data[113] = Int_to_ASCLL(BC95.IMEI[10]/0x10);
  data[114] = Int_to_ASCLL(BC95.IMEI[10]%0x10);
  data[115] = Int_to_ASCLL(BC95.IMEI[11]/0x10);
  data[116] = Int_to_ASCLL(BC95.IMEI[11]%0x10);
  data[117] = Int_to_ASCLL(BC95.IMEI[12]/0x10);
  data[118] = Int_to_ASCLL(BC95.IMEI[12]%0x10);
  data[119] = Int_to_ASCLL(BC95.IMEI[13]/0x10);
  data[120] = Int_to_ASCLL(BC95.IMEI[13]%0x10);
  data[121] = Int_to_ASCLL(BC95.IMEI[14]/0x10);
  data[122] = Int_to_ASCLL(BC95.IMEI[14]%0x10);
  //ICCID
  data[123] = Int_to_ASCLL(BC95.ICCID[0]/0x10);
  data[124] = Int_to_ASCLL(BC95.ICCID[0]%0x10);
  data[125] = Int_to_ASCLL(BC95.ICCID[1]/0x10);
  data[126] = Int_to_ASCLL(BC95.ICCID[1]%0x10);
  data[127] = Int_to_ASCLL(BC95.ICCID[2]/0x10);
  data[128] = Int_to_ASCLL(BC95.ICCID[2]%0x10);
  data[129] = Int_to_ASCLL(BC95.ICCID[3]/0x10);
  data[130] = Int_to_ASCLL(BC95.ICCID[3]%0x10);
  data[131] = Int_to_ASCLL(BC95.ICCID[4]/0x10);
  data[132] = Int_to_ASCLL(BC95.ICCID[4]%0x10);
  data[133] = Int_to_ASCLL(BC95.ICCID[5]/0x10);
  data[134] = Int_to_ASCLL(BC95.ICCID[5]%0x10);
  data[135] = Int_to_ASCLL(BC95.ICCID[6]/0x10);
  data[136] = Int_to_ASCLL(BC95.ICCID[6]%0x10);
  data[137] = Int_to_ASCLL(BC95.ICCID[7]/0x10);
  data[138] = Int_to_ASCLL(BC95.ICCID[7]%0x10);
  data[139] = Int_to_ASCLL(BC95.ICCID[8]/0x10);
  data[140] = Int_to_ASCLL(BC95.ICCID[8]%0x10);
  data[141] = Int_to_ASCLL(BC95.ICCID[9]/0x10);
  data[142] = Int_to_ASCLL(BC95.ICCID[9]%0x10);
  data[143] = Int_to_ASCLL(BC95.ICCID[10]/0x10);
  data[144] = Int_to_ASCLL(BC95.ICCID[10]%0x10);
  data[145] = Int_to_ASCLL(BC95.ICCID[11]/0x10);
  data[146] = Int_to_ASCLL(BC95.ICCID[11]%0x10);
  data[147] = Int_to_ASCLL(BC95.ICCID[12]/0x10);
  data[148] = Int_to_ASCLL(BC95.ICCID[12]%0x10);
  data[149] = Int_to_ASCLL(BC95.ICCID[13]/0x10);
  data[150] = Int_to_ASCLL(BC95.ICCID[13]%0x10);
  data[151] = Int_to_ASCLL(BC95.ICCID[14]/0x10);
  data[152] = Int_to_ASCLL(BC95.ICCID[14]%0x10);
  data[153] = Int_to_ASCLL(BC95.ICCID[15]/0x10);
  data[154] = Int_to_ASCLL(BC95.ICCID[15]%0x10);
  data[155] = Int_to_ASCLL(BC95.ICCID[16]/0x10);
  data[156] = Int_to_ASCLL(BC95.ICCID[16]%0x10);
  data[157] = Int_to_ASCLL(BC95.ICCID[17]/0x10);
  data[158] = Int_to_ASCLL(BC95.ICCID[17]%0x10);
  data[159] = Int_to_ASCLL(BC95.ICCID[18]/0x10);
  data[160] = Int_to_ASCLL(BC95.ICCID[18]%0x10);
  data[161] = Int_to_ASCLL(BC95.ICCID[19]/0x10);
  data[162] = Int_to_ASCLL(BC95.ICCID[19]%0x10);

  
  BC95_Data_Send(data,165);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void Report_HC_Flow(void)
{
  uint8_t data[128] = "AT+NMGS=53,0100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\r\n";
  union flow_union flow;
  
  //��1���ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF1_ADDR,&flow);
  data[13] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[14] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[15] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[16] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[17] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[18] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[19] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[20] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��2�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF2_ADDR,&flow);
  data[21] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[22] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[23] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[24] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[25] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[26] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[27] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[28] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��3�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF3_ADDR,&flow);
  data[29] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[30] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[31] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[32] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[33] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[34] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[35] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[36] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��4�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF4_ADDR,&flow);
  data[37] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[38] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[39] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[40] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[41] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[42] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[43] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[44] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��5�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF5_ADDR,&flow);
  data[45] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[46] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[47] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[48] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[49] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[50] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[51] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[52] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��6�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF6_ADDR,&flow);
  data[53] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[54] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[55] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[56] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[57] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[58] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[59] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[60] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��7�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF7_ADDR,&flow);
  data[61] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[62] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[63] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[64] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[65] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[66] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[67] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[68] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��8�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF8_ADDR,&flow);
  data[69] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[70] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[71] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[72] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[73] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[74] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[75] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[76] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��9�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF9_ADDR,&flow);
  data[77] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[78] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[79] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[80] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[81] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[82] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[83] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[84] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��10�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF10_ADDR,&flow);
  data[85] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[86] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[87] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[88] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[89] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[90] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[91] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[92] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��11�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF11_ADDR,&flow);
  data[93] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[94] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[95] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[96] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[97] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[98] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[99] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[100] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��12�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF12_ADDR,&flow); 
  data[101] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[102] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[103] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[104] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[105] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[106] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[107] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[108] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��13�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF13_ADDR,&flow);
  data[109] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[110] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[111] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[112] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[113] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[114] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[115] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[116] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  BC95_Data_Send(data,119);
}
/******************************************END********************************************************/