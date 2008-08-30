#pragma once
#include "Mouse.h"
#include "ColorWnd.h"
#include "HandPoint.h"


// CMSPaint

class CMSPaint : public CWnd
{
	DECLARE_DYNAMIC(CMSPaint)

public:
	CMSPaint();
	virtual ~CMSPaint();

	void setPointer  (CHandPoint handPt);
	void movePointer (int x, int y);
	void clickPointer(int x, int y);
	void wheelPointer(int x, int y);

	void Initialize();

private:
	CMouse m_Mouse;			// mouse Window
	CColorWnd m_redWnd,  m_blueWnd,  m_pupleWnd;
	CRect     m_redRegn, m_blueRegn, m_pupleRegn;
	CRect  m_region;		// MSPaint 전체 영역
	int    m_nX, m_nY;		// 바로 전 좌표를 저장하기 위한 변수
	COLORREF m_color;		// Pen Color

	CBitmap m_bufBmp;		// 더블버퍼링을 위한 Bitmap

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
};


