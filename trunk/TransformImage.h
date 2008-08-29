#pragma once
#include "HandPoint.h"


class CTransformImage
{
public:
	CTransformImage(void);
	~CTransformImage(void);

	void setDC(CDC* dc);
	void setOriginImage(IplImage* image);
	void drawTransImage(int x, int y, int width, int height);

	void findSkinColor()	{ ThresholdYCbCr(); }
	void deleteNoise()		{ Morphology(); /*deleteHole();*/ Labeling(); }
	CvPoint findCenter();
	CHandPoint findFinger();

	void ThresholdYCbCr(int cbmin = 77, int cbmax = 127, int crmin = 133, int crmax = 173);
	void Labeling();
	void Morphology();
	void deleteHole();
	void drawHandLine();

//	BOOL CurToPast(CvPoint* past, CvPoint* cur) { *past = *cur;	return TRUE; }

private:
	CDC* m_pDC;
	IplImage* m_image;
	IplImage* m_transImage;

	CvPoint m_center;
	CvPoint m_pastPt;
};
