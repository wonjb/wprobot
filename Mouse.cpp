// Mouse.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wpRobot(ver1.0).h"
#include "Mouse.h"



// CMouse

IMPLEMENT_DYNAMIC(CMouse, CWnd)

CMouse::CMouse()
{
	m_mouseImage = new Image(_T("pen.png"));
}

CMouse::~CMouse()
{
	delete m_mouseImage;
}


BEGIN_MESSAGE_MAP(CMouse, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CMouse 메시지 처리기입니다.


void CMouse::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.

	Graphics graphics(dc);

	graphics.DrawImage(m_mouseImage, 0, 0, 20, 20);
}
