#include "StdAfx.h"
#include "Pattern.h"

#include "wpRobot(ver1.0).h"
#include "wpRobot(ver1.0)Dlg.h"


CPattern::CPattern(CvPoint center, std::vector<CvPoint> ptList)
: m_center(center)
, m_ptList(ptList)
{
}

CPattern::~CPattern(void)
{
}

void CPattern::getMotion()
{
	CvPoint pt = cvPoint(0,0);
	for(int i = 0; i < m_ptList.size(); ++i)
	{
		if(pt.y < m_ptList[i].y)
			pt = m_ptList[i];
	}

	((CwpRobotver10Dlg*)(theApp.m_pMainWnd))->m_paint.movePointer(pt.x, pt.y);

// 	int maxX = GetSystemMetrics(SM_CXSCREEN);
// 	int maxY = GetSystemMetrics(SM_CYSCREEN);
// 
// 	pt.x *= (maxX/320);
// 	pt.y *= (maxY/240);

// 	switch(m_ptList.size())
// 	{
// 	case 1: /*::mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE, pt.x, pt.y, 0, GetMessageExtraInfo());*/
//		SetCursorPos(pt.x, pt.y);
// 		((CwpRobotver10Dlg*)(theApp.m_pMainWnd))->m_paint.movePointer(pt.x, pt.y);
// 		break;
//	case 2: ::mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN, pt.x, pt.y, 0, GetMessageExtraInfo());
//		break;
// 	default: /*return NOTHING;*/break;
// 	}
}