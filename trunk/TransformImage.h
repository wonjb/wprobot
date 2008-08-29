#pragma once
#include <vector>


class CTransformImage
{
public:
	CTransformImage(void);
	~CTransformImage(void);

	void setDC(CDC* dc);
	void setOriginImage(IplImage* image);
	void drawTransImage(int x, int y, int width, int height);

	void findSkinColor()	{ ThresholdYCbCr(); }
	void deleteNoise()		{ Morphology(CV_MOP_CLOSE); deleteHole(); }
	CvPoint findCenter();
	void findCenterDistance();
	std::vector<CvPoint> findFinger();

	void ThresholdYCbCr(int cbmin = 77, int cbmax = 127, int crmin = 133, int crmax = 173);
	void Labeling();
	void Morphology(int type);
	void deleteHole();
	void drawHandLine();

	BOOL CurToPast(CvPoint* past, CvPoint* cur) { *past = *cur;	return TRUE; }
	
//	POINT getHandPoint();

private:
	CDC* m_pDC;
	IplImage* m_image;
	IplImage* m_transImage;

	CvPoint m_center;	
};
