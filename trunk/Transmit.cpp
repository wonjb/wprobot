#include "stdafx.h"
#include "Transmit.h"

#include "wpRobot(ver2.0).h"
#include "wpRobot(ver2.0)Dlg.h"


CTransmit::CTransmit()
: m_handPt(CHandPoint::NOTHING)
, m_pastPt(CHandPoint::NOTHING)
{
}

CTransmit::~CTransmit(void)
{
}

void CTransmit::Initalize()
{
	setWindowRegn();
}

void CTransmit::setWindowRegn()
{
	m_winRegn = ((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->m_paint.getMSPaintRegn();
}

void CTransmit::setHandPointer(CHandPoint handPt)
{
	m_handPt = handPt;
}

void CTransmit::Transmit()
{
	transmitWindow();
	m_pastPt = m_handPt;
}

void CTransmit::transmitWindow()
{
	////////// 여러번 나오는 현상을 막기위한!
// 	TCHAR buf[256] = {0,};
// 	static int n, test = 0;
// 	if(m_pastPt.m_mode == m_handPt.m_mode && m_handPt.m_mode == CHandPoint::CLEAR)
// 	{
// 		++n, ++test;
// 		swprintf(buf, sizeof(buf), _T("통신 Connecting...%d"), n/10);
// 		((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->SetWindowText(buf);
// 		if(n == 100)
// 		{
// 			m_robot.CComStart() == TRUE ? ((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->SetWindowText(_T("Robot 통신 Start!"))
// 				: ((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->SetWindowText(_T("Robot 통신 End!"));
// 			n = 0;
// 		}
// 	}
// 	else if(m_pastPt.m_mode == m_handPt.m_mode)
// 		++test;
// 	else
// 		n = 0, test = 0;
	
// 	if(test < 10)
// 		return;

// 	////////// Hardware 와 Test하기 위해!
 	m_robot.CComStart() == TRUE ? ((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->SetWindowText(_T("Robot 통신 Start!"))
 								: ((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->SetWindowText(_T("Robot 통신 End!"));
	
	///////////////
	m_robot.SendPointer(0,0);
	::Sleep(4000);
	m_robot.SendPointer(200,0);
	::Sleep(4000);

// 	m_robot.SendPointer(0,200);
// 	::Sleep(4000);
// 	m_robot.SendPointer(0,40);
// 	::Sleep(4000);
// 	m_robot.SendPointer(0,60);
// 	::Sleep(4000);
// 	m_robot.SendPointer(0,0);
// 	::Sleep(4000);
// 	m_robot.SendPointer(0,0);
// 	::Sleep(4000);
// 	m_robot.SendPointer(0,0);
// 	::Sleep(4000);
// 	m_robot.SendPointer(0,0);
// 	::Sleep(4000);
// 	m_robot.SendPointer(0,0);
// 	::Sleep(4000);
// 	m_robot.SendPointer(0,0);
// 	::Sleep(4000);
	///////////////

	////////// Software 정말 소스!
// 	if(m_pastPt.m_mode == m_handPt.m_mode && (m_handPt.m_mode != CHandPoint::DRAW && m_handPt.m_mode != CHandPoint::MOVE))
// 		return;
// 
// 	m_pParam.m_address = this;
// 	m_pParam.m_rt      = m_winRegn;
// 	CMSPaint* pPaint = &(((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->m_paint);
// 	switch(m_handPt.m_mode)
// 	{
// 	case CHandPoint::CIRCLE	: ::CloseHandle(::CreateThread(NULL, NULL, CallBackDrawCIRCLE, &m_pParam, 0, 0));
// 							  return;
//  	case CHandPoint::RECT	: ::CloseHandle(::CreateThread(NULL, NULL, CallBackDrawRECT, &m_pParam, 0, 0));
//  							  return;
// 	case CHandPoint::TRIANGE: ::CloseHandle(::CreateThread(NULL, NULL, CallBackDrawTRIANGLE, &m_pParam, 0, 0));
// 							  return;
// 	case CHandPoint::STAR	: ::CloseHandle(::CreateThread(NULL, NULL, CallBackDrawSTAR, &m_pParam, 0, 0));
// 							  return;
// 	case CHandPoint::DRAW	: pPaint->clickPointer(m_handPt.m_nX,m_handPt.m_nY);
// 							  m_robot.SendPointer(m_handPt.m_nX,m_handPt.m_nY);
// 							  break;
// 	case CHandPoint::MOVE	: pPaint->movePointer(m_handPt.m_nX,m_handPt.m_nY);
// 							  m_robot.SetColor(pPaint->inColorRegn(m_handPt.m_nX,m_handPt.m_nY));
// 							  break;
// 	case CHandPoint::CLEAR	: m_robot.CComStart() == TRUE ? ((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->SetWindowText(_T("Robot 통신 Start!"))
// 	  												      : ((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->SetWindowText(_T("Robot 통신 End!"));
// 							  pPaint->InitializeRegn();
// 							  break;
// 	case CHandPoint::SETTING: 
// 							  break;
// 	}
}

void CTransmit::convertCIRCLE(CRect regn)
{
	int x, y, tx, ty;
	double pi = 3.1415;
	int radius = 50;

	CwpRobotver20Dlg* pDlg = (CwpRobotver20Dlg*)(theApp.m_pMainWnd);

	tx = (int)(m_handPt.m_nX + radius*cos(360*pi/180));
	ty = (int)(m_handPt.m_nY - radius*sin(360*pi/180));
	pDlg->m_paint.movePointer(tx, ty);

	for(double theta = 0; theta <= 360; ++theta)
	{
		x = (int)(m_handPt.m_nX + radius*cos(theta*pi/180));
		y = (int)(m_handPt.m_nY - radius*sin(theta*pi/180));

		pDlg->m_paint.clickPointer(x, y);
		m_robot.SendPointer(x, y);

		tx = x, ty = y;
	}
}

void CTransmit::convertRECT(CRect regn)
{
	int x, y, tx, ty;
	int w = 70, h = 50;

	CwpRobotver20Dlg* pDlg = (CwpRobotver20Dlg*)(theApp.m_pMainWnd);

	tx = (int)(m_handPt.m_nX - w);
	ty = (int)(m_handPt.m_nY - h);
	pDlg->m_paint.movePointer(tx, ty);

	x = (int)(m_handPt.m_nX + w);
	y = (int)(m_handPt.m_nY - h);
	pDlg->m_paint.clickPointer(x, y);

	x = (int)(m_handPt.m_nX + w);
	y = (int)(m_handPt.m_nY + h);
	pDlg->m_paint.clickPointer(x, y);

	x = (int)(m_handPt.m_nX - w);
	y = (int)(m_handPt.m_nY + h);
	pDlg->m_paint.clickPointer(x, y);

	pDlg->m_paint.clickPointer(tx, ty);
}

void CTransmit::convertTRIANGLE(CRect regn)
{
	int x, y, tx, ty;
	int radius = 50;

	CwpRobotver20Dlg* pDlg = (CwpRobotver20Dlg*)(theApp.m_pMainWnd);

	tx = (int)(m_handPt.m_nX);
	ty = (int)(m_handPt.m_nY - radius);
	pDlg->m_paint.movePointer(tx, ty);

	x = (int)(m_handPt.m_nX + 25*sqrt(3.f));
	y = (int)(m_handPt.m_nY + 25);
	pDlg->m_paint.clickPointer(x, y);

	x = (int)(m_handPt.m_nX - 25*sqrt(3.f));
	y = (int)(m_handPt.m_nY + 25);
	pDlg->m_paint.clickPointer(x, y);

	pDlg->m_paint.clickPointer(tx, ty);
}

void CTransmit::convertSTAR(CRect regn)
{
	int x, y, tx, ty;

	CwpRobotver20Dlg* pDlg = (CwpRobotver20Dlg*)(theApp.m_pMainWnd);

	tx = (int)(m_handPt.m_nX);
	ty = (int)(m_handPt.m_nY - 50);
	pDlg->m_paint.movePointer(tx, ty);

	x = (int)(m_handPt.m_nX + 20);
	y = (int)(m_handPt.m_nY - 20);
	pDlg->m_paint.clickPointer(x, y);

	x = (int)(m_handPt.m_nX + 50);
	y = (int)(m_handPt.m_nY - 10);
	pDlg->m_paint.clickPointer(x, y);

	x = (int)(m_handPt.m_nX + 20);
	y = (int)(m_handPt.m_nY + 30);
	pDlg->m_paint.clickPointer(x, y);

	x = (int)(m_handPt.m_nX + 30);
	y = (int)(m_handPt.m_nY + 50);
	pDlg->m_paint.clickPointer(x, y);

	x = (int)(m_handPt.m_nX);
	y = (int)(m_handPt.m_nY + 25);
	pDlg->m_paint.clickPointer(x, y);

	x = (int)(m_handPt.m_nX - 30);
	y = (int)(m_handPt.m_nY + 50);
	pDlg->m_paint.clickPointer(x, y);

	x = (int)(m_handPt.m_nX - 20);
	y = (int)(m_handPt.m_nY + 30);
	pDlg->m_paint.clickPointer(x, y);

	x = (int)(m_handPt.m_nX - 50);
	y = (int)(m_handPt.m_nY - 10);
	pDlg->m_paint.clickPointer(x, y);

	x = (int)(m_handPt.m_nX - 20);
	y = (int)(m_handPt.m_nY - 20);
	pDlg->m_paint.clickPointer(x, y);

	pDlg->m_paint.clickPointer(tx, ty);
}

DWORD WINAPI CallBackDrawCIRCLE(LPVOID lpParam)
{
	PARAM* pParam = (PARAM*)lpParam;
	((CTransmit*)(pParam->m_address))->convertCIRCLE(pParam->m_rt);

	return 0;
}

DWORD WINAPI CallBackDrawRECT(LPVOID lpParam)
{
	PARAM* pParam = (PARAM*)lpParam;
	((CTransmit*)(pParam->m_address))->convertRECT(pParam->m_rt);

	return 0;
}

DWORD WINAPI CallBackDrawTRIANGLE(LPVOID lpParam)
{
	PARAM* pParam = (PARAM*)lpParam;
	((CTransmit*)(pParam->m_address))->convertTRIANGLE(pParam->m_rt);

	return 0;
}

DWORD WINAPI CallBackDrawSTAR(LPVOID lpParam)
{
	PARAM* pParam = (PARAM*)lpParam;
	((CTransmit*)(pParam->m_address))->convertSTAR(pParam->m_rt);

	return 0;
}