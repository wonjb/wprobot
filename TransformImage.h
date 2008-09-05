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
	CvPoint m_lkPt;					// ã�� ��ǥ (Ʈ��ŷ�� ��ǥ)

	char* m_lkStatus;				// status=1:�̵��� ��ġ�� ã�� ���, status=0:�̵��� ��ġ�� ã�� ���� ���
	int m_lkCount;					// ã������ ���� ����
	int m_lkAddRemovePt;			// ã�� ��ǥ�� ��
	IplImage* m_lkSwapTemp;			//
	IplImage* m_lkGrey;
	IplImage* m_lkPrevGrey;
	IplImage* m_lkPyramid;
	IplImage* m_lkPrevPyramid;
	int m_lkFlag;					// CV_LKFLOW_INITIAL_GUESSES ���� �ɼ� ���� ��
	bool m_lkStart;					// ó�� �ѹ��� IplImage�� �����ϱ����� ����
	BOOL m_isTracking;
};
