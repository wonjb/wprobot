// wpRobot(ver1.0)Dlg.h : ��� ����
//

#pragma once
#include "Cam.h"
#include "MSPaint.h"



// CwpRobotver10Dlg ��ȭ ����
class CwpRobotver10Dlg : public CDialog
{
// �����Դϴ�.
public:
	CwpRobotver10Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_WPROBOTVER10_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

/////////////////////////////////////
public:
	CCam m_cam;
	CMSPaint m_paint;

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
