// Mouse.cpp : ���� �����Դϴ�.
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



// CMouse �޽��� ó�����Դϴ�.


void CMouse::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CWnd::OnPaint()��(��) ȣ������ ���ʽÿ�.

	Graphics graphics(dc);

	graphics.DrawImage(m_mouseImage, 0, 0, 20, 20);
}
