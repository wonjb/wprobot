//////////////////////////////////////////////////////////////////////////
// YCbCr을 이용한 SkinColor Deteching
//		77 127 153 220 -> 손을 거의 못잡음
//		77 127 77  173 -> 손, 얼굴 등 대부분 다 잘 잡음
//////////////////////////////////////////////////////////////////////////
void CTransformImage::findSkinColor(int cbmin /*= 77*/, int cbmax /*= 127*/, int crmin /*= 153*/, int crmax /*= 220*/)
{
	if(!m_transImage)
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

//////////////////////////////////////////////////////////////////////////
// Hue 를 이용한 SkinColor 추출
//		임계치 0 15 -> 거의 다 잡아버림
//		임계치 0 35 -> 살색만 잡음
//////////////////////////////////////////////////////////////////////////
void CTransformImage::findSkinColor(int cbmin /*= 77*/, int cbmax /*= 127*/, int crmin /*= 153*/, int crmax /*= 220*/)
{
	if(!m_transImage)
		cvReleaseImage(&m_transImage);

	m_transImage = cvCreateImage(cvGetSize(m_image), IPL_DEPTH_8U, 1);

	int height = m_image->height;
	int width  = m_image->width;
	int step   = m_image->widthStep;

	// 이 줄이 없으면 이미지가 거꾸로 출력됨
	// m_transImage 의 origin 과 m_image 의 origin 을 일치시켜줌
	m_transImage->origin = m_image->origin;

	unsigned char r, g, b;
	float h, s, v, minc, angle;
	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			// 컬러영상은 안에 b,g,r order 저장
			r = m_image->imageData[i*step+j*3+2];
			g = m_image->imageData[i*step+j*3+1];
			b = m_image->imageData[i*step+j*3+0];

			minc = MIN(r,g);
			minc = MIN(minc,b);

			v = (r + g + b)/3.f;
			if( (r==g) && (g==b) )
			{
				s = 0.0f;
				h = 0.0f;
			}
			else
			{
				s = 1.0f - (3.0f/(r + g + b))*minc;
				angle = (r - 0.5f*g - 0.5f*b)/(float)sqrt((float)((r-g)*(r-g)+(r-b)*(g-b)));
				h = (float)acos(angle);
				h *= 57.29577951f;
			}

			if( b > g )
				h = 360.f - h;

			h = h*255.0f/360.f;


			if( h >= 0  && h <= 35 )
				m_transImage->imageData[i*width+j] = (unsigned char)255;
			else
				m_transImage->imageData[i*width+j] = 0;
		}
	}
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

void CTransformImage::Erode()
{
	if(!m_transImage)
		return;

	IplImage* dest = cvCreateImage(cvGetSize(m_transImage), IPL_DEPTH_8U, 1);

	IplConvKernel* element = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT, NULL);

	// 거꾸로 출력되는 것을 보완
	dest->origin = m_transImage->origin;
	cvErode(m_transImage, dest, element, 1);

	cvReleaseImage(&m_transImage);
	cvReleaseStructuringElement(&element);

	m_transImage = dest;
}

