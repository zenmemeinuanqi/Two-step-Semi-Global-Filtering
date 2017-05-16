//OGRBase.h
/********************************************************************
	OGRBase
	created:	2014/12/12
	author:		LX_whu 
	purpose:	This file is for OGRBase function
*********************************************************************/
#if !defined OGRBase_h__LX_whu_2014_12_12
#define OGRBase_h__LX_whu_2014_12_12

#ifdef PACKAGE_GDAL_EXPORTS
#define OGRBASE_API __declspec(dllexport)
#else
#define OGRBASE_API __declspec(dllimport)
#if defined(WIN64)  || defined(X64)

#pragma comment(lib,"package_gdal_x64.lib") 
#pragma message("Automatically linking with package_gdal_x64.lib") 

//#else
//#ifdef _DEBUG_OGRBASE_API
//#pragma comment(lib,"package_gdalD.lib") 
//#pragma message("Automatically linking with package_gdalD.lib") 
//#else
//#pragma comment(lib,"package_gdal.lib") 
//#pragma message("Automatically linking with package_gdal.lib") 
//#endif
#endif

#endif
/**** exsample
COGRDataSetBase ogr;
ogr.Open("D:\\Data\\bou2_4m\\bou2_4p.shp");

LogPrint(0, "layer num= %d", ogr.GetLayerCount());

COGRLayerBase layer;
ogr.InitLayerByIdx(layer,0);

LogPrint(0, "layer name = %s\nfeature num= %d", layer.GetName(),layer.GetFeatureCount());

COGRFeatureBase feature;
layer.ResetReading();

while ( layer.GetNextFeature(feature) )
{
LogPrint(0, "feature name= %s\n field num= %d", feature.GetName(),feature.GetFieldCount());

int iField;
for (iField = 0; iField < feature.GetFieldCount(); iField++)
{
printf("%s=", feature.GetFieldName(iField));
switch (feature.GetFieldType(iField))
{
case OFT_Int:
printf("%d,", feature.GetFieldAsInteger(iField));	break;
default:
printf("%s,", feature.GetFieldAsString(iField));	break;
}
}

LogPrint(0, "\n%d", feature.GetGeometryVerticesNum() );

double x, y, z;
while (feature.GetNextVertices(&x, &y, &z)){
LogPrint(0, "%lf\t%lf\t%lf", x, y, z);
}
}
****/
	

typedef enum
{
	wkb_Unknown = 0,         /**< unknown type, non-standard */
	wkb_Point = 1,           /**< 0-dimensional geometric object, standard WKB */
	wkb_LineString = 2,      /**< 1-dimensional geometric object with linear
							*   interpolation between Points, standard WKB */
	wkb_Polygon = 3,         /**< planar 2-dimensional geometric object defined
							*   by 1 exterior boundary and 0 or more interior
							*   boundaries, standard WKB */
	wkb_MultiPoint = 4,      /**< GeometryCollection of Points, standard WKB */
	wkb_MultiLineString = 5, /**< GeometryCollection of LineStrings, standard WKB */
	wkb_MultiPolygon = 6,    /**< GeometryCollection of Polygons, standard WKB */
	wkb_GeometryCollection = 7, /**< geometric object that is a collection of 1
								or more geometric objects, standard WKB */
	wkb_None = 100,          /**< non-standard, for pure attribute records */
	wkb_LinearRing = 101,    /**< non-standard, just for createGeometry() */
	wkb_Point25D = 0x80000001, /**< 2.5D extension as per 99-402 */
	wkb_LineString25D = 0x80000002, /**< 2.5D extension as per 99-402 */
	wkb_Polygon25D = 0x80000003, /**< 2.5D extension as per 99-402 */
	wkb_MultiPoint25D = 0x80000004, /**< 2.5D extension as per 99-402 */
	wkb_MultiLineString25D = 0x80000005, /**< 2.5D extension as per 99-402 */
	wkb_MultiPolygon25D = 0x80000006, /**< 2.5D extension as per 99-402 */
	wkb_GeometryCollection25D = 0x80000007 /**< 2.5D extension as per 99-402 */
} OGRwkbType;

