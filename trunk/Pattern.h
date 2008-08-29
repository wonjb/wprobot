#pragma once
#include <vector>

class CPattern
{
public:
	enum MOTION { MOVE = 1, CLICK = 2, NOTHING = 0 };
public:
	CPattern(CvPoint center, std::vector<CvPoint> ptList);
	~CPattern(void);

	void getMotion();

private:
	CvPoint m_center;
	std::vector<CvPoint> m_ptList;
};
