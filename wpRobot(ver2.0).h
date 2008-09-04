// wpRobot(ver2.0).h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.


// CwpRobotver20App:
// 이 클래스의 구현에 대해서는 wpRobot(ver2.0).cpp을 참조하십시오.
//

class CwpRobotver20App : public CWinApp
{
public:
	CwpRobotver20App();

// 재정의입니다.
	public:
	virtual BOOL InitInstance();

// 작업입니다.
public:
	ULONG_PTR m_gdiplusToken;

// 구현입니다.

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CwpRobotver20App theApp;

enum COLOR{ RED = 1, BLUE = 2, PUPLE = 4, BLACK = 8, NOTHING = 0 };