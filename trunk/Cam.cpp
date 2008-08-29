// Cam.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wpRobot(ver1.0).h"
#include "Cam.h"

#include "Pattern.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTransformImage g_transform;

// CCam

IMPLEMENT_DYNAMIC(CCam, CStatic)

CCam::CCam()
{

}

CCam::~CCam()
{
}


BEGIN_MESSAGE_MAP(CCam, CStatic)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CCam 메시지 처리기입니다.

int CCam::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	int nCamCount = cvcamGetCamerasCount();			//////////////////// Why? 1이 리턴되는 걸까?
	if(nCamCount == 0)
	{
		MessageBox(_T("No Connected Camera"));
		return -1;
	}

	cvcamSetProperty(0, CVCAM_PROP_ENABLE, CVCAMTRUE);
	cvcamSetProperty(0, CVCAM_PROP_RENDER, CVCAMTRUE);
	cvcamSetProperty(0, CVCAM_PROP_WINDOW, &(this->m_hWnd));

	cvcamSetProperty(0, CVCAM_PROP_CALLBACK, cvcamCallBack);

	cvcamInit();
	cvcamStart();

	return 0;
}

void CCam::OnDestroy()
{
	CStatic::OnDestroy();

	cvcamStop();
	cvcamExit();
}

void cvcamCallBack(IplImage* image)
{
	g_transform.setOriginImage(image);
	g_transform.findSkinColor();
	g_transform.deleteNoise();
	g_transform.drawTransImage(550, 250, 320, 230);

// 	CvPoint center = g_transform.findCenter();
// 	std::vector<CvPoint> ptList = g_transform.findFinger();
//
// 	TCHAR buf[256] = {0,};
// 	swprintf(buf, sizeof(buf), _T("%d\n"), ptList.size());
// 	::OutputDebugString(buf);

//	g_transform.findCenter();
//	g_transform.findFinger();
//	CPattern pattern(g_transform.findCenter(), g_transform.findFinger());
//	pattern.getMotion();
}