void CTransformImage::Thining()
{
	int height = m_transImage->height,
		width  = m_transImage->widthStep;

	const CvPoint nei[8] = 
	{	// width, height == x, y
		{1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1, 1}, {0,1}, {1,1}
	};

	// 첫번째 루틴
	int whiteCount, connectCount;		// 하얀 픽셀 개수, 연결가능성(connectivity) 개수
	unsigned char ch;
	for( int y = 2; y < height-2; ++y )
	{
		for( int x = 2; x < width-2; ++x )
		{
			ch = m_transImage->imageData[y*width+x];
			if( ch != 255 )		// 하얀픽셀이 아니면 continue
				continue;

			// 하얀픽셀일 때...
			whiteCount = 0, connectCount = 0;
			for( int k = 0; k < 8; ++k )
			{
				short u0 = (short)(x+nei[k].x);		// 대상 픽셀
				short v0 = (short)(y+nei[k].y);

				int n = (k+1)%8;
				short u1 = (short)(x+nei[n].x);		// 비교할 픽셀
				short v1 = (short)(y+nei[n].y);

				// 세선화 조건 1 검사 - 픽셀[x,y]에 대해서 connectivity가 1인 경우
				if( (BYTE)(m_transImage->imageData[v0*width+u0]) == 255 &&
					(BYTE)(m_transImage->imageData[v1*width+u1]) == 0 )
					++connectCount;

				// 세선화 조건 2 검사 - 픽셀[x,y]의 주변 픽셀 중 하얀 픽셀이 2~6사이에 있는 경우
				if( (BYTE)(m_transImage->imageData[v0*width+u0]) == 255 )
					++whiteCount;
			}

			// 세선화 조건1,2에 만족하지 않으면 continue
			if( !(connectCount == 1 &&
				(whiteCount >= 2 && whiteCount <= 6)) )
				continue;

			// 세선화 조건 1,2에 만족했다면...
			int cnt0 = 0;
			for( int i = 0; i <= 4; i+=2 )
			{
				short u0 = (short)(x+nei[i].x);
				short v0 = (short)(y+nei[i].y);

				if( (BYTE)(m_transImage->imageData[v0*width+u0]) == 0 )
					++cnt0;
			}

			int cnt1 = 0;
			for( int i = 2; i <= 6; i+=2 )
			{
				short u0 = (short)(x+nei[i].x);
				short v0 = (short)(y+nei[i].y);

				if( (BYTE)(m_transImage->imageData[v0*width+u0]) == 0 )
					++cnt1;
			}

			// 세선화 조건 3에 만족하지 않으면 continue
			if( !(cnt1 >= 1 && cnt0 >= 1) )
				continue;

			// 세선화 조건에 만족하는 경우!!!
			// 하얀 픽셀을 까만 픽셀로 바꾼다
			m_transImage->imageData[y*width+x] = 0;
		}
	}

	// 두번째 루틴
	for( int y = 2; y < height-2; ++y )
	{
		for( int x = 2; x < width-2; ++x )
		{
			ch = m_transImage->imageData[y*width+x];
			if( ch != 255 )		// 하얀픽셀이 아니면 continue
				continue;

			// 하얀픽셀일 때...
			whiteCount = 0, connectCount = 0;
			for( int k = 0; k < 8; ++k )
			{
				short u0 = (short)(x+nei[k].x);		// 대상 픽셀
				short v0 = (short)(y+nei[k].y);

				int n = (k+1)%8;
				short u1 = (short)(x+nei[n].x);		// 비교할 픽셀
				short v1 = (short)(y+nei[n].y);

				// 세선화 조건 1 검사 - 픽셀[x,y]에 대해서 connectivity가 1인 경우
				if( (BYTE)(m_transImage->imageData[v0*width+u0]) == 255 &&
					(BYTE)(m_transImage->imageData[v1*width+u1]) == 0 )
					++connectCount;

				// 세선화 조건 2 검사 - 픽셀[x,y]의 주변 픽셀 중 하얀 픽셀이 2~6사이에 있는 경우
				if( (BYTE)(m_transImage->imageData[v0*width+u0]) == 255 )
					++whiteCount;
			}

			// 세선화 조건1,2에 만족하지 않으면 continue
			if( !(connectCount == 1 &&
				(whiteCount >= 2 && whiteCount <= 6)) )
				continue;

			// 세선화 조건 1,2에 만족했다면...
			int cnt0 = 0;
			for( int i = 6; i != 4 ; i=(i+2)%8 )
			{
				short u0 = (short)(x+nei[i].x);
				short v0 = (short)(y+nei[i].y);

				if( (BYTE)(m_transImage->imageData[v0*width+u0]) == 0 )
					++cnt0;
			}

			int cnt1 = 0;
			for( int i = 4; i != 2 ; i=(i+2)%8 )
			{
				short u0 = (short)(x+nei[i].x);
				short v0 = (short)(y+nei[i].y);

				if( (BYTE)(m_transImage->imageData[v0*width+u0]) == 0 )
					++cnt1;
			}

			// 세선화 조건 3에 만족하지 않으면 continue
			if( !(cnt1 >= 1 && cnt0 >= 1) )
				continue;

			// 세선화 조건에 만족하는 경우!!!
			// 하얀 픽셀을 까만 픽셀로 바꾼다
			m_transImage->imageData[y*width+x] = 0;
		}
	}
}

