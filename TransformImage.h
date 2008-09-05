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
	CHandPoint findFingerInfo();

	void Tracking();

private:
	void ThresholdYCbCr(int cbmin = 77, int cbmax = 127, int crmin = 133, int crmax = 173);
	void Labeling();
	void Morphology();
	void deleteHole();
	void drawHandLine();
	void Thinning();
	CvPoint findCenter();
	void findEndPoint(int* x, int* y);

public:
	IplImage* m_transImage;
private:
	CDC* m_pDC;
	IplImage* m_image;

	CvPoint m_center;
	CvPoint m_pastPt;

private:
	CvPoint2D32f* m_lkPoints[2];	//
	CvPoint2D32f* m_lkSwapPoints; 
	CvPoint m_lkPt;					// 찾은 좌표 (트래킹할 좌표)

	char* m_lkStatus;				// status=1:이동된 위치를 찾은 경우, status=0:이동된 위치를 찾지 못한 경우
	int m_lkCount;					// 찾으려는 점의 갯수
	int m_lkAddRemovePt;			// 찾은 좌표의 수
	IplImage* m_lkSwapTemp;			//
	IplImage* m_lkGrey;
	IplImage* m_lkPrevGrey;
	IplImage* m_lkPyramid;
	IplImage* m_lkPrevPyramid;
	int m_lkFlag;					// CV_LKFLOW_INITIAL_GUESSES 등의 옵션 설정 값
	bool m_lkStart;					// 처음 한번만 IplImage들 생성하기위한 변수
	BOOL m_isTracking;
};
