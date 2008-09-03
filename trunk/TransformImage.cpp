#include "StdAfx.h"
#include "TransformImage.h"
#include "Stack.h"
#include <vector>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WINNAME		"Binary Test"


CTransformImage::CTransformImage(void)
: m_image(NULL), m_transImage(NULL)
{
	cvNamedWindow(WINNAME);
}

CTransformImage::~CTransformImage(void)
{
	if(!m_image)
		cvReleaseImage(&m_image);
	if(!m_transImage)
		cvReleaseImage(&m_transImage);

	cvDestroyWindow(WINNAME);
}

void CTransformImage::setDC(CDC* dc)
{
	m_pDC = dc;
}

void CTransformImage::setOriginImage(IplImage* image)
{
	m_image = image;
}

void CTransformImage::drawTransImage(int x, int y, int width, int height)
{
	cvShowImage(WINNAME, m_transImage);
}

void CTransformImage::ThresholdYCbCr(int cbmin /*= 77*/, int cbmax /*= 127*/, int crmin /*= 133*/, int crmax /*= 173*/)
{
	if(m_transImage)
		cvReleaseImage(&m_transImage);

	m_transImage = cvCreateImage(cvGetSize(m_image), IPL_DEPTH_8U, 1);

	int height = m_image->height;
	int width  = m_image->width;
	int step   = m_image->widthStep;

	// �� ���� ������ �̹����� �Ųٷ� ��µ�
	// m_transImage �� origin �� m_image �� origin �� ��ġ������
	m_transImage->origin = m_image->origin;

	unsigned char r, g, b;
	double y, cb, cr;
	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			// �÷������� �ȿ� b,g,r order ����
			r = m_image->imageData[i*step+j*3+2];
			g = m_image->imageData[i*step+j*3+1];
			b = m_image->imageData[i*step+j*3+0];

			// R, G, B ���� �о�鿩 Y, Cb, Cr �� ��ȯ�ϴ� ��
			y  = 0.299*r+0.587*g+0.114*b;
			cb = (b-y)*0.564+128;
			cr = (r-y)*0.713+128;

			// ����� �Ӱ谪���� Skin region ã��
			if( (cb >= cbmin && cb <= cbmax) && (cr >= crmin && cr <= crmax) )
				m_transImage->imageData[i*width+j] = (unsigned char)255;
			else
				m_transImage->imageData[i*width+j] = 0;
		}
	}
}

void CTransformImage::Labeling()
{
	int height = m_transImage->height;
	int	width  = m_transImage->width;

	// �ݺ����� �̿��� �󺧸�

	// ������ ���� �����ϱ� ���� �޸� �Ҵ�
	int* visited = new int[height*width];
	memset( visited, 0, height*width*sizeof(int) );

	// ������ ���� ���� ����
	std::vector<int> deleted;	

	cSTACK stack;
	stack.setSize(height*width);

	short r, c, curColor = 0;
	int iByWidth, mByWidth, area;
	unsigned char ch;
	for(int i = 0; i < height; ++i)
	{
		iByWidth = i*width;
		for(int j = 0; j < width; ++j)
		{
			ch = m_transImage->imageData[iByWidth+j];
			if(visited[iByWidth+j] != 0 || ch != 255)		// �������� ���̰ų� ����̸�
				continue;

			stack.setEmpty();

			area = 1;
			r = i, c = j;
			curColor++;
			while(1)
			{
GRASSFIRE:
				for(int m = r-1; m <= r+1; ++m)			// y��
				{
					mByWidth = m*width;
					for(int n = c-1; n <= c+1; ++n)		// x��
					{
						if(m < 0 || m >= height || n < 0 || n >= width)
							continue;

						ch = m_transImage->imageData[mByWidth+n];
						if(ch == 255 && visited[mByWidth+n] == 0)
						{
							visited[mByWidth+n] = curColor;

							if(!stack.push(n,m))
								continue;

							r = m, c = n;
							area++;
							goto GRASSFIRE;
						}
					}
				}				
				if(!stack.pop(&c, &r))
					break;
			}
			deleted.push_back(area);
		}
	}

	for(int i = 0; i < height; ++i)
	{
		iByWidth = i*width;
		for(int j = 0; j < width; ++j)
		{
			ch = m_transImage->imageData[iByWidth+j];
			if(ch != 255)		// ����̸�
				continue;

			if(deleted[visited[iByWidth+j]-1] < 1000)
				m_transImage->imageData[iByWidth+j] = 0;
		}
	}

	delete[] visited;
}

