#pragma once
#include "Cam.h"


// CSettingDlg ��ȭ �����Դϴ�.

class CSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSettingDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SETTINGDLG };

// �۾��Դϴ�.
public:
	void drawImage(IplImage* image);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
};