typedef enum
{
	/** Simple 32bit integer */                   OFT_Int = 0,
	/** List of 32bit integers */                 OFT_IntList = 1,
	/** Double Precision floating point */        OFT_Real = 2,
	/** List of doubles */                        OFT_RealList = 3,
	/** String of ASCII chars */                  OFT_String = 4,
	/** Array of strings */                       OFT_StringList = 5,
	/** deprecated */                             OFT_WideString = 6,
	/** deprecated */                             OFT_WideStringList = 7,
	/** Raw Binary data */                        OFT_Binary = 8,
	/** Date */                                   OFT_Date = 9,
	/** Time */                                   OFT_Time = 10,
	/** Date and Time */                          OFT_DateTime = 11,
	OFT_Max = 11
} OGRFieldDataType;

class OGRBASE_API COGRFeatureBase
{
public:
	COGRFeatureBase();
	~COGRFeatureBase();

	int		GetFieldCount();
	int     GetFieldIndex(const char * pszName);
	OGRFieldDataType	GetFieldType(int idx);

	const char*			GetFieldName(int i);
	int                 GetFieldAsInteger(int i);
	double              GetFieldAsDouble(int i);
	const char         *GetFieldAsString(int i);
	const int          *GetFieldAsIntegerList(int i, int *pnCount);
	const double       *GetFieldAsDoubleList(int i, int *pnCount);
	char              **GetFieldAsStringList(int i);
	BYTE              *GetFieldAsBinary(int i, int *pnCount);
	int                 GetFieldAsDateTime(int i,
		int *pnYear, int *pnMonth, int *pnDay,
		int *pnHour, int *pnMinute, int *pnSecond,
		int *pnTZFlag);

	int                 GetFieldAsInteger(const char *pszFName);
	double              GetFieldAsDouble(const char *pszFName);
	const char         *GetFieldAsString(const char *pszFName);
	const int          *GetFieldAsIntegerList(const char *pszFName,int *pnCount);
	const double       *GetFieldAsDoubleList(const char *pszFName, int *pnCount);
	char              **GetFieldAsStringList(const char *pszFName);

	OGRwkbType			GetGeometryType();
	const char*			GetGeometryTypeToName();
	static const char*	GetGeometryTypeToName(OGRwkbType type);

	int					GetGeometryVerticesNum();
	void				ResetVerticesReading();
	bool				GetNextVertices(double* x,double* y,double* z);

	const char*			GetName();
public:
	void				Attach(void* pFeature);
	void				Detach(){ m_pOGRFeatureAttach = NULL; }
protected:
	void*				GetActive(){ return m_pOGRFeatureAttach ? m_pOGRFeatureAttach : m_pOGRFeature; }
private:
	void*	m_pOGRFeature;
	void*	m_pOGRFeatureAttach;

	void*	m_pVertice;
};

class OGRBASE_API COGRLayerBase
{
public:
	COGRLayerBase();
	~COGRLayerBase();

	void        SetSpatialFilterRect(double dfMinX, double dfMinY, double dfMaxX, double dfMaxY);
	bool		SetAttributeFilter(const char * filter);

	void		ResetReading();
	bool		GetNextFeature(COGRFeatureBase& feature);
	int			GetFeatureCount(bool bForce = true );

	const char*	GetName();
public:
	void				Attach(void* pLayer) { m_pOGRLayerAttach = pLayer; }
	void				Detach(){ m_pOGRLayerAttach = NULL; }
protected:
	void*				GetActive(){ return m_pOGRLayerAttach ? m_pOGRLayerAttach : m_pOGRLayer; }

private:
	void*	m_pOGRLayer;
	void*	m_pOGRLayerAttach;
};


class OGRBASE_API COGRDataSetBase
{
public:
	COGRDataSetBase();
	~COGRDataSetBase();

	bool	IsLoaded() { return m_pOGRSet ? true : false; }
	const char* GetFilePath() { return m_strOGRPath; }

	bool	Open(const char* lpstrPath);
	void	Close();

	int		GetLayerCount();
	bool	InitLayerByIdx(COGRLayerBase& layer, int idx);
	bool	InitLayerByName(COGRLayerBase& layer, const char* layerName);

	const char* GetLastErrorMsg();
protected:
	void	Reset();
	void	SetFilePath(const char* lpstrPath) { strcpy(m_strOGRPath, lpstrPath); }
	const char* GetDescription(const char* lpstrExt);
private:
	void*	m_pOGRSet;
	char	m_strOGRPath[512];
};

#endif // OGRBase_h__LX_whu_2014_12_12

