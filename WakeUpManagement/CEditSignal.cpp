// CEditSignal.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "afxdialogex.h"
#include "CEditSignal.h"
#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>

// CEditSignal dialog

IMPLEMENT_DYNAMIC(CEditSignal, CDialogEx)

CEditSignal::CEditSignal(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDIT_SIGNAL, pParent)
{

}

CEditSignal::~CEditSignal()
{
}

void CEditSignal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, cb_trigger_name);
	DDX_Control(pDX, IDC_COMBO2, cb_trigger_action);
	DDX_Control(pDX, IDC_EDIT1, editBox_trigger_value);
	DDX_Control(pDX, IDC_COMBO3, cb_target_id);
	DDX_Control(pDX, IDC_COMBO4, cb_target_action);
}


BEGIN_MESSAGE_MAP(CEditSignal, CDialogEx)
END_MESSAGE_MAP()


// CEditSignal message handlers


BOOL CEditSignal::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	m_ToolTip.RelayEvent(pMsg);
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CEditSignal::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Title_Font.CreatePointFont(120, _T("Calibri"));
	GetDlgItem(IDC_TEXT_USER_ID)->SetWindowTextW(user_id);
	GetRequestForTriggersCombo();
	GetRequestForTargetsCombo();

	int trigger_name_index = cb_trigger_name.FindString(-1, trigger_name);
	if (trigger_name_index != CB_ERR) {
		cb_trigger_name.SetCurSel(trigger_name_index);
	}

	int target_id_index = cb_target_id.FindString(-1, target_id);
	if (target_id_index != CB_ERR) {
		cb_target_id.SetCurSel(target_id_index);
	}

	editBox_trigger_value.SetWindowTextW(trigger_value);

	if (cb_trigger_name.GetCount() != 0) {
		OnCbnSelchangeCombo1();
	}
	if (cb_target_id.GetCount() != 0) {
		OnCbnSelchangeCombo3();
	}

	// TODO:  Add extra initialization here
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



void CEditSignal::GetRequestForTriggersCombo()
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

void CEditSignal::GetRequestForTargetsCombo()
{
	cpr::Response r_target_devices = cpr::Get(cpr::Url{ "http://localhost:5001/target_devices" });
	nlohmann::json jsonList_target_devices = nlohmann::json::parse(r_target_devices.text);

	for (const auto& item : jsonList_target_devices) {
		CString id = CString(item["matter_id"].get<std::string>().c_str());
		cb_target_id.AddString(id);
	}
}


void CEditSignal::OnCbnSelchangeCombo1()
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

	int trigger_action_index = cb_trigger_action.FindString(-1, trigger_action);
	if (trigger_action_index != CB_ERR) {
		cb_trigger_action.SetCurSel(trigger_action_index);
	}
}


void CEditSignal::OnCbnSelchangeCombo3()
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
	int target_action_index = cb_target_action.FindString(-1, target_action);
	if (target_action_index != CB_ERR) {
		cb_target_action.SetCurSel(target_action_index);
	}
}