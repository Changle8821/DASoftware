//
// Created by Administrator on 2017/4/10.
//

#include "GraphDSP.h"

#define GEQ_CH_NUM        8							          // 图示均衡器通道数
#define GEQ_SECT_NUM      31                                  // 图示均衡器段数
#define GEQ_COEFF_NUM     5                                   // 每段图示均衡器的滤波器系数个数

#define MaxXN             945                                 // 各种图形中x轴最大的象素点
/*#define PixelYN           165                                 // y轴的象素点
#define GEQMaxdB          15
#define GEQMindB          -30
#define GEQdBRange        GEQMaxdB - GEQMindB                 // GEQ的增益范围45dB,   -30~+15dB
#define dBPerPixel        ((float)GEQdBRange)/(float)PixelYN  // 一个象素点对应的dB
#define PixelZerodB       (int)(0.5 + 30/0.271084)            // GEQMINdB/dBPerPixel
*/


#define INPEQ_CH_NUM      8							          // 通道数,目前只用了4个
#define INPEQ_SECT_NUM    7                                   // 参量均衡器段数
#define INPEQ_COEFF_NUM   5                                   // 每段参量均衡器的滤波器系数个数

#define InMaxXN           865                                 // x轴最大的象素点
/*#define InPixelYN         310                                 // y轴的象素点
#define InPEQMaxdB        20
#define InPEQMindB        -40
#define InPEQdBRange      GEQMaxdB - GEQMindB                 // GEQ的增益范围45dB,   -30~+15dB
#define InPEQdBPerPixel   ((float)GEQdBRange)/(float)PixelYN  // 一个象素点对应的dB
#define InPEQPixelZerodB  (int)(0.5 + 30/0.271084)            // GEQMINdB/dBPerPixel
*/
#define peak              0
#define low_pass          1
#define high_pass         2
#define low_shelf         3
#define high_shelf        4
#define all_pass          5



#define OUTPEQ_CH_NUM      8							      // 通道数，目前只用了4个
#define OUTPEQ_NUM         7                                  // PEQ的个数
#define OUTLF_NUM          4                                  // LF的二阶节数
#define OUTHF_NUM          4                                  // HF的二阶节数
#define OUTPEQ_SECT_NUM    OUTPEQ_NUM+OUTLF_NUM+OUTHF_NUM     // 参量均衡器段数(7) + 低通二阶节数（4） +高通二阶节数（4）
#define OUTPEQ_COEFF_NUM   5                                  // 每段参量均衡器的滤波器系数个数

#define OutMaxXN           865                          // x轴最大的象素点
/*#define OutPixelYN         310                          // y轴的象素点
#define OutPEQMaxdB        20
#define OutPEQMindB        -40
#define OutPEQdBRange      GEQMaxdB - GEQMindB          // GEQ的增益范围45dB,   -30~+15dB
#define OutPEQdBPerPixel   ((float)GEQdBRange)/(float)PixelYN  // 一个象素点对应的dB
#define OutPEQPixelZerodB  (int)(0.5 + 30/0.271084)      // GEQMINdB/dBPerPixel
*/

#define NOALOGRITHM        0
#define BUTTERWORTH		   1
#define LINKRILEY          2
#define BESSEL             3
#define divFreqLOW         0 /*divFreqLOW,divFreqHIGH必须定义为0和1*/
#define divFreqHIGH        1 /*数组使用的时候为0，1*/

#define GlobalMaxXN        945


//butterworth滤波器原型系数
float butterTab[16]=
        {1.41421f,//二阶
         1.0f,//三阶
         0.7654f,1.8478f,//四阶
         0.6180f,1.6180f,//五阶
         0.5176f,1.41421f,1.9319f,//六阶
         0.445f,1.247f,1.8019f,//七阶
         0.3902f,1.1111f,1.6629f,1.9616f//八阶
        };

//float Qtab[256]=
//{};

float geqfc[31]=
        {20.0f,25.0f,31.5f,40.0f,50.0f,63.0f,80.0f,100.0f,125.0f,160.0f,
         200,250,315,400,500,630,800,1000,1250,1600,
         2000,2500,3150,4000,5000,6300,8000,10000,12500,16000,20000
        };

float geq_eff[GEQ_CH_NUM][GEQ_SECT_NUM][GEQ_COEFF_NUM];
int   geqInitFlag = 1;

float in_peq_eff[INPEQ_CH_NUM][INPEQ_SECT_NUM][INPEQ_COEFF_NUM];
float in_peq_fc[INPEQ_CH_NUM][INPEQ_SECT_NUM];
float in_peq_gain[INPEQ_CH_NUM][INPEQ_SECT_NUM];
int   InPeqInitFlag = 1;

float out_peq_eff[OUTPEQ_CH_NUM][OUTPEQ_SECT_NUM][OUTPEQ_COEFF_NUM];
float out_peq_fc[INPEQ_CH_NUM][INPEQ_SECT_NUM];
float out_peq_gain[INPEQ_CH_NUM][INPEQ_SECT_NUM];
int   OutPeqInitFlag = 1;

float   InPeqFc;
float   OutPeqFc;
float   InPeqGain;
float   OutputPeqGain;


