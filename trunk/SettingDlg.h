#pragma once
#include "Cam.h"


// CSettingDlg 대화 상자입니다.

class CSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSettingDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETTINGDLG };

// 작업입니다.
public:
	void drawImage(IplImage* image);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
};
