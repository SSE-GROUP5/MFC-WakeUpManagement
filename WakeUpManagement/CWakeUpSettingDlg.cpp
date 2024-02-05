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
	DDX_Control(pDX, IDC_COMBO1, cb_controller);
	DDX_Control(pDX, IDC_COMBO2, cb_trigger_controller);
	DDX_Control(pDX, IDC_COMBO3, cb_matter_devices);
	DDX_Control(pDX, IDC_COMBO4, cb_matter_action);
	DDX_Control(pDX, IDC_COMBO5, cb_users);
	DDX_Control(pDX, IDC_EDIT1, edit_trigger_numbers);
}

BEGIN_MESSAGE_MAP(CWakeUpSettingDlg, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CWakeUpSettingDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CWakeUpSettingDlg::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CWakeUpSettingDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CWakeUpSettingDlg::OnCbnSelchangeCombo5)
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


	m_wake_up_setting_list.InsertColumn(0, TEXT("Controller ID"), LVCFMT_LEFT, 360);
	m_wake_up_setting_list.InsertColumn(1, TEXT("Controller Name"), LVCFMT_LEFT, 170);
	m_wake_up_setting_list.InsertColumn(2, TEXT("Trigger Controller"), LVCFMT_LEFT, 170);
	m_wake_up_setting_list.InsertColumn(3, TEXT("Trigger Numbers"), LVCFMT_LEFT, 170);
	m_wake_up_setting_list.InsertColumn(4, TEXT("Matter Device ID"), LVCFMT_LEFT, 250);
	m_wake_up_setting_list.InsertColumn(5, TEXT("Matter Action"), LVCFMT_LEFT, 170);
	m_wake_up_setting_list.SetExtendedStyle(m_wake_up_setting_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	GetRequestUsers();
	GetRequestSignalsForDefaultUser();
	GetRequestControllers();
	GetRequestMatterDevices();

	cb_controller.SetCurSel(0);
	cb_matter_devices.SetCurSel(0);

	if (cb_controller.GetCount() != 0) {
		OnCbnSelchangeCombo1();
	}
	if (cb_matter_devices.GetCount() != 0) {
		OnCbnSelchangeCombo3();
	}
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
				CString interactive_action = CString(item["interactive_action"].get<std::string>().c_str());
				CString interactive_id = CString(item["interactive_id"].get<std::string>().c_str());
				CString interactive_device_name = CString(item["interactive_device_name"].get<std::string>().c_str());
				CString target_action = CString(item["target_action"].get<std::string>().c_str());
				CString target_id = CString(item["target_id"].get<std::string>().c_str());

				CString interactive_device_num_actions;
				interactive_device_num_actions.Format(_T("%d"), item["interactive_device_num_actions"].get<int>());

				// Add the data to the list control
				int index = m_wake_up_setting_list.InsertItem(m_wake_up_setting_list.GetItemCount(), interactive_id);
				m_wake_up_setting_list.SetItemText(index, 1, interactive_device_name);
				m_wake_up_setting_list.SetItemText(index, 2, interactive_action);
				m_wake_up_setting_list.SetItemText(index, 3, interactive_device_num_actions);
				m_wake_up_setting_list.SetItemText(index, 4, target_id);
				m_wake_up_setting_list.SetItemText(index, 5, target_action);
			}
		}
	}

}

