/*******************************************************************************
【平    台】龙邱K66FX智能车VD母板
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2018年1月8日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR7.80.4及以上
【Target  】K66FX1M0VLQ18
【Crystal 】 50.000Mhz
【busclock】180.000MHz
【pllclock】180.000MHz
******************************************************************************/
#include "LQMT9V034M.h"

//初始化GPIO口，作为模拟IIC驱动管脚
void LQ_IIC_GPIO_Init(void)
{
  GPIO_Init(PORTE,0,GPO,1); //配置作为输出作为SDA_Out
  GPIO_Init(PORTE,1,GPO,1); //配置作为输出作为SCL_Out
}
//延时函数
void LQ_IIC_Delay(void)
{  //延时函数，用户可以调用系统延时，也可以自己定义延时
  int i;
  for(i=0;i<360;i++)
  {
    asm("NOP");asm("NOP");
  }
}