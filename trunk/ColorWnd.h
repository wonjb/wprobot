#pragma once


// CColorWnd

class CColorWnd : public CWnd
{
	DECLARE_DYNAMIC(CColorWnd)

public:
	CColorWnd(COLORREF color);
	virtual ~CColorWnd();

	void Animation();
	void SetTransparency(int percent);

private:
	COLORREF m_color;
	CRect    m_drawRegn;
	CRect    m_allRegn;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


