#include "SlopeFilter.h"
#include "PixelLabDem.h"
#include "package_gdal.h"
#include "PixelLabSGO.h"
#include <iostream>
using namespace std;
CSlopefilter::CSlopefilter()
{
}

CSlopefilter::~CSlopefilter()
{
}
bool CSlopefilter::Open(LPCTSTR filepath_slope)
{
	CImageBase Img;
	if (!Img.Open(filepath_slope)) return false;
	m_width = Img.GetCols();
	m_height = Img.GetRows();
	m_buf_slope = new float[m_width*m_height];
	if (!Img.Read(m_buf_slope, 0, 0, m_width, m_height)) {
		delete[] m_buf_slope;
		return false;
	}
	Img.Close();
	
	float minS = 90, maxS = 0;
	for (int i = 0; i < m_width*m_height; i++){
		if (m_buf_slope[i] == NOVALUE) continue;
		if (m_buf_slope[i] < minS) minS = m_buf_slope[i];
		if (m_buf_slope[i] > maxS) maxS = m_buf_slope[i];
	}
	m_stepSlope = 1;
	m_level = (maxS - minS) / m_stepSlope + 1;
	m_minSlope = minS;

	m_costmatrix = new float[m_width*m_height*m_level];
	m_result_f = new float[m_width*m_height];
	return true;
}
void CSlopefilter::Close()
{
	if (m_buf_slope) delete[] m_buf_slope;
	if (m_costmatrix) delete[] m_costmatrix;
	if (m_result_f) delete[] m_result_f;
}
bool CSlopefilter::Save(LPCTSTR filepath_slope_flt)
{
	CImageBase ImgF;
	if (!ImgF.Create(filepath_slope_flt, m_width, m_height, 1, 32)) return false;
	if (!ImgF.Write(m_result_f, 0, 0, m_width, m_height)) return false;

	return true;
}
bool CSlopefilter::filtering()
{
	printf("CSlopefilter::filtering..calculate costmatrix..\n");
	calculateCostmatrix();

	printf("CSlopefilter::filtering..semiglobalFiltering..\n");
	semiglobalFiltering();
	
	printf("CSlopefilter::filtering..WTA..\n");
	WTA();
	return true;
}
void CSlopefilter::calculateCostmatrix()
{
	memset(m_costmatrix, 0, sizeof(float)*m_width*m_height*m_level);
	for (int i = 0; i < m_width*m_height; i++){
		if (m_buf_slope[i] == NOVALUE) continue;
		for (int l = 0; l < m_level; l++){
			float _slope = m_minSlope + l*m_stepSlope;
			m_costmatrix[i*m_level + l] = fabs(m_buf_slope[i] - _slope) / 90;
		}
	}
}
void CSlopefilter::semiglobalFiltering()
{
	SimpleGlobalAggregation(m_costmatrix, m_width, m_height, m_level,0.1,1.2);
}
void CSlopefilter::WTA()
{
	int dispnum = m_level;
	float tempdisp[10000];
	memset(m_result_f, 0, sizeof(float)*m_width*m_height);
	int tempi; float tempf;
	for (int i = 0; i < m_width*m_height; i++){
		if (m_buf_slope[i] == NOVALUE) {
			m_result_f[i] = NOVALUE;
			continue; 
		}
		for (int d = 0; d < dispnum; d++){
			tempdisp[d] = m_costmatrix[i*dispnum + d];
		}
		GetMin(tempdisp, dispnum, &tempf, &tempi);
		float _slope = m_minSlope + tempi*m_stepSlope;
		m_result_f[i] = _slope;
	}
}
void slopemapSGF(LPCTSTR filepath_slope, LPCTSTR filepath_slope_sgf)
{
	CSlopefilter sf;
	if (!sf.Open(filepath_slope)) return;
	if (!sf.filtering()) { sf.Close();  return;}
	if (!sf.Save(filepath_slope_sgf)) { sf.Close();  return;}
	sf.Close();
}
#include "PixelLabSpeckleChecker.h"
#include "PixelLabSegmentation.h"
void calFlatMsk(LPCTSTR filepath_slope, LPCTSTR filepath_msk, float threshold, bool isSeg)
{
	CImageBase ImgSlp;
	if(!ImgSlp.Open(filepath_slope)) return;
	int width = ImgSlp.GetCols();
	int height = ImgSlp.GetRows();
	int nBits = ImgSlp.GetBits();
	if (32 != nBits) return;
	float *bufSlp = new float[width*height];
	ImgSlp.Read(bufSlp, 0, 0, width, height); ImgSlp.Close();
	unsigned char* bufMsk = new unsigned char[width*height];
	memset(bufMsk, 0, sizeof(unsigned char)*width*height);
	for (int i = 0; i < width*height;i++){
		if (bufSlp[i] < -1000)
		{
			bufMsk[i] = 0;
			continue;
		}
		if (bufSlp[i] < threshold)
		{
			bufMsk[i] = 255;
		}
	}
	BinaryRemoveSpeckle(bufMsk, width, height,100);
	//如果需要，则对平坦区域掩膜的超像素分割
	if (isSeg)
	{
		int *buf_segflag = new int[width*height];
		int num = oversegmentation(bufMsk, width, height, buf_segflag, 20);
		for (int i = 0; i < width*height; i++) {
			if (bufMsk[i] == 0) continue;
			if (buf_segflag[i] == 0) { bufMsk[i] = 200; continue; }
			bufMsk[i] = buf_segflag[i] % 200;
		}
		delete[] buf_segflag;
	}

	CImageBase ImgMsk;
	ImgMsk.Create(filepath_msk, width, height, 1);
	ImgMsk.Write(bufMsk, 0, 0, width, height);
	ImgMsk.Close();

	delete[] bufSlp;
	delete[] bufMsk;
}

void calScale(LPCTSTR filepath_src, LPCTSTR filepath_dst, float &scaleX, float &scaleY)
{
	CImageBase ImgSrc;
	if(!ImgSrc.Open(filepath_src)) return;
	int widthSrc = ImgSrc.GetCols();
	int heightSrc = ImgSrc.GetRows();
	ImgSrc.Close();
	CImageBase ImgDst;
	if (!ImgDst.Open(filepath_dst)) return;
	int widthDst = ImgDst.GetCols();
	int heightDst = ImgDst.GetRows();
	ImgDst.Close();
	scaleX = (float)widthDst / widthSrc;
	scaleY = (float)heightDst / heightSrc;
}