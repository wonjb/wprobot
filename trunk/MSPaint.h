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
	CRect  m_region;		// MSPaint ��ü ����
	CRect  m_redRegn, m_blueRegn, m_pupleRegn, m_blackRegn;		// ���� ����

	COLORREF m_color;		// Pen Color

	CBitmap m_bufBmp;		// ������۸��� ���� Bitmap

	POINT m_pastPt;	// �ٷ� �� ��ǥ�� �����ϱ� ���� ����

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


