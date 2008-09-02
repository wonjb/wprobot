#pragma once
#include "HandPoint.h"

class CTransmit
{
public:
	CTransmit(CHandPoint handPt);
	~CTransmit(void);

	void transmitWindow();
	void transmitRobot();

private:
	CHandPoint m_handPt;
};