void CTransformImage::deleteHole()
{
	const CvPoint nei[8] = 
	{	// width, height == x, y
		{1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1, 1}, {0,1}, {1,1}
	};

	int height = m_transImage->height;
	int width  = m_transImage->width;

	int wcnt, bcnt;
	unsigned char ch;
	for(int y = 0; y < height; ++y)
	{
		for(int x = 0; x < width; ++x)
		{
			wcnt = bcnt = 0;
			for( int k = 0; k < 8; ++k )
			{
				short u = (short)(x + nei[k].x);
				short v = (short)(y + nei[k].y);

				if( u < 0 || u >= width || v < 0 || v >= height )
					continue;

				ch = m_transImage->imageData[v*width+u];
				if(ch == 255)
					wcnt++;
				else
					bcnt++;
			}

			if(wcnt == 8)
				m_transImage->imageData[y*width+x] = (unsigned char)255;
			else if(bcnt == 8)
				m_transImage->imageData[y*width+x] = (unsigned char)0;
		}
	}
}

void CTransformImage::findCenterDistance()
{
	double pi = 3.1415;
	int width = m_transImage->width;
	int x, y, cnt, radius = 0;
	int bgmin = m_center.y < 240-m_center.y ? m_center.y : 240-m_center.y;
	unsigned char ch;
	for(int r = 1; r < bgmin; ++r)
	{
		cnt = 0;
		for(double theta = 180; theta <= 360; ++theta)
		{
			x = (int)(m_center.x + r*cos(theta*pi/180));
			y = (int)(m_center.y - r*sin(theta*pi/180));

			ch = m_transImage->imageData[y*width+x];
			if(ch < 255)
				++cnt;

// 			CvBox2D box;
// 			box.center = cvPoint2D32f(x, y);
// 			box.size   = cvSize2D32f(1, 1);
// 			box.angle  = 90;
// 			cvEllipseBox(m_image, box, CV_RGB(255,242,0), 1);
		}

		if(cnt > 100)
		{
			radius = r;
			break;
		}
	}

//  	for(int r = 0; r < radius; ++r)
//  	{for(double theta = 180; theta > 0; --theta)
//  		for(double theta = 0; theta <= 180; ++theta)
//  		{
//  			x = (int)(m_center.x + r*cos(theta*pi/180));
//  			y = (int)(m_center.y - r*sin(theta*pi/180));
//  
//  			m_transImage->imageData[y*width+x] = (unsigned char)0;
//  		}
//  	}

	CvBox2D box;
	box.center = cvPoint2D32f(m_center.x, m_center.y);
	box.size   = cvSize2D32f(radius+25, radius+25);
	box.angle  = 0;
	cvEllipseBox(m_image, box, CV_RGB(255,242,0), 1);
}


void CTransformImage::findCenterDistance()
{
	double pi = 3.1415;
	int width = m_transImage->width;
	int x, y, cnt, radius = 0;

	int tempcnt = 0;
	unsigned char ch, ch2 = 0;
	for(int r = 1; r < 240; ++r)
	{
		tempcnt = 0;
		cnt = 0;
		for(double theta = 180; theta <= 360; ++theta)
		{
			x = (int)(m_center.x + r*cos(theta*pi/180));
			y = (int)(m_center.y - r*sin(theta*pi/180));

			ch = m_transImage->imageData[y*width+x];
			if(ch < 255)
				++cnt;

			if(ch == 255 && ch2 == 0)
				++tempcnt;

			ch2 = ch;

// 			CvBox2D box;
// 			box.center = cvPoint2D32f(x, y);
// 			box.size   = cvSize2D32f(1, 1);
// 			box.angle  = 90;
// 			cvEllipseBox(m_image, box, CV_RGB(255,242,0), 1);
		}

		if(cnt > 150)
		{
			radius = r;
			break;
		}
	}

//  	for(int r = 1; r < radius; ++r)
//  	{
// 		for(double theta = 180; theta <= 360; ++theta)
//  		{
//  			x = (int)(m_center.x + r*cos(theta*pi/180));
//  			y = (int)(m_center.y - r*sin(theta*pi/180));
//  
//  			m_transImage->imageData[y*width+x] = (unsigned char)255;
//  		}
//  	}

	CvBox2D box;
	box.center = cvPoint2D32f(m_center.x, m_center.y);
	box.size   = cvSize2D32f(radius+25, radius+25);
	box.angle  = 0;
	cvEllipseBox(m_image, box, CV_RGB(255,242,0), 1);

	TCHAR buf[256] = {0,};
	swprintf(buf, sizeof(buf), _T("%d\n"), tempcnt);
	::OutputDebugString(buf);
}

