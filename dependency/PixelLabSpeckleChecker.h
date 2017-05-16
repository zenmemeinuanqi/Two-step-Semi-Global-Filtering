/********************************************************************
created:	2016/03/20
author:		Zhang Yanfeng
e-mail:     zhang_yanfeng_3d@foxmail.com
purpose:	This file is for check speckle
*********************************************************************/
#ifndef _PIXELLABSPECKLECHECKER_H_
#define _PIXELLABSPECKLECHECKER_H_
#include <windows.h>

#ifdef PIXELLABSPECKLECHECKER_EXPORTS
#define PIXELLABSPECKLECHECKER_API _declspec(dllexport)
#else

#ifndef _X64
#define PIXELLABSPECKLECHECKER_API _declspec(dllimport)
#pragma comment(lib,"PixelLabSpeckleChecker.lib")
#pragma message("Automatically linking width PixelLabSpeckleChecker.lib")

#else

#define PIXELLABSPECKLECHECKER_API _declspec(dllimport)
#pragma comment(lib,"PixelLabSpeckleChecker_x64.lib")
#pragma message("Automatically linking width PixelLabSpeckleChecker_x64.lib")
#endif

#endif
/*****************函数说明***************/
//函数名：BinaryRemoveSpeckle
//功能：对输入二值图像去除碎片
//参数说明：
//buf：输入二值图像数据
//width：DSM的宽度
//height：DSM的高度
//p：移除碎片大小，包含像素数目，小于该值的碎片将被移除
/*****************函数说明***************/
PIXELLABSPECKLECHECKER_API void BinaryRemoveSpeckle(unsigned char* buf, int width, int height, int p = 100, int type = 1);

/*****************函数说明***************/
//函数名：RemoveSpeckle
//功能：对输入浮点型图像去除碎片
//参数说明：
//buf：输入浮点型图像数据
//width：DSM的宽度
//height：DSM的高度
//p1：连通域值
//p2：移除碎片大小，包含像素数目，小于该值的碎片将被移除
/*****************函数说明***************/
PIXELLABSPECKLECHECKER_API void RemoveSpeckle(float* buf, int width, int height, float p1 = 1.0, int p2 = 100);

/*****************函数说明***************/
//函数名：RegionGrow
//功能：对输入浮点型图像根据种子点进行生长
//参数说明：
//buf：输入浮点型图像数据
//width：DSM的宽度
//height：DSM的高度
//p1：连通域值1，向下
//p2：连通阈值2，向上
/*****************函数说明***************/
PIXELLABSPECKLECHECKER_API void RegionGrow(float* buf, unsigned char *mask, int width, int height, float p1 = -1.0, float p2 = 1.0);

/*****************函数说明***************/
//函数名：Cluster4Segmentation
//功能：对输入的分割图像进行聚类，返回类别数
//参数说明：
//seg：输入分割图像
//width：图像宽度
//height：图像高度
//num_cluster：类别包含元素数，num_cluster[i]表示第i个类别包含的元素数目，需要事先申请内存
//x_cluster：聚类结果之x坐标，按聚类结果顺序存储，需要事先申请内存
//y_cluster：聚类结果之y坐标，按聚类结果顺序存储，需要事先申请内存
//p：聚类元素上限
/*****************函数说明***************/
PIXELLABSPECKLECHECKER_API int Cluster4Segmentation(int* seg, int width, int height, int *num_cluster, int *x_cluster, int *y_cluster, int p = 9999999);

/*****************函数说明***************/
//函数名：Segmentation
//功能：对输入的灰度图像进行分割聚类，返回类别数
//参数说明：
//seg：输入灰度图像
//width：图像宽度
//height：图像高度
//num_cluster：类别包含元素数，num_cluster[i]表示第i个类别包含的元素数目，需要事先申请内存
//x_cluster：聚类结果之x坐标，按聚类结果顺序存储，需要事先申请内存
//y_cluster：聚类结果之y坐标，按聚类结果顺序存储，需要事先申请内存
//p：聚类元素上限
/*****************函数说明***************/
PIXELLABSPECKLECHECKER_API int Segmentation(unsigned char* buf, int width, int height, int *num_cluster, int *x_cluster, int *y_cluster, int p = 9999999);

/*****************函数说明***************/
//函数名：Extract4Segmentation
//功能：对输入的浮点型图像，在分割图像结果的引导下，生成分割块最大最小值
//参数说明：
//buf：输入待提取信息浮点型图像
//min：输出分割块最小值
//max：输出分割块最大值
//seg：输入分割图像
//width：图像宽度
//height：图像高度
//p1：分割块元素数下限
//p2：分割块元素数上限
/*****************函数说明***************/
PIXELLABSPECKLECHECKER_API int Extract4Segmentation(float* buf, float* min, float *max, int* seg, int width, int height, int p1 = 80, int p2 = 9999999);

/*****************函数说明***************/
//函数名：Extract4Segmentation
//功能：对输入的整型图像，在分割图像结果的引导下，生成分割块最大最小值
//参数说明：
//buf：输入待提取信息int型图像
//min：输出分割块最小值
//max：输出分割块最大值
//seg：输入分割图像
//width：图像宽度
//height：图像高度
//p1：分割块元素数下限
//p2：分割块元素数上限
/*****************函数说明***************/
PIXELLABSPECKLECHECKER_API int Extract4Segmentation(int* buf, int* min, int *max, int* seg, int width, int height, int p1 = 80, int p2 = 9999999);
#endif