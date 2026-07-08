#include "include.h"


unsigned char Image_data[ROW][LIN] = {0};
unsigned char Copy_Image[ROW][LIN] = {0};
unsigned char ROW_Count = 0;

unsigned char FieldCounter = 0;
unsigned char Is_SendPhoto = 0;

unsigned char a=1;

void OV7620_Init(void)
{
  EXTI_Init(PTC,6,rising_down);   //行中断
  EXTI_Init(PTC,7,falling_up);    //场中断
}


void Copy_Arr(unsigned char aa[],unsigned char b[])            //拷贝图像
{
  unsigned int i,n=V*H;
  for(i= 0;i < n;i++)
    b[i] = aa[i];
}
void BinaryZation(unsigned char (*p)[H])  //二值化
{
    unsigned int i,j;
    for(i=0;i<V;i++)
      for(j=0;j<H;j++)
      {
        if(*(*(p + i)+ j)<120+60)    //固定阈值
        {
          *(*(p + i)+ j) = 1;    //黑色是1
        }
        else
        {
          *(*(p + i)+ j) = 0;   //白色是0
        }
      }
}
void Peng_Fushi(unsigned char (*t)[H],unsigned char (*temp)[H],unsigned char b)
{
   a = b;
    unsigned int i,j;
    for(i=1;i<V-1;i++)
      for(j=1;j<H-1;j++)
          if(t[i][j-1]==a||t[i][j+1]==a||t[i-1][j]==a||t[i+1][j]==a)  //对图像内部进行置一处理
               temp[i][j]=a;
    for(i=1;i<V-1;i++)
    {
        if(t[i-1][0]==a||t[i+1][0]==a)   //对图像第一列数据置一处理
             temp[i][0]=a;
        if(t[i-1][H-1]==a||t[i+1][H-1]==a) //对图像最后一列置一处理
             temp[i][H-1]=a;
    }
    for(j=0;j<H-1;j++)
    {
        if(t[0][j-1]==a||t[0][j+1]==a)//对第一行进行置一处理
             temp[0][j]=a;
        if(t[V-1][j-1]==a||t[V-1][j+1]==a)//对最后一行进行置一处理
             temp[V-1][j]=a;
    }
    if(t[0][1]==a||t[1][0]==a)   //对四个角进行置一处理
           temp[0][0]=a;
    if(t[0][H-2]==a||t[1][H-1]==a)
           temp[0][H-1]=a;
    if(t[V-2][0]==a||t[V-1][1]==a)
           temp[V-1][0]=a;
    if(t[V-2][H-1]==a||t[V-1][H-2]==a)
           temp[V-1][H-1]=a; 
}
void step_end(void)                               //一步到位函数，二值化，滤波
{
   unsigned char t[V][H];          //定义参考图像
    
   Copy_Arr(Image_data[0],Copy_Image[0]);  //拷贝图像
   BinaryZation(Copy_Image);             //二值化
   Copy_Arr(Copy_Image[0],t[0]);         //拷贝出来一份参考图像
    
  /**********************膨胀腐蚀闭处理**********************/  
   
    Peng_Fushi(t,Copy_Image,1);           //膨胀处理
    Peng_Fushi(t,Copy_Image,0);           //腐蚀处理
}