void CTransformImage::Morphology(int type)
{
	if(!m_transImage)
		return;

	IplImage* temp = cvCreateImage(cvGetSize(m_transImage), IPL_DEPTH_8U, 1);
	IplImage* dest = cvCreateImage(cvGetSize(m_transImage), IPL_DEPTH_8U, 1);

	IplConvKernel* element = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT, NULL);

	// 거꾸로 출력되는 것을 보완
	dest->origin = m_transImage->origin;
	//	cvMorphologyEx(m_transImage, dest, temp, element, type);
	cvDilate(m_transImage, dest, element, 1);

	cvReleaseImage(&m_transImage);
	cvReleaseImage(&temp);
	cvReleaseStructuringElement(&element);

	m_transImage = dest;
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

std::vector<CvPoint> CTransformImage::findFinger()
{
	if(!m_transImage)
		return std::vector<CvPoint>();

	int width  = m_transImage->width;
	int height = m_transImage->height;
	int midX, midY;
	unsigned char ch;
	//	CvBox2D box;
	for(midY = m_center.y, midX = m_center.x; midY < height; ++midY)
	{
		ch = m_transImage->imageData[midY*width+midX];
		if(ch == 0)
			break;

		// 		box.center = cvPoint2D32f(midX, midY);
		// 		box.size   = cvSize2D32f(1, 1);
		// 		box.angle  = 90;
		// 		cvEllipseBox(m_image, box, CV_RGB(255,242,0), 1);
	}

	CvBox2D box;
	box.center = cvPoint2D32f(midX, midY);
	box.size   = cvSize2D32f(2, 2);
	box.angle  = 90;
	cvEllipseBox(m_image, box, CV_RGB(0,255,0), 1);


	double pi = 3.1415;
	int x, y, cnt, radius = 0;
	int st, ed;
	int bgmin = m_center.y < 240-m_center.y ? m_center.y : 240-m_center.y;
	for(int r = 1; r < bgmin; ++r)
	{
		cnt = 0;
		for(double theta = 180; theta <= 360; ++theta)
		{
			x = (int)(m_center.x + r*cos(theta*pi/180));
			y = (int)(m_center.y - r*sin(theta*pi/180));

			ch = m_transImage->imageData[y*width+x];
			if(ch == 255)
				continue;

			// 까만색이면...


			// 			CvBox2D box;
			// 			box.center = cvPoint2D32f(x, y);
			// 			box.size   = cvSize2D32f(1, 1);
			// 			box.angle  = 90;
			// 			cvEllipseBox(m_image, box, CV_RGB(255,242,0), 1);
		}

		if(cnt > 100)
		{
			radius = r;
			break;
		}
	}

	//  	for(int r = 0; r < radius; ++r)
	//  	{for(double theta = 180; theta > 0; --theta)
	//  		for(double theta = 0; theta <= 180; ++theta)
	//  		{
	//  			x = (int)(m_center.x + r*cos(theta*pi/180));
	//  			y = (int)(m_center.y - r*sin(theta*pi/180));
	//  
	//  			m_transImage->imageData[y*width+x] = (unsigned char)0;
	//  		}
	//  	}

	CvBox2D box2;
	box2.center = cvPoint2D32f(m_center.x, m_center.y);
	box2.size   = cvSize2D32f(radius+25, radius+25);
	box2.angle  = 0;
	cvEllipseBox(m_image, box2, CV_RGB(255,242,0), 1);

	return std::vector<CvPoint>();
}

