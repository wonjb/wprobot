// wpRobot(ver2.0).h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CwpRobotver20App:
// �� Ŭ������ ������ ���ؼ��� wpRobot(ver2.0).cpp�� �����Ͻʽÿ�.
//

class CwpRobotver20App : public CWinApp
{
public:
	CwpRobotver20App();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �۾��Դϴ�.
public:
	ULONG_PTR m_gdiplusToken;

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CwpRobotver20App theApp;