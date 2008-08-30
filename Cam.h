#pragma once


// CCam

class CCam : public CStatic
{
	DECLARE_DYNAMIC(CCam)

public:
	CCam();
	virtual ~CCam();


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};


void cvcamCallBack(IplImage* image);
