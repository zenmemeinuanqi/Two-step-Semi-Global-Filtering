//rpcbase.h
/********************************************************************
	rpcbase
	created:	2014/11/02
	author:		LX_whu 
	purpose:	This file is for rpcbase function
*********************************************************************/
#if !defined rpcbase_h__LX_whu_2014_11_2
#define rpcbase_h__LX_whu_2014_11_2

#ifdef PACKAGE_GDAL_EXPORTS
#define RPCBASE_API __declspec(dllexport)
#else
#define RPCBASE_API __declspec(dllimport)
#if defined(WIN64)  || defined(X64)

#pragma comment(lib,"package_gdal_x64.lib") 
#pragma message("Automatically linking with package_gdal_x64.lib") 

//#else
//#ifdef _DEBUG_RPCBASE_API
//#pragma comment(lib,"package_gdalD.lib") 
//#pragma message("Automatically linking with package_gdalD.lib") 
//#else
//#pragma comment(lib,"package_gdal.lib") 
//#pragma message("Automatically linking with package_gdal.lib") 
//#endif
#endif

#endif

#ifndef _RpcPara
#define _RpcPara
typedef struct tagRpcPara
{
	double line_off;     // line offset in pixels
	double samp_off;     // sample offset in pixels
	double lat_off;      // latitude offset in degrees
	double long_off;     // longitude offset in degrees
	double height_off;   // height offset in meters
	double line_scale;   // line scale in pixels
	double samp_scale;   // sample scale in pixels
	double lat_scale;    // latitude scale in degrees
	double long_scale;   // longitude scale in degrees
	double height_scale; // height scale in meters
	double c[20];        // 20 line numerator coefficients
	double d[20];        // 20 line denominator coefficients
	double a[20];        // 20 sample numerator coefficients
	double b[20];        // 20 sample denominator coefficients

}RpcPara;
#endif

#define M_LOWORD(l)           ((unsigned short)(l))
#define M_HIWORD(l)           ((unsigned short)(((unsigned int)(l) >> 16) & 0xFFFF))

#ifndef _DXDY
#define _DXDY
typedef struct tagDXDY
{
	short dx, dy;
}DXDY;
#endif

class RPCBASE_API CRpcBase
{
public:
	CRpcBase();
	~CRpcBase();

	void SetAop6(const double aop6[6]) { memcpy(m_aop6, aop6, 6 * sizeof(double)); }

	bool Load4File(const char* lpstrPathName);

	bool Save2File(const char* lpstrPathName, double *sx = NULL, double *sy = NULL, double *mx = NULL, double *my = NULL);

	void Calc_rpc_78(const double* samp, const  double* line, const  double* lat, const double* lon, const  double* alt, int num,
		double *sx = NULL, double *sy = NULL, double *mx = NULL, double *my = NULL);

	void RPCGrd2Pho(double lat, double lon, double alt, double *px, double *py);
	void RPCGrd2Pho(double lat, double lon, double alt, double *x, double *y, bool bAop6)
	{
		RPCGrd2Pho(lat, lon, alt, x, y);
		if (bAop6) {  RPC_pxy_to_ixy(*x, *y, x, y); }
	}
	void RPCGrd2Pho(const double* lat, const double* lon, double* alt, int nPtNum, double *px, double *py);
	void RPCGrd2Pho(const double* lat, const double* lon, double* alt, int nPtNum, double *x, double *y, bool bAop6)
	{
		RPCGrd2Pho(lat, lon, alt, nPtNum, x, y);
		if (bAop6){
			for (int i = 0; i < nPtNum; i++){ RPC_pxy_to_ixy(*x, *y, x, y); x++; y++; }
		}
	}

	void RPCPhoZ2Grd(double px, double py, double gz, double *lat, double *lon, double *alt);
	void RPCPhoZ2Grd(double x, double y, double gz, double *lat, double *lon, double *alt, bool bAop6)
	{
		if (bAop6) { RPC_ixy_to_pxy(x, y, &x, &y); }
		RPCPhoZ2Grd(x, y, gz, lat, lon, alt);
	}
	void RPCPhoZ2Grd(const double* px, const  double* py, const  double* gz,int nPtNum, double *lat, double *lon, double *alt);
	void RPCPhoZ2Grd(const double* x, const  double* y, const double* gz, int nPtNum, double *lat, double *lon, double *alt, bool bAop6);

