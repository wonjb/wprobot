#pragma once
#include "HandPoint.h"

class CTransmit
{
public:
	CTransmit();
	~CTransmit(void);

	void Initalize();
	void setWindowRegn();
	void setRobotRegn();

	void setHandPointer(CHandPoint handPt);
	void convertCIRCLE  (CRect regn);
	void convertRECT    (CRect regn);
	void convertTRIANGLE(CRect regn);
	void convertSTAR    (CRect regn);

	void Transmit();
	void transmitWindow();
	void transmitRobot();

private:
	CHandPoint m_handPt;
	CHandPoint m_pastPt;

	CRect      m_winRegn;
	CRect      m_robotRegn;

	COLOR      m_color;
	BOOL       m_startRobot;
};

struct PARAM
{
	int*  m_address;
	CRect m_rt;

	PARAM() {}
	PARAM(int* address, CRect rt) : m_address(address), m_rt(rt) {}
};

DWORD WINAPI CallBackDrawCIRCLE(LPVOID lpParam);