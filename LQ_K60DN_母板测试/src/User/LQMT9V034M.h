/*******************************************************************************
【平    台】龙邱K66FX智能车VD母板
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2017年12月22日
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

#ifndef __LQMT9V034M_H__
#define __LQMT9V034M_H__

#include "include.h"

/*
//自定义工程加入下面的头文件
#include "MK66F18.h" 
#include "common.h"
#include "GPIO.h"
#include "GPIO_Cfg.h"
*/

#define LQ_IIC_SCL        PTE1_OUT 		//SCL
#define LQ_IIC_SDA_OUT    PTE0_OUT		//SDA	 
#define LQ_IIC_SDA_IN     PTE0_IN		//SDA

#define LQ_SCL_OUT()  GPIO_Init(PORTE,1, 1,1) //配置作为输出作为SCL_Out
#define LQ_SDA_IN()   GPIO_Init(PORTE,0, 0,1) //配置作为输入作为SDA_In
#define LQ_SDA_OUT()  GPIO_Init(PORTE,0, 1,1) //配置作为输出作为SDA_Out

//延时函数
extern void LQ_IIC_Delay(void);
//初始化GPIO口，作为模拟IIC驱动管脚
extern void LQ_IIC_GPIO_Init(void);
extern unsigned int MTV_ReadReg16(unsigned char address);
extern unsigned char MTV_WriteReg16(unsigned char address, unsigned int Data);
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数功能】龙邱MT9V034模块分辨率大小设置
【作  者】CHIUSIR
【参数值】
【软件版本】V1.0
【最后更新】2018年1月8日
【函数名】void LQMT9V034M_Set_Resolution(unsigned int width,unsigned int height)
【返回值】无
【参数值】
unsigned int width，自定义摄像头采集数据的横向列数
unsigned int height，自定义摄像头采集数据的纵向行数
【实例】 LQMT9V034M_Set_Resolution(320,240);//自定义图像大小为320*240
【提示】 重新定义摄像头分辨率后，需要重新定义每行采集像素的数量为width，行数为height。                                             
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
/* 
//此处为MT9V034窗口大小设置，默认最大为752*480像素，或者使用下面的分辨率   
    // C/SIF Resolutions
    FRAMESIZE_QQCIF,    // 88x72
    FRAMESIZE_QCIF,     // 176x144
    FRAMESIZE_CIF,      // 352x288
    FRAMESIZE_QQSIF,    // 88x60
    FRAMESIZE_QSIF,     // 176x120
    FRAMESIZE_SIF,      // 352x240
    // VGA Resolutions
    FRAMESIZE_QQQQVGA,  // 40x30
    FRAMESIZE_QQQVGA,   // 80x60
    FRAMESIZE_QQVGA,    // 160x120
    FRAMESIZE_QVGA,     // 320x240,可用
    FRAMESIZE_VGA,      // 640x480，可用
    FRAMESIZE_HQQQVGA,  // 60x40
    FRAMESIZE_HQQVGA,   // 120x80
    FRAMESIZE_HQVGA,    // 240x160
    // Other
    FRAMESIZE_LCD,      // 128x160
    FRAMESIZE_QQVGA2,   // 128x160
    FRAMESIZE_40x30,    // 40x30
    FRAMESIZE_64x32,    // 64x32
    FRAMESIZE_64x64,    // 64x64
*/
extern void LQMT9V034M_Set_Resolution(unsigned int width,unsigned int height);


#endif // __MT9V034_H__
