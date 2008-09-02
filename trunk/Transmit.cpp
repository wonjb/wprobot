#include "StdAfx.h"
#include "Transmit.h"

#include "wpRobot(ver2.0).h"
#include "wpRobot(ver2.0)Dlg.h"

CTransmit::CTransmit(CHandPoint handPt)
: m_handPt(handPt)
{
}

CTransmit::~CTransmit(void)
{
}

void CTransmit::transmitWindow()
{
	if(m_handPt.m_bClick)
		((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->m_paint.clickPointer(m_handPt.m_nX, m_handPt.m_nY);
// 	else if(m_handPt.m_bWheel)
// 	 	((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->m_setting.DoModal();
	else
		((CwpRobotver20Dlg*)(theApp.m_pMainWnd))->m_paint.movePointer(m_handPt.m_nX, m_handPt.m_nY);

	TCHAR buf[256] = {0,};
	swprintf(buf, _T("%d\t%d\t%s\t%s\n"), m_handPt.m_nX, m_handPt.m_nY, m_handPt.m_bClick ? _T("TRUE") : _T("FALSE"), m_handPt.m_bWheel ? _T("TRUE") : _T("FALSE"));
	::OutputDebugString(buf);
}