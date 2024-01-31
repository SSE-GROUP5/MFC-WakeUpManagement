// CWakeUpSettingDlg.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "CWakeUpSettingDlg.h"

#include <cpr/cpr.h>
#include <iostream>
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
	m_wake_up_setting_list.InsertColumn(0, TEXT("Controller ID"), LVCFMT_LEFT, 320);
	m_wake_up_setting_list.InsertColumn(1, TEXT("Trigger Controller"), LVCFMT_LEFT, 320);
	m_wake_up_setting_list.InsertColumn(2, TEXT("Matter Device ID"), LVCFMT_LEFT, 320);
	m_wake_up_setting_list.InsertColumn(3, TEXT("Matter Action"), LVCFMT_LEFT, 315);

	cpr::Response r = cpr::Get(cpr::Url{ "http://localhost:5001/signals" });
	nlohmann::json jsonList = nlohmann::json::parse(r.text);

	if (jsonList.contains("signals")) {
		// Access the array under the "signals" key
		const auto& signalsArray = jsonList["signals"];

		// Iterate through each element in the array
		for (const auto& item : signalsArray) {
			// Access properties inside each element
			CString interactive_action = CString(item["interactive_action"].get<std::string>().c_str());
			CString interactive_id = CString(item["interactive_id"].get<std::string>().c_str());
			CString target_action = CString(item["target_action"].get<std::string>().c_str());
			CString target_id = CString(item["target_id"].get<std::string>().c_str());

			// Add the data to the list control
			int index = m_wake_up_setting_list.InsertItem(m_wake_up_setting_list.GetItemCount(), interactive_id);
			m_wake_up_setting_list.SetItemText(index, 1, interactive_action);
			m_wake_up_setting_list.SetItemText(index, 2, target_id);
			m_wake_up_setting_list.SetItemText(index, 3, target_action);
		}
	}

	//property (show table lines)
	m_wake_up_setting_list.SetExtendedStyle(m_wake_up_setting_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


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
