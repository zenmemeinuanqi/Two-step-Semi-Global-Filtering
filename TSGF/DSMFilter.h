/********************************************************************
created:	2016/6/12
author:		Zhang Yanfeng
e-mail:     zhang_yanfeng_3d@foxmail.com
purpose:	This file is for FILTERINGDSM2DTM
*********************************************************************/
#ifndef FILTERINGDSM2DTM
#define FILTERINGDSM2DTM

#include <windows.h>

class CDSMFilter
{
public:
	CDSMFilter();
	~CDSMFilter();

	bool Open(LPCTSTR filepath_dsm, LPCTSTR filepath_msk);
	bool filtering();
	bool Save(LPCTSTR filepath_dtm);
	void Close();
protected:
	void preprocess();
	void calculateCostmatrix();
	void semiglobalFiltering();
	void WTA();
	void classify();

	void calgamma();
private:
	//input
	int m_width;
	int m_height;
	float *m_buf_DSM;
	unsigned char* m_buf_msk;

	//temporal vairances
	int m_level;
	float m_minHei[10000];
	float m_stepHei[10000];
	int* m_buf_segflag;
	float *m_gamma;
	float *m_costmatrix;

	//output
	float *m_buf_DTM;
	unsigned char* m_buf_flag;
};

bool DSM2DTM(LPCTSTR filepath_dsm, LPCTSTR filepath_cs, LPCTSTR filepath_msk = NULL);

#endif