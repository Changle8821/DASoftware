//
// Created by Administrator on 2017/4/10.
//

#ifndef DASOFTWARE_GRAPHDSP_H
#define DASOFTWARE_GRAPHDSP_H



#include "stdio.h"
#include "string.h"
#include "math.h"
#define  PI 3.1415926535897932384626433832795
#define  Fs  48000
#define OneDivFs 0.000020833333//  1/48000
#define CoeffectF 1.3089969389957471826927680763665e-4 //2*PI/48000
#define BUTTERWORTH 1
#define LINKRILEY   2
#define BESSEL      3
#define divFreqLOW   0 /*divFreqLOW,divFreqHIGH必须定义为0和1*/
#define divFreqHIGH  1 /*数组使用的时候为0，1*/
#define INCH  8
#define OUTCH 8

extern int geqInitFlag;
extern int InPeqInitFlag;


/****************************************************************************************
 函数名：coef_BA2HW
 功能：根据滤波器的系数b=[b0 b1 b2], a=[1 a1 a2]计算其复数形式的频谱数据
 输入：
       滤波器分子系数：b0 b1 b2
	   滤波器分母系数：a1 a2
 输出：
       R 滤波器频谱的实部
       I 滤波器频谱的虚部
*******************************************************************************************/
void coef_BA2HW(float b0,float b1,float b2, float a1,float a2,float *R,float *I,unsigned int XN);


/****************************************************************************************
 函数名：ComTwoHw
 功能：将两个滤波器串联起来
 输入：HR_A 滤波器1的实部
       HI_A 滤波器1的虚部
	   HR_B 滤波器2的实部
       HI_B 滤波器2的虚部
 输出：
       HR 滤波器并联后的实部
       HI 滤波器并联后的虚部
*******************************************************************************************/
void ComTwoHw(float *HR_A,float *HI_A,float *HR_B,float *HI_B,float *HR,float *HI,unsigned int XN);


/****************************************************************************************
 函数名：GetGraphicData
 功能：得到频谱的画图数据
 输入：HR 滤波器频谱的实部
       HI 滤波器频谱的虚部

 输出：
       x 画图的横轴数据
	     x的取值范围：OutX的取值范围：0 ~ N-1，对应的频率f: 20 ~ 20000Hz
       y 画图的纵轴数据
*******************************************************************************************/
void GetGraphicData(float *HR,float *HI,int *x,int *y,unsigned int XN,unsigned int YN,int MaxPdB,int MinNdB);


/****************************************************************************************
 函数名：AddTwoHw
 功能：将两个滤波器并联起来：
 输入：HR_A 滤波器1的实部
       HI_A 滤波器1的虚部
	   HR_B 滤波器2的实部
       HI_B 滤波器2的虚部
 输出：
       R 滤波器并联后的实部
       I 滤波器并联后的虚部
*******************************************************************************************/
void AddTwoHw(float *HR_A,float *HI_A,float *HR_B,float *HI_B,float *R,float *I, unsigned int XN);


// 参数说明 x: X轴坐标值； XN：X轴总的象素值, 返回值：频率
float ConverX2Fc(int x,unsigned int XN);

// 参数说明 y: y轴坐标值； YN：Y轴总的象素值； MaxPdB: 增益上限值；MinNdB：增益下限值；返回值：增益
float ConverY2Gain(int y,unsigned int YN, int MaxPdB,int MinNdB);

// 参数说明 f: 频率； XN：X轴总的象素值, 返回值：X轴坐标值
int ConverFc2X(float f,unsigned int XN);

// 参数说明 gain: 增益； YN：Y轴总的象素值； MaxPdB: 增益上限值；MinNdB：增益下限值；返回值：y轴坐标值
int ConverGain2Y(float gain,unsigned int YN, int MaxPdB,int MinNdB);


//GEQ滤波系数生成函数
void UpdateGEQ(unsigned short ch,unsigned short sec,float Gain);


/****************************************************************************************
 函数名：GEQgraph
 功能：得到GEQ频谱的画图数据
 输入：ch 第几通道
       XN 横轴数据长度

 输出：
       x 画图的横轴数据
	     x的取值范围：0 ~ XN-1，对应的频率f: 20 ~ 20000Hz
       y 画图的纵轴数据
*******************************************************************************************/
void GEQgraph(unsigned short ch,int *x,int *y,unsigned int XN,unsigned int YN,int MaxPdB,int MinNdB);


/****************************************************************************************
 函数名：UpdateInPEQ
 功能：IN PEQ滤波系数生成函数
 输入：ch 第几通道
       sec 第几段PEQ
	   type 滤波器类型（PEQ、LP、HP、LShelf、HShelf、allpass)
       *Param 参数指针，3个参数的顺序：、fc、Q、g

 输出：
      in_peq_eff数据
*******************************************************************************************/
void UpdateInPEQ(unsigned short ch,unsigned short sec,unsigned short type,float *Param);


/****************************************************************************************
 函数名：InPEQgraph
 功能：得到InPEQ频谱的画图数据
 输入：ch 第几通道
       XN 横轴数据长度

 输出：
       x 画图的横轴数据
	     x的取值范围：0 ~ XN-1，对应的频率f: 20 ~ 20000Hz
       y 画图的纵轴数据
*******************************************************************************************/
void InPEQgraph(unsigned short ch,int *x,int *y,unsigned int XN,unsigned int YN,int MaxPdB,int MinNdB);


/****************************************************************************************
 函数名：OutPEQgraph
 功能：得到InPEQ频谱的画图数据
 输入：ch 第几通道
       XN 横轴数据长度

 输出：
       x 画图的横轴数据
	     x的取值范围：0 ~ XN-1，对应的频率f: 20 ~ 20000Hz
       y 画图的纵轴数据
*******************************************************************************************/
void OutPEQgraph(unsigned short ch,int *x,int *y,unsigned int XN,unsigned int YN,int MaxPdB,int MinNdB);


/****************************************************************************************
 函数名：UpdateOutLpHp
 功能：OUT的LP或HP滤波系数生成函数
 输入：ch 第几通道
       sec 第几段PEQ
	   type 滤波器类型（PEQ、LP、HP、LShelf、HShelf、allpass)
       *Param 参数指针，3个参数的顺序：、fc、Q、g

 输出：
      out_peq_eff数据
*******************************************************************************************/
void UpdateOutLpHp(unsigned int ch,unsigned int filterType,unsigned int filterAlgorithm,
                   unsigned int filter_order, float fc);

/****************************************************************************************
 函数名：UpdateOutPEQ
 功能：OUT PEQ滤波系数生成函数
 输入：ch 第几通道
       sec 第几段PEQ
	   type 滤波器类型（PEQ、LP、HP、LShelf、HShelf、allpass)
       *Param 参数指针，3个参数的顺序：、fc、Q、g

 输出：
      out_peq_eff数据
*******************************************************************************************/
void UpdateOutPEQ(unsigned short ch,unsigned short sec,unsigned short type,float *Param);

/****************************************************************************************
 函数名：GlobalGraph
 功能：得到GlobalGraph频谱的画图数据
 输入：ch 第几通道
       XN 横轴数据长度

 输出：
       x 画图的横轴数据
	     x的取值范围：0 ~ XN-1，对应的频率f: 20 ~ 20000Hz
       y 画图的纵轴数据
*******************************************************************************************/
void GlobalGraph(unsigned short ch,int *x,int *y,unsigned int XN,unsigned int YN,int MaxPdB,int MinNdB,float gain);

#endif //DASOFTWARE_GRAPHDSP_H