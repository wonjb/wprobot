#include "StdAfx.h"
#include "Transmit.h"

#include "wpRobot(ver2.0).h"
#include "wpRobot(ver2.0)Dlg.h"

CTransmit::CTransmit()
{
}

CTransmit::~CTransmit(void)
{
}

void CTransmit::setHandPointer(CHandPoint handPt)
{
	m_handPt = handPt;
 	if(m_handPt.m_bWheel)		// setting 창인지 check
	{
		((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->RunSettingDlg();
		m_handPt.m_bWheel = FALSE;
	}
	
	m_color = (COLOR)((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->m_paint.inColorRegn(handPt.m_nX, handPt.m_nY);

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

void CTransmit::transmitWindow()
{
	unsigned short x, y;
	// cam input 크기 320, 240 -> mspaint region 으로 변환
	x = m_handPt.m_nX*(m_winRegn.Width()/240.f);
	y = m_handPt.m_nY*(m_winRegn.Height()/180.f);

	if(x >= m_winRegn.Width()-1)
		x = m_winRegn.Width()-1;
	if(y >= m_winRegn.Height()-1)
		y = m_winRegn.Height()-1;

	if(m_handPt.m_bClick)
		((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->m_paint.clickPointer(x,y);
	else
		((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->m_paint.movePointer(x,y);
}

void CTransmit::transmitRobot()
{
	unsigned short x, y;
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