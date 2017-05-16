//GeoTransfer.h
/********************************************************************
	GeoTransfer
	created:	2014/11/02
	author:		LX_whu 
	purpose:	This file is for GeoTransfer function
*********************************************************************/
#if !defined GeoTransfer_h__LX_whu_2014_11_2
#define GeoTransfer_h__LX_whu_2014_11_2

#ifdef PACKAGE_GDAL_EXPORTS
#define GEOTRANSFER_API __declspec(dllexport)
#else
#define GEOTRANSFER_API __declspec(dllimport)
#if defined(WIN64)  || defined(X64)

#pragma comment(lib,"package_gdal_x64.lib") 
#pragma message("Automatically linking with package_gdal_x64.lib") 

//#else
//#ifdef _DEBUG_GEOTRANSFER_API
//#pragma comment(lib,"package_gdalD.lib") 
//#pragma message("Automatically linking with package_gdalD.lib") 
//#else
//#pragma comment(lib,"package_gdal.lib") 
//#pragma message("Automatically linking with package_gdal.lib") 
//#endif
#endif

#endif

class GEOTRANSFER_API CGeoTransferBase
{
public:
	CGeoTransferBase();
	virtual ~CGeoTransferBase();

	bool		Import4WKT(const char* pszWkt);
	bool		Import4GCD(const char* pszGcdPath);
	bool		Export2GCD(const char* pszGcdPath);
	const char* GetWKT();

	bool		SetUTM_Zone(int nZone, bool bNorth = true);
	bool		SetUTM_CenMeri(float lon, bool bNorth = true);

	bool    Cvt2LBH(int nCount,
		double *x, double *y, double *z = NULL,
		int *pabSuccess = NULL);
	bool    Prj2LBH(int nCount,
		double *x, double *y, double *z = NULL,
		int *pabSuccess = NULL);
	bool    LBH2Prj(int nCount,
		double *x, double *y, double *z = NULL,
		int *pabSuccess = NULL);
	bool    LBH2CXYZ(int nCount,
		double *x, double *y, double *z = NULL,
		int *pabSuccess = NULL);
	bool    CXYZ2LBH(int nCount,
		double *x, double *y, double *z = NULL,
		int *pabSuccess = NULL);

	bool	Prj2LBH(float* x, float* y, float* z = NULL);
	bool	LBH2Prj(float* x, float* y, float* z = NULL);

	bool        IsGeographic() const;
	bool        IsProjected() const;
	bool        IsGeocentric() const;

	bool		IsSameSpatialReference(CGeoTransferBase* sr);
	bool		IsSameGeogCS(CGeoTransferBase* sr);
	bool		IsSameVertCS(CGeoTransferBase* sr);
	
private:
	class CGeoTransfer* m_pGeoTransfer;
public:
	const CGeoTransfer*	GetInteriorPtr() { return m_pGeoTransfer; }
};

class GEOTRANSFER_API CCoordinationTransferBase{
public:
	CCoordinationTransferBase();
	virtual ~CCoordinationTransferBase();
	bool	init(CGeoTransferBase& src, CGeoTransferBase& dst);
	bool    Src2Dst(int nCount,
		double *x, double *y, double *z = NULL,
		int *pabSuccess = NULL );
	bool    Dst2Src(int nCount,
		double *x, double *y, double *z = NULL,
		int *pabSuccess = NULL);
protected:
	class CCoordinationTransfer* m_pCoordTransfer;
};
#endif // GeoTransfer_h__LX_whu_2014_11_2

