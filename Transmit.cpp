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
	if(m_pastPt.m_mode == m_handPt.m_mode && (m_handPt.m_mode != CHandPoint::DRAW && m_handPt.m_mode != CHandPoint::MOVE))
		return;

	m_robot.CComStart() == TRUE ? ((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->SetWindowText(_T("Robot 통신 Start!"))
								: ((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->SetWindowText(_T("Robot 통신 End!"));
	///////////////
	m_robot.SendPointer(5,0);
	m_robot.SendPointer(5,5);
	m_robot.SendPointer(0,5);
	m_robot.SendPointer(0,0);
	///////////////

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
// // 	case CHandPoint::STAR	: convertSTAR(m_winRegn);
// // 							  return;
// 	case CHandPoint::DRAW	: pPaint->clickPointer(m_handPt.m_nX,m_handPt.m_nY);
// 							  m_robot.SendPointer(m_handPt.m_nX,m_handPt.m_nY);
// 							  break;
// 	case CHandPoint::MOVE	: pPaint->movePointer(m_handPt.m_nX,m_handPt.m_nY);
// 							  m_robot.SetColor(pPaint->inColorRegn(m_handPt.m_nX,m_handPt.m_nY));
// 							  break;
// 	case CHandPoint::CLEAR	: m_robot.CComStart() == TRUE ? ((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->SetWindowText(_T("Robot 통신 Start!"))
// 														  : ((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->SetWindowText(_T("Robot 통신 End!"));
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
// 	int x, y, tx, ty;
// 	double pi = 3.1415;
// 	int radius = 50;
// 
// 	CwpRobotver20Dlg* pDlg = (CwpRobotver20Dlg*)(theApp.m_pMainWnd);
// 
// 	tx = (int)(m_handPt.m_nX + radius*cos(360*pi/180));
// 	ty = (int)(m_handPt.m_nY - radius*sin(360*pi/180));
// 	pDlg->m_paint.movePointer(tx, ty);
// 
// 	for(double theta = 0; theta <= 360; ++theta)
// 	{
// 		x = (int)(m_handPt.m_nX + radius*cos(theta*pi/180));
// 		y = (int)(m_handPt.m_nY - radius*sin(theta*pi/180));
// 
// 		pDlg->m_paint.clickPointer(x, y);
// 		m_robot.SendPointer(x, y);
// 
// 		tx = x, ty = y;
// 	}
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