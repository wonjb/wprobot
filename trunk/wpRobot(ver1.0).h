// wpRobot(ver1.0).h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CwpRobotver10App:
// �� Ŭ������ ������ ���ؼ��� wpRobot(ver1.0).cpp�� �����Ͻʽÿ�.
//

class CwpRobotver10App : public CWinApp
{
public:
	CwpRobotver10App();

public:
	ULONG_PTR m_gdiplusToken;

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CwpRobotver10App theApp;

