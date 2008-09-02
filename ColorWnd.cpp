// ColorWnd.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "wpRobot(ver2.0).h"
#include "ColorWnd.h"

#define WS_EX_LAYERED           0x00080000
#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002
#define ULW_COLORKEY            0x00000001
#define ULW_ALPHA               0x00000002
#define ULW_OPAQUE              0x00000004

typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);

// CColorWnd

IMPLEMENT_DYNAMIC(CColorWnd, CWnd)

CColorWnd::CColorWnd(COLORREF color)
: m_color(color)
, m_drawRegn(0,0,10,60)
, m_allRegn (0,0,200,60)
{

}

CColorWnd::~CColorWnd()
{
	this->KillTimer(1);
}


BEGIN_MESSAGE_MAP(CColorWnd, CWnd)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CColorWnd �޽��� ó�����Դϴ�.

BOOL CColorWnd::OnEraseBkgnd(CDC* pDC)
{
	CBrush brush;
	brush.CreateSolidBrush(m_color);

	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brush);

	pDC->FillRect(m_drawRegn, &brush);

	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();

	return TRUE;
}

void CColorWnd::Animation()
{
	TCHAR buf[256] = {0,};
	for(int i = m_drawRegn.Width(); i < m_allRegn.Width(); i+=10)
	{
		m_drawRegn.SetRect(0,0,i,60);
		this->Invalidate();

		swprintf(buf, _T("%d\n"), i);
		::OutputDebugString(buf);
		::Sleep(1);
	}
}

void CColorWnd::SetTransparency(int percent)
{
	SLWA pSetLayeredWindowAttributes = NULL;  // �Լ������� ����, �ʱ�ȭ.
	HINSTANCE hmodUSER32 = LoadLibrary(_T("USER32.DLL")); // �ν��Ͻ� ����.
	pSetLayeredWindowAttributes=(SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");
	//�Լ������� ����.
	HWND hwnd = this->m_hWnd; //���̾�α��� �ڵ� ����.
	SetWindowLong(hwnd, GWL_EXSTYLE,GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	pSetLayeredWindowAttributes(hwnd, 0, (255 * percent) / 100, LWA_ALPHA);
}
