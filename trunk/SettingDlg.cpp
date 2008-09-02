// SettingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wpRobot(ver2.0).h"
#include "SettingDlg.h"

#include "Cam.h"


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
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CSettingDlg 메시지 처리기입니다.

BOOL CSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSettingDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.

	m_view.Show(dc.GetSafeHdc(), 0, 0, 240, 180);
}

void CSettingDlg::saveMotion(int time)
{
	CRect rt(0,0,240,180);
// 	for(int i = 0; i < time; ++i)
// 	{
		m_view.CopyOf(g_transform.m_transImage, g_transform.m_transImage->nChannels*8);
		this->Invalidate();
 		::Sleep(1);
// 	}
}
void CSettingDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	for(int i = 0; i < 50; ++i)
		saveMotion(50);

	CDialog::OnLButtonDown(nFlags, point);
}
