#include "DSMFilter.h"
#include "package_gdal.h"
#include "PixelLabSGO.h"
#include "PixelLabSpeckleChecker.h"
#include <iostream>
using namespace std;
#define DELTZ 1
static void GetMin(float *val, int num, float *minval, int *index)
{
	float mintemp = 999999.0f;
	int tempindex = 0;
	for (int i = 0; i < num; i++){
		if (val[i] < mintemp){
			mintemp = val[i];
			tempindex = i;
		}
	}
	*minval = mintemp;
	*index = tempindex;
}
CDSMFilter::CDSMFilter()
{
}
CDSMFilter::~CDSMFilter()
{
}
bool CDSMFilter::Open(LPCTSTR filepath_dsm, LPCTSTR filepath_msk)
{
	CImageBase Img, ImgMsk;
	if (!Img.Open(filepath_dsm) || !ImgMsk.Open(filepath_msk)) return false;
	m_width = Img.GetCols();
	m_height = Img.GetRows();
	m_buf_DSM = new float[m_width*m_height];
	Img.Read(m_buf_DSM, 0, 0, m_width, m_height);
	Img.Close();
	int t = 0;
	for (int i = 0; i < m_width*m_height; i++) {
		if (m_buf_DSM[i] > -1000) t++;
	}
	if (t < 10) {
		delete[] m_buf_DSM;  return false;
	}

	m_level = 30;
	
	if (filepath_msk == NULL) {
		delete[] m_buf_DSM;  return false;
	}

	m_buf_msk = new unsigned char[m_width*m_height];
	ImgMsk.ReadGray8(m_buf_msk, 0, 0, m_width, m_height);
	ImgMsk.Close();

	m_buf_DTM = new float[m_width*m_height];
	m_gamma = new float[m_width*m_height];
	m_costmatrix = new float[m_width*m_height*m_level];
	m_buf_flag = new unsigned char[m_width*m_height];
	m_buf_segflag = new int[m_width*m_height];
	
	return true;
}
void CDSMFilter::Close()
{
	if (m_buf_DSM) delete[] m_buf_DSM;
	if (m_costmatrix) delete[] m_costmatrix;
	if (m_buf_DTM) delete[] m_buf_DTM;
	if (m_buf_msk) delete[] m_buf_msk; 
	if (m_buf_flag) delete[] m_buf_flag;
	if (m_gamma) delete[] m_gamma;
	if (m_buf_segflag) delete[] m_buf_segflag;
}
bool CDSMFilter::Save(LPCTSTR filepath_dtm)
{
	CImageBase ImgF;
	if (!ImgF.Create(filepath_dtm, m_width, m_height, 1, 32)) return false;
	if (!ImgF.Write(m_buf_DTM, 0, 0, m_width, m_height)) return false;

	char filepath_flag[FILENAME_MAX];
	strcpy(filepath_flag, filepath_dtm);
	strcpy(strrchr(filepath_flag, '.'), "_flag.tif");
	CImageBase Img;
	if (!Img.Create(filepath_flag, m_width, m_height,1)) return false;
	if (!Img.Write(m_buf_flag, 0, 0, m_width, m_height)) return false;

	return true;
}
bool CDSMFilter::filtering()
{
	printf("CDSMFilter::filtering..preprocessing..\n");
	preprocess();
	
	printf("CDSMFilter::filtering..calculate costmatrix..\n");
	calculateCostmatrix();
	
	printf("CDSMFilter::filtering..semiglobalFiltering..\n");
	semiglobalFiltering();
	
	printf("CDSMFilter::filtering..WTA..\n");
	WTA();
	
	printf("CDSMFilter::filtering..classify..\n");
	classify();
	return true;
}
void CDSMFilter::preprocess()
{
	for (int i = 0; i < m_width*m_height; i++) {
		m_buf_segflag[i] = m_buf_msk[i];
	}
	float _maxZ[10000];
	int num = Extract4Segmentation(m_buf_DSM, m_minHei, _maxZ, m_buf_segflag, m_width, m_height);

	for (int i = 0; i < num; i++) 
		m_stepHei[i] = (_maxZ[i] - m_minHei[i]) / (m_level - 1);
	calgamma();
}
void CDSMFilter::calgamma()
{
	
	memset(m_gamma, 0, sizeof(float)*m_width*m_height);
	for (int i = 0; i < m_width*m_height;i++){
		int idx = m_buf_segflag[i];
		if (idx == -1) continue;
		if (m_stepHei[idx] == 0) continue;
		m_gamma[i] = 0.5*exp((m_minHei[idx] - m_buf_DSM[i]) / (m_stepHei[idx]*m_level));
	}
}
void CDSMFilter::calculateCostmatrix()
{
	float *buf_minval = new float[m_width*m_height];
	float _minval[49];
	float minval;
	int idx;
	for (int i = 3; i < m_height - 3;i++){
		for (int j = 3; j < m_width - 3; j++){
			int t = 0;
			for (int m = -3; m <= 3;m++){
				for (int n = -3; n <= 3;n++){
					_minval[t++] = m_buf_DSM[(i + m)*m_width + j + n];
				}
			}
			GetMin(_minval, 49, &minval, &idx);
			if (minval < -1000) {
				buf_minval[i*m_width + j] = m_buf_DSM[i*m_width + j]; continue;
			}
			buf_minval[i*m_width + j] = minval;
		}
	}
	memset(m_costmatrix, 0, sizeof(float)*m_width*m_height*m_level);
	for (int i = 0; i < m_width*m_height; i++){
		if (m_buf_DSM[i] < -2000) continue;
		if (m_buf_msk[i] == 0 || m_buf_segflag[i] == -1) continue;
		int idx = m_buf_segflag[i];
		for (int l = 0; l < m_level; l++){
			float _DeltLabel = (m_buf_DSM[i] - (m_minHei[idx] + l*m_stepHei[idx])) / m_stepHei[idx];
			if (_DeltLabel < -1){
				m_costmatrix[i*m_level + l] = 999;
			}
			else{
				_DeltLabel = fabs(buf_minval[i] - (m_minHei[idx] + l*m_stepHei[idx])) / m_stepHei[idx];
				_DeltLabel = _DeltLabel < 10 ? _DeltLabel : 10;
				m_costmatrix[i*m_level + l] = _DeltLabel / 30;// exp(0.1*_DeltLabel) - 1;
			}
		}
	}
	delete[] buf_minval;
}
void CDSMFilter::semiglobalFiltering()
{
	SegmentConstrainedSemiGlobalAggregation(m_costmatrix, m_buf_segflag, m_gamma, m_width, m_height, m_level,0.3, 6);
}
void CDSMFilter::WTA()
{
	float tempdisp[10000];
	int tempi; float tempf;
	for (int i = 0; i < m_width*m_height; i++){
		if (m_buf_msk[i] == 0 || m_buf_segflag[i] == -1 || m_buf_DSM[i] < -1000) {
			m_buf_DTM[i] = m_buf_DSM[i];
			continue;
		}
		int idx = m_buf_segflag[i];
		for (int l = 0; l < m_level; l++){
			tempdisp[l] = m_costmatrix[i*m_level + l];
		}
		GetMin(tempdisp, m_level, &tempf, &tempi);

		float _Z = m_minHei[idx] + tempi*m_stepHei[idx];
		m_buf_DTM[i] = _Z;
	}
}
void CDSMFilter::classify()
{
	memset(m_buf_flag, 0, sizeof(unsigned char)*m_width*m_height);
	for (int i = 0; i < m_width*m_height; i++){
		if (m_buf_msk[i] == 0 || m_buf_segflag[i] == -1) {
			m_buf_flag[i] = 255; continue;
		}
		int idx = m_buf_segflag[i];
		float _delt = m_buf_DSM[i] - m_buf_DTM[i];
		if (_delt > m_stepHei[idx]){
			m_buf_flag[i] = 0;
		}
		else{
			m_buf_flag[i] = 255;
		}
	}
}

bool DSM2DTM(LPCTSTR filepath_dsm, LPCTSTR filepath_cs, LPCTSTR filepath_msk)
{
	CDSMFilter filter;
	if (!filter.Open(filepath_dsm, filepath_msk)) return false;
	if (!filter.filtering()) { filter.Close(); return false; }
	if (!filter.Save(filepath_cs)) { filter.Close(); return false; }
	filter.Close();
	return true;
}