// CWakeUpSettingDlg.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "CWakeUpSettingDlg.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>


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
	ON_BN_CLICKED(IDC_BUTTON1, &CWakeUpSettingDlg::OnBnClickedButton1)
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

	//for (int i = 0; i < 3; i++) {
	//	int j = 0;
	//	CString controller;
	//	controller.Format(TEXT("Controller_%d"), i);
	//	CString matter_device;
	//	matter_device.Format(TEXT("Matter_device_%d"), i);
	//	m_wake_up_setting_list.InsertItem(i, controller);
	//	m_wake_up_setting_list.SetItemText(i, ++j, TEXT("Tap"));
	//	m_wake_up_setting_list.SetItemText(i, ++j, matter_device);
	//	m_wake_up_setting_list.SetItemText(i, ++j, TEXT("2 times: turn on; 3 times: turn off; 4 times: Toggle"));
	//}
	//for (int i = 3; i < 7; i++) {
	//	int j = 0;
	//	CString controller;
	//	controller.Format(TEXT("Controller_%d"), i);
	//	CString matter_device;
	//	matter_device.Format(TEXT("Matter_device_%d"), i);
	//	m_wake_up_setting_list.InsertItem(i, controller);
	//	m_wake_up_setting_list.SetItemText(i, ++j, TEXT("Blink"));
	//	m_wake_up_setting_list.SetItemText(i, ++j, matter_device);
	//	m_wake_up_setting_list.SetItemText(i, ++j, TEXT("2 times: turn on; 3 times: turn off; 4 times: Toggle"));
	//}

	//property (show table lines)
	//m_wake_up_setting_list.SetExtendedStyle(m_wake_up_setting_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//int nCol = 1;    // to search in the second column (like your question)
	//CString m_SearchThisItemText = _T("Banana");

	//for (int i = 0; i < m_wake_up_setting_list.GetItemCount(); ++i)
	//{
	//	CString szText = m_wake_up_setting_list.GetItemText(i, nCol);
	//	if (szText == m_SearchThisItemText)
	//	{
	//		// found it - do something
	//		break;
	//	}
	//}
}


void CWakeUpSettingDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString controller;
	CString trigger_controller;
	CString matter_devices;
	CString matter_action;
	GetDlgItem(IDC_COMBO1)->GetWindowTextW(controller);
	GetDlgItem(IDC_COMBO2)->GetWindowTextW(trigger_controller);
	GetDlgItem(IDC_COMBO3)->GetWindowTextW(matter_devices);
	GetDlgItem(IDC_COMBO4)->GetWindowTextW(matter_action);
	int nitem = m_wake_up_setting_list.InsertItem(0, controller);
	m_wake_up_setting_list.SetItemText(nitem, 1, trigger_controller);
	m_wake_up_setting_list.SetItemText(nitem, 2, matter_devices);
	m_wake_up_setting_list.SetItemText(nitem, 3, matter_action);

	nlohmann::json jsonList;

	// Convert CString to std::string
	std::string std_controller = CT2A(controller.GetBuffer());
	std::string std_trigger_controller = CT2A(trigger_controller.GetBuffer());
	std::string std_matter_devices = CT2A(matter_devices.GetBuffer());
	std::string std_matter_action = CT2A(matter_action.GetBuffer());

	// Add key-value pairs to the JSON object
	jsonList[std_controller] = { std_trigger_controller, std_matter_devices, std_matter_action };

	// Write JSON to file
    //std::ofstream file("output.json");
    //file << std::setw(4) << jsonList << std::endl;

	std::ofstream file("wake_up_setting_data.json", std::ios::app);
	file << std::setw(4) << jsonList << std::endl;

}
