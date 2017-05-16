#ifndef _SLOPEFILETR_H_
#define _SLOPEFILETR_H_

#include <windows.h>
class CSlopefilter
{
public:
	CSlopefilter();
	~CSlopefilter();

	bool Open(LPCTSTR filepath_slope);
	bool filtering();
	bool Save(LPCTSTR filepath_slope_flt);
	void Close();
protected:
	void calculateCostmatrix();
	void semiglobalFiltering();
	void WTA();
private:
	int m_width;
	int m_height;
	int m_level;
	float m_stepSlope;
	float m_minSlope;
	float *m_buf_slope;
	float *m_costmatrix;
	float *m_result_f;
};
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
//Semi-global filtering of a slope map.
void slopemapSGF(LPCTSTR filepath_slope, LPCTSTR filepath_slope_sgf);
//Given a slope map(semi-globally filtered slope map), this function outputs a flat terrain mask.
void calFlatMsk(LPCTSTR filepath_slope, LPCTSTR filepath_msk, float threshold = 4.0, bool isSeg = true);
//Calculate the scale difference between two DSMs (or slope maps).
void calScale(LPCTSTR filepath_src, LPCTSTR filepath_dst, float &scaleX, float &scaleY);
#endif // !_SLOPEFILETR_H_