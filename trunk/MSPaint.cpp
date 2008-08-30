// MSPaint.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "wpRobot(ver2.0).h"
#include "MSPaint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable:4244)

#define MOUSEWIDTH		20
#define MOUSEHEIGHT		20
// CMSPaint

IMPLEMENT_DYNAMIC(CMSPaint, CWnd)

CMSPaint::CMSPaint()
: m_color(RGB(0,0,0))
, m_redWnd(RGB(255,115,115))
, m_blueWnd(RGB(115,115,255))
, m_pupleWnd(RGB(225,50,225))
, m_redRegn(620,20,700,80)
, m_blueRegn(620,100,700,160)
, m_pupleRegn(620,180,700,240)
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



int CMSPaint::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_Mouse.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,MOUSEWIDTH,MOUSEHEIGHT), this, 1))
		return -1;

	if(!m_redWnd.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, m_redRegn, this, 1))
		return -1;
	if(!m_blueWnd.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, m_blueRegn, this, 1))
		return -1;
	if(!m_pupleWnd.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, m_pupleRegn, this, 1))
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
	brush.CreateSolidBrush(RGB(220,235,235));
	
	CBrush* pOldBrush = (CBrush*)bufDC.SelectObject(&brush);

	bufDC.FillRect(m_region, &brush);

	bufDC.SelectObject(pOldBrush);
	brush.DeleteObject();

	bufDC.SelectObject(pOldBmp);
	bufDC.DeleteDC();
}

void CMSPaint::setPointer(CHandPoint handPt)
{
	// cam input ũ�� 320, 240 -> mspaint region ���� ��ȯ
	POINT pt;	
	pt.x = handPt.m_nX*(m_region.Width()/240.f);
	pt.y = handPt.m_nY*(m_region.Height()/180.f);

	if(m_redRegn.PtInRect(pt))
	{	m_redWnd.Animation();	return;	}

	if(handPt.m_bClick)
		clickPointer(pt.x,pt.y);
// 	else if(handPt.m_bWheel)
// 		wheelPointer(handPt.x, handPt.y);
	else
		movePointer(pt.x,pt.y);
}

void CMSPaint::movePointer(int x, int y)
{
	CClientDC dc(this);

	CDC bufDC;
	bufDC.CreateCompatibleDC(&dc);

	CBitmap* pOldBmp = (CBitmap*)bufDC.SelectObject(&m_bufBmp);

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

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, m_color);

	CPen* pOldPen = (CPen*)bufDC.SelectObject(&pen);

	if(m_nX > m_region.Width() || m_nY > m_region.Height())
		m_nX = x, m_nY = y;

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