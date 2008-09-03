#pragma once
#include "HandPoint.h"

class CTransmit
{
public:
	CTransmit();
	~CTransmit(void);

	void setHandPointer(CHandPoint handPt);
	void setWindowRegn();
	void setRobotRegn();

	void transmitWindow();
	void transmitRobot();

private:
	CHandPoint m_handPt;
	CHandPoint m_pastPt;

	CRect      m_winRegn;
	CRect      m_robotRegn;

	COLOR      m_color;
};
