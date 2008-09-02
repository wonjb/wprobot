// wpRobot(ver2.0)Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "wpRobot(ver2.0).h"
#include "wpRobot(ver2.0)Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CwpRobotver20Dlg 대화 상자




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


// CwpRobotver20Dlg 메시지 처리기

BOOL CwpRobotver20Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	if(!m_cam.Create(NULL, WS_CHILD|WS_VISIBLE, CRect(640+5,480+5-180,640+5+240,480+5), this))
		return FALSE;

	if(!m_paint.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0+5,0+5,640+5,480+5), this, 1))
		return FALSE;

	m_setting = new CSettingDlg;
	m_setting->Create(IDD_SETTINGDLG, this);

	this->GetClientRect(&m_region);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CwpRobotver20Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CwpRobotver20Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CwpRobotver20Dlg::RunSettingDlg()
{
//	m_setting.DoModal();
	m_setting->SetFocus();
}