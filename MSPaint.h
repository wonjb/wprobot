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

	void setPointer  (CHandPoint handPt);
	void movePointer (int x, int y);
	void clickPointer(int x, int y);
	void wheelPointer(int x, int y);

	void Initialize();

private:
	CMouse m_Mouse;			// mouse Window
	CRect  m_region;		// MSPaint ��ü ����
	CRect  m_redRegn, m_blueRegn, m_pupleRegn;		// ���� ����

	POINT  m_pastPt;		// �ٷ� �� ��ǥ�� �����ϱ� ���� ����
	COLORREF m_color;		// Pen Color

	CBitmap m_bufBmp;		// ������۸��� ���� Bitmap

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


