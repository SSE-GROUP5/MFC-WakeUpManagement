// CWakeUpSettingDlg.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "CWakeUpSettingDlg.h"
#include "CDeleteSignal.h"
#include "CAddSignal.h"
#include "CEditSignal.h"

#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>

extern CString global_wake_up_server_url;
extern BOOL getWakeUpServerMode();

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
	DDX_Control(pDX, IDC_COMBO5, cb_users);
	DDX_Control(pDX, IDC_COMBO2, cb_triggers);
	DDX_Control(pDX, IDC_COMBO1, cb_targets);
}

BEGIN_MESSAGE_MAP(CWakeUpSettingDlg, CFormView)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CWakeUpSettingDlg::OnCbnSelchangeCombo5)
	ON_BN_CLICKED(IDC_BUTTON1, &CWakeUpSettingDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CWakeUpSettingDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CWakeUpSettingDlg::OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CWakeUpSettingDlg::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CWakeUpSettingDlg::OnCbnSelchangeCombo1)
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

	CRect rect;
	m_wake_up_setting_list.GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_wake_up_setting_list.MoveWindow(rect.left, rect.top, 1454, 750, TRUE);

	m_wake_up_setting_list.InsertColumn(1, TEXT("Trigger Name"), LVCFMT_CENTER, 350);
	m_wake_up_setting_list.InsertColumn(2, TEXT("Trigger Action"), LVCFMT_CENTER, 250);
	m_wake_up_setting_list.InsertColumn(3, TEXT("Trigger Value"), LVCFMT_CENTER, 250);
	m_wake_up_setting_list.InsertColumn(4, TEXT("Target ID"), LVCFMT_CENTER, 350);
	m_wake_up_setting_list.InsertColumn(5, TEXT("Target Action"), LVCFMT_CENTER, 250);
	m_wake_up_setting_list.SetExtendedStyle(m_wake_up_setting_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	checkWakeUpServerMode();
	// Creates a 12-point-Courier-font
	m_Title_Font.CreatePointFont(180, _T("Calibri"));
	GetDlgItem(IDC_STATIC)->SetFont(&m_Title_Font);
}

void CWakeUpSettingDlg::checkWakeUpServerMode()
{
	if (getWakeUpServerMode()) {
		// Update the control after the HTTP response is checked
		SetDlgItemTextW(IDC_WAKE_UP_SERVER, TEXT("Wake Up Server: ON"));
		GetRequestUsers();
		GetRequestTriggers();
		GetRequestTargets();
		GetRequestForSignals();
	}
	else {
		SetDlgItemTextW(IDC_WAKE_UP_SERVER, TEXT("Wake Up Server: OFF"));
	}
}

void CWakeUpSettingDlg::GetRequestSignalsForDefaultUser()
{
	m_wake_up_setting_list.DeleteAllItems();
	std::string wake_up_server_url = CT2A(global_wake_up_server_url);
	cpr::Response r = cpr::Get(cpr::Url{ wake_up_server_url + "signals" });
	nlohmann::json jsonList = nlohmann::json::parse(r.text);

	CString trigger_name;
	int selectedIndex_trigger = cb_triggers.GetCurSel();
	if (selectedIndex_trigger != CB_ERR) {
		cb_triggers.GetLBText(selectedIndex_trigger, trigger_name);
		// Now trigger_name contains the selected string
	}

	CString target_name;
	int selectedIndex_target = cb_targets.GetCurSel();
	if (selectedIndex_target != CB_ERR) {
		cb_targets.GetLBText(selectedIndex_target, target_name);
		// Now trigger_name contains the selected string
	}

	if (jsonList.contains("signals")) {
		// Access the array under the "signals" key
		const auto& signalsArray = jsonList["signals"];

		// Iterate through each element in the array
		for (const auto& item : signalsArray) {
			// Access properties inside each element
			if (item["user_id"].is_null() ) 
			{
				if (target_name != "All") {
					if (target_name != CString(item["target_id"].get<std::string>().c_str())) {
						continue;
					}
				}
				if (trigger_name != "All") {
					if ( trigger_name != CString(item["trigger_name"].get<std::string>().c_str())) {
						continue;
					}
				}
				CString trigger_name = CString(item["trigger_name"].get<std::string>().c_str());
				CString trigger_action = CString(item["trigger_action"].get<std::string>().c_str());
				CString target_id = CString(item["target_id"].get<std::string>().c_str());
				CString target_action = CString(item["target_action"].get<std::string>().c_str());
				CString trigger_num_actions;
				try {
					trigger_num_actions = CString(item["trigger_num_actions"].get<std::string>().c_str());
				}
				catch (...) {
					trigger_num_actions.Format(_T("%d"), item["trigger_num_actions"].get<int>());
				}
				
				// Add the data to the list control
				int index = m_wake_up_setting_list.InsertItem(m_wake_up_setting_list.GetItemCount(), trigger_name);
				m_wake_up_setting_list.SetItemText(index, 1, trigger_action);
				m_wake_up_setting_list.SetItemText(index, 2, trigger_num_actions);
				m_wake_up_setting_list.SetItemText(index, 3, target_id);
				m_wake_up_setting_list.SetItemText(index, 4, target_action);
			}
		}
	}

}

