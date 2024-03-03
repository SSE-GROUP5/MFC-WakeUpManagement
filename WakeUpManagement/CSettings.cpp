// CSettings.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "CSettings.h"

extern CString global_wake_up_server_url;
extern BOOL getWakeUpServerMode();

// CSettings

IMPLEMENT_DYNCREATE(CSettings, CFormView)

CSettings::CSettings()
	: CFormView(IDD_SETTINGS)
{

}

CSettings::~CSettings()
{
}

void CSettings::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, wake_up_server_url);
}

BEGIN_MESSAGE_MAP(CSettings, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CSettings::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSettings diagnostics

#ifdef _DEBUG
void CSettings::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSettings::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSettings message handlers


void CSettings::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_Title_Font.CreatePointFont(200, _T("Calibri"));
	GetDlgItem(IDC_STATIC)->SetFont(&m_Title_Font);
	// TODO: Add your specialized code here and/or call the base class
	wake_up_server_url.SetWindowTextW(global_wake_up_server_url);
}



void CSettings::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString str;
	wake_up_server_url.GetWindowTextW(str);
	global_wake_up_server_url = str;
	AfxGetApp()->WriteProfileString(_T("Settings"), _T("GlobalVariable"), global_wake_up_server_url);
	if (getWakeUpServerMode()) {
		AfxMessageBox(_T("Successly saved!"), MB_ICONINFORMATION | MB_OK);
	}
	else {
		AfxMessageBox(TEXT("Invalid Wake Up Server URL!"));
	}
}

