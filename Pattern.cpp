#include "StdAfx.h"
#include "Pattern.h"

#include "wpRobot(ver1.0).h"
#include "wpRobot(ver1.0)Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPattern::CPattern(CvPoint center, CHandPoint handPt)
: m_center(center)
, m_handPt(handPt)
{
}

CPattern::~CPattern(void)
{
}

void CPattern::getMotion()
{
	if(m_handPt.m_bClick)
		((CwpRobotver10Dlg*)(theApp.m_pMainWnd))->m_paint.clickPointer(m_handPt.m_nX, m_handPt.m_nY);
// 	else if(m_handPt.m_bWheel)
// 		((CwpRobotver10Dlg*)(theApp.m_pMainWnd))->m_paint.wheelPointer(m_handPt.x, m_handPt.y);
	else
		((CwpRobotver10Dlg*)(theApp.m_pMainWnd))->m_paint.movePointer(m_handPt.m_nX, m_handPt.m_nY);

	TCHAR buf[256] = {0,};
	swprintf(buf, _T("%d\t%d\t%s\t%s\n"), m_handPt.m_nX, m_handPt.m_nY, m_handPt.m_bClick ? _T("TRUE") : _T("FALSE"), m_handPt.m_bWheel ? _T("TRUE") : _T("FALSE"));
	::OutputDebugString(buf);
}