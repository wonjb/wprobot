// SettingDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "wpRobot(ver2.0).h"
#include "SettingDlg.h"

#include "Cam.h"


// CSettingDlg ��ȭ �����Դϴ�.

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


// CSettingDlg �޽��� ó�����Դϴ�.

BOOL CSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CSettingDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialog::OnPaint()��(��) ȣ������ ���ʽÿ�.

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
