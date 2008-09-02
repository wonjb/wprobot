// wpRobot(ver2.0)Dlg.cpp : ���� ����
//

#include "stdafx.h"
#include "wpRobot(ver2.0).h"
#include "wpRobot(ver2.0)Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CwpRobotver20Dlg ��ȭ ����




CwpRobotver20Dlg::CwpRobotver20Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CwpRobotver20Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CwpRobotver20Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CwpRobotver20Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CwpRobotver20Dlg �޽��� ó����

BOOL CwpRobotver20Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	if(!m_cam.Create(NULL, WS_CHILD|WS_VISIBLE, CRect(640+5,480+5-180,640+5+240,480+5), this))
		return FALSE;

	if(!m_paint.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0+5,0+5,640+5,480+5), this, 1))
		return FALSE;

	m_setting = new CSettingDlg;
	m_setting->Create(IDD_SETTINGDLG, this);

	this->GetClientRect(&m_region);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CwpRobotver20Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CwpRobotver20Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CwpRobotver20Dlg::RunSettingDlg()
{
//	m_setting.DoModal();
	m_setting->SetFocus();
}