#include "stdafx.h"
#include "Transmit.h"

#include "wpRobot(ver2.0).h"
#include "wpRobot(ver2.0)Dlg.h"

#pragma warning(disable:4244)

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

	PARAM* pParam = new PARAM((int*)this, m_winRegn);
	CMSPaint* pPaint = &(((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->m_paint);
	switch(m_handPt.m_mode)
	{
	case CHandPoint::CIRCLE: ::CloseHandle(::CreateThread(NULL, NULL, CallBackDrawCIRCLE, pParam, 0, 0));
		return;
// 	case CHandPoint::RECT: convertRECT(m_winRegn);
// 		return;
// 	case CHandPoint::TRIANGE: convertTRIANGLE(m_winRegn);
// 		return;
// 	case CHandPoint::STAR: convertSTAR(m_winRegn);
// 		return;
	case CHandPoint::DRAW: pPaint->clickPointer(m_handPt.m_nX,m_handPt.m_nY);
						   m_robot.SendPointer(m_handPt.m_nX,m_handPt.m_nY);
		break;
	case CHandPoint::MOVE: pPaint->movePointer(m_handPt.m_nX,m_handPt.m_nY);
						   m_robot.SetColor(pPaint->inColorRegn(m_handPt.m_nX,m_handPt.m_nY));
		break;
	case CHandPoint::CLEAR: m_robot.CComStart() == TRUE ? ((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->SetWindowText(_T("Robot Send Start!"))
														: ((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->SetWindowText(_T("Robot Send End!"));
							pPaint->InitializeRegn();
		break;
	case CHandPoint::SETTING: 
		break;
	}

	delete pParam;
}

void CTransmit::convertCIRCLE(CRect regn)
{
	int x, y, tx, ty;
	double pi = 3.1415;
	int radius = 50;

	CwpRobotver20Dlg* pDlg = (CwpRobotver20Dlg*)(theApp.m_pMainWnd);

	tx = (unsigned short)(m_handPt.m_nX + radius*cos(360*pi/180));
	ty = (unsigned short)(m_handPt.m_nY - radius*sin(360*pi/180));
	pDlg->m_paint.movePointer(tx, ty);

	for(double theta = 0; theta <= 360; ++theta)
	{
		x = (unsigned short)(m_handPt.m_nX + radius*cos(theta*pi/180));
		y = (unsigned short)(m_handPt.m_nY - radius*sin(theta*pi/180));

		pDlg->m_paint.clickPointer(tx, ty);
		m_robot.SendPointer(x, y);

//		pDlg->m_paint.movePointer(tx,ty);
//		pDlg->m_paint.clickPointer(x,y);

		tx = x, ty = y;

// 		setHandPointer(m_handPt);
// 		Transmit();
//		m_handPt = tempPt;
	}
}

DWORD WINAPI CallBackDrawCIRCLE(LPVOID lpParam)
{
	PARAM* pParam = (PARAM*)lpParam;
	((CTransmit*)(pParam->m_address))->convertCIRCLE(pParam->m_rt);

	return 0;
}
