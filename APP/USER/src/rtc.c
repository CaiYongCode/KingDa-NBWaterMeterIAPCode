/*********************************************************************************
//������
//���ߣ����ƮԾ     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
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
TimerData_Struct  Time;
RTC_TimeTypeDef   RTC_TimeStr;        //RTCʱ��ṹ��
RTC_DateTypeDef   RTC_DateStr;        //RTC���ڽṹ��
RTC_AlarmTypeDef  RTC_AlarmStr;       //���ӽṹ��


/*********************************************************************************
�ⲿ����������
*********************************************************************************/
/*********************************************************************************
˽�б���������
*********************************************************************************/ 
unsigned short Last_Vol;
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
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void Rtc_Config(void)
{
  RTC_InitTypeDef   RTC_InitStr;        //����RTC��ʼ���ṹ��
  
  CLK_LSEConfig(CLK_LSE_ON);//��LSEʱ��
  while(CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);//�ȴ�LSEʱ�Ӿ���
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);//
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
  
  RTC_WakeUpCmd(DISABLE);                                   //RTC���ѹر�
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);      //��������Ϊ16��Ƶ Ҳ����32.768k/16
  RTC_SetWakeUpCounter(2047);                               //���û��ѼĴ���ʱ��Ϊ 1s

  RTC_ITConfig(RTC_IT_WUT, ENABLE);                         //ʹ��RTC�����ж�ʹ��
  RTC_ClearITPendingBit(RTC_IT_WUT);                        //���RTC���ѱ�־
  RTC_WakeUpCmd(ENABLE);                                    //�Զ�����ʹ��
  
  RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;  //����24Сʱ��
  RTC_InitStr.RTC_AsynchPrediv = 127;     //127*256 = 32768 ����1Hz
  RTC_InitStr.RTC_SynchPrediv = 255;
  RTC_Init(&RTC_InitStr);
  
  ITC_SetSoftwarePriority(RTC_CSSLSE_IRQn, ITC_PriorityLevel_2);   //���ȼ�
  
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
void Set_RTC(void)
{
//  RTC_TimeTypeDef  RTC_Time;
//  RTC_DateTypeDef  RTC_Date;
//  
//  RTC_Date.RTC_Year = BC95.R_Buffer[22]*10+BC95.R_Buffer[24];
//  RTC_Date.RTC_Month = BC95.R_Buffer[26]*10+BC95.R_Buffer[28];
//  RTC_Date.RTC_Date = BC95.R_Buffer[30]*10+BC95.R_Buffer[32];
//  RTC_Date.RTC_WeekDay = RTC_Weekday_Monday;
//  
//  RTC_Time.RTC_Hours   = BC95.R_Buffer[34]*10+BC95.R_Buffer[36];
//  RTC_Time.RTC_Minutes = BC95.R_Buffer[38]*10+BC95.R_Buffer[40];
//  RTC_Time.RTC_Seconds = BC95.R_Buffer[42]*10+BC95.R_Buffer[44];
//  
//  RTC_SetDate(RTC_Format_BIN, &RTC_Date);
//  RTC_SetTime(RTC_Format_BIN, &RTC_Time);
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
//void Set_Time(RTC_TimeTypeDef  *RTC_Time) //����ʱ��
//{
//  RTC_TimeStructInit(RTC_Time);   //Уʱ ʱ��
//  RTC_TimeStr.RTC_Hours   = RTC_Time->RTC_Hours;
//  RTC_TimeStr.RTC_Minutes = RTC_Time->RTC_Minutes;
//  RTC_TimeStr.RTC_Seconds = RTC_Time->RTC_Seconds;
//  RTC_SetTime(RTC_Format_BIN, RTC_Time);
//}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
//void Set_Date(RTC_DateTypeDef  *RTC_Date) //��������
//{
//  RTC_DateStructInit(RTC_Date);  //Уʱ ����
//  RTC_DateStr.RTC_WeekDay = RTC_Date->RTC_WeekDay;
//  RTC_DateStr.RTC_Date = RTC_Date->RTC_Date;
//  RTC_DateStr.RTC_Month = RTC_Date->RTC_Month;
//  RTC_DateStr.RTC_Year = RTC_Date->RTC_Year;
//  
//  RTC_SetDate(RTC_Format_BIN, RTC_Date);
//}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void Set_Alarm(void) //��������
{
//  RTC_AlarmStructInit(RTC_Alarm);    //��������
  RTC_AlarmStr.RTC_AlarmTime.RTC_Hours   = 1;
  RTC_AlarmStr.RTC_AlarmTime.RTC_Minutes = 0;
  RTC_AlarmStr.RTC_AlarmTime.RTC_Seconds = 0;
  RTC_AlarmStr.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
 
  RTC_AlarmCmd(DISABLE);  
  
  RTC_SetAlarm(RTC_Format_BIN,&RTC_AlarmStr);

  RTC_ITConfig(RTC_IT_ALRA, ENABLE);                                            //���������ж�
  RTC_ClearITPendingBit(RTC_IT_ALRA);                                           //��������ж�
  RTC_AlarmCmd(ENABLE);                                                         //��������
  ITC_SetSoftwarePriority(RTC_CSSLSE_IRQn, ITC_PriorityLevel_2);                //���ȼ�
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
void Close_Alarm(void)
{
  RTC_ITConfig(RTC_IT_ALRA, DISABLE);  //�ر������ж�
  RTC_AlarmCmd(DISABLE);    //�ر�����
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
void Alarm_Interrupt (void)                        //�����ж�
{
  if(RESET != RTC_GetITStatus(RTC_IT_ALRA))
  {   
    RTC_GetDate(RTC_Format_BCD, &RTC_DateStr);
    if(RTC_DateStr.RTC_Date == Settle_Date)       //�����ձ����ۻ�����
    {
      Save_SDCF_Flow(&Cal.Water_Data);
    }
    RTC_ClearITPendingBit(RTC_IT_ALRA);              //��������ж�
  }
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
void Wake_Interrupt (void)                        //�����ж�
{
  if(RESET != RTC_GetITStatus(RTC_IT_WUT))
  {
    Device_Status = RUN_MODE;
    RCC_Configuration();
    
    //ÿ5��ִ��һ�ε�ѹ�ɼ�   
    Gather_Cycle_counter++;
    if(Gather_Cycle_counter >= 5)
    {
      rtc_interrupt();
      Gather_Cycle_counter = 0;
    }

    //�����ϱ��豸����
    Report_Cycle_counter++;
    if( (Report_Cycle_counter/60) >= Report_Cycle )
    {
    
      BC95.Report_Bit = 1;
      if(BC95.Start_Process == BC95_POWER_DOWN)
      {
        BC95.Start_Process = BC95_RECONNECT;
      }     
      Report_Cycle_counter = 0;
    }
    else
    {   
      if(BC95.Start_Process == BC95_POWER_DOWN)
      {
        Device_Status = SLEEP_MODE;
      }
    }
    
    //BC95���г�ʱ�ж�
    if(BC95.Start_Process != BC95_POWER_DOWN)
    {
      BC95.Run_Time++;
      if(BC95.Run_Time > 300)
      {
        Device_Status = SLEEP_MODE;
      }
    }
    RTC_ClearITPendingBit(RTC_IT_WUT);                        //���RTC���ѱ�־
  }
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
void delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=16000;  //�Լ�����
      while(i--) ;    
   }
}
void rtc_interrupt (void)  //RTC�жϴ����� 5sһ��
{
    //�ɼ���ѹ
    delay_ms(3);
    Read_Voltage();
    
    if(BAT_Vol < BAT_Alarm_Vol)                                         //������Ƿ�Ƿѹ
    {
      if(Last_Vol >= BAT_Alarm_Vol)                                   //�ж��Ƿ�Ϊ��һ��Ƿѹ
      {
        Save_Add_Flow(ADD_FLOW_ADD,&Cal.Water_Data);                                  //���浱ǰˮ��
        //����
      }
    }
    Last_Vol = BAT_Vol;
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

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

