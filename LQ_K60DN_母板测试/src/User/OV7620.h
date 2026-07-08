#ifndef __OV7620_H_
#define __OV7620_H_

//图像采集大小
#define LIN  320  //列320 //
#define ROW  240   //行240 //


//图像处理大小
#define H   128 //LIN  //
#define V   64  //ROW  //

extern unsigned char Image_data[ROW][LIN];
extern unsigned char Copy_Image[ROW][LIN];//拷贝后图像
extern unsigned char ROW_Count;
extern unsigned char FieldCounter;
extern unsigned char Is_SendPhoto;


extern void Copy_Arr(unsigned char aa[],unsigned char b[]);            //拷贝图像
extern void BinaryZation(unsigned char (*p)[H]);  //二值化
extern void Peng_Fushi(unsigned char (*t)[H],unsigned char (*temp)[H],unsigned char b);
extern void step_end(void);                               //一步到位函数，二值化，滤波


void OV7620_Init(void);

#endif