// function to start continuous
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
void AddTwoHw(float *HR_A,float *HI_A,float *HR_B,float *HI_B,float *R,float *I, unsigned int XN)
{
    unsigned int i;
    for(i=0;i<XN;i++)
    {
        R[i]=HR_A[i]+HR_B[i];
        I[i]=HI_A[i]+HI_B[i];

    }
}

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
void GetGraphicData(float *HR,float *HI,int *x,int *y,unsigned int XN,unsigned int YN,int MaxPdB,int MinNdB)
{
    float tempR,tempI,tempH,tempy;
    float tempP;
    unsigned int i;

    int GEQdBRange;     // GEQ的增益范围,比如-30~+15dB，对应45dB
    float dBPerPixel;   // 一个象素点对应的dB
    int PixelZerodB;  // 0dB对应的象素点

    GEQdBRange = MaxPdB - MinNdB;
    dBPerPixel = ((float)GEQdBRange)/(float)YN;  // 一个象素点对应的dB
    PixelZerodB = (int)(0.5 - ((float)MinNdB)/dBPerPixel) ;  // 0dB对应的象素点

    tempP=dBPerPixel;
    for(i=0;i<XN;i++)
    {
        tempR=HR[i];
        tempI=HI[i];
        tempH=(float)sqrt(tempR*tempR+tempI*tempI);
        if(tempH==0)
        {
            tempy=-100;
        }
        else
        {
            tempy=20*(float)log10(tempH);
        }
        y[i] = PixelZerodB + (int)(tempy / tempP);
        //	if (y[i]>=PixelYN)
        //	{
        //y[i]=PixelYN-1;
        //	}
        if (y[i]<0)
        {
            y[i]=0;
        }
        x[i]=i;
    }
}

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
void ComTwoHw(float *HR_A,float *HI_A,float *HR_B,float *HI_B,float *HR,float *HI,unsigned int XN)
{
    float tempR,tempI;
    unsigned int i;

    for(i=0;i<XN;i++)
    {
        tempR=HR_A[i]*HR_B[i]-HI_A[i]*HI_B[i];
        tempI=HR_A[i]*HI_B[i]+HI_A[i]*HR_B[i];
        HR[i]=tempR;
        HI[i]=tempI;
        HR_A[i]=tempR;
        HI_A[i]=tempI;
    }
}

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
void coef_BA2HW(float b0,float b1,float b2, float a1,float a2,float *R,float *I, unsigned int XN)
{
    float B1[MaxXN],A1[MaxXN],B2[MaxXN],A2[MaxXN],w,tempPower;
    int k;
    int M;
    int N0;
    int i;
    M= (int)(1.30103f*(XN-1)/3+XN+0.5f);
    N0=(int)(1.30103f*(XN-1)/3+0.5f);
    k= M-1;
    i=XN-1;
    do
    {
        w=2*3.1415926f*(float)pow(10,4.30103f*k/(M-1))/Fs;
        B1[i]=b0+b1*(float)cos(w)+b2*(float)cos(2*w);
        B2[i]=b1*(float)sin(w)+b2*(float)sin(2*w);
        A1[i]=1+a1*(float)cos(w)+a2*(float)cos(2*w);
        A2[i]=a1*(float)sin(w)+a2*(float)sin(2*w);
        tempPower=A1[i]*A1[i]+A2[i]*A2[i];
        R[i]=(A1[i]*B1[i]+B2[i]*A2[i])/tempPower;
        I[i]=(A1[i]*B2[i]-B1[i]*A2[i])/tempPower;
        k--;
        i--;
    }while(k>=N0);
}
//画图函数end.


/*******************************************************************************************/
/* GEQ A ~ D
/*******************************************************************************************/
//GEQ滤波系数生成函数
void UpdateGEQ(unsigned short ch,unsigned short sec,float Gain)
{
    float Ag,wf,sinw,cosw,alpha,Fc;
    float temp1,temp2,temp3;
    //系数开始运算的时候，必须采取保护的措施，不能随意改动系数表，导致滤波器溢出。
    //GEQbusy_status=busy;
    Fc = geqfc[sec];
    Ag=(float)pow(10,(Gain*0.025));
    wf=(float)CoeffectF*Fc;//sample rate 48K
    sinw=(float)sin(wf);
    cosw=(float)cos(wf);
    alpha=sinw*0.5f/4.318473f;//sinw/(2Q)
    temp1=alpha*Ag;
    temp2=alpha/Ag;
    geq_eff[ch][sec][0]=1+temp1;//b0
    geq_eff[ch][sec][1]=(-2)*cosw;//b1
    geq_eff[ch][sec][2]=1-temp1;//b2
    geq_eff[ch][sec][3]=1+temp2;//a0
    geq_eff[ch][sec][4]=1-temp2;//a2
    temp3=1/geq_eff[ch][sec][3];//归一化处理
    geq_eff[ch][sec][0]*=temp3;
    geq_eff[ch][sec][1]*=temp3;
    geq_eff[ch][sec][2]*=temp3;
    geq_eff[ch][sec][3]=geq_eff[ch][sec][1];
    geq_eff[ch][sec][4]*=temp3;
//	GEQbusy_status=free;
}

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
void GEQgraph(unsigned short ch,int *x,int *y,unsigned int XN,unsigned int YN,int MaxPdB,int MinNdB)
{
    float b0,b1,b2,a1,a2;
    int i,j;
    float HR[MaxXN], HI[MaxXN],HR1[MaxXN], HI1[MaxXN],HR2[MaxXN], HI2[MaxXN];


    if ( geqInitFlag == 1 )
    {
        for ( j = 0; j < GEQ_CH_NUM; j++ )
        {
            for ( i = 0; i < GEQ_SECT_NUM; i++ )
            {
                geq_eff[j][i][0] = 1;
                geq_eff[j][i][1] = 0;
                geq_eff[j][i][2] = 0;
                geq_eff[j][i][3] = 0;
                geq_eff[j][i][4] = 0;
            }
        }
        geqInitFlag = 0;
    }

    b0 = geq_eff[ch][0][0];
    b1 = geq_eff[ch][0][1];
    b2 = geq_eff[ch][0][2];
    a1 = geq_eff[ch][0][3];
    a2 = geq_eff[ch][0][4];

    coef_BA2HW( b0, b1, b2,  a1, a2, HR1,HI1,XN);

    for ( j = 1; j < GEQ_SECT_NUM; j++ )
    {
        b0 = geq_eff[ch][j][0];
        b1 = geq_eff[ch][j][1];
        b2 = geq_eff[ch][j][2];
        a1 = geq_eff[ch][j][3];
        a2 = geq_eff[ch][j][4];
        coef_BA2HW( b0, b1, b2,  a1, a2, HR2,HI2,XN);

        ComTwoHw(HR1,HI1,HR2,HI2,HR,HI,XN);

    }

    //X的取值范围：0 ~ N-1，对应的频率f: 20 ~ 20000Hz
    GetGraphicData(HR1,HI1,x,y,XN,YN,MaxPdB,MinNdB);

}

/****************************************************************************************/
/****************************************************************************************/


// In A ~ D

