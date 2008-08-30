#pragma once
#include "HandPoint.h"

class CPattern
{
public:
	enum MOTION { MOVE = 1, CLICK = 2, NOTHING = 0 };
public:
	CPattern(CvPoint center, CHandPoint handPt);
	~CPattern(void);

	void getMotion();

private:
	CvPoint    m_center;
	CHandPoint m_handPt;
};
