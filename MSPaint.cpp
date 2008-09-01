// MSPaint.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "wpRobot(ver2.0).h"
#include "MSPaint.h"

#include "wpRobot(ver2.0)Dlg.h"


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
, m_redRegn  (620, 20,640, 80)
, m_blueRegn (620,100,640,160)
, m_pupleRegn(620,180,640,240)
{

}

CMSPaint::~CMSPaint()
{
	m_bufBmp.DeleteObject();
}


BEGIN_MESSAGE_MAP(CMSPaint, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
//	ON_WM_PAINT()
END_MESSAGE_MAP()



// CMSPaint �޽��� ó�����Դϴ�.



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

BOOL CMSPaint::OnEraseBkgnd(CDC* pDC)
{
	CDC bufDC;
	bufDC.CreateCompatibleDC(pDC);

	CBitmap* pOldBmp = (CBitmap*)bufDC.SelectObject(&m_bufBmp);

	pDC->BitBlt(0, 0, m_region.Width(), m_region.Height(), &bufDC, 0, 0, SRCCOPY);

	bufDC.SelectObject(pOldBmp);
	bufDC.DeleteDC();

	return TRUE;
}

// ����� ���� �Լ�

void CMSPaint::Initialize()
{
	CClientDC dc(this);

	CDC bufDC;
	bufDC.CreateCompatibleDC(&dc);

	m_bufBmp.CreateCompatibleBitmap(&dc, m_region.Width(), m_region.Height());
	CBitmap* pOldBmp = (CBitmap*)bufDC.SelectObject(&m_bufBmp);

	CBrush brush, redBrush, blueBrush, pupleBrush;
	brush.CreateSolidBrush(RGB(220,235,235));
	redBrush.CreateSolidBrush(RGB(255,115,115));
	blueBrush.CreateSolidBrush(RGB(115,115,255));
	pupleBrush.CreateSolidBrush(RGB(225,50,225));
	CBrush* pOldBrush = (CBrush*)bufDC.SelectObject(&brush);

	bufDC.FillRect(m_region, &brush);
	bufDC.FillRect(m_redRegn, &redBrush);
	bufDC.FillRect(m_blueRegn, &blueBrush);
	bufDC.FillRect(m_pupleRegn, &pupleBrush);

	bufDC.SelectObject(pOldBrush);
	brush.DeleteObject();
	redBrush.DeleteObject();
	blueBrush.DeleteObject();
	pupleBrush.DeleteObject();

	bufDC.SelectObject(pOldBmp);
	bufDC.DeleteDC();
}

// void CMSPaint::setPointer(CHandPoint handPt)
// {
// 	// cam input ũ�� 320, 240 -> mspaint region ���� ��ȯ
// 	m_pt.x = handPt.m_nX*(m_region.Width()/240.f);
// 	m_pt.y = handPt.m_nY*(m_region.Height()/180.f);
// 
// 	if(m_pt.x >= m_region.Width()-1)
// 		m_pt.x = m_region.Width()-1;
// 	if(m_pt.y >= m_region.Height()-1)
// 		m_pt.y = m_region.Height()-1;
// 
// 	TCHAR buf[256] = {0,};
// 	swprintf(buf, _T("%d\t%d\t%s\t%s\n"), m_pt.x, m_pt.y, handPt.m_bClick ? _T("TRUE") : _T("FALSE"), handPt.m_bWheel ? _T("TRUE") : _T("FALSE"));
// 	::OutputDebugString(buf);
// 
// //   	if(m_redRegn.PtInRect(pt))
// // 	{	((CwpRobotver20Dlg*)(this->GetParent()))->ColorAnimation(CwpRobotver20Dlg::RED);	handPt.m_bClick = FALSE;	}
// // 	else if(m_blueRegn.PtInRect(pt))
// // 	{	((CwpRobotver20Dlg*)(this->GetParent()))->ColorAnimation(CwpRobotver20Dlg::BLUE);	handPt.m_bClick = FALSE;	}
// // 	else if(m_pupleRegn.PtInRect(pt))
// // 	{	((CwpRobotver20Dlg*)(this->GetParent()))->ColorAnimation(CwpRobotver20Dlg::PUPLE);	handPt.m_bClick = FALSE;	}
// 
// 	if(handPt.m_bClick)
// 		clickPointer();
// // 	else if(handPt.m_bWheel)
// // 		wheelPointer();
// 	else
// 		movePointer();
// }

void CMSPaint::convertScreenToMSPaintPt(int* x, int* y)
{
	*x *= (m_region.Width()/240.f);
	*y *= (m_region.Height()/180.f);

	if(*x >= m_region.Width()-1)
		*x = m_region.Width()-1;
	if(*y >= m_region.Height()-1)
		*y = m_region.Height()-1;
}

void CMSPaint::movePointer(int x, int y)
{
	CClientDC dc(this);

	CDC bufDC;
	bufDC.CreateCompatibleDC(&dc);
	CBitmap* pOldBmp = (CBitmap*)bufDC.SelectObject(&m_bufBmp);

	convertScreenToMSPaintPt(&x, &y);

	m_Mouse.MoveWindow(x, y, MOUSEWIDTH, MOUSEHEIGHT, FALSE);
	m_pastPt.x = x, m_pastPt.y = y;

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

	convertScreenToMSPaintPt(&x, &y);

	if(m_pastPt.x > m_region.Width() || m_pastPt.y > m_region.Height())
		m_pastPt.x = x, m_pastPt.y = y;

	bufDC.MoveTo(m_pastPt.x, m_pastPt.y);
	bufDC.LineTo(x, y);
	m_Mouse.MoveWindow(x, y, MOUSEWIDTH, MOUSEHEIGHT, FALSE);
	m_pastPt.x = x, m_pastPt.y = y;

	bufDC.SelectObject(pOldPen);
	pen.DeleteObject();

	bufDC.SelectObject(pOldBmp);
	bufDC.DeleteDC();

	this->Invalidate();
}

// void CMSPaint::wheelPointer()
// {
// 	((CwpRobotver20Dlg*)(this->GetParent()))->m_setting.DoModal();
// }

// void CMSPaint::OnPaint()
// {
// 	CPaintDC dc(this); // device context for painting
// 	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
// 	// �׸��� �޽����� ���ؼ��� CWnd::OnPaint()��(��) ȣ������ ���ʽÿ�.
// 
// 	CDC bufDC;
// 	bufDC.CreateCompatibleDC(&dc);
// 
// 	CBitmap* pOldBmp = (CBitmap*)bufDC.SelectObject(&m_bufBmp);
// 
// 	dc.BitBlt(0, 0, m_region.Width(), m_region.Height(), &bufDC, 0, 0, SRCCOPY);
// 
// 	bufDC.SelectObject(pOldBmp);
// 	bufDC.DeleteDC();
// }