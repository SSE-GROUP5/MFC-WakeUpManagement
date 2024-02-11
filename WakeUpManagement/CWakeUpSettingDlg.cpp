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
}

BEGIN_MESSAGE_MAP(CWakeUpSettingDlg, CFormView)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CWakeUpSettingDlg::OnCbnSelchangeCombo5)
	ON_BN_CLICKED(IDC_BUTTON1, &CWakeUpSettingDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CWakeUpSettingDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CWakeUpSettingDlg::OnBnClickedButton2)
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


	m_wake_up_setting_list.InsertColumn(1, TEXT("Trigger Name"), LVCFMT_CENTER, 250);
	m_wake_up_setting_list.InsertColumn(2, TEXT("Trigger Action"), LVCFMT_CENTER, 250);
	m_wake_up_setting_list.InsertColumn(3, TEXT("Trigger Value"), LVCFMT_CENTER, 250);
	m_wake_up_setting_list.InsertColumn(4, TEXT("Target ID"), LVCFMT_CENTER, 250);
	m_wake_up_setting_list.InsertColumn(5, TEXT("Target Action"), LVCFMT_CENTER, 250);
	m_wake_up_setting_list.SetExtendedStyle(m_wake_up_setting_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	GetRequestUsers();
	OnCbnSelchangeCombo5();
}

void CWakeUpSettingDlg::GetRequestSignalsForDefaultUser()
{
	m_wake_up_setting_list.DeleteAllItems();
	cpr::Response r = cpr::Get(cpr::Url{ "http://localhost:5001/signals" });
	nlohmann::json jsonList = nlohmann::json::parse(r.text);

	if (jsonList.contains("signals")) {
		// Access the array under the "signals" key
		const auto& signalsArray = jsonList["signals"];

		// Iterate through each element in the array
		for (const auto& item : signalsArray) {
			// Access properties inside each element
			if (item["user_id"].is_null()) {
				CString trigger_name = CString(item["trigger_name"].get<std::string>().c_str());
				CString trigger_action = CString(item["trigger_action"].get<std::string>().c_str());
				CString target_id = CString(item["target_id"].get<std::string>().c_str());
				CString target_action = CString(item["target_action"].get<std::string>().c_str());
				CString trigger_num_actions;
				trigger_num_actions.Format(_T("%d"), item["trigger_num_actions"].get<int>());

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
	std::string user_id_str = CT2A(user_id);
	cpr::Response r_signals_for_user = cpr::Get(cpr::Url{ "http://localhost:5001/signals/users/" + user_id_str });
	nlohmann::json jsonList_signals_for_user = nlohmann::json::parse(r_signals_for_user.text);

	if (jsonList_signals_for_user.contains("signals")) {
		// Access the array under the "signals" key
		const auto& signalsArray = jsonList_signals_for_user["signals"];

		// Iterate through each element in the array
		for (const auto& item : signalsArray) {
			// Access properties inside each element

			CString trigger_name = CString(item["trigger_name"].get<std::string>().c_str());
			CString trigger_action = CString(item["trigger_action"].get<std::string>().c_str());
			CString target_id = CString(item["target_id"].get<std::string>().c_str());
			CString target_action = CString(item["target_action"].get<std::string>().c_str());
			CString trigger_num_actions;
			trigger_num_actions.Format(_T("%d"), item["trigger_num_actions"].get<int>());

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

	cpr::Response r_users = cpr::Get(cpr::Url{ "http://localhost:5001/users" });
	nlohmann::json jsonList_users = nlohmann::json::parse(r_users.text);

	for (const auto& item : jsonList_users) {
		CString gosh_id = CString(item["gosh_id"].get<std::string>().c_str());
		CString first_name = CString(item["first_name"].get<std::string>().c_str());
		CString last_name = CString(item["last_name"].get<std::string>().c_str());
		CString id = CString(item["id"].get<std::string>().c_str());
		cb_users.AddString(gosh_id + ": " + first_name + " " + last_name + "                                                                  #" + id);
	}
}

void CWakeUpSettingDlg::OnCbnSelchangeCombo5()
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
			OnCbnSelchangeCombo5();
		}
	}
	else
	{
		MessageBox(TEXT("Please select a signal!"));
	}
}


void CWakeUpSettingDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
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
		OnCbnSelchangeCombo5();
	}
}


void CWakeUpSettingDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
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
			OnCbnSelchangeCombo5();
		}
	}
	else
	{
		MessageBox(TEXT("Please select a signal!"));
	}
}
