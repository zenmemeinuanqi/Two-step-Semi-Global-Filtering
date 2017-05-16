/********************************************************************
created:	2016/04/01
author:		Zhang Yanfeng
e-mail:     zhang_yanfeng_3d@foxmail.com
purpose:	This file is for Segmentation of Image(RGB, Gray, float).
*********************************************************************/
#ifndef _PIXELLABSEGMENTATION_H_
#define _PIXELLABSEGMENTATION_H_
#include <windows.h>

#ifdef PIXELLABSEGMENTATION_EXPORTS
#define PIXELLABSEGMENTATION_API _declspec(dllexport)
#else

#ifndef _X64
#define PIXELLABSEGMENTATION_API _declspec(dllimport)
#pragma comment(lib,"PixelLabSegmentation.lib")
#pragma message("Automatically linking width PixelLabSegmentation.lib")

#else

#define PIXELLABSEGMENTATION_API _declspec(dllimport)
#pragma comment(lib,"PixelLabSegmentation_x64.lib")
#pragma message("Automatically linking width PixelLabSegmentation_x64.lib")
#endif

#endif

PIXELLABSEGMENTATION_API int segmentation(float* buf, int width, int height, int *label, float p = 1.0);
PIXELLABSEGMENTATION_API int segmentation(unsigned char* buf, int width, int height, int *label, int p = 1);
PIXELLABSEGMENTATION_API int segmentation3C(unsigned char* buf, int width, int height, int *label);
PIXELLABSEGMENTATION_API int oversegmentation(unsigned char* buf, int width, int height, int *label, int spcount = 100);

#endif