// wpRobot(ver1.0)Dlg.h : 헤더 파일
//

#pragma once
#include "Cam.h"
#include "MSPaint.h"



// CwpRobotver10Dlg 대화 상자
class CwpRobotver10Dlg : public CDialog
{
// 생성입니다.
public:
	CwpRobotver10Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WPROBOTVER10_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

/////////////////////////////////////
public:
	CCam m_cam;
	CMSPaint m_paint;

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