/****************************************************************************************/
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
void UpdateInPEQ(unsigned short ch,unsigned short sec,unsigned short type,float *Param)
{

    short i;
    float Ag,wf,sinw,cosw,alpha,Q,Gain,Fc;
    float temp1,temp2,temp3,temp4,pitatmp,Agsub1tmp,Agplus1tmp;
    Fc   = Param[0];
    in_peq_fc[ch][sec] = Fc;
    Q    = Param[1];
    Gain = Param[2];
    in_peq_gain[ch][sec] = Gain;
    //Ag   = exp10sp(Gain*0.025);
    Ag=(float)pow(10,(Gain*0.025));
    wf   = (float)CoeffectF*Fc;//sample rate 48K
    sinw=(float)sin(wf);
    cosw=(float)cos(wf);
    alpha=sinw*0.5f/Q;//sinw/(2Q)
    switch(type)
    {
        case peak:
            temp1=alpha*Ag;
            temp2=alpha/Ag;
            in_peq_eff[ch][sec][0]=(float)(1+temp1);//b0
            in_peq_eff[ch][sec][1]=(float)((-2)*cosw);//b1
            in_peq_eff[ch][sec][2]=(float)(1-temp1);//b2
            in_peq_eff[ch][sec][3]=(float)(1+temp2);//a0
            in_peq_eff[ch][sec][4]=(float)(1-temp2);//a2
            temp3=1/in_peq_eff[ch][sec][3];//归一化处理
            in_peq_eff[ch][sec][0]*=(float)temp3;
            in_peq_eff[ch][sec][1]*=(float)temp3;
            in_peq_eff[ch][sec][2]*=(float)temp3;
            in_peq_eff[ch][sec][3]=in_peq_eff[ch][sec][1];
            in_peq_eff[ch][sec][4]*=(float)temp3;
            break;
        case high_shelf:
        case low_shelf:
            pitatmp=(float)sqrt(Ag);
            Agsub1tmp =(Ag-1);
            Agplus1tmp=(Ag+1);
            temp1=Agsub1tmp*cosw;
            temp2=Agplus1tmp*cosw;
            temp3=2*pitatmp*alpha;
            if(type==low_shelf)
            {
                temp4=Agplus1tmp-temp1;
                in_peq_eff[ch][sec][0]=Ag*(temp4+temp3);//b0
                in_peq_eff[ch][sec][1]=2*Ag*(Agsub1tmp-temp2);      //b1
                in_peq_eff[ch][sec][2]=Ag*(temp4-temp3);//b2
                temp4=Agplus1tmp+temp1+temp3;  //a0
                in_peq_eff[ch][sec][3]=(-2)*(Agsub1tmp+temp2);//a1
                in_peq_eff[ch][sec][4]=Agplus1tmp+temp1-temp3;//a2
            }
            else
            {
                temp4=Agplus1tmp+temp1;
                in_peq_eff[ch][sec][0]=Ag*(temp4+temp3);//b0
                in_peq_eff[ch][sec][1]=(-2)*Ag*(Agsub1tmp+temp2);      //b1
                in_peq_eff[ch][sec][2]=Ag*(temp4-temp3);//b2
                temp4=Agplus1tmp-temp1+temp3;  //a0
                in_peq_eff[ch][sec][3]=2*(Agsub1tmp-temp2);//a1
                in_peq_eff[ch][sec][4]=Agplus1tmp-temp1-temp3;//a2
            }
            temp4=1/temp4;
            for(i=0;i<5;i++)
            {
                in_peq_eff[ch][sec][i]*=temp4;
            }
            break;
        case low_pass:
        case high_pass:
            temp4=1+alpha;//a0
            temp1=cosw;
            in_peq_eff[ch][sec][3]=-2*cosw;
            in_peq_eff[ch][sec][4]=1-alpha;
            if(type==low_pass)
            {
                in_peq_eff[ch][sec][0]=(1-temp1)*0.5f;
                in_peq_eff[ch][sec][1]=1-cosw;
                in_peq_eff[ch][sec][2]=in_peq_eff[ch][sec][0];
            }
            else
            {
                in_peq_eff[ch][sec][0]=(1+temp1)*0.5f;
                in_peq_eff[ch][sec][1]=-1-cosw;
                in_peq_eff[ch][sec][2]=in_peq_eff[ch][sec][0];
            }
            temp4=1/temp4;
            for(i=0;i<5;i++)
            {
                in_peq_eff[ch][sec][i]*=temp4;
            }
            break;
        case all_pass:
            temp1=1+alpha;//a0
            in_peq_eff[ch][sec][2]=temp1;//b2
            in_peq_eff[ch][sec][1]=(-2)*cosw;//b1
            in_peq_eff[ch][sec][3]=in_peq_eff[ch][sec][1];//a1
            in_peq_eff[ch][sec][0]=1-alpha;//b0
            in_peq_eff[ch][sec][4]=in_peq_eff[ch][sec][0];//a2
            temp4=1/temp1;
            for(i=0;i<5;i++)
            {
                in_peq_eff[ch][sec][i]*=temp4;
            }
            break;
        default:
            break;
    }
    //数据运算完毕，计算后的结果可以给主处理段应用
//	GEQbusy_status=free;
}

