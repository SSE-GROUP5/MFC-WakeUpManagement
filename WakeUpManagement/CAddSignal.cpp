// CAddSignal.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "afxdialogex.h"
#include "CAddSignal.h"

#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>



// CAddSignal dialog

IMPLEMENT_DYNAMIC(CAddSignal, CDialogEx)

CAddSignal::CAddSignal(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADD_SIGNAL, pParent)
{

}

CAddSignal::~CAddSignal()
{
}

void CAddSignal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, cb_trigger_name);
	DDX_Control(pDX, IDC_COMBO2, cb_trigger_action);
	DDX_Control(pDX, IDC_EDIT1, editBox_trigger_value);
	DDX_Control(pDX, IDC_COMBO3, cb_target_id);
	DDX_Control(pDX, IDC_COMBO4, cb_target_action);
}


BEGIN_MESSAGE_MAP(CAddSignal, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAddSignal::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CAddSignal::OnCbnSelchangeCombo3)
	ON_BN_CLICKED(IDOK, &CAddSignal::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddSignal message handlers

void CAddSignal::GetRequestForTriggersCombo()
{
	cpr::Response r_triggers = cpr::Get(cpr::Url{ "http://localhost:5001/triggers" });
	nlohmann::json jsonList_triggers = nlohmann::json::parse(r_triggers.text);

	for (const auto& item : jsonList_triggers) {
		CString name = CString(item["name"].get<std::string>().c_str());
		CString id = CString(item["id"].get<std::string>().c_str());
		GetDlgItem(IDC_TEXT_TRIGGER_ID)->SetWindowTextW(id);
		cb_trigger_name.AddString(name);
	}
}

void CAddSignal::GetRequestForTargetsCombo()
{
	cpr::Response r_target_devices = cpr::Get(cpr::Url{ "http://localhost:5001/target_devices" });
	nlohmann::json jsonList_target_devices = nlohmann::json::parse(r_target_devices.text);

	for (const auto& item : jsonList_target_devices) {
		CString id = CString(item["matter_id"].get<std::string>().c_str());
		cb_target_id.AddString(id);
	}
}

BOOL CAddSignal::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_Title_Font.CreatePointFont(120, _T("Calibri"));
	GetDlgItem(IDC_STATIC)->SetFont(&m_Title_Font);
	GetDlgItem(IDC_TEXT_USER_ID)->SetWindowTextW(user_id);
	GetRequestForTriggersCombo();
	GetRequestForTargetsCombo();

	cb_trigger_name.SetCurSel(0);
	cb_target_id.SetCurSel(0);

	if (cb_trigger_name.GetCount() != 0) {
		OnCbnSelchangeCombo1();
	}
	if (cb_target_id.GetCount() != 0) {
		OnCbnSelchangeCombo3();
	}

	if (!m_ToolTip.Create(this))
	{
		TRACE0("Unable to create the ToolTip!");
	}
	else
	{
		// Add tool tips to the controls, either by hard coded string 
		// or using the string table resource
		CWnd* tooltip_trigger_name = GetDlgItem(IDC_TOOLTIP_TRIGGER_NAME);
		m_ToolTip.AddTool(tooltip_trigger_name, _T("Trigger Name: XXXXX"));
		CWnd* tooltip_trigger_action = GetDlgItem(IDC_TOOLTIP_TRIGGER_ACTION);
		m_ToolTip.AddTool(tooltip_trigger_action, _T("Trigger action: XXXXX"));
		CWnd* tooltip_trigger_value = GetDlgItem(IDC_TOOLTIP_TRIGGER_VALUE);
		m_ToolTip.AddTool(tooltip_trigger_value, _T("Trigger value: XXXXX"));
		CWnd* tooltip_target_id = GetDlgItem(IDC_TOOLTIP_TARGET_ID);
		m_ToolTip.AddTool(tooltip_target_id, _T("Target ID: XXXXX"));
		CWnd* tooltip_target_action = GetDlgItem(IDC_TOOLTIP_TARGET_ACTION);
		m_ToolTip.AddTool(tooltip_target_action, _T("Target action: XXXXX"));
		m_ToolTip.Activate(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAddSignal::OnCbnSelchangeCombo1()
{
	GetDlgItem(IDC_COMBO2)->SendMessage(CB_RESETCONTENT);
	int index = cb_trigger_name.GetCurSel();

	CString trigger_id;
	GetDlgItem(IDC_TEXT_TRIGGER_ID)->GetWindowTextW(trigger_id);
	std::string trigger_id_str = CT2A(trigger_id);
	
	cpr::Response r_controllers = cpr::Get(cpr::Url{ "http://localhost:5001/triggers" });
	nlohmann::json jsonList_controllers = nlohmann::json::parse(r_controllers.text);
	
	for (const auto& item : jsonList_controllers) {
		if (item["id"] == trigger_id_str) {
			CString type = CString(item["type"].get<std::string>().c_str());
			cb_trigger_action.AddString(type);
		}
	}
	cb_trigger_action.SetCurSel(0);
}


void CAddSignal::OnCbnSelchangeCombo3()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_COMBO4)->SendMessage(CB_RESETCONTENT);
	int index = cb_target_id.GetCurSel();
	
	CString str;
	cb_target_id.GetLBText(index, str);
	
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
				cb_target_action.AddString(action);
			}
		}
	}
	cb_target_action.SetCurSel(0);
}