std::vector<CvPoint> CTransformImage::findFinger()
{
	if(!m_transImage)
		return std::vector<CvPoint>();

	int width  = m_transImage->width;
	int height = m_transImage->height;
	int midX, midY;
	unsigned char ch;
	for(midY = m_center.y, midX = m_center.x; midY < height; ++midY)
	{
		ch = m_transImage->imageData[midY*width+midX];
		if(ch == 0)
			break;
	}

	CvBox2D box;
	box.center = cvPoint2D32f(midX, midY);
	box.size   = cvSize2D32f(2, 2);
	box.angle  = 90;
	cvEllipseBox(m_image, box, CV_RGB(0,255,0), 1);

	std::vector<CvPoint> ptList;

	double pi = 3.1415;
	int x, y, radius = m_center.y + (midY-m_center.y)/5;
	int tx, ty, mx, my;
	unsigned char pastCh = 0;
	int cnt = 0;
	for(double theta = 180; theta <= 360; ++theta)
	{
		x = (int)(m_center.x + radius*cos(theta*pi/180));
		y = (int)(m_center.y - radius*sin(theta*pi/180));

		if(x < 0 || x >= width || y < 0 || y >= height)
			continue;

		ch = m_transImage->imageData[y*width+x];
		if(pastCh == 255 && ch == 0)
		{
			ptList.push_back(cvPoint(x,y));
			cnt++;

			CvBox2D box;
			box.center = cvPoint2D32f(x, y);
			box.size   = cvSize2D32f(2, 2);
			box.angle  = 90;
			cvEllipseBox(m_image, box, CV_RGB(0,0,255), 2);
			// 
			// 			mx = my = 0;
			// 			for(double theta = 180; theta <= 360; ++theta)
			// 			{
			// 				tx = (int)(x + radius*cos(theta*pi/180));
			// 				ty = (int)(y - radius*sin(theta*pi/180));
			// 
			// 				if(tx < 0 || tx >= width || ty < 0 || ty >= height)
			// 					continue;
			// 				
			// 				ch = m_transImage->imageData[ty*width+tx];
			// 				if(ch == 255)
			// 				{
			// 					if(my < ty)
			// 						mx = tx, my = ty;
			// 				}
			// 			}
			// 
			// 			CvBox2D box2;
			// 			box2.center = cvPoint2D32f(mx, my);
			// 			box2.size   = cvSize2D32f(2, 2);
			// 			box2.angle  = 90;
			// 			cvEllipseBox(m_image, box2, CV_RGB(0,255,255), 2);
		}

		pastCh = ch;
	}

	TCHAR buf[256] = {0,};
	swprintf(buf, _T("%d\n"), cnt);
	::OutputDebugString(buf);

	return ptList;
}

void CTransformImage::deleteHole()
{
	const CvPoint nei[8] = 
	{	// width, height == x, y
		{1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1, 1}, {0,1}, {1,1}
	};

	std::vector<CvPoint> deleted;

	int height = m_transImage->height;
	int width  = m_transImage->width;

	unsigned char ch;
	for(int y = 0; y < height; ++y)
	{
		for(int x = 0; x < width; ++x)
		{
			for(int k = 0; k < 8; ++k)
			{
				short u = (short)(x + nei[k].x);
				short v = (short)(y + nei[k].y);

				if(u < 0 || u >= width || v < 0 || v >= height)
					continue;

				ch = m_transImage->imageData[v*width+u];
				if(ch != 255)
					deleted

					m_transImage->imageData[y*width+x] = (unsigned char)255;
			}				
		}
	}
}

CHandPoint CTransformImage::findFinger()
{
	if(!m_transImage)
		return CHandPoint();

	int width   = m_transImage->width;
	int height  = m_transImage->height;
	int moveX   = 0,     moveY  = height;
	BOOL bClick = FALSE, bWheel = FALSE;
	unsigned char ch;
	for(int y = m_center.y; y < height; ++y)
	{
		for(int x = m_center.x-100; x < m_center.x+50; ++x)
		{
			if(x < 0 || x >= width || y < 0 || y >= height)
				continue;

			ch = m_transImage->imageData[y*width+x];
			if(ch == 255)
			{
				moveX = x, moveY = y;
				if(x < m_center.x-70)
					bClick = TRUE;
				break;
			}

			CvBox2D box;
			box.center = cvPoint2D32f(x, y);
			box.size   = cvSize2D32f(2, 2);
			box.angle  = 90;
			cvEllipseBox(m_image, box, CV_RGB(0,255,255), 1);
		}

		if(moveY != y)
			break;
	}

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