void CTransformImage::Morphology()
{
	if(!m_transImage)
		return;

	IplConvKernel* element = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT, NULL);

	cvDilate(m_transImage, m_transImage, element, 1);
	cvDilate(m_transImage, m_transImage, element, 1);
	cvErode (m_transImage, m_transImage, element, 1);
	cvErode (m_transImage, m_transImage, element, 1);

	cvReleaseStructuringElement(&element);
}

void CTransformImage::drawHandLine()
{
	if(!m_transImage)
		return;

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;

	cvFindContours(m_transImage, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	//			Drawing image, point, external color,     internal color,      ?, thickness, linetype, offset
	cvDrawContours(m_image, contours, CV_RGB(255,242,0), CV_RGB(255,242,0), 1, 2, CV_AA);

	cvReleaseMemStorage(&storage);
}

CvPoint CTransformImage::findCenter()
{
	IplImage* dist8u  = cvCloneImage(m_transImage);
	IplImage* dist32f = cvCreateImage(cvGetSize(m_transImage), IPL_DEPTH_32F, 1);
	IplImage* dist32s = cvCreateImage(cvGetSize(m_transImage), IPL_DEPTH_32S, 1);

	// �Ÿ� ��ȯ ���
	float mask[3] = {1.f, 1.5f, 0};

	// �Ÿ� ��ȯ �Լ� ���
	cvDistTransform(m_transImage, dist32f, CV_DIST_USER, 3, mask, NULL);

	// ���� ���̰� ��ȯ
	cvConvertScale(dist32f, dist32f, 1000, 0);
	cvPow(dist32f, dist32f, 0.5);

	cvConvertScale(dist32f, dist32s, 1.0, 0.5);
	cvAndS(dist32s, cvScalarAll(255), dist32s, 0);
	cvConvertScale(dist32s, dist8u, 1, 0);

	// ���� ū ��ǥ�� ã�´�
	int max;
	for(int i = max = 0; i < dist8u->height; ++i)
	{
		int index = i * dist8u->widthStep;
		for(int j = 0; j < dist8u->width; ++j)
		{
			if((unsigned char)dist8u->imageData[index+j] > max)
			{
				max = (unsigned char)dist8u->imageData[index+j];
				m_center.x = j, m_center.y = i;
			}
		}
	}

	cvReleaseImage(&dist8u);
	cvReleaseImage(&dist32f);
	cvReleaseImage(&dist32s);

	if(m_center.x < 0 || m_center.y < 0)
		m_center.x = 0, m_center.y = 0;

	CvBox2D box;
	box.center = cvPoint2D32f(m_center.x, m_center.y);
	box.size   = cvSize2D32f(3, 3);
	box.angle  = 90;
	cvEllipseBox(m_image, box, CV_RGB(255,242,0), 3);

	return m_center;
}

CHandPoint CTransformImage::findFingerInfo()
{
	if(!m_transImage)
		return CHandPoint();

	findCenter();

 	CHandPoint handPt;
 	std::vector<CvPoint> ptList;

	double pi = 3.1415;
	int width  = m_transImage->width;
	int height = m_transImage->height;
	int fingerCnt = 0;
	int x, y, radius = 80;
	unsigned char ch, pastCh = 0;
	for(double theta = 180; theta <= 360; ++theta)
	{
		x = (int)(m_center.x + radius*cos(theta*pi/180));
		y = (int)(m_center.y - radius*sin(theta*pi/180));

		if(x < 0 || x >= width || y < 0 || y >= height)
			continue;

		ch = m_transImage->imageData[y*width+x];
		if(ch == 255 && pastCh == 0)		// Counting Finger
			ptList.push_back(cvPoint(x,y)), ++fingerCnt;

		pastCh = ch;

		// Draw OutLine
		CvBox2D box;
		box.center = cvPoint2D32f(x, y);
		box.size   = cvSize2D32f(1, 1);
		box.angle  = 90;
		cvEllipseBox(m_image, box, CV_RGB(255,242,0), 1);
	}

	// handPt Setting
	switch(fingerCnt)
	{
	case 0: handPt.m_mode = CHandPoint::CLEAR;
		break;
	case 1: handPt.m_mode = CHandPoint::MOVE;	findEndPoint(&handPt.m_nX, &handPt.m_nY);
		break;
	case 2:
		{
			CvPoint a = ptList[0], b = ptList[1];
			float dist = sqrt((float)(abs(a.x-b.x)*abs(a.x-b.x) + abs(a.y-b.y)*abs(a.y-b.y)));
			if(dist < 70)		// DRAW mode
			{	handPt.m_mode = CHandPoint::CIRCLE;	handPt.m_nX = m_center.x, handPt.m_nY = m_center.y;	}
			else
			{	handPt.m_mode = CHandPoint::DRAW;	findEndPoint(&handPt.m_nX, &handPt.m_nY);	}
		}
		break;
	case 3: 
		{
			CvPoint a = ptList[0], b = ptList[1];
			float dist = sqrt((float)(abs(a.x-b.x)*abs(a.x-b.x) + abs(a.y-b.y)*abs(a.y-b.y)));
			if(dist < 50)
			{	handPt.m_mode = CHandPoint::TRIANGE;	handPt.m_nX = m_center.x, handPt.m_nY = m_center.y;	}
			else
			{	handPt.m_mode = CHandPoint::SETTING;	}
		}
		break;
	case 4: handPt.m_mode = CHandPoint::RECT;	handPt.m_nX = m_center.x, handPt.m_nY = m_center.y;
		break;
	case 5: handPt.m_mode = CHandPoint::STAR;	handPt.m_nX = m_center.x, handPt.m_nY = m_center.y;
		break;
	default: handPt.m_mode = CHandPoint::NOTHING;
		break;
	}

	TCHAR buf[256] = {0,};
	swprintf(buf, sizeof(buf), _T("%d\n"), fingerCnt);
	::OutputDebugString(buf);

	return handPt;
}

void CTransformImage::findEndPoint(unsigned short* x, unsigned short* y)
{
	int width   = m_transImage->width;
	int height  = m_transImage->height;
	unsigned char ch;
	for(int ty = m_center.y; ty < height; ++ty)
	{
		for(int tx = m_center.x-100; tx < m_center.x+40; ++tx)
		{
			if(tx < 0 || tx >= width || ty < 0 || ty >= height)
				continue;

			ch = m_transImage->imageData[ty*width+tx];
			if(ch == 255)
			{
				*x = tx, *y = ty;
				break;
			}
		}
		if(*y != ty)
			break;
	}

	*y = height - *y;
}

void CTransformImage::deleteHole()
{
	CvPoint st, ed;

	int height = m_transImage->height;
	int width  = m_transImage->width;

	unsigned char ch, pastCh = 255;
	for(int y = 0; y < height; ++y)
	{
		st = ed = cvPoint(0,0);
		for(int x = 0; x < width; ++x)
		{
			ch = m_transImage->imageData[y*width+x];
			if(pastCh == 0 && ch == 255)
				st.x = x, st.y = y;
			else if(st.x != 0 && pastCh == 255 && ch == 0)
				ed.x = x, ed.y = y;

			pastCh = ch;
		}

		if(ed.x - st.x > 150)
			continue;

		for(int x = st.x; x < ed.x; ++x)
			m_transImage->imageData[y*width+x] = (unsigned char)255;
	}
}

