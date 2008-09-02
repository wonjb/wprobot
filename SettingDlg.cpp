// SettingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wpRobot(ver2.0).h"
#include "SettingDlg.h"

#include "wpRobot(ver2.0)Dlg.h"


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
	ON_BN_CLICKED(IDC_CANCEL, &CSettingDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSettingDlg 메시지 처리기입니다.

void CSettingDlg::drawImage(IplImage* image)
{
	CClientDC dc(this);
	CvvImage view;
	view.CopyOf(image, image->nChannels*8);
	view.Show(dc.GetSafeHdc(), 5, 130, 240, 180);
}

void CSettingDlg::OnBnClickedCancel()
{
//	((CwpRobotver20Dlg*)(this->GetParent()))->UnRunSettingDlg();
	HANDLE hEvent = ::CreateEvent(NULL, FALSE, FALSE, _T("ENDSETTING"));
	::SetEvent(hEvent);

	this->ShowWindow(SW_HIDE);
}
