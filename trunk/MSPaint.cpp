// MSPaint.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "wpRobot(ver1.0).h"
#include "MSPaint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MOUSEWIDTH		20
#define MOUSEHEIGHT		20
// CMSPaint

IMPLEMENT_DYNAMIC(CMSPaint, CWnd)

CMSPaint::CMSPaint()
{

}

CMSPaint::~CMSPaint()
{
	m_bufBmp.DeleteObject();
}


BEGIN_MESSAGE_MAP(CMSPaint, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CMSPaint �޽��� ó�����Դϴ�.



BOOL CMSPaint::OnEraseBkgnd(CDC* pDC)
{
	pDC->FillSolidRect(m_region, RGB(255,255,255));

	return TRUE;
}

int CMSPaint::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_Mouse.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,MOUSEWIDTH,MOUSEHEIGHT), this, 1))
		return -1;

	this->GetClientRect(&m_region);
	Initialize();

	return 0;
}


// ����� ���� �Լ�

void CMSPaint::Initialize()
{
	CClientDC dc(this);

	CDC bufDC;
	bufDC.CreateCompatibleDC(&dc);

	m_bufBmp.CreateCompatibleBitmap(&dc, m_region.Width(), m_region.Height());
	CBitmap* pOldBmp = (CBitmap*)bufDC.SelectObject(&m_bufBmp);

	CBrush brush;
	brush.CreateSolidBrush(RGB(240,240,200));
	
	CBrush* pOldBrush = (CBrush*)bufDC.SelectObject(&brush);

	bufDC.FillRect(m_region, &brush);
//	bufDC.PatBlt(0,0,m_region.Width(),m_region.Height(),WHITENESS);

	bufDC.SelectObject(pOldBrush);
	brush.DeleteObject();

	bufDC.SelectObject(pOldBmp);
	bufDC.DeleteDC();
}

void CMSPaint::movePointer(int x, int y)
{
	CClientDC dc(this);

	CDC bufDC;
	bufDC.CreateCompatibleDC(&dc);

	CBitmap* pOldBmp = (CBitmap*)bufDC.SelectObject(&m_bufBmp);

	x *= (m_region.Width()/320.f);
	y *= (m_region.Height()/240.f);

	// cam input ũ�� 320, 240 -> mspaint region ���� ��ȯ
	x *= (m_region.Width()/320.f);
	y *= (m_region.Height()/240.f);

	m_Mouse.MoveWindow(x, y, MOUSEWIDTH, MOUSEHEIGHT, FALSE);

	m_nX = x, m_nY = y;

	bufDC.SelectObject(pOldBmp);
	bufDC.DeleteDC();

	this->Invalidate();
}

void CMSPaint::clickPointer(int x, int y)
{
	CClientDC dc(this);

	CDC bufDC;
	bufDC.CreateCompatibleDC(&dc);

	CBitmap* pOldBmp = (CBitmap*)bufDC.SelectObject(&m_bufBmp);

	x *= (m_region.Width()/320.f);
	y *= (m_region.Height()/240.f);

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0,0,0));

	CPen* pOldPen = (CPen*)bufDC.SelectObject(&pen);

	bufDC.MoveTo(m_nX,m_nY);
	bufDC.LineTo(x,y);

	m_Mouse.MoveWindow(x, y, MOUSEWIDTH, MOUSEHEIGHT, FALSE);

	m_nX = x, m_nY = y;

	bufDC.SelectObject(pOldPen);
	pen.DeleteObject();

	bufDC.SelectObject(pOldBmp);
	bufDC.DeleteDC();

	this->Invalidate();
}

void CMSPaint::wheelPointer(int x, int y)
{

}

void CMSPaint::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CWnd::OnPaint()��(��) ȣ������ ���ʽÿ�.

	CDC bufDC;
	bufDC.CreateCompatibleDC(&dc);

	CBitmap* pOldBmp = (CBitmap*)bufDC.SelectObject(&m_bufBmp);

	dc.BitBlt(0, 0, m_region.Width(), m_region.Height(), &bufDC, 0, 0, SRCCOPY);

	bufDC.SelectObject(pOldBmp);
	bufDC.DeleteDC();
}