void CWakeUpSettingDlg::GetRequestSignalsForAUser(CString str)
{
	m_wake_up_setting_list.DeleteAllItems();
	CString user_id;
	int colonIndex = str.Find(_T("#"));
	if (colonIndex != -1) {
		user_id = str.Mid(colonIndex + 1);
	}

	CString trigger_name;
	int selectedIndex_trigger = cb_triggers.GetCurSel();
	if (selectedIndex_trigger != CB_ERR) {
		cb_triggers.GetLBText(selectedIndex_trigger, trigger_name);
		// Now trigger_name contains the selected string
	}

	CString target_name;
	int selectedIndex_target = cb_targets.GetCurSel();
	if (selectedIndex_target != CB_ERR) {
		cb_targets.GetLBText(selectedIndex_target, target_name);
		// Now trigger_name contains the selected string
	}

	std::string user_id_str = CT2A(user_id);
	std::string wake_up_server_url = CT2A(global_wake_up_server_url);
	cpr::Response r_signals_for_user = cpr::Get(cpr::Url{ wake_up_server_url + "signals/users/" + user_id_str });
	nlohmann::json jsonList_signals_for_user = nlohmann::json::parse(r_signals_for_user.text);

	if (jsonList_signals_for_user.contains("signals")) {
		// Access the array under the "signals" key
		const auto& signalsArray = jsonList_signals_for_user["signals"];

		// Iterate through each element in the array
		for (const auto& item : signalsArray) {
			// Access properties inside each element
			if (target_name != "All") {
				if (target_name != CString(item["target_id"].get<std::string>().c_str())) {
					continue;
				}
			}
			if (trigger_name != "All") {
				if (trigger_name != CString(item["trigger_name"].get<std::string>().c_str())) {
					continue;
				}
			}
			CString trigger_name = CString(item["trigger_name"].get<std::string>().c_str());
			CString trigger_action = CString(item["trigger_action"].get<std::string>().c_str());
			CString target_id = CString(item["target_id"].get<std::string>().c_str());
			CString target_action = CString(item["target_action"].get<std::string>().c_str());
			CString trigger_num_actions;
			try {
				trigger_num_actions = CString(item["trigger_num_actions"].get<std::string>().c_str());
			}
			catch (...) {
				trigger_num_actions.Format(_T("%d"), item["trigger_num_actions"].get<int>());
			}

			// Add the data to the list control
			int index = m_wake_up_setting_list.InsertItem(m_wake_up_setting_list.GetItemCount(), trigger_name);
			m_wake_up_setting_list.SetItemText(index, 1, trigger_action);
			m_wake_up_setting_list.SetItemText(index, 2, trigger_num_actions);
			m_wake_up_setting_list.SetItemText(index, 3, target_id);
			m_wake_up_setting_list.SetItemText(index, 4, target_action);

		}
	}
}

void CWakeUpSettingDlg::GetRequestUsers()
{

	cb_users.AddString(TEXT("Default User"));
	cb_users.SetCurSel(0);

	std::string wake_up_server_url = CT2A(global_wake_up_server_url);
	cpr::Response r_users = cpr::Get(cpr::Url{ wake_up_server_url + "users" });
	nlohmann::json jsonList_users = nlohmann::json::parse(r_users.text);

	for (const auto& item : jsonList_users) {
		CString gosh_id = CString(item["gosh_id"].get<std::string>().c_str());
		CString first_name = CString(item["first_name"].get<std::string>().c_str());
		CString last_name = CString(item["last_name"].get<std::string>().c_str());
		CString id = CString(item["id"].get<std::string>().c_str());
		cb_users.AddString(gosh_id + ": " + first_name + " " + last_name + "                                                                  #" + id);
	}
}

void CWakeUpSettingDlg::GetRequestTriggers()
{

	cb_triggers.AddString(TEXT("All"));
	cb_triggers.SetCurSel(0);

	std::string wake_up_server_url = CT2A(global_wake_up_server_url);
	cpr::Response r_triggers = cpr::Get(cpr::Url{ wake_up_server_url + "triggers" });
	nlohmann::json jsonList_triggers = nlohmann::json::parse(r_triggers.text);

	for (const auto& item : jsonList_triggers) {
		CString name = CString(item["name"].get<std::string>().c_str());
		CString id = CString(item["id"].get<std::string>().c_str());
		cb_triggers.AddString(name);
	}
}

