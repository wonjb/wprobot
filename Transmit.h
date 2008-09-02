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

	CRect      m_winRegn;
	CRect      m_robotRegn;

	COLOR      m_color;
public:
	BOOL       m_continue;
};
