// CWakeUpSettingDlg.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "CWakeUpSettingDlg.h"


// CWakeUpSettingDlg

IMPLEMENT_DYNCREATE(CWakeUpSettingDlg, CFormView)

CWakeUpSettingDlg::CWakeUpSettingDlg()
	: CFormView(IDD_WAKE_UP_SETTING)
{

}

CWakeUpSettingDlg::~CWakeUpSettingDlg()
{
}

void CWakeUpSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_wake_up_setting_list);
}

BEGIN_MESSAGE_MAP(CWakeUpSettingDlg, CFormView)
END_MESSAGE_MAP()


// CWakeUpSettingDlg diagnostics

#ifdef _DEBUG
void CWakeUpSettingDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CWakeUpSettingDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CWakeUpSettingDlg message handlers


void CWakeUpSettingDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	MoveWindow(200, 0, 1000, 700);
	// Creates a 12-point-Courier-font
	m_Title_Font.CreatePointFont(120, _T("Calibri"));
	GetDlgItem(IDC_STATIC)->SetFont(&m_Title_Font);


	m_Table_Font.CreatePointFont(100, _T("Calibri"));
	m_wake_up_setting_list.SetFont(&m_Table_Font);

	// TODO: Add your specialized code here and/or call the base class
	//CString str[] = { TEXT("Controller"), TEXT("Trigger Controller") };
	//for (int i = 0; i < 2; i++) {
	//	//title
	//	m_wake_up_setting_list.InsertColumn(i, str[i], LVCFMT_LEFT, 120);
	//}
	m_wake_up_setting_list.InsertColumn(0, TEXT("Controller"), LVCFMT_LEFT, 120);
	m_wake_up_setting_list.InsertColumn(1, TEXT("Trigger Controller"), LVCFMT_LEFT, 160);
	m_wake_up_setting_list.InsertColumn(2, TEXT("Matter Device"), LVCFMT_LEFT, 200);
	m_wake_up_setting_list.InsertColumn(3, TEXT("Matter Action"), LVCFMT_LEFT, 440);

	CString NameArray[10] = { TEXT("budz"), TEXT("pain"), TEXT("konan"), TEXT("nagato"), TEXT("itachi"), TEXT("tobi"), TEXT("madara"), TEXT("naruto"), TEXT("danzou"), TEXT("kakashi") };

	for (int i = 0; i < 3; i++) {
		int j = 0;
		CString controller;
		controller.Format(TEXT("Controller_%d"), i);
		CString matter_device;
		matter_device.Format(TEXT("Matter_device_%d"), i);
		m_wake_up_setting_list.InsertItem(i, controller);
		m_wake_up_setting_list.SetItemText(i, ++j, TEXT("Tap"));
		m_wake_up_setting_list.SetItemText(i, ++j, matter_device);
		m_wake_up_setting_list.SetItemText(i, ++j, TEXT("2 times: turn on; 3 times: turn off; 4 times: Toggle"));
	}
	for (int i = 3; i < 7; i++) {
		int j = 0;
		CString controller;
		controller.Format(TEXT("Controller_%d"), i);
		CString matter_device;
		matter_device.Format(TEXT("Matter_device_%d"), i);
		m_wake_up_setting_list.InsertItem(i, controller);
		m_wake_up_setting_list.SetItemText(i, ++j, TEXT("Blink"));
		m_wake_up_setting_list.SetItemText(i, ++j, matter_device);
		m_wake_up_setting_list.SetItemText(i, ++j, TEXT("2 times: turn on; 3 times: turn off; 4 times: Toggle"));
	}

	//property (show table lines)
	m_wake_up_setting_list.SetExtendedStyle(m_wake_up_setting_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}
