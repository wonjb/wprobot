#include "StdAfx.h"
#include "TransformImage.h"

CTransformImage::CTransformImage(void)
: m_image(NULL), m_transImage(NULL)
{
}

CTransformImage::~CTransformImage(void)
{
	if(!m_image)
		cvReleaseImage(&m_image);
	if(!m_transImage)
		cvReleaseImage(&m_transImage);
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
	CvvImage image;
	image.CopyOf(m_transImage, m_transImage->nChannels*8);
	image.Show(m_pDC->GetSafeHdc(), x, y, width, height);
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
// 	int height = m_transImage->height;
// 	int	width  = m_transImage->width;
// 
// 	///////////////////////////////////////////
// 	// �ݺ����� �̿��� �󺧸�
// 
// 	// ������ ���� �����ϱ� ���� �޸� �Ҵ�
// 	char* visited = new char[height*width];
// 	memset( visited, 0, height*width*sizeof(char) );
// 
// 	// ������ ���� ���� ����
// 	vector<int> deleted;
// 
// 	short r, c, curColor = 0, leftIndex = 0;
// 	int iByWidth, mByWidth, cnt;
// 	unsigned char ch;
// 	for( int i = 0; i < height; ++i )
// 	{
// 		iByWidth = i*width;
// 		for( int j = 0; j < width; ++j )
// 		{
// 			ch = m_transImage->imageData[iByWidth+j];
// 			if( visited[iByWidth+j] != 0 || ch != 255 )		// �������� ���̰ų� ����̸�
// 				continue;
// 
// 			cnt = 0;
// 			r = i, c = j;
// 			curColor++;
// 			while(1)
// 			{
// GRASSFIRE:
// 				for( int m = r-1; m <= r+1; ++m )			// y��
// 				{
// 					mByWidth = m*width;
// 					for( int n = c-1; n <= c+1; ++n )		// x��
// 					{
// 						if( m < 0 || m >= height || n < 0 || n >= width )
// 							continue;
// 
// 						ch = m_transImage->imageData[mByWidth+n];
// 						if( ch == 255 && visited[mByWidth+n] == 0 )
// 						{
// 							visited[mByWidth+n] = curColor;
// 							r = m, c = n;
// 							cnt++;
// 							goto GRASSFIRE;
// 						}
// 					}
// 				}
// 				break;
// 			}
// 		}
// 	}
// 	//////////////////////////////////////////////
// 
// 	for()
}

void CTransformImage::Morphology(int type)
{
	if(!m_transImage)
		return;

	IplImage* temp = cvCreateImage(cvGetSize(m_transImage), IPL_DEPTH_8U, 1);
	IplImage* dest = cvCreateImage(cvGetSize(m_transImage), IPL_DEPTH_8U, 1);

	IplConvKernel* element = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT, NULL);

	// �Ųٷ� ��µǴ� ���� ����
	dest->origin = m_transImage->origin;
	cvMorphologyEx(m_transImage, dest, temp, element, type);

	cvReleaseImage(&m_transImage);
	cvReleaseImage(&temp);
	cvReleaseStructuringElement(&element);

	m_transImage = dest;
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

std::vector<CvPoint> CTransformImage::findFinger()
{
	if(!m_transImage)
		return std::vector<CvPoint>();

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;

	cvFindContours(m_transImage, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	std::vector<CvPoint> ptList;
	CvPoint past, cur;
	for( ; contours != 0; contours = contours->h_next)
	{
		CvSeq* hull;
		hull = cvConvexHull2(contours, 0, CV_CLOCKWISE, 0);
		past = **CV_GET_SEQ_ELEM( CvPoint*, hull, hull->total-1);

		for(int l = 0; l < hull->total; ++l)
		{
			cur = **CV_GET_SEQ_ELEM( CvPoint*, hull, l );

			if(cur.y < m_center.y    && CurToPast(&past, &cur))
				continue;
			if(cur.x > m_center.x+25 && CurToPast(&past, &cur))
				continue;
 			if((abs(past.x - cur.x) < 15 && abs(past.y - cur.y) < 15) && CurToPast(&past, &cur))
				continue;

			CvBox2D box;
			box.center = cvPoint2D32f(cur.x, cur.y);
			box.size   = cvSize2D32f(2,2);
			cvEllipseBox(m_image, box, CV_RGB(0,255,0), 2);

			ptList.push_back(cvPoint(cur.x, 240-cur.y));

			CurToPast(&past, &cur);
		}
	}

	cvReleaseMemStorage(&storage);

	return ptList;
}

void CTransformImage::deleteHole()
{
	const CvPoint nei[8] = 
	{	// width, height == x, y
		{1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1, 1}, {0,1}, {1,1}
	};

	int height = m_transImage->height;
	int width  = m_transImage->width;

	unsigned char ch;
	for(int y = 0; y < height; ++y)
	{
		for(int x = 0; x < width; ++x)
		{
			for( int k = 0; k < 8; ++k )
			{
				short u = (short)(x + nei[k].x);
				short v = (short)(y + nei[k].y);

				if( u < 0 || u >= width || v < 0 || v >= height )
					continue;

				ch = m_transImage->imageData[v*width+u];
				if(ch != 255)
					break;

				m_transImage->imageData[y*width+x] = (unsigned char)255;
			}				
		}
	}
}