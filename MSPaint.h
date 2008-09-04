#pragma once
#include "Mouse.h"
#include "HandPoint.h"


// CMSPaint

class CMSPaint : public CWnd
{
	DECLARE_DYNAMIC(CMSPaint)

public:
	CMSPaint();
	virtual ~CMSPaint();

	void setPointer  (unsigned short* x, unsigned short* y);
	void movePointer ();
	void movePointer (unsigned short x, unsigned short y);
	void clickPointer();
	void clickPointer(unsigned short x, unsigned short y);

	void Initialize();
	void InitializeRegn();
	CRect getMSPaintRegn()	{ return m_region; }

	COLOR inColorRegn(int x, int y);

private:
	CMouse m_Mouse;			// mouse Window
	CRect  m_region;		// MSPaint 전체 영역
	CRect  m_redRegn, m_blueRegn, m_pupleRegn, m_blackRegn;		// 색상 영역

	COLORREF m_color;		// Pen Color

	CBitmap m_bufBmp;		// 더블버퍼링을 위한 Bitmap

	POINT m_pastPt;	// 바로 전 좌표를 저장하기 위한 변수

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