void Adjust_H_Fc(float b0,float b1,float b2, float a1,float a2,float *R,float *I,float fc,unsigned int XN,unsigned int YN,int MaxPdB,int MinNdB)
{

    float fcR1,fcI1,fcR2,fcI2,fcR,fcI;
    int k;
    int M;
    int N0;
    int i;
    float w;
    float tempP;
    int GEQdBRange;     // GEQ的增益范围,比如-30~+15dB，对应45dB
    float dBPerPixel;   // 一个象素点对应的dB
    int PixelZerodB;  // 0dB对应的象素点
    float tempy = 0.0;
    float tempH = 0.0;
    float tempPower;
    int outy = 0;

    GEQdBRange = MaxPdB - MinNdB;
    dBPerPixel = ((float)GEQdBRange)/(float)YN;  // 一个象素点对应的dB
    PixelZerodB = (int)(0.5 - ((float)MinNdB)/dBPerPixel);  // 0dB对应的象素点

    tempP=dBPerPixel;

    M= (int)(1.30103f*(XN-1)/3+XN+0.5f);
    N0=(int)(1.30103f*(XN-1)/3+0.5f);
    k= M-1;
    i=XN-1;
    do
    {
        w=2*3.1415926f*(float)pow(10,4.30103f*k/(M-1))/Fs;
        if (w < 2*3.1415926f*fc/Fs)
        {
            break;
        }
        k--;
        i--;
    }while(k>=N0);

    w=2*3.1415926f*fc/Fs;
    fcR1 = b0+b1*(float)cos(w)+b2*(float)cos(2*w);
    fcI1 = b1*(float)sin(w)+b2*(float)sin(2*w);
    fcR2 = 1+a1*(float)cos(w)+a2*(float)cos(2*w);
    fcI2 = a1*(float)sin(w)+a2*(float)sin(2*w);
    tempPower=fcR2 * fcR2 + fcI2 * fcI2;
    fcR=(fcR2*fcR1+fcI1*fcI2)/tempPower;
    fcI=(fcR2*fcI1-fcR1*fcI2)/tempPower;

/*	tempH=(float)sqrt(fcR*fcR+fcI*fcI);
	if(tempH==0)
	{
		tempy=-100;
	}
	else
	{
		tempy=20*(float)log10(tempH);
	}
	outy = PixelZerodB + (int)(tempy / tempP);
	//	if (y[i]>=PixelYN)
	//	{
			//y[i]=PixelYN-1;
	//	}
	if (outy < 0)
	{
		outy = 0;
	}
	*/

    if (i > 1)
    {
        //y[i-1] = PixelZerodB + (int)(gain / tempP);
        R[i-1] = fcR;
        R[i] = fcR;
        I[i-1] = fcI;
        I[i] = fcI;
    }
}

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
void InPEQgraph(unsigned short ch,int *x,int *y,unsigned int XN,unsigned int YN,int MaxPdB,int MinNdB)
{
    float b0,b1,b2,a1,a2;
    int i,j;
    float HR[InMaxXN], HI[InMaxXN],HR1[InMaxXN], HI1[InMaxXN],HR2[InMaxXN], HI2[InMaxXN];

    if ( InPeqInitFlag == 1 )
    {
        for ( j = 0; j < INPEQ_CH_NUM; j++ )
        {
            for ( i = 0; i < INPEQ_SECT_NUM; i++ )
            {
                in_peq_eff[j][i][0] = 1;
                in_peq_eff[j][i][1] = 0;
                in_peq_eff[j][i][2] = 0;
                in_peq_eff[j][i][3] = 0;
                in_peq_eff[j][i][4] = 0;
            }
        }
        InPeqInitFlag = 0;
    }

    b0 = in_peq_eff[ch][0][0];
    b1 = in_peq_eff[ch][0][1];
    b2 = in_peq_eff[ch][0][2];
    a1 = in_peq_eff[ch][0][3];
    a2 = in_peq_eff[ch][0][4];

    coef_BA2HW( b0, b1, b2,  a1, a2, HR1,HI1,XN);

    Adjust_H_Fc(b0, b1, b2,  a1, a2, HR1,HI1,in_peq_fc[ch][0],XN,YN,MaxPdB,MinNdB);

    for ( j = 1; j < INPEQ_SECT_NUM; j++ )
    {
        b0 = in_peq_eff[ch][j][0];
        b1 = in_peq_eff[ch][j][1];
        b2 = in_peq_eff[ch][j][2];
        a1 = in_peq_eff[ch][j][3];
        a2 = in_peq_eff[ch][j][4];
        coef_BA2HW( b0, b1, b2,  a1, a2, HR2,HI2,XN);
        Adjust_H_Fc(b0, b1, b2,  a1, a2, HR2,HI2,in_peq_fc[ch][j],XN,YN,MaxPdB,MinNdB);

        ComTwoHw(HR1,HI1,HR2,HI2,HR,HI,XN);

    }

    //X的取值范围：0 ~ N-1，对应的频率f: 20 ~ 20000Hz
    GetGraphicData(HR1,HI1,x,y,XN,YN,MaxPdB,MinNdB);


}

/**************************************************************************************/
/**************************************************************************************/




/*该程序段是完成后面的2阶节的数值填充*/
void fillcoeff(unsigned int ch,unsigned int filterType,unsigned int f_order)
{
    unsigned int secnum,i;
    if(filterType==divFreqLOW)
    {
        secnum=((f_order-1)>>1)+1;
        for(i=secnum + OUTPEQ_NUM; i < OUTPEQ_NUM + OUTLF_NUM ;i++)
        {
            out_peq_eff[ch][i][0]=1;//b0
            out_peq_eff[ch][i][1]=0;//b1
            out_peq_eff[ch][i][2]=0;//b2
            out_peq_eff[ch][i][3]=0;//a1
            out_peq_eff[ch][i][4]=0;//a2
        }
    }
    else
    {
        secnum=((f_order-1)>>1)+1;
        for(i=secnum + OUTPEQ_NUM + OUTLF_NUM; i < OUTPEQ_NUM + OUTLF_NUM + OUTHF_NUM ;i++ )
        {
            out_peq_eff[ch][i][0]=1;//b0
            out_peq_eff[ch][i][1]=0;// b1
            out_peq_eff[ch][i][2]=0;//b2
            out_peq_eff[ch][i][3]=0;//a1
            out_peq_eff[ch][i][4]=0;//a2
        }
    }
}
void Get_FOButterFilter(float fc,unsigned int ch,unsigned int filterType)
{
    float wc;
    float temp1,temp2,temp3;
    temp3=(float)PI*fc*(float)OneDivFs;//temp3=pi*fc/Fs;
    temp1=(float)sin(temp3);
    temp2=(float)cos(temp3);
    wc=temp1/temp2;
    temp1=1+wc;
    temp2=wc-1;
    if(filterType==divFreqLOW)
    {
        out_peq_eff[ch][OUTPEQ_NUM][0]=wc/temp1;//b0
        out_peq_eff[ch][OUTPEQ_NUM][1]=out_peq_eff[ch][OUTPEQ_NUM][0];//b1
        out_peq_eff[ch][OUTPEQ_NUM][2]=0;//b2
        out_peq_eff[ch][OUTPEQ_NUM][3]=temp2/temp1;//a1
        out_peq_eff[ch][OUTPEQ_NUM][4]=0;//a2
    }
    else
    {
        out_peq_eff[ch][OUTPEQ_NUM + OUTLF_NUM][0]=1/temp1;//b0
        out_peq_eff[ch][OUTPEQ_NUM + OUTLF_NUM][1]=-out_peq_eff[ch][OUTPEQ_NUM + OUTLF_NUM][0];// b1
        out_peq_eff[ch][OUTPEQ_NUM + OUTLF_NUM][2]=0;//b2
        out_peq_eff[ch][OUTPEQ_NUM + OUTLF_NUM][3]=temp2/temp1;//a1
        out_peq_eff[ch][OUTPEQ_NUM + OUTLF_NUM][4]=0;//a2
    }

}
void Get_SOButterFilter(float fc,unsigned int ch,
                        unsigned int filterType,unsigned int sec,float coeff)
{
    float wc;
    float temp1,temp2,temp3;
    temp3=(float)PI*fc*(float)OneDivFs;//temp3=pi*fc/Fs;
    temp1=(float)sin(temp3);
    temp2=(float)cos(temp3);
    wc=temp1/temp2;
    temp1=wc*wc;
    temp2=wc*coeff+temp1+1;
    temp3=-wc*coeff+temp1+1;
    if(filterType==divFreqLOW)
    {
        out_peq_eff[ch][sec+OUTPEQ_NUM][0]=temp1/temp2;//b0
        out_peq_eff[ch][sec+OUTPEQ_NUM][1]=2*out_peq_eff[ch][sec+OUTPEQ_NUM][0];//b1
        out_peq_eff[ch][sec+OUTPEQ_NUM][2]=out_peq_eff[ch][sec+OUTPEQ_NUM][0];//b2
        out_peq_eff[ch][sec+OUTPEQ_NUM][3]=(2*temp1-2)/temp2;//a1
        out_peq_eff[ch][sec+OUTPEQ_NUM][4]=temp3/temp2;//a2
    }
    else
    {
        out_peq_eff[ch][sec+OUTPEQ_NUM+OUTLF_NUM][0]=1/temp2;//b0
        out_peq_eff[ch][sec+OUTPEQ_NUM+OUTLF_NUM][1]=-2*out_peq_eff[ch][sec+OUTPEQ_NUM+OUTLF_NUM][0];//b1
        out_peq_eff[ch][sec+OUTPEQ_NUM+OUTLF_NUM][2]=out_peq_eff[ch][sec+OUTPEQ_NUM+OUTLF_NUM][0];//b2
        out_peq_eff[ch][sec+OUTPEQ_NUM+OUTLF_NUM][3]=(2*temp1-2)/temp2;//a1
        out_peq_eff[ch][sec+OUTPEQ_NUM+OUTLF_NUM][4]=temp3/temp2;//a2
    }

}

void Get_FOBesselFilter(float fc,float k,unsigned int ch,unsigned int filterType)
{
    float wc;
    float temp1,temp2,temp3;
    temp3=(float)PI*fc*(float)OneDivFs;//temp3=pi*fc/Fs;
    temp1=(float)sin(temp3);
    temp2=(float)cos(temp3);
    wc=temp1/temp2;
    temp1=k+wc;
    temp2=wc-k;
    temp3=1+k*wc;
    if(filterType==divFreqLOW)
    {
        out_peq_eff[ch][OUTPEQ_NUM][0]=wc/temp3;//b0
        out_peq_eff[ch][OUTPEQ_NUM][1]=out_peq_eff[ch][OUTPEQ_NUM][0];//b1
        out_peq_eff[ch][OUTPEQ_NUM][2]=0;//b2
        out_peq_eff[ch][OUTPEQ_NUM][3]=temp2/temp1;//a1
        out_peq_eff[ch][OUTPEQ_NUM][4]=0;//a2
    }
    else
    {
        out_peq_eff[ch][OUTPEQ_NUM+OUTLF_NUM][0]=1/temp1;//b0
        out_peq_eff[ch][OUTPEQ_NUM+OUTLF_NUM][1]=-out_peq_eff[ch][OUTPEQ_NUM+OUTLF_NUM][0];// b1
        out_peq_eff[ch][OUTPEQ_NUM+OUTLF_NUM][2]=0;//b2
        out_peq_eff[ch][OUTPEQ_NUM+OUTLF_NUM][3]=temp2/temp1;//a1
        out_peq_eff[ch][OUTPEQ_NUM+OUTLF_NUM][4]=0;//a2
    }

}
void Get_SOBesselFilter(float fc,float k1,float k2,unsigned int ch,
                        unsigned int filterType,unsigned int sec)
{
    float wc;
    float temp1,temp2,temp3;
    temp3=(float)PI*fc*(float)OneDivFs;//temp3=pi*fc/Fs;
    temp1=(float)sin(temp3);
    temp2=(float)cos(temp3);
    wc=temp1/temp2;//*******///////
    temp1=wc*wc;
    temp3=k1*wc;
    if(filterType==divFreqLOW)
    {
        temp2=k2*temp1;
        out_peq_eff[ch][sec+OUTPEQ_NUM][0]=temp1/(temp2+temp3+1);//b0
        out_peq_eff[ch][sec+OUTPEQ_NUM][1]=2*out_peq_eff[ch][sec+OUTPEQ_NUM][0];//b1
        out_peq_eff[ch][sec+OUTPEQ_NUM][2]=out_peq_eff[ch][sec+OUTPEQ_NUM][0];//b2
        out_peq_eff[ch][sec+OUTPEQ_NUM][3]=(2*temp2-2)/(temp2+temp3+1);//a1
        out_peq_eff[ch][sec+OUTPEQ_NUM][4]=(temp2-temp3+1)/(temp2+temp3+1);//a2
    }
    else
    {
        temp2=temp1+temp3+k2;
        out_peq_eff[ch][sec+OUTPEQ_NUM+OUTLF_NUM][0]=1/temp2;//b0
        out_peq_eff[ch][sec+OUTPEQ_NUM+OUTLF_NUM][1]=-2*out_peq_eff[ch][sec+OUTPEQ_NUM+OUTLF_NUM][0];//b1
        out_peq_eff[ch][sec+OUTPEQ_NUM+OUTLF_NUM][2]=out_peq_eff[ch][sec+OUTPEQ_NUM+OUTLF_NUM][0];//b2
        out_peq_eff[ch][sec+OUTPEQ_NUM+OUTLF_NUM][3]=(2*temp1-2*k2)/temp2;//a1
        out_peq_eff[ch][sec+OUTPEQ_NUM+OUTLF_NUM][4]=(temp1-temp3+k2)/temp2;//a2
    }

}



//
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
void UpdateOutPEQ(unsigned short ch,unsigned short sec,unsigned short type,float *Param)
{

    short i;
    float Ag,wf,sinw,cosw,alpha,Q,Gain,Fc;
    float temp1,temp2,temp3,temp4,pitatmp,Agsub1tmp,Agplus1tmp;
    Fc   = Param[0];
    OutPeqFc = Fc;
    Q    = Param[1];
    Gain = Param[2];
    OutputPeqGain = Gain;
    //Ag   = exp10sp(Gain*0.025);
    Ag=(float)pow(10,(Gain*0.025));
    wf   = (float)CoeffectF*Fc;//sample rate 48K
    sinw=(float)sin(wf);
    cosw=(float)cos(wf);
    alpha=sinw*0.5f/Q;//sinw/(2Q)
    switch(type)
    {
        case peak:
            temp1=alpha*Ag;
            temp2=alpha/Ag;
            out_peq_eff[ch][sec][0]=(float)(1+temp1);//b0
            out_peq_eff[ch][sec][1]=(float)((-2)*cosw);//b1
            out_peq_eff[ch][sec][2]=(float)(1-temp1);//b2
            out_peq_eff[ch][sec][3]=(float)(1+temp2);//a0
            out_peq_eff[ch][sec][4]=(float)(1-temp2);//a2
            temp3=1/out_peq_eff[ch][sec][3];//归一化处理
            out_peq_eff[ch][sec][0]*=(float)temp3;
            out_peq_eff[ch][sec][1]*=(float)temp3;
            out_peq_eff[ch][sec][2]*=(float)temp3;
            out_peq_eff[ch][sec][3]=out_peq_eff[ch][sec][1];
            out_peq_eff[ch][sec][4]*=(float)temp3;
            break;
        case high_shelf:
        case low_shelf:
            pitatmp=(float)sqrt(Ag);
            Agsub1tmp =(Ag-1);
            Agplus1tmp=(Ag+1);
            temp1=Agsub1tmp*cosw;
            temp2=Agplus1tmp*cosw;
            temp3=2*pitatmp*alpha;
            if(type==low_shelf)
            {
                temp4=Agplus1tmp-temp1;
                out_peq_eff[ch][sec][0]=Ag*(temp4+temp3);//b0
                out_peq_eff[ch][sec][1]=2*Ag*(Agsub1tmp-temp2);      //b1
                out_peq_eff[ch][sec][2]=Ag*(temp4-temp3);//b2
                temp4=Agplus1tmp+temp1+temp3;  //a0
                out_peq_eff[ch][sec][3]=(-2)*(Agsub1tmp+temp2);//a1
                out_peq_eff[ch][sec][4]=Agplus1tmp+temp1-temp3;//a2
            }
            else
            {
                temp4=Agplus1tmp+temp1;
                out_peq_eff[ch][sec][0]=Ag*(temp4+temp3);//b0
                out_peq_eff[ch][sec][1]=(-2)*Ag*(Agsub1tmp+temp2);      //b1
                out_peq_eff[ch][sec][2]=Ag*(temp4-temp3);//b2
                temp4=Agplus1tmp-temp1+temp3;  //a0
                out_peq_eff[ch][sec][3]=2*(Agsub1tmp-temp2);//a1
                out_peq_eff[ch][sec][4]=Agplus1tmp-temp1-temp3;//a2
            }
            temp4=1/temp4;
            for(i=0;i<5;i++)
            {
                out_peq_eff[ch][sec][i]*=temp4;
            }
            break;
        case low_pass:
        case high_pass:
            temp4=1+alpha;//a0
            temp1=cosw;
            out_peq_eff[ch][sec][3]=-2*cosw;
            out_peq_eff[ch][sec][4]=1-alpha;
            if(type==low_pass)
            {
                out_peq_eff[ch][sec][0]=(1-temp1)*0.5f;
                out_peq_eff[ch][sec][1]=1-cosw;
                out_peq_eff[ch][sec][2]=out_peq_eff[ch][sec][0];
            }
            else
            {
                out_peq_eff[ch][sec][0]=(1+temp1)*0.5f;
                out_peq_eff[ch][sec][1]=-1-cosw;
                out_peq_eff[ch][sec][2]=out_peq_eff[ch][sec][0];
            }
            temp4=1/temp4;
            for(i=0;i<5;i++)
            {
                out_peq_eff[ch][sec][i]*=temp4;
            }
            break;
        case all_pass:
            temp1=1+alpha;//a0
            out_peq_eff[ch][sec][2]=temp1;//b2
            out_peq_eff[ch][sec][1]=(-2)*cosw;//b1
            out_peq_eff[ch][sec][3]=out_peq_eff[ch][sec][1];//a1
            out_peq_eff[ch][sec][0]=1-alpha;//b0
            out_peq_eff[ch][sec][4]=in_peq_eff[ch][sec][0];//a2
            temp4=1/temp1;
            for(i=0;i<5;i++)
            {
                out_peq_eff[ch][sec][i]*=temp4;
            }
            break;
        default:
            break;
    }
    //数据运算完毕，计算后的结果可以给主处理段应用
//	GEQbusy_status=free;
}

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
                   unsigned int filter_order, float fc)
{
    //阶数必须是1到8，不然程序会出错，要加一些限制上去
    unsigned int CoeffSec;
//	int i;
    //float fc=1000;
    //divFreqbusystatus=busy;

    if(filterAlgorithm==BUTTERWORTH)
    {
        switch(filter_order)
        {
            case 1:
                Get_FOButterFilter(fc,ch,filterType);
                break;
            case 2:
                CoeffSec=0;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[0]);
                break;
            case 3:
                Get_FOButterFilter(fc,ch,filterType);
                CoeffSec=1;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[1]);
                break;
            case 4:
                CoeffSec=0;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[2]);
                CoeffSec=1;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[3]);
                break;
            case 5:
                Get_FOButterFilter(fc,ch,filterType);
                CoeffSec=1;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[4]);
                CoeffSec=2;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[5]);
                break;
            case 6:
                CoeffSec=0;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[6]);
                CoeffSec=1;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[7]);
                CoeffSec=2;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[8]);
                break;
            case 7:
                Get_FOButterFilter(fc,ch,filterType);
                CoeffSec=1;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[9]);
                CoeffSec=2;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[10]);
                CoeffSec=3;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[11]);
                break;
            case 8:
                CoeffSec=0;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[12]);
                CoeffSec=1;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[13]);
                CoeffSec=2;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[14]);
                CoeffSec=3;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[15]);
                break;
            default:
                break;
        }
        fillcoeff(ch,filterType,filter_order);
    }
    else if(filterAlgorithm==LINKRILEY)
    {
        switch(filter_order)
        {
            case 1:
            case 2:
                CoeffSec=0;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,2);
                break;
            case 3:
            case 4:
                CoeffSec=0;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,1.41421f);
                CoeffSec=1;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,1.41421f);
                break;
            case 5:
            case 6:
                CoeffSec=0;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,2);
                CoeffSec=1;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,1.41421f);
                CoeffSec=2;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,1.41421f);
                break;
            case 7:
            case 8: //两个四阶butter级联即为8阶LinkRiley
                CoeffSec=0;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[2]);
                CoeffSec=1;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[3]);
                CoeffSec=2;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[2]);
                CoeffSec=3;
                Get_SOButterFilter(fc,ch,filterType,CoeffSec,butterTab[3]);
                break;
            default:
                break;
        }
        fillcoeff(ch,filterType,filter_order);
    }
    else if(filterAlgorithm==BESSEL)
    {
        switch(filter_order)
        {
            case 1:
                Get_FOBesselFilter(fc,1,ch,filterType);
                break;
            case 2:
                CoeffSec=0;
                Get_SOBesselFilter(fc,1.7321f,1,ch,filterType,CoeffSec);
                break;
            case 3:
                Get_FOBesselFilter(fc,0.9416f,ch,filterType);
                CoeffSec=1;
                Get_SOBesselFilter(fc,1.4913f,1.062f,ch,filterType,CoeffSec);
                break;
            case 4:
                CoeffSec=0;
                Get_SOBesselFilter(fc,1.3144f,1.1211f,ch,filterType,CoeffSec);
                CoeffSec=1;
                Get_SOBesselFilter(fc,1.8095f,0.8920f,ch,filterType,CoeffSec);
                break;
            case 5:
                Get_FOBesselFilter(fc,0.9264f,ch,filterType);
                CoeffSec=1;
                Get_SOBesselFilter(fc,1.1812f,1.1718f,ch,filterType,CoeffSec);
                CoeffSec=2;
                Get_SOBesselFilter(fc,1.7031f,0.9211f,ch,filterType,CoeffSec);
                break;
            case 6:
                CoeffSec=0;
                Get_SOBesselFilter(fc,1.0771f,1.2149f,ch,filterType,CoeffSec);
                CoeffSec=1;
                Get_SOBesselFilter(fc,1.8188f,0.8615f,ch,filterType,CoeffSec);
                CoeffSec=2;
                Get_SOBesselFilter(fc,1.5993f,0.9555f,ch,filterType,CoeffSec);
                break;
            case 7:
                Get_FOBesselFilter(fc,0.9195f,ch,filterType);
                CoeffSec=1;
                Get_SOBesselFilter(fc,0.9934f,1.2517f,ch,filterType,CoeffSec);
                CoeffSec=2;
                Get_SOBesselFilter(fc,1.76f,0.8779f,ch,filterType,CoeffSec);
                CoeffSec=3;
                Get_SOBesselFilter(fc,1.5055f,0.9897f,ch,filterType,CoeffSec);
                break;
            case 8:
                CoeffSec=0;
                Get_SOBesselFilter(fc,0.9243f,1.2836f,ch,filterType,CoeffSec);
                CoeffSec=1;
                Get_SOBesselFilter(fc,1.8194f,0.8475f,ch,filterType,CoeffSec);
                CoeffSec=2;
                Get_SOBesselFilter(fc,1.6947f,0.8994f,ch,filterType,CoeffSec);
                CoeffSec=3;
                Get_SOBesselFilter(fc,1.4223f,1.0222f,ch,filterType,CoeffSec);
                break;
            default:
                break;
        }
        fillcoeff(ch,filterType,filter_order);
    }
    else
    {
        unsigned int i;
        if(filterType==divFreqLOW)
        {
            for(i=OUTPEQ_NUM; i < OUTPEQ_NUM + OUTLF_NUM ;i++)
            {
                out_peq_eff[ch][i][0]=1;//b0
                out_peq_eff[ch][i][1]=0;//b1
                out_peq_eff[ch][i][2]=0;//b2
                out_peq_eff[ch][i][3]=0;//a1
                out_peq_eff[ch][i][4]=0;//a2
            }
        }
        else
        {
            for(i=OUTPEQ_NUM + OUTLF_NUM; i < OUTPEQ_NUM + OUTLF_NUM + OUTHF_NUM ;i++ )
            {
                out_peq_eff[ch][i][0]=1;//b0
                out_peq_eff[ch][i][1]=0;// b1
                out_peq_eff[ch][i][2]=0;//b2
                out_peq_eff[ch][i][3]=0;//a1
                out_peq_eff[ch][i][4]=0;//a2
            }
        }
    }
//	divFreqbusystatus=free;//处理完毕，忙标志置闲；


}




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
void OutPEQgraph(unsigned short ch,int *x,int *y,unsigned int XN,unsigned int YN,int MaxPdB,int MinNdB)
{
    float b0,b1,b2,a1,a2;
    int i,j;
    float HR[OutMaxXN], HI[OutMaxXN],HR1[OutMaxXN], HI1[OutMaxXN],HR2[OutMaxXN], HI2[OutMaxXN];
    float tempP;

    int GEQdBRange;     // GEQ的增益范围,比如-30~+15dB，对应45dB
    float dBPerPixel;   // 一个象素点对应的dB
    int PixelZerodB;  // 0dB对应的象素点
    int k;
    int M;
    int N0;
    float w;
    float fcR1,fcI1,fcR2,fcI2,fcR,fcI;
    fcR1 =0.0;
    fcI1 = 0.0;
    fcR2 = 0.0;
    fcI2 = 0.0;
    fcR = 0.0;
    fcI = 0.0;


    GEQdBRange = MaxPdB - MinNdB;
    dBPerPixel = ((float)GEQdBRange)/(float)YN;  // 一个象素点对应的dB
    PixelZerodB = (int)(0.5 - ((float)MinNdB)/dBPerPixel) ;  // 0dB对应的象素点

    tempP=dBPerPixel;

    if ( OutPeqInitFlag == 1 )
    {
        for ( j = 0; j < OUTPEQ_CH_NUM; j++ )
        {
            for ( i = 0; i < OUTPEQ_SECT_NUM; i++ )
            {
                out_peq_eff[j][i][0] = 1;
                out_peq_eff[j][i][1] = 0;
                out_peq_eff[j][i][2] = 0;
                out_peq_eff[j][i][3] = 0;
                out_peq_eff[j][i][4] = 0;
            }
        }
        OutPeqInitFlag = 0;
    }

    b0 = out_peq_eff[ch][0][0];
    b1 = out_peq_eff[ch][0][1];
    b2 = out_peq_eff[ch][0][2];
    a1 = out_peq_eff[ch][0][3];
    a2 = out_peq_eff[ch][0][4];

    coef_BA2HW( b0, b1, b2,  a1, a2, HR1,HI1,XN);
    Adjust_H_Fc(b0, b1, b2,  a1, a2, HR1,HI1,out_peq_fc[ch][0],XN,YN,MaxPdB,MinNdB);

    for ( j = 1; j < OUTPEQ_SECT_NUM; j++ )
    {
        b0 = out_peq_eff[ch][j][0];
        b1 = out_peq_eff[ch][j][1];
        b2 = out_peq_eff[ch][j][2];
        a1 = out_peq_eff[ch][j][3];
        a2 = out_peq_eff[ch][j][4];
        coef_BA2HW( b0, b1, b2,  a1, a2, HR2,HI2,XN);
        Adjust_H_Fc(b0, b1, b2,  a1, a2, HR2,HI2,in_peq_fc[ch][j],XN,YN,MaxPdB,MinNdB);


        ComTwoHw(HR1,HI1,HR2,HI2,HR,HI,XN);

    }

    //X的取值范围：0 ~ N-1，对应的频率f: 20 ~ 20000Hz
    GetGraphicData(HR1,HI1,x,y,XN,YN,MaxPdB,MinNdB);

    M= (int)(1.30103f*(XN-1)/3+XN+0.5f);
    N0=(int)(1.30103f*(XN-1)/3+0.5f);
    k= M-1;
    i=XN-1;
    do
    {
        w=2*3.1415926f*(float)pow(10,4.30103f*k/(M-1))/Fs;
        if (w < 2*3.1415926f*InPeqFc/Fs)
        {
            break;
        }
        k--;
        i--;
    }while(k>=N0);

    if (i > 1)
    {
        y[i-1] = PixelZerodB + (int)(OutputPeqGain / tempP);
        y[i] = y[i-1];
    }

}


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
void GlobalGraph(unsigned short ch,int *x,int *y,unsigned int XN,unsigned int YN,int MaxPdB,int MinNdB,float gain)
{
    float b0,b1,b2,a1,a2;
    int i,j;
    float HR[GlobalMaxXN], HI[GlobalMaxXN],HR1[GlobalMaxXN], HI1[GlobalMaxXN],HR2[GlobalMaxXN], HI2[GlobalMaxXN];
    float coef;

    if ( OutPeqInitFlag == 1 )
    {
        for ( j = 0; j < OUTPEQ_CH_NUM; j++ )
        {
            for ( i = 0; i < OUTPEQ_SECT_NUM; i++ )
            {
                out_peq_eff[j][i][0] = 1;
                out_peq_eff[j][i][1] = 0;
                out_peq_eff[j][i][2] = 0;
                out_peq_eff[j][i][3] = 0;
                out_peq_eff[j][i][4] = 0;
            }
        }
        OutPeqInitFlag = 0;
    }

    b0 = out_peq_eff[ch][0][0];
    b1 = out_peq_eff[ch][0][1];
    b2 = out_peq_eff[ch][0][2];
    a1 = out_peq_eff[ch][0][3];
    a2 = out_peq_eff[ch][0][4];

    coef_BA2HW( b0, b1, b2,  a1, a2, HR1,HI1,XN);

    for ( j = 1; j < OUTPEQ_SECT_NUM; j++ )
    {
        b0 = out_peq_eff[ch][j][0];
        b1 = out_peq_eff[ch][j][1];
        b2 = out_peq_eff[ch][j][2];
        a1 = out_peq_eff[ch][j][3];
        a2 = out_peq_eff[ch][j][4];
        coef_BA2HW( b0, b1, b2,  a1, a2, HR2,HI2,XN);

        ComTwoHw(HR1,HI1,HR2,HI2,HR,HI,XN);

    }

    coef = (float)pow(10,(gain*0.05));
    for (j = 0; j < GlobalMaxXN; j++)
    {
        HR1[j] = HR1[j] * coef;
        HI1[j] = HI1[j] * coef;
    }
    //X的取值范围：0 ~ N-1，对应的频率f: 20 ~ 20000Hz
    GetGraphicData(HR1,HI1,x,y,XN,YN,MaxPdB,MinNdB);

}


float ConverX2Fc(int x,unsigned int XN)
{

    int M;
    int N0;
    float f;

    M= (int)(1.30103f*(XN-1)/3+XN+0.5f);
    N0=(int)(1.30103f*(XN-1)/3+0.5f);

    f=(float)pow(10,4.30103f*(x+N0)/(M-1));
    return f;
}

float ConverY2Gain(int y,unsigned int YN, int MaxPdB,int MinNdB)
{

    float tempP;
    float gain;
    int GEQdBRange;     // GEQ的增益范围,比如-30~+15dB，对应45dB
    float dBPerPixel;   // 一个象素点对应的dB
    int PixelZerodB;  // 0dB对应的象素点
    GEQdBRange = MaxPdB - MinNdB;
    dBPerPixel = ((float)GEQdBRange)/(float)YN;  // 一个象素点对应的dB
    PixelZerodB = (int)(0.5 - ((float)MinNdB)/dBPerPixel) ;  // 0dB对应的象素点

    tempP = (float)(y - PixelZerodB);
    gain = tempP * dBPerPixel;
    return gain;

}


// 参数说明 f: 频率； XN：X轴总的象素值, 返回值：X轴坐标值
int ConverFc2X(float f,unsigned int XN)
{

    int M;
    int N0;
    int x;

    M= (int)(1.30103f*(XN-1)/3+XN+0.5f);
    N0=(int)(1.30103f*(XN-1)/3+0.5f);

    x=(int)(log10(f)*(M-1)/4.30103-N0);
    return x;
}

// 参数说明 gain: 增益； YN：Y轴总的象素值； MaxPdB: 增益上限值；MinNdB：增益下限值；返回值：y轴坐标值
int ConverGain2Y(float gain,unsigned int YN, int MaxPdB,int MinNdB)
{

    float tempP;
    int y;
    int GEQdBRange;     // GEQ的增益范围,比如-30~+15dB，对应45dB
    float dBPerPixel;   // 一个象素点对应的dB
    int PixelZerodB;  // 0dB对应的象素点
    GEQdBRange = MaxPdB - MinNdB;
    dBPerPixel = ((float)GEQdBRange)/(float)YN;  // 一个象素点对应的dB
    PixelZerodB = (int)(0.5 - ((float)MinNdB)/dBPerPixel) ;  // 0dB对应的象素点

    tempP = gain / dBPerPixel;
    y = (int)(PixelZerodB + tempP);

    return y;

}

//zhangcy 20111207



