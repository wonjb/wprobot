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
// 	CvvImage image;
// 	image.CopyOf(m_transImage, m_transImage->nChannels*8);
// 	image.Show(m_pDC->GetSafeHdc(), x, y, width, height);

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

	// 이 줄이 없으면 이미지가 거꾸로 출력됨
	// m_transImage 의 origin 과 m_image 의 origin 을 일치시켜줌
	m_transImage->origin = m_image->origin;

	unsigned char r, g, b;
	double y, cb, cr;
	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			// 컬러영상은 안에 b,g,r order 저장
			r = m_image->imageData[i*step+j*3+2];
			g = m_image->imageData[i*step+j*3+1];
			b = m_image->imageData[i*step+j*3+0];

			// R, G, B 값을 읽어들여 Y, Cb, Cr 로 변환하는 식
			y  = 0.299*r+0.587*g+0.114*b;
			cb = (b-y)*0.564+128;
			cr = (r-y)*0.713+128;

			// 살색의 임계값으로 Skin region 찾음
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

	// 반복문을 이용한 라벨링

	// 지나간 곳을 저장하기 위해 메모리 할당
	int* visited = new int[height*width];
	memset( visited, 0, height*width*sizeof(int) );

	// 삭제할 것을 담을 벡터
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
			if(visited[iByWidth+j] != 0 || ch != 255)		// 지나갔던 점이거나 까만점이면
				continue;

			stack.setEmpty();

			area = 1;
			r = i, c = j;
			curColor++;
			while(1)
			{
GRASSFIRE:
				for(int m = r-1; m <= r+1; ++m)			// y축
				{
					mByWidth = m*width;
					for(int n = c-1; n <= c+1; ++n)		// x축
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
			if(ch != 255)		// 까만점이면
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

	// 거리 변환 행렬
	float mask[3] = {1.f, 1.5f, 0};

	// 거리 변환 함수 사용
	cvDistTransform(m_transImage, dist32f, CV_DIST_USER, 3, mask, NULL);

	// 눈에 보이게 변환
	cvConvertScale(dist32f, dist32f, 1000, 0);
	cvPow(dist32f, dist32f, 0.5);

	cvConvertScale(dist32f, dist32s, 1.0, 0.5);
	cvAndS(dist32s, cvScalarAll(255), dist32s, 0);
	cvConvertScale(dist32s, dist8u, 1, 0);

	// 가장 큰 좌표를 찾는다
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

CHandPoint CTransformImage::findFinger()
{
	findCenter();

	if(!m_transImage)
		return CHandPoint();

	int width   = m_transImage->width;
	int height  = m_transImage->height;
	int moveX   = 0,     moveY  = height;
	BOOL bClick = FALSE, bWheel = FALSE;
	unsigned char ch;
	for(int y = m_center.y; y < height; ++y)
	{
		for(int x = m_center.x-100; x < m_center.x+40; ++x)
		{
			if(x < 0 || x >= width || y < 0 || y >= height)
				continue;

			ch = m_transImage->imageData[y*width+x];
			if(ch == 255)
			{
				moveX = x, moveY = y;
				if(x < m_center.x-50 && y < m_center.y+30)
					bClick = TRUE;
				break;
			}
		}

		if(moveY != y)
			break;
	}

	int y = abs(moveY-m_center.y)/2+m_center.y;
	for(int x = moveX+20; x < moveX+50; ++x)
	{
		if(x < 0 || x >= width)
			continue;

		ch = m_transImage->imageData[y*width+x];
		if(ch == 255)
		{
			bWheel = TRUE;
			break;
		}
	}

	// 좌표가 조금씩 흔들리는 것을 방지하기 위한 부분
	if(abs(m_pastPt.x-moveX) < 2 || abs(m_pastPt.y-moveY) < 2)
		moveX = m_pastPt.x, moveY = m_pastPt.y;

	m_pastPt.x = moveX, m_pastPt.y = moveY;

	CvBox2D box;
	box.center = cvPoint2D32f(moveX, moveY);
	box.size   = cvSize2D32f(2, 2);
	box.angle  = 90;
	cvEllipseBox(m_image, box, CV_RGB(0,255,0), 1);

	return CHandPoint(moveX, height-moveY, bClick, bWheel);
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

