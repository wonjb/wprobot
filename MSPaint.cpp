// MSPaint.cpp : 구현 파일입니다.
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
, m_redRegn  (620, 20,640, 60)
, m_blueRegn (620, 80,640,120)
, m_pupleRegn(620,140,640,180)
, m_blackRegn(620,200,640,240)
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



// CMSPaint 메시지 처리기입니다.



int CMSPaint::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_Mouse.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,MOUSEWIDTH,MOUSEHEIGHT), this, 1))
		return -1;

	this->GetClientRect(&m_region);
	Initialize();
	InitializeRegn();

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

// 사용자 정의 함수

void CMSPaint::Initialize()
{
	CClientDC dc(this);

	CDC bufDC;
	bufDC.CreateCompatibleDC(&dc);

	m_bufBmp.CreateCompatibleBitmap(&dc, m_region.Width(), m_region.Height());

	bufDC.DeleteDC();
}

void CMSPaint::InitializeRegn()
{
	CClientDC dc(this);

	CDC bufDC;
	bufDC.CreateCompatibleDC(&dc);

	CBitmap* pOldBmp = (CBitmap*)bufDC.SelectObject(&m_bufBmp);

	CBrush brush, redBrush, blueBrush, pupleBrush, blackBrush;
	brush.CreateSolidBrush(RGB(220,235,235));
	redBrush.CreateSolidBrush  (RGB(255,115,115));
	blueBrush.CreateSolidBrush (RGB(115,115,255));
	pupleBrush.CreateSolidBrush(RGB(225, 50,225));
	blackBrush.CreateSolidBrush(RGB( 10, 10, 10));
	CBrush* pOldBrush = (CBrush*)bufDC.SelectObject(&brush);

	bufDC.FillRect(m_region, &brush);
	bufDC.FillRect(m_redRegn, &redBrush);
	bufDC.FillRect(m_blueRegn, &blueBrush);
	bufDC.FillRect(m_pupleRegn, &pupleBrush);
	bufDC.FillRect(m_blackRegn, &blackBrush);

	bufDC.SelectObject(pOldBrush);
	brush.DeleteObject();
	redBrush.DeleteObject();
	blueBrush.DeleteObject();
	pupleBrush.DeleteObject();
	blackBrush.DeleteObject();

	bufDC.SelectObject(pOldBmp);
	bufDC.DeleteDC();

	this->Invalidate();
}

void CMSPaint::setPointer(unsigned short* x, unsigned short* y)
{
	// cam input 크기 320, 240 -> mspaint region 으로 변환
	*x *= (m_region.Width()/240.f);
	*y *= (m_region.Height()/180.f);

	if(*x >= m_region.Width()-1)
		*x = m_region.Width()-1;
	if(*y >= m_region.Height()-1)
		*y = m_region.Height()-1;
}

void CMSPaint::movePointer(unsigned short x, unsigned short y)
{
	setPointer(&x, &y);

	CClientDC dc(this);

	CDC bufDC;
	bufDC.CreateCompatibleDC(&dc);
	CBitmap* pOldBmp = (CBitmap*)bufDC.SelectObject(&m_bufBmp);

	m_Mouse.MoveWindow(x, y, MOUSEWIDTH, MOUSEHEIGHT, FALSE);
	m_pastPt.x = x, m_pastPt.y = y;

	bufDC.SelectObject(pOldBmp);
	bufDC.DeleteDC();

	this->Invalidate();

	TCHAR buf[256] = {0,};
	swprintf(buf, sizeof(buf), _T("MOVE: %d\t%d\n"), x,y);
	::OutputDebugString(buf);
}

void CMSPaint::clickPointer(unsigned short x, unsigned short y)
{
	setPointer(&x, &y);

	CClientDC dc(this);

	CDC bufDC;
	bufDC.CreateCompatibleDC(&dc);
	CBitmap* pOldBmp = (CBitmap*)bufDC.SelectObject(&m_bufBmp);

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, m_color);
	CPen* pOldPen = (CPen*)bufDC.SelectObject(&pen);

	if(m_pastPt.x > m_region.Width() || m_pastPt.y > m_region.Height())
		m_pastPt.x = x, m_pastPt.y = y;

	bufDC.MoveTo(m_pastPt.x, m_pastPt.y);
	bufDC.LineTo(x,y);
	m_Mouse.MoveWindow(x, y, MOUSEWIDTH, MOUSEHEIGHT, FALSE);
	m_pastPt.x = x, m_pastPt.y = y;

	bufDC.SelectObject(pOldPen);
	pen.DeleteObject();

	bufDC.SelectObject(pOldBmp);
	bufDC.DeleteDC();

	this->Invalidate();

	TCHAR buf[256] = {0,};
	swprintf(buf, sizeof(buf), _T("CLICK: %d\t%d\n"), x,y);
	::OutputDebugString(buf);
}

COLOR CMSPaint::inColorRegn(int x, int y)
{
	POINT pt;
	pt.x = x*(m_region.Width()/240.f);
	pt.y = y*(m_region.Height()/180.f);

	if(pt.x >= m_region.Width()-1)
		pt.x = m_region.Width()-1;
	if(pt.y >= m_region.Height()-1)
		pt.y = m_region.Height()-1;
	
	if(m_redRegn.PtInRect(pt))
	{	/*((CwpRobotver20Dlg*)(this->GetParent()))->ColorAnimation(CwpRobotver20Dlg::RED);*/
		m_color = RGB(255,115,115);			return (RED);	}
	else if(m_blueRegn.PtInRect(pt))
	{	/*((CwpRobotver20Dlg*)(this->GetParent()))->ColorAnimation(CwpRobotver20Dlg::BLUE);*/
		m_color = RGB(115,115,255);			return (BLUE);	}
	else if(m_pupleRegn.PtInRect(pt))
	{	/*((CwpRobotver20Dlg*)(this->GetParent()))->ColorAnimation(CwpRobotver20Dlg::PUPLE);*/
		m_color = RGB(225, 50,225);			return (PUPLE);	}
	else if(m_blackRegn.PtInRect(pt))
	{	/*((CwpRobotver20Dlg*)(this->GetParent()))->ColorAnimation(CwpRobotver20Dlg::BLACK);*/
		m_color = RGB( 10, 10, 10);			return (BLACK);	}
	else
	{	return (NOTHING);	}
}