void CWakeUpSettingDlg::GetRequestSignalsForAUser(CString str)
{
	m_wake_up_setting_list.DeleteAllItems();
	CString user_id;
	int colonIndex = str.Find(_T(":"));
	if (colonIndex != -1) {
		user_id = str.Mid(colonIndex + 2);
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

			CString interactive_action = CString(item["interactive_action"].get<std::string>().c_str());
			CString interactive_id = CString(item["interactive_id"].get<std::string>().c_str());
			CString interactive_device_name = CString(item["interactive_device_name"].get<std::string>().c_str());
			CString target_action = CString(item["target_action"].get<std::string>().c_str());
			CString target_id = CString(item["target_id"].get<std::string>().c_str());

			CString interactive_device_num_actions;
			interactive_device_num_actions.Format(_T("%d"), item["interactive_device_num_actions"].get<int>());

			// Add the data to the list control
			int index = m_wake_up_setting_list.InsertItem(m_wake_up_setting_list.GetItemCount(), interactive_id);
			m_wake_up_setting_list.SetItemText(index, 1, interactive_device_name);
			m_wake_up_setting_list.SetItemText(index, 2, interactive_action);
			m_wake_up_setting_list.SetItemText(index, 3, interactive_device_num_actions);
			m_wake_up_setting_list.SetItemText(index, 4, target_id);
			m_wake_up_setting_list.SetItemText(index, 5, target_action);

		}
	}
}

void CWakeUpSettingDlg::GetRequestControllers()
{
	cpr::Response r_controllers = cpr::Get(cpr::Url{ "http://localhost:5001/interactive_devices" });
	nlohmann::json jsonList_controllers = nlohmann::json::parse(r_controllers.text);

	for (const auto& item : jsonList_controllers) {
		CString id = CString(item["id"].get<std::string>().c_str());
		cb_controller.AddString(id);
	}
}

void CWakeUpSettingDlg::GetRequestMatterDevices()
{
	cpr::Response r_matter_devices = cpr::Get(cpr::Url{ "http://localhost:5001/target_devices" });
	nlohmann::json jsonList_matter_devices = nlohmann::json::parse(r_matter_devices.text);

	for (const auto& item : jsonList_matter_devices) {
		CString id = CString(item["matter_id"].get<std::string>().c_str());
		cb_matter_devices.AddString(id);
	}
}

void CWakeUpSettingDlg::GetRequestUsers()
{

	cb_users.AddString(TEXT("Default User"));
	cb_users.SetCurSel(0);

	cpr::Response r_users = cpr::Get(cpr::Url{ "http://localhost:5001/users" });
	nlohmann::json jsonList_users = nlohmann::json::parse(r_users.text);

	for (const auto& item : jsonList_users) {
		CString first_name = CString(item["first_name"].get<std::string>().c_str());
		CString last_name = CString(item["last_name"].get<std::string>().c_str());
		CString id = CString(item["id"].get<std::string>().c_str());
		cb_users.AddString(first_name + " " + last_name + "                                                                  : " + id);
	}
}

void CWakeUpSettingDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString controller;
	CString trigger_controller;
	CString trigger_numbers;
	CString matter_devices;
	CString matter_action;
	cb_controller.GetWindowTextW(controller);
	cb_trigger_controller.GetWindowTextW(trigger_controller);
	edit_trigger_numbers.GetWindowTextW(trigger_numbers);
	cb_matter_devices.GetWindowTextW(matter_devices);
	cb_matter_action.GetWindowTextW(matter_action);
	int index = cb_users.GetCurSel();
	CString user;
	CString user_id;
	cb_users.GetLBText(index, user);

	boolean postRequest = true;
	for (int i = 0; i < m_wake_up_setting_list.GetItemCount(); ++i) {
		if (m_wake_up_setting_list.GetItemText(i, 0) == controller &&
			m_wake_up_setting_list.GetItemText(i, 1) == trigger_controller &&
			m_wake_up_setting_list.GetItemText(i, 2) == trigger_numbers &&
			m_wake_up_setting_list.GetItemText(i, 3) == matter_devices &&
			m_wake_up_setting_list.GetItemText(i, 4) == matter_action) {
			// Item already exists, handle accordingly (e.g., show a message)
			MessageBox(TEXT("You already have the setting!"));
			postRequest = false;
			break;
		}
	}

	if (postRequest) {
		std::string strController = CT2A(controller);
		std::string stdTriggerController = CT2A(trigger_controller);
		std::string stdMatterDevices = CT2A(matter_devices);
		std::string stdMatterAction = CT2A(matter_action);
		std::string intTriggerNumbers = CT2A(trigger_numbers);

		cpr::Response response;
		if (user != TEXT("Default User"))
		{
			int colonIndex = user.Find(_T(":"));
			if (colonIndex != -1) {
				user_id = user.Mid(colonIndex + 2);
			}
			std::string strUserId = CT2A(user_id);

			response = cpr::Post(cpr::Url{ "http://localhost:5001/signals/set" },
				cpr::Header{ {"Content-Type", "application/json"} },
				cpr::Body{ "{ \"interactive_device_id\": \"" + strController +
						   "\", \"interactive_device_action\": \"" + stdTriggerController +
						   "\", \"interactive_device_num_actions\": \"" + intTriggerNumbers +
						   "\", \"target_device_id\": \"" + stdMatterDevices +
						   "\", \"target_action\": \"" + stdMatterAction +
						   "\", \"user_id\": \"" + strUserId + "\" }" });
		}
		else
		{
			response = cpr::Post(cpr::Url{ "http://localhost:5001/signals/set" },
				cpr::Header{ {"Content-Type", "application/json"} },
				cpr::Body{ "{ \"interactive_device_id\": \"" + strController +
						   "\", \"interactive_device_action\": \"" + stdTriggerController +
						   "\", \"interactive_device_num_actions\": \"" + intTriggerNumbers +
						   "\", \"target_device_id\": \"" + stdMatterDevices +
						   "\", \"target_action\": \"" + stdMatterAction + "\" }" });
		}

		if (response.status_code == 200) {
			OnCbnSelchangeCombo5();
			MessageBox(TEXT("Successly added!"));
		}
		else {
			CString statusMessage;
			statusMessage.Format(_T("Failed! Please Check Your Input! HTTP Status Code: %d"), response.status_code);

			// Display a message box with the status code
			AfxMessageBox(statusMessage);

		}
	}

}

void CWakeUpSettingDlg::OnCbnSelchangeCombo1()
{
	GetDlgItem(IDC_COMBO2)->SendMessage(CB_RESETCONTENT);
	int index = cb_controller.GetCurSel();

	CString str;
	cb_controller.GetLBText(index, str);

	cpr::Response r_controllers = cpr::Get(cpr::Url{ "http://localhost:5001/interactive_devices" });
	nlohmann::json jsonList_controllers = nlohmann::json::parse(r_controllers.text);

	for (const auto& item : jsonList_controllers) {
		if (item["id"] == CT2A(str)) {
			CString type = CString(item["type"].get<std::string>().c_str());
			cb_trigger_controller.AddString(type);
		}
	}
	cb_trigger_controller.SetCurSel(0);
}

void CWakeUpSettingDlg::OnCbnSelchangeCombo3()
{

	GetDlgItem(IDC_COMBO4)->SendMessage(CB_RESETCONTENT);
	int index = cb_matter_devices.GetCurSel();

	CString str;
	cb_matter_devices.GetLBText(index, str);

	cpr::Response r_matter_devices = cpr::Get(cpr::Url{ "http://localhost:5001/target_devices" });
	nlohmann::json jsonList_matter_devices = nlohmann::json::parse(r_matter_devices.text);

	for (const auto& item : jsonList_matter_devices) {
		// Check if the "matter_id" matches
		if (item["matter_id"] == CT2A(str)) {
			const auto& possibleActionArray = item["possible_actions"];

			// Iterate through each element in the array
			for (const auto& possibleAction : possibleActionArray) {
				// Access properties inside each element
				CString action = CString(possibleAction["action"].get<std::string>().c_str());

				// Add the data to the combo
				cb_matter_action.AddString(action);
			}
		}
	}
	cb_matter_action.SetCurSel(0);
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
}