void CWakeUpSettingDlg::GetRequestTargets()
{

	cb_targets.AddString(TEXT("All"));
	cb_targets.SetCurSel(0);

	std::string wake_up_server_url = CT2A(global_wake_up_server_url);
	cpr::Response r_targets = cpr::Get(cpr::Url{ wake_up_server_url + "target_devices" });
	nlohmann::json jsonList_triggers = nlohmann::json::parse(r_targets.text);

	for (const auto& item : jsonList_triggers) {
		CString name = CString(item["name"].get<std::string>().c_str());
		CString id = CString(item["matter_id"].get<std::string>().c_str());
		cb_targets.AddString(id);
	}
}

void CWakeUpSettingDlg::OnCbnSelchangeCombo5()
{
	GetRequestForSignals();
}

void CWakeUpSettingDlg::GetRequestForSignals()
{
	int index = cb_users.GetCurSel();
	CString str;
	cb_users.GetLBText(index, str);

	if (str == TEXT("Default User"))
	{
		GetRequestSignalsForDefaultUser();
	}
	else
	{
		GetRequestSignalsForAUser(str);
	}

	CString total_signal_number;
	total_signal_number.Format(_T("%d"), m_wake_up_setting_list.GetItemCount());

	GetDlgItem(IDC_SIGNAL_NUMBER)->SetWindowTextW(TEXT("Total Signal Number: ") + total_signal_number);
}

void CWakeUpSettingDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if (getWakeUpServerMode()) {
		CDeleteSignal dlg;
		POSITION pos = m_wake_up_setting_list.GetFirstSelectedItemPosition();
		if (pos != nullptr)
		{
			int index = cb_users.GetCurSel();
			CString str;
			cb_users.GetLBText(index, str);
			CString user_id;
			int colonIndex = str.Find(_T("#"));
			if (colonIndex != -1) {
				user_id = str.Mid(colonIndex + 1);
			}
			int nItem = m_wake_up_setting_list.GetNextSelectedItem(pos);
			dlg.trigger_name = m_wake_up_setting_list.GetItemText(nItem, 0);
			dlg.trigger_action = m_wake_up_setting_list.GetItemText(nItem, 1);
			dlg.trigger_value = m_wake_up_setting_list.GetItemText(nItem, 2);
			dlg.target_id = m_wake_up_setting_list.GetItemText(nItem, 3);
			dlg.target_action = m_wake_up_setting_list.GetItemText(nItem, 4);
			dlg.user_id = user_id;
			if (dlg.DoModal() == IDOK)
			{
				GetRequestForSignals();
			}
		}
		else
		{
			MessageBox(TEXT("Please select a signal!"));
		}
	}
	else {
		AfxMessageBox(TEXT("Wake Up Server has not been Connected!"));
	}
}


void CWakeUpSettingDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	if (getWakeUpServerMode()) {
		CAddSignal dlg;
		int index = cb_users.GetCurSel();
		CString str;
		cb_users.GetLBText(index, str);
		CString user_id;
		int colonIndex = str.Find(_T("#"));
		if (colonIndex != -1) {
			user_id = str.Mid(colonIndex + 1);
		}
		dlg.user_id = user_id;
		if (dlg.DoModal() == IDOK)
		{
			GetRequestForSignals();
		}
	}
	else {
		AfxMessageBox(TEXT("Wake Up Server has not been Connected!"));
	}
}


void CWakeUpSettingDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	if (getWakeUpServerMode()) {
		CEditSignal dlg;
		POSITION pos = m_wake_up_setting_list.GetFirstSelectedItemPosition();
		if (pos != nullptr)
		{
			int index = cb_users.GetCurSel();
			CString str;
			cb_users.GetLBText(index, str);
			CString user_id;
			int colonIndex = str.Find(_T("#"));
			if (colonIndex != -1) {
				user_id = str.Mid(colonIndex + 1);
			}
			int nItem = m_wake_up_setting_list.GetNextSelectedItem(pos);
			dlg.trigger_name = m_wake_up_setting_list.GetItemText(nItem, 0);
			dlg.trigger_action = m_wake_up_setting_list.GetItemText(nItem, 1);
			dlg.trigger_value = m_wake_up_setting_list.GetItemText(nItem, 2);
			dlg.target_id = m_wake_up_setting_list.GetItemText(nItem, 3);
			dlg.target_action = m_wake_up_setting_list.GetItemText(nItem, 4);
			dlg.user_id = user_id;
			if (dlg.DoModal() == IDOK)
			{
				GetRequestForSignals();
			}
		}
		else
		{
			MessageBox(TEXT("Please select a signal!"));
		}
	}
	else {
		AfxMessageBox(TEXT("Wake Up Server has not been Connected!"));
	}
}


void CWakeUpSettingDlg::OnCbnSelchangeCombo2()
{
	// TODO: Add your control notification handler code here
	GetRequestForSignals();
}


void CWakeUpSettingDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	GetRequestForSignals();
}
