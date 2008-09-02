#pragma once
#include "TransformImage.h"
#include "Transmit.h"

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

extern CTransformImage g_transform;
extern CTransmit       g_transmit;
void cvcamCallBack(IplImage* image);
