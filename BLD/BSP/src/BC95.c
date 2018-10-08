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
  
  BC95.Report_Bit = 1;
  
  Create_Timer(ONCE,1,
                     BC95_Reset,0,PROCESS); 
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
  
  Create_Timer(ONCE,5,
                     BC95_Start,0,PROCESS); 
}
/*********************************************************************************
 Function:      //
 Description:   ////TPB21����
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void BC95_Start(void)        
{
  Uart2.Sent_Length = 0;                                //��մ���2���ͳ���
  Uart2.Receive_Length = 0;                             //��մ���2���ճ���
  Uart2.Send_Busy = FALSE;                              //��մ���2����æ��־  
  Uart2.Receive_Busy = FALSE;                           //��մ���2����æ
  Uart2.Receive_Pend = FALSE;                           //��մ���2����
  
  BC95.Start_Process = NRB; //ָ����һ������
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
  char *str1 = NULL;
  char *str2 = NULL;

  //�����Ҫ�ϱ���Ϣ������BC95
  if(BC95.Report_Bit != 0)
  {
    if(BC95.Start_Process == BC95_RECONNECT)
    {
      if(BC95.Reconnect_Times >= 3)  //������������˯��
      {
        BC95.Reconnect_Times = 0;  
        BC95.FailTimes++;  
        MCU_DeInit(); 
      }
      else     //��������
      {
        MeterParameter.DeviceStatus = RUN;
        BC95_Start();
      }
    }
  }
  
  if((BC95.Incident_Pend != FALSE)||(Upgrade.Incident_Pend != FALSE)) //����Ƿ����¼�����
  {
    BC95.Incident_Pend = FALSE; //����¼�����
    Upgrade.Incident_Pend = FALSE;
    switch(BC95.Start_Process)
    {
    case NRB:                  //����
      {
        BC95_Data_Send("AT+NRB\r\n",8);
        Create_Timer(ONCE,10,
                     BC95_Recv_Timeout_CallBack,0,PROCESS); 
      }
      break;
    case AT:                  //ͬ��������
      {
        BC95_Data_Send("AT\r\n",4);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Recv_Timeout_CallBack,0,PROCESS); 
      }
      break;
    case GETNBAND:               //��ѯƵ��
      {
        
        BC95_Data_Send("AT+NBAND?\r\n",11);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Recv_Timeout_CallBack,0,PROCESS); 
      }
      break;
    case GETCFUN:                //��ѯ�绰����
      {
        BC95_Data_Send("AT+CFUN?\r\n",10);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Recv_Timeout_CallBack,0,PROCESS); 
      } 
      break;
    case CGSN:                 //��ѯIMEI      
      {
        BC95_Data_Send("AT+CGSN=1\r\n",11);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Recv_Timeout_CallBack,0,PROCESS); 
      }
      break;  
     case CCID:                 //��ѯCCID
      {
        BC95_Data_Send("AT+NCCID\r\n",10); 
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Recv_Timeout_CallBack,0,PROCESS); 
      }
      break;  
    case CSQ:                //��ѯ�ź�ǿ��
      {
        BC95_Data_Send("AT+CSQ\r\n",8);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Recv_Timeout_CallBack,0,PROCESS); 
      } 
      break;
    case GETCGATT:                //��ѯ���缤��״̬    
      {
        BC95_Data_Send("AT+CGATT?\r\n",11);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Recv_Timeout_CallBack,0,PROCESS); 
      }
      break;    
    case CEREG:                //��ѯ����ע��״̬     
      {
        BC95_Data_Send("AT+CEREG?\r\n",11);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Recv_Timeout_CallBack,0,PROCESS); 
      }
      break;
    case CCLK:                //��ѯʵʱʱ�� 
      {
        BC95_Data_Send("AT+CCLK?\r\n",10);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Recv_Timeout_CallBack,0,PROCESS); 
      }
      break;
    case GETNCDP:                 //��ѯCDP������
      {
        BC95_Data_Send("AT+NCDP?\r\n",10);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Recv_Timeout_CallBack,0,PROCESS); 
      }
      break;
    case SETNCDP:                 //����CDP������ 
      {
        BC95_Data_Send("AT+NCDP=180.101.147.115,5683\r\n",30);   
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Recv_Timeout_CallBack,0,PROCESS); 
      }
      break; 
    case NSMI:                 //���÷�����Ϣָʾ
      {
        BC95_Data_Send("AT+NSMI=1\r\n",11);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Recv_Timeout_CallBack,0,PROCESS); 
      }
      break;
    case NNMI:                 //���ý�����Ϣָʾ
      {
        BC95_Data_Send("AT+NNMI=1\r\n",11);
        Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                     BC95_Recv_Timeout_CallBack,0,PROCESS); 
      }
      break;
    case NMGS:                 //������Ϣ     
      { 
        //��ִ���ϱ����Ա�֤�����ɹ�
        if(BC95.Report_Bit != 0)
        {
          Report_History_Data();          //��������
          Create_Timer(ONCE,BC95_R_TIMEROUT_TIME,
                       BC95_Recv_Timeout_CallBack,0,PROCESS); 
        }
        else
        {
          if(Upgrade.Process != IDLE)
          {
            Upgrade_Send_Process();       //��������
          }
          else
          {
            Create_Timer(ONCE,10,
                         MCU_DeInit,0,PROCESS); //10s��ػ�
          }
        }
      }
      break;
    case BC95_CONNECT_ERROR:      //����ʧ��
      BC95.Start_Process = BC95_RECONNECT;
      BC95.Reconnect_Times++;
      break;
    case BC95_POWER_DOWN:       //���ͽ��������ֱ��˯��  
      MCU_DeInit(); 
      break;
    default:
      break;
    }
  }
  
  if(Uart2.Receive_Pend != FALSE)//�ж�������
  { 
    
    switch(BC95.Start_Process)
    {
    case NRB:                  //����
      {
        if(strstr(Uart2.R_Buffer,"REBOOT_CAUSE_APPLICATION_AT") != NULL)
        {
          BC95.Start_Process = AT;
          Delete_Timer(BC95_Recv_Timeout_CallBack);//ɾ����ʱ�ص�
          Create_Timer(ONCE,20,
                       BC95_Delay_CallBack,0,PROCESS); 
        }
      }
      break;
    case AT:            //ͬ��������
      {
        if(strstr(Uart2.R_Buffer,"OK") != NULL)
        {         
          BC95.Start_Process = GETNBAND;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Recv_Timeout_CallBack);//ɾ����ʱ�ص�
        }
      }
      break;
    case GETNBAND:               //��ѯƵ��
      {       
        if(strstr(Uart2.R_Buffer,"+NBAND:5") != NULL)    //֧��Ƶ��5
        {
          BC95.Start_Process = GETCFUN;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Recv_Timeout_CallBack);//ɾ����ʱ�ص�
        }
      }
      break;
    case GETCFUN:                //��ѯ�绰����
      {
        if(strstr(Uart2.R_Buffer,"+CFUN:1") != NULL)     //ȫ����
        {         
          BC95.Start_Process = CGSN;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Recv_Timeout_CallBack);//ɾ����ʱ�ص�
        }
      } 
      break;
    case CGSN:         // ��ѯIMEI
      {
        str = strstr(Uart2.R_Buffer,"+CGSN");
        if( str != NULL)//��ȡ��IMEI
        {
          BC95.Start_Process = CCID;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Recv_Timeout_CallBack);//ɾ����ʱ�ص�
        }
      }
      break;
     case CCID:          //��ѯCCID
      {
        str = strstr(Uart2.R_Buffer,"+NCCID");
        if( str != NULL)
        {
          BC95.Start_Process = CSQ;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Recv_Timeout_CallBack);//ɾ����ʱ�ص�
        }
      }
      break;
    case CSQ:           //��ѯ�ź�ǿ��
      {
        str = strstr(Uart2.R_Buffer,"+CSQ");
        if( str != NULL)
        {
          BC95.Rssi =0;//�����ź�ǿ��
          if( (str[5] >= '0') && (str[5] <= '9') )
          {
            BC95.Rssi += str[5]-0x30;
          }
          if( (str[6] >= '0') && (str[6] <= '9') )
          {
            BC95.Rssi *=10;
            BC95.Rssi += str[6]-0x30;
          }
    
          if(BC95.Rssi < 99)
          {
            BC95.Start_Process = GETCGATT;
            BC95.Incident_Pend = TRUE;//��ǹ���
            Delete_Timer(BC95_Recv_Timeout_CallBack);//ɾ����ʱ�ص�
          }
        }
      }
      break;
    case GETCGATT:       //��ѯ���缤��״̬
      {
        if( strstr(Uart2.R_Buffer,"+CGATT:1") != NULL)//���缤��
        {        
          BC95.Start_Process = CEREG;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Recv_Timeout_CallBack);//ɾ����ʱ�ص�
        }       
      }
      break;
    case CEREG:       //��ѯ����ע��״̬
      {
        if( strstr(Uart2.R_Buffer,"+CEREG:0,1") != NULL)//����ע��
        {        
          BC95.Start_Process = CCLK;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Recv_Timeout_CallBack);//ɾ����ʱ�ص�
        }
      }
      break;   
    case CCLK:       //��ѯʵʱʱ��
      {
        str = strstr(Uart2.R_Buffer,"+CCLK:");
        if( str != NULL)
        {  
          GMT_to_BT((unsigned char*)str);
        }
        BC95.Start_Process = GETNCDP;
        BC95.Incident_Pend = TRUE;//��ǹ���
        Delete_Timer(BC95_Recv_Timeout_CallBack);//ɾ����ʱ�ص�
      }
      break;   
    case  GETNCDP:                 //��ѯCDP������
      {
        if( strstr(Uart2.R_Buffer,"+NCDP:180.101.147.115,5683") != NULL)//��ȡ��NCDP
        {        
          BC95.Start_Process = NSMI;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Recv_Timeout_CallBack);//ɾ����ʱ�ص�
        }
        else
        {        
          BC95.Start_Process = SETNCDP;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Recv_Timeout_CallBack);//ɾ����ʱ�ص�
        }
      }
      break;
    case  SETNCDP:                 //����CDP������ 
      {
        if(strstr(Uart2.R_Buffer,"OK") != NULL)
        {         
          BC95.Start_Process = GETNCDP;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Recv_Timeout_CallBack);//ɾ����ʱ�ص�
        }  
      }
      break;
    case NSMI:                 //���÷�����Ϣָʾ
      {
        if(strstr(Uart2.R_Buffer,"OK") != NULL)
        {         
          BC95.Start_Process = NNMI;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Recv_Timeout_CallBack);//ɾ����ʱ�ص�
        }  
      }
      break;
    case  NNMI:                 //���ý�����Ϣָʾ 
      {
        if(strstr(Uart2.R_Buffer,"OK") != NULL)
        {         
          BC95.Start_Process = NMGS;
          BC95.Incident_Pend = TRUE;//��ǹ���
          Delete_Timer(BC95_Recv_Timeout_CallBack);//ɾ����ʱ�ص�
        }  
      }
      break;
    case NMGS:        //������Ϣ
      {  
        str1 = strstr(Uart2.R_Buffer,"+NNMI:"); 
        //������Ϣճ��
        while(str1 != NULL)
        { 
          str2 = strstr(str1+6,"+NNMI:"); 
          if(strnstr(str1,"+NNMI:4,AAAA0002",16) != NULL)     //�ϱ���ʷ���ݵ���Ӧ
          {
            BC95.Report_Bit = 0;
            BC95.Incident_Pend = TRUE;//��ǹ���
            Delete_Timer(BC95_Recv_Timeout_CallBack);//ɾ�����ճ�ʱ�ص�
          }
          else if(strnstr(str1,",FFFE",16) != NULL)           //�����������
          { 
            Delete_Timer(MCU_DeInit);//ɾ����ʱ�ص�
            Upgrade_Recv_Process((unsigned char*)str1);
          }
          
          str1 = str2;
        }
      }
      break;
    case BC95_CONNECT_ERROR:
      //����쳣����
      break;
    default:
      break;
    }
    
    memset(Uart2.R_Buffer,'\0',Uart2.Receive_Length);//����ջ�����
    Uart2.Receive_Length = 0;
    Uart2.Receive_Pend = FALSE;
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
void BC95_Recv_Timeout_CallBack(void)//������ʱ�ط�
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
 Description:   //��ʱ�ص�����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void BC95_Delay_CallBack(void)
{
  BC95.Incident_Pend = TRUE;//��ǹ���
}
/*********************************************************************************
 Function:      //
 Description:   //�ϱ���ʷ����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
unsigned char Report_History_Data(void)
{
  uint8_t data[40] = "AT+NMGS=12,080000000000000000000000\r\n";

   //��ȡʱ��   
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
  
  //�ۻ�����
  data[13] = Int_to_ASCLL(Cal.Water_Data.flow8[0]/0x10);
  data[14] = Int_to_ASCLL(Cal.Water_Data.flow8[0]%0x10);
  data[15] = Int_to_ASCLL(Cal.Water_Data.flow8[1]/0x10);
  data[16] = Int_to_ASCLL(Cal.Water_Data.flow8[1]%0x10);
  data[17] = Int_to_ASCLL(Cal.Water_Data.flow8[2]/0x10);
  data[18] = Int_to_ASCLL(Cal.Water_Data.flow8[2]%0x10);
  data[19] = Int_to_ASCLL(Cal.Water_Data.flow8[3]/0x10);
  data[20] = Int_to_ASCLL(Cal.Water_Data.flow8[3]%0x10);
  //��
  data[21] = Int_to_ASCLL((RTC_DateStr.RTC_Year+2000)/0x1000);
  data[22] = Int_to_ASCLL((RTC_DateStr.RTC_Year+2000)%0x1000/0x100);
  data[23] = Int_to_ASCLL((RTC_DateStr.RTC_Year+2000)%0x100/0x10);
  data[24] = Int_to_ASCLL((RTC_DateStr.RTC_Year+2000)%0x10); 
  //��
  data[25] = Int_to_ASCLL(RTC_DateStr.RTC_Month/0x10);
  data[26] = Int_to_ASCLL(RTC_DateStr.RTC_Month%0x10);
  //��
  data[27] = Int_to_ASCLL(RTC_DateStr.RTC_Date/0x10);
  data[28] = Int_to_ASCLL(RTC_DateStr.RTC_Date%0x10);
  //ʱ
  data[29] = Int_to_ASCLL(RTC_TimeStr.RTC_Hours/0x10);
  data[30] = Int_to_ASCLL(RTC_TimeStr.RTC_Hours%0x10);
  //��
  data[31] = Int_to_ASCLL(RTC_TimeStr.RTC_Minutes/0x10);
  data[32] = Int_to_ASCLL(RTC_TimeStr.RTC_Minutes%0x10);
  //��
  data[33] = Int_to_ASCLL(RTC_TimeStr.RTC_Seconds/0x10);
  data[34] = Int_to_ASCLL(RTC_TimeStr.RTC_Seconds%0x10);
  
  BC95_Data_Send(data,37);
  
  return 1;
}
/******************************************END********************************************************/