	void GetRpcPara(RpcPara* para);

	bool AttachDemFile(const char* lpstrPathName);

	void RPC_pxy_to_ixy(double px, double py, double *ix, double *iy){ RPC_pxy_to_ixy(px, py, ix, iy, m_aop6); };
	void RPC_ixy_to_pxy(double ix, double iy, double *px, double *py){ RPC_ixy_to_pxy(ix, iy, px, py, m_aop6); };

	static void RPC_pxy_to_ixy(double px, double py, double *ix, double *iy, double *ab6){
		*ix = px + ab6[3] + ab6[4] * px + ab6[5] * py;
		*iy = py + ab6[0] + ab6[1] * px + ab6[2] * py;
	}
	static void RPC_ixy_to_pxy(double ix, double iy, double *px, double *py, double *ab6){
		double t = 1 + ab6[2] + ab6[4] + ab6[4] * ab6[2] - ab6[5] * ab6[1];
		*px = (ix - ab6[3] + ab6[2] * ix - ab6[2] * ab6[3] - ab6[5] * iy + ab6[5] * ab6[0]) / t;
		*py = (iy - ab6[1] * ix + ab6[1] * ab6[3] - ab6[0] + ab6[4] * iy - ab6[4] * ab6[0]) / t;
	}
protected:
	void Reset();

	int*    m_pDxyGrid; // It saved value is: dx= (realx-rpcx)*1000 , dy=(realy-rpcy)*1000;
	int     m_gridC, m_gridR, m_gridDx, m_gridDy;
	int*    NewGrid(int gridC, int gridR, int dx, int dy){
		m_gridC = gridC; m_gridR = gridR;
		m_gridDx = dx;    m_gridDy = dy;
		if (m_pDxyGrid) delete m_pDxyGrid;
		m_pDxyGrid = new int[m_gridC*m_gridR + 8];
		memset(m_pDxyGrid, 0, sizeof(int)*m_gridC*m_gridR);
		return m_pDxyGrid;
	};
	void    GetDxy(double px, double py, double *dx, double *dy){
		float   x0 = (float)(px / m_gridDx);
		float   y0 = (float)(py / m_gridDy);
		int     lbGridx = int(x0);
		int     lbGridy = int(y0);
		float   dx0 = (x0 - lbGridx);
		float   dy0 = (y0 - lbGridy);
		*dx = *dy = 0;
		if (lbGridx >= 0 && lbGridx < m_gridC - 1 &&
			lbGridy >= 0 && lbGridy < m_gridR - 1){

			float z00, z01, z10, z11;
			int lbOffset = lbGridy * m_gridC + lbGridx;
			int ltOffset = lbOffset + m_gridC;
			DXDY* pDxy = (DXDY*)m_pDxyGrid;

			z00 = pDxy[lbOffset].dx / 1000.f;
			z01 = pDxy[lbOffset + 1].dx / 1000.f;
			z10 = pDxy[ltOffset].dx / 1000.f;
			z11 = pDxy[ltOffset + 1].dx / 1000.f;
			z00 += dx0*(z01 - z00);
			z10 += dx0*(z11 - z10);
			*dx = float(z00 + dy0*(z10 - z00));

			z00 = pDxy[lbOffset].dy / 1000.f;
			z01 = pDxy[lbOffset + 1].dy / 1000.f;
			z10 = pDxy[ltOffset].dy / 1000.f;
			z11 = pDxy[ltOffset + 1].dy / 1000.f;
			z00 += dx0*(z01 - z00);
			z10 += dx0*(z11 - z10);
			*dy = float(z00 + dy0*(z10 - z00));
		}
	};
private:
	class CRpc* m_rpc;
	double		m_aop6[6];
};

void RPCBASE_API RPCPho2Grd(CRpcBase *pRpcL, double pxl, double pyl,
	CRpcBase *pRpcR, double pxr, double pyr,
	double *lat, double *lon, double *alt);

#endif // rpcbase_h__LX_whu_2014_11_2
