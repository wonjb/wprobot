// wpRobot(ver2.0)Dlg.h : 헤더 파일
//

#pragma once
#include "Cam.h"
#include "MSPaint.h"
#include "SettingDlg.h"


// CwpRobotver20Dlg 대화 상자
class CwpRobotver20Dlg : public CDialog
{
// 생성입니다.
public:
	CwpRobotver20Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WPROBOTVER20_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 작업입니다.
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
	

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void PostNcDestroy();
};