void CAddSignal::OnBnClickedOk()
{
	CString trigger_id;
	CString trigger_name;
	CString trigger_action;
	CString trigger_num_actions;
	CString target_id;
	CString target_action;
	CString user_id;
	GetDlgItem(IDC_TEXT_TRIGGER_ID)->GetWindowTextW(trigger_id);
	cb_trigger_name.GetWindowTextW(trigger_name);
	cb_trigger_action.GetWindowTextW(trigger_action);
	editBox_trigger_value.GetWindowTextW(trigger_num_actions);
	cb_target_id.GetWindowTextW(target_id);
	cb_target_action.GetWindowTextW(target_action);
	GetDlgItem(IDC_TEXT_USER_ID)->GetWindowTextW(user_id);

	boolean postRequest = true;
	cpr::Response r = cpr::Get(cpr::Url{ "http://localhost:5001/signals" });
	nlohmann::json jsonList = nlohmann::json::parse(r.text);
	cpr::Response response_delete;

	if (jsonList.contains("signals")) {
		const auto& signalsArray = jsonList["signals"];
		for (const auto& item : signalsArray) {
			CString get_trigger_value;
			get_trigger_value.Format(_T("%d"), item["trigger_num_actions"].get<int>());
			if (trigger_name == CString(item["trigger_name"].get<std::string>().c_str()) &&
				trigger_action == CString(item["trigger_action"].get<std::string>().c_str()) &&
				trigger_num_actions == get_trigger_value &&
				target_id == CString(item["target_id"].get<std::string>().c_str()) &&
				target_action == CString(item["target_action"].get<std::string>().c_str()) &&
				(user_id.IsEmpty() && item["user_id"].is_null() || user_id == CString(item["user_id"].get<std::string>().c_str())))
			{
				MessageBox(TEXT("You already have the setting!"));
				postRequest = false;
				break;
			}
		}
	}

	if (postRequest) {
		std::string str_trigger_id = CT2A(trigger_id);
		std::string std_trigger_name = CT2A(trigger_name);
		std::string std_trigger_action = CT2A(trigger_action);
		std::string std_trigger_num_actions = CT2A(trigger_num_actions);
		std::string std_target_id = CT2A(target_id);
		std::string std_target_action = CT2A(target_action);
		std::string std_user_id = CT2A(user_id);

		cpr::Response response;
		if (!user_id.IsEmpty())
		{
			response = cpr::Post(cpr::Url{ "http://localhost:5001/signals/set" },
				cpr::Header{ {"Content-Type", "application/json"} },
				cpr::Body{ "{ \"trigger_id\": \"" + str_trigger_id +
						   "\", \"trigger_name\": \"" + std_trigger_name +
						   "\", \"trigger_action\": \"" + std_trigger_action +
						   "\", \"trigger_num_actions\": \"" + std_trigger_num_actions +
						   "\", \"target_device_id\": \"" + std_target_id +
						   "\", \"target_action\": \"" + std_target_action +
						   "\", \"user_id\": \"" + std_user_id + "\" }" });
		}
		else
		{
			response = cpr::Post(cpr::Url{ "http://localhost:5001/signals/set" },
				cpr::Header{ {"Content-Type", "application/json"} },
				cpr::Body{ "{ \"trigger_id\": \"" + str_trigger_id +
						   "\", \"trigger_name\": \"" + std_trigger_name +
						   "\", \"trigger_action\": \"" + std_trigger_action +
						   "\", \"trigger_num_actions\": \"" + std_trigger_num_actions +
						   "\", \"target_device_id\": \"" + std_target_id +
						   "\", \"target_action\": \"" + std_target_action + "\" }" });
		}

		if (response.status_code == 201) {
			CDialogEx::OnOK();
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


BOOL CAddSignal::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	m_ToolTip.RelayEvent(pMsg);
	return CDialogEx::PreTranslateMessage(pMsg);
}
