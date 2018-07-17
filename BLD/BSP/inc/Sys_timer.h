/*********************************************************************************************************
//概述：
//作者：随风飘跃    时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************************************/
#ifndef  __SYS_TIMER_H
#define  __SYS_TIMER_H
/*********************************************************************************************************
文件包含区
*********************************************************************************************************/
//#include "include_file.h"
#include "include.h"
/*********************************************************************************************************
宏定义区
*********************************************************************************************************/
#define SYS_TIMER_NUMBER          10                                           //定时器数量
/*********************************************************************************************************
数据类型定义
*********************************************************************************************************/
enum Sys_Mode_En
{
  ONCE,//单次执行
  CONTINUOUSS,//连续执行
  SET_TIME//设置的次数
};
enum Sys_Run_Mode_En
{
  PROCESS,//进程执行
  INTERRUPT2//中断执行
};
struct Str_Sys_Timer                                                           //用于系统定时器
{
  unsigned long Counter;                                                       //定时用计数器
  unsigned long Duration;                                                     //定时时长
  FunctionalState Start;                                                        //定时器启动标志
  enum Sys_Run_Mode_En Run_Mode;                                               //执行方式
  FunctionalState Pend;                                                         //定时器挂起标志
  void (*Call_Back)(void);                                                     //回调函数
  enum Sys_Mode_En Mode;                                                        //定时模式
  unsigned short Run_Nuber;                                                   //运行次数
};
/*********************************************************************************************************
外部变量声明区
*********************************************************************************************************/
/*********************************************************************************************************
函数声明区
*********************************************************************************************************/
void SysTick_Handler(void);                                                     //滴答中断服务函数
ErrorStatus Create_Timer(enum Sys_Mode_En Mode,unsigned long Duration,
                         void (*Call_Back)(void),unsigned short Run_Nuber,
                         enum Sys_Run_Mode_En Run_Mode);
ErrorStatus Delete_Timer(void (*Call_Back)(void));
void Sys_Timer_Process(void);
/********************************************************************************************************/
#endif

/******************************************END********************************************************/