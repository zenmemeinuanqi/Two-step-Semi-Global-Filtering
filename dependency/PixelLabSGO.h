/********************************************************************
created:	2016/03/29
author:		Zhang Yanfeng
e-mail:     zhang_yanfeng_3d@foxmail.com
purpose:	This file is for enegy optimization with semi-global optimization.
*********************************************************************/
#ifndef _PIXELLABSGO_H_
#define _PIXELLABSGO_H_
#include <windows.h>

#ifdef PIXELLABSGO_EXPORTS
#define PIXELLABSGO_API _declspec(dllexport)
#else

#ifndef _X64
#define PIXELLABSGO_API _declspec(dllimport)
#pragma comment(lib,"PixelLabSGO.lib")
#pragma message("Automatically linking width PixelLabSGO.lib")

#else

#define PIXELLABSGO_API _declspec(dllimport)
#pragma comment(lib,"PixelLabSGO_x64.lib")
#pragma message("Automatically linking width PixelLabSGO_x64.lib")
#endif

#endif

/*****************函数说明***************/
//函数名：SimpleGlobalAggregation
//功能：对输入的代价矩阵进行半全局积聚
//参数说明：
//costmatrix：输入代价矩阵，三维数组，维度为width*height*numLevel
//width：图像宽度
//height：图像高度
//numLevel：标号数目
/*****************函数说明***************/
PIXELLABSGO_API void SimpleGlobalAggregation(float *costmatrix, int width, int height, int numLevel,
	float p1 = 0.3, float p2 = 1.2);

/*****************函数说明***************/
//函数名：ImageGuidedSemiGlobalAggregation
//功能：对输入的代价矩阵进行自适应惩罚的半全局积聚，专为正射引导的DSM匹配设计
//参数说明：
//costmatrix：输入代价矩阵，三维数组，维度为width*height*numLevel
//bufImg：输入真正射图像，二维数组，维度为width*heihgt
//weight：输入权重数组，二维数组，维度为width*height
//width：图像宽度
//height：图像高度
//numLevel：标号数目
//p1:平滑惩罚1，p2:平滑惩罚2，p3:弱纹理区域惩罚增益系数，p4:弱纹理区域元素阈值
/*****************函数说明***************/
PIXELLABSGO_API void ImageGuidedSemiGlobalAggregation(float *costmatrix, unsigned char *bufImg, int width, int height, int numLevel,
	float p1 = 0.3, float p2 = 1.2, float p3 = 2.0, int p4 = 100);

/*****************函数说明***************/
//函数名：HierSimpleGlobalAggregation
//功能：对输入的代价矩阵进行半全局积聚
//参数说明：
//costmatrix：输入代价矩阵，三维数组，维度为width*height*numLevel
//width：图像宽度
//height：图像高度
//numLevel：标号数目
//initlabel:初始标号，记录最小标号
//wholerange:整个标号范围
//p1，p2:惩罚
/*****************函数说明***************/
PIXELLABSGO_API void HierSimpleGlobalAggregation(float *costmatrix, int width, int height, int numLevel, int* initlabel, int wholerange,
	float p1 = 0.3, float p2 = 1.2);

/*****************函数说明***************/
//函数名：HierSemiGlobalAggregation
//功能：对输入的代价矩阵进行保边金字塔半全局积聚
//参数说明：
//costmatrix：输入代价矩阵，三维数组，维度为width*height*numLevel
//width：图像宽度
//height：图像高度
//numLevel：标号数目
//initlabel:初始标号，记录最小标号
//range:初始标号范围，[initlabel,initlabel+numLevel]
//wholerange:整个标号范围
//p1，p2:惩罚
/*****************函数说明***************/
PIXELLABSGO_API void HierSemiGlobalAggregation(float *costmatrix, int width, int height, int* initlabel, int *range, int wholerange,
	float p1 = 0.3, float p2 = 1.2);
/*****************函数说明***************/
//函数名：HierImageGuidedSemiGlobalAggregation
//功能：对输入的代价矩阵进行自适应惩罚的半全局积聚，弱纹理区域进行强平滑约束，专为正射引导的DSM匹配设计
//参数说明：
//costmatrix：输入代价矩阵，三维数组，维度为width*height*numLevel
//bufImg：输入真正射图像，二维数组，维度为width*heihgt
//weight：输入权重数组，二维数组，维度为width*height
//width：图像宽度
//height：图像高度
//numLevel：标号数目
//p1:平滑惩罚1，p2:平滑惩罚2，p3:弱纹理区域惩罚增益系数，p4:弱纹理区域元素阈值
/*****************函数说明***************/
PIXELLABSGO_API void HierImageGuidedSemiGlobalAggregation(float *costmatrix, unsigned char *bufImg, int width, int height, int numLevel, int* initlabel, int wholerange,
	float p1 = 0.3, float p2 = 1.2, float p3 = 2.0, int p4 = 100);

/*****************函数说明***************/
//函数名：SegmentConstrainedSemiGlobalAggregation
//功能：对输入的代价矩阵进行分割约束的半全局积聚，各分割块独立处理，专为分割约束的DSM滤波设计
//参数说明：
//costmatrix：输入代价矩阵，三维数组，维度为width*height*numLevel
//segflag：输入分割标记，二维数组，维度为width*heihgt
//gamma:输入点的权重，维度为width*height
//weight：输入权重数组，二维数组，维度为width*height
//width：图像宽度
//height：图像高度
//numLevel：标号数目
/*****************函数说明***************/
PIXELLABSGO_API void SegmentConstrainedSemiGlobalAggregation(float *costmatrix, int* segflag, float *gamma, int width, int height, int numLevel,
	float p1 = 0.3, float p2 = 1.2);

/*****************函数说明***************/
//函数名：PixelwiseSmoothSemiGlobalAggregation
//功能：对输入的代价矩阵进行逐像素平滑的半全局积聚
//参数说明：
//costmatrix：输入代价矩阵，三维数组，维度为width*height*numLevel
//width：图像宽度
//height：图像高度
//numLevel：标号数目
/*****************函数说明***************/
PIXELLABSGO_API void PixelwiseSmoothSemiGlobalAggregation(float *costmatrix, int width, int height, int numLevel);

/*****************函数说明***************/
//函数名：NovelSimpleGlobalAggregation
//功能：对输入的代价矩阵进行二阶半全局积聚
//参数说明：
//costmatrix：输入代价矩阵，三维数组，维度为width*height*numLevel
//width：图像宽度
//height：图像高度
//numLevel：标号数目
/*****************函数说明***************/
PIXELLABSGO_API void NovelSimpleGlobalAggregation(float *costmatrix, int width, int height, int numLevel,
	float p1 = 0.3, float p2 = 1.2);

/*****************函数说明***************/
//函数名：NovelSimpleGlobalAggregation
//功能：对输入的代价矩阵进行地形引导的二阶半全局积聚
//参数说明：
//costmatrix：输入代价矩阵，三维数组，维度为width*height*numLevel
//width：图像宽度
//height：图像高度
//numLevel：标号数目
/*****************函数说明***************/
PIXELLABSGO_API void NovelHierSimpleGlobalAggregation(float *costmatrix, int width, int height, int numLevel, int* initlabel, int wholerange,
	float p1 = 0.3, float p2 = 1.2);
#endif