// TSGF.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DSMFilter.h"
#include "SlopeFilter.h"
#include "PixelLabDem.h"
void testSlopeMapSGF()
{
	slopemapSGF("TestData\\slopemap.tif","TestData\\slopemap_sgf.tif");
	slopemapSGF("TestData\\slopemap_srtm.tif", "TestData\\slopemap_srtm_sgf.tif");
}
void testFlatTerrainMask()
{
	calFlatMsk("TestData\\slopemap_sgf.tif","TestData\\mask.tif",4,true);
	calFlatMsk("TestData\\slopemap_srtm_sgf.tif", "TestData\\mask_srtm.tif", 4, true);
}
void testSegmentationConstrainedSGF()
{
	DSM2DTM("TestData\\dsm.tif","TestData\\cs.tif","TestData\\mask.tif");
	DSM2DTM("TestData\\dsm.tif", "TestData\\cs2.tif", "TestData\\mask2.tif");
}
int _tmain(int argc, _TCHAR* argv[])
{
	testSlopeMapSGF();
	testFlatTerrainMask();
	testSegmentationConstrainedSGF();
	return 0;
}