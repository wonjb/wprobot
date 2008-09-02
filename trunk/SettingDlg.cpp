// SettingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wpRobot(ver2.0).h"
#include "SettingDlg.h"


// CSettingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSettingDlg, CDialog)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDlg::IDD, pParent)
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
END_MESSAGE_MAP()


// CSettingDlg 메시지 처리기입니다.
