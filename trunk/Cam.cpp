// Cam.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "wpRobot(ver2.0).h"
#include "Cam.h"

#include "wpRobot(ver2.0)Dlg.h"



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



// CCam �޽��� ó�����Դϴ�.

int CCam::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	int nCamCount = cvcamGetCamerasCount();
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

BOOL            g_Main = TRUE;
CTransformImage g_transform;
CTransmit       g_transmit;
void cvcamCallBack(IplImage* image)
{
	g_transform.setOriginImage(image);
	g_transform.findSkinColor();
	g_transform.deleteNoise();
	g_transform.drawTransImage(550, 250, 320, 230);

	((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->m_setting->drawImage(g_transform.m_transImage);

	g_transmit.setHandPointer(g_transform.findFingerInfo());
	g_transmit.transmitWindow();
	g_transmit.transmitRobot();
}
