// wpRobot(ver2.0)Dlg.h : ��� ����
//

#pragma once
#include "Cam.h"
#include "MSPaint.h"
#include "SettingDlg.h"


// CwpRobotver20Dlg ��ȭ ����
class CwpRobotver20Dlg : public CDialog
{
// �����Դϴ�.
public:
	CwpRobotver20Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_WPROBOTVER20_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

// �۾��Դϴ�.
public:
	enum COLOR{ RED = 1, BLUE = 2, PUPLE = 4, BLACK = 8, NOTHING = 0 };
public:
	CCam         m_cam;
	CMSPaint     m_paint;
	CSettingDlg* m_setting;

	CRect    m_region;

	void ColorAnimation(COLOR color);
	void RunSettingDlg();
	void UnRunSettingDlg();
	

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void PostNcDestroy();
};
