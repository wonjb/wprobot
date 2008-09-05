#pragma once
#include "HandPoint.h"
#include "SendRobot.h"

struct PARAM
{
	void* m_address;
	CRect m_rt;

	PARAM() {}
	PARAM(void* address, CRect rt) : m_address(address), m_rt(rt) {}
};

class CTransmit
{
public:
	CTransmit();
	~CTransmit(void);

	void Initalize();
	void setWindowRegn();

	void setHandPointer(CHandPoint handPt);
	void convertCIRCLE  (CRect regn);
	void convertRECT    (CRect regn);
	void convertTRIANGLE(CRect regn);
	void convertSTAR    (CRect regn);

	void Transmit();
	void transmitWindow();

private:
	CHandPoint m_handPt;
	CHandPoint m_pastPt;
	CSendRobot m_robot;
	PARAM      m_pParam;

	CRect      m_winRegn;
};

DWORD WINAPI CallBackDrawCIRCLE  (LPVOID lpParam);
DWORD WINAPI CallBackDrawRECT    (LPVOID lpParam);
DWORD WINAPI CallBackDrawTRIANGLE(LPVOID lpParam);
DWORD WINAPI CallBackDrawSTAR    (LPVOID lpParam);