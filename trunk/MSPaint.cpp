// MSPaint.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wpRobot(ver1.0).h"
#include "MSPaint.h"

#define MOUSEWIDTH		20
#define MOUSEHEIGHT		20
// CMSPaint

IMPLEMENT_DYNAMIC(CMSPaint, CWnd)

CMSPaint::CMSPaint()
{

}

CMSPaint::~CMSPaint()
{
}


BEGIN_MESSAGE_MAP(CMSPaint, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CMSPaint 메시지 처리기입니다.



BOOL CMSPaint::OnEraseBkgnd(CDC* pDC)
{
	this->GetClientRect(&m_region);

	pDC->FillSolidRect(m_region, RGB(255,255,255));

	return TRUE;
}

int CMSPaint::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_Mouse.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,MOUSEWIDTH,MOUSEHEIGHT), this, 1))
		return -1;

	return 0;
}


// 사용자 정의 함수
void CMSPaint::movePointer(int x, int y)
{
	// cam input 크기 320, 240 -> mspaint region 으로 변환
	x *= (m_region.Width()/320.f);
	y *= (m_region.Height()/240.f);

	m_Mouse.MoveWindow(x, y, MOUSEWIDTH, MOUSEHEIGHT, TRUE);
}