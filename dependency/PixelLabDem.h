/********************************************************************
created:	2016/05/30
author:		Zhang Yanfeng
e-mail:     zhang_yanfeng_3d@foxmail.com
purpose:	This file is for DEM processing.
*********************************************************************/
#ifndef _PIXELLABDEM_H_
#define _PIXELLABDEM_H_
#include <windows.h>

#ifdef PIXELLABDEM_EXPORTS
#define PIXELLABDEM_API _declspec(dllexport)
#else

#ifndef _X64
#define PIXELLABDEM_API _declspec(dllimport)
#pragma comment(lib,"PixelLabDem.lib")
#pragma message("Automatically linking width PixelLabDem.lib")

#else

#define PIXELLABDEM_API _declspec(dllimport)
#pragma comment(lib,"PixelLabDem_x64.lib")
#pragma message("Automatically linking width PixelLabDem_x64.lib")
#endif

#endif

#define       NOVALUE                -99999

typedef struct tagPLDEMHDR
{
	double  stX;
	double  stY;	//左上角纵坐标
	double  GSD;
	int     cols;
	int     rows;
}PLDEMHDR;
class PIXELLABDEM_API CPixelLabDem
{
public:
	CPixelLabDem();
	virtual ~CPixelLabDem();
public:
	//！！！默认pZ是左上角为原点，stY是左上角纵坐标！！！
	bool Create(LPCTSTR filepath, float *pZ, int cols, int rows, double stX, double stY, double GSD, bool IsTransY = false);
	void ReSetTFW(LPCTSTR filepath, double leftX, double TopY, double GSD);
	bool LoadDem(LPCTSTR filepath);
	void Close();

	float* denoise(int *cols, int *rows, double *x0, double *y0);

	float GetDemZ(int col, int row);
	float GetZ(double x, double y);
	inline float GetMinZ() { return m_minZ; }
	inline float GetMaxZ() { return m_maxZ; }
	inline float GetMidZ() { return m_midZ; }
	inline PLDEMHDR* GetHdr() { return &m_demHdr; }
	float *GetBuf() { return m_pGrid; }

public:
	void calslope(float *slopeBuf, float scale);
	void calaspect(float *aspectBuf, float scale);
	void calcosa(float *cosaBuf, float scale, float Solar_zenith, float Solar_azimuth);
	void render(unsigned char* renderBuf, float scale, float Solar_zenith=55, float Solar_azimuth=25);
private:
	PLDEMHDR    m_demHdr;
	float*      m_pGrid;
	float       m_midZ, m_minZ, m_maxZ;

	void GetMaxMinMid();
}; 
PIXELLABDEM_API void Save2Dem(LPCTSTR filepath, float *pZ, int cols, int rows, double stX, double stY, double GSD, bool IsTransY = false);
PIXELLABDEM_API bool DemDeNoise(LPCTSTR filepath_src, LPCTSTR filepath_dst);
PIXELLABDEM_API void SetTFW(LPCTSTR filepath_dem, double leftX, double TopY, double GSD);
PIXELLABDEM_API bool Prj2LBH(LPCTSTR filepath_src, LPCTSTR filepath_dst, int CenMeri, bool IsNorth = true);
PIXELLABDEM_API bool LBH2Prj(LPCTSTR filepath_src, LPCTSTR filepath_dst, int CenMeri, bool IsNorth = true);
PIXELLABDEM_API bool CalSlope(LPCTSTR filepath_src, LPCTSTR filepath_dst, float scale);
PIXELLABDEM_API bool CalAspect(LPCTSTR filepath_src, LPCTSTR filepath_dst, float scale);
PIXELLABDEM_API bool CalCosa(LPCTSTR filepath_src, LPCTSTR filepath_dst, float scale, float Solar_zenith, float Solar_azimuth);
PIXELLABDEM_API bool RenderMap(LPCTSTR filepath_src, LPCTSTR filepath_dst, float scale, float Solar_zenith = 55, float Solar_azimuth=25);
PIXELLABDEM_API void CalDiff(LPCTSTR filepath_dem1, LPCTSTR filepath_dem2, LPCTSTR filepath_dem_dif);
PIXELLABDEM_API void CalDiffRobust(LPCTSTR filepath_ref, LPCTSTR filepath_dem1, LPCTSTR filepath_dem2, LPCTSTR filepath_dem1_dif, LPCTSTR filepath_dem2_dif, float p1 = 1, float p2 = 0);
PIXELLABDEM_API void Filter(LPCTSTR filepath_src, LPCTSTR filepath_msk, LPCTSTR filepath_dst);
#endif