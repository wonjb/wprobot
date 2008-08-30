#pragma once

// CMouse

class CMouse : public CWnd
{
	DECLARE_DYNAMIC(CMouse)

public:
	CMouse();
	virtual ~CMouse();

private:
	Image* m_mouseImage;	

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


