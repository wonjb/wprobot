void CTransmit::setHandPointer(CHandPoint handPt)
{
	m_handPt = handPt;
	if(m_handPt.m_mode == m_pastPt.m_mode)
	{	m_pastPt = m_handPt;	return;	}


	m_color = (COLOR)((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->m_paint.inColorRegn(handPt.m_nX, handPt.m_nY);

	setWindowRegn();
	setRobotRegn();

	m_pastPt = m_handPt;
}


#include "StdAfx.h"
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
	setRobotRegn();
}

void CTransmit::setWindowRegn()
{
	m_winRegn = ((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->m_paint.getMSPaintRegn();
}

void CTransmit::setRobotRegn()
{
	m_robotRegn.SetRect(0,0,240,180);
}

void CTransmit::setHandPointer(CHandPoint handPt)
{
	m_handPt = handPt;
}

void CTransmit::Transmit()
{
	transmitWindow();
	//	transmitRobot();

	m_pastPt = m_handPt;
}

void CTransmit::transmitWindow()
{
	if(m_pastPt.m_mode == m_handPt.m_mode && (m_handPt.m_mode != CHandPoint::DRAW && m_handPt.m_mode != CHandPoint::MOVE))
		return;

	CMSPaint* pPaint = &(((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->m_paint);
	switch(m_handPt.m_mode)
	{
	case CHandPoint::CIRCLE: convertCIRCLE(m_winRegn);
		return;
		// 	case CHandPoint::RECT: convertRECT(m_winRegn);
		// 		return;
		// 	case CHandPoint::TRIANGE: convertTRIANGLE(m_winRegn);
		// 		return;
		// 	case CHandPoint::STAR: convertSTAR(m_winRegn);
		// 		return;
	case CHandPoint::DRAW: pPaint->clickPointer(m_handPt.m_nX,m_handPt.m_nY);
		break;
	case CHandPoint::MOVE: pPaint->movePointer(m_handPt.m_nX,m_handPt.m_nY);
		m_color = (COLOR)pPaint->inColorRegn(m_handPt.m_nX,m_handPt.m_nY);
		break;
	case CHandPoint::CLEAR: pPaint->InitializeRegn();
		break;
	case CHandPoint::SETTING: 
		break;
	}
}

void CTransmit::transmitRobot()
{
	int x, y;
	// cam input 크기 240, 180 -> Robot region 으로 변환
	x = m_handPt.m_nX*(m_robotRegn.Width()/240.f);
	y = m_handPt.m_nY*(m_robotRegn.Height()/180.f);

	if(x >= m_winRegn.Width()-1)
		x = m_winRegn.Width()-1;
	if(y >= m_winRegn.Height()-1)
		y = m_winRegn.Height()-1;

	// Send to Robot
	SENDDATA data;
	data._color = m_color;
	data._x     = x;
	data._y     = y;
}

void CTransmit::convertCIRCLE(CRect regn)
{
	CHandPoint tempPt = m_handPt;

	double pi = 3.1415;
	int radius = regn.Height()/2;
	for(double theta = 0; theta <= 360; ++theta)
	{
		m_handPt.m_mode = CHandPoint::DRAW;
		m_handPt.m_nX = (int)(m_handPt.m_nX + radius*cos(theta*pi/180));
		m_handPt.m_nY = (int)(m_handPt.m_nY - radius*sin(theta*pi/180));

		// 		setHandPointer(m_handPt);
		// 		Transmit();
		//		m_handPt = tempPt;
	}
}