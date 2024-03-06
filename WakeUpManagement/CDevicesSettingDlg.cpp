// CDevicesSettingDlg.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "CDevicesSettingDlg.h"
#include "CAddTrigger.h"
#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>

extern CString global_wake_up_server_url;
extern BOOL getWakeUpServerMode();

// CDevicesSettingDlg

IMPLEMENT_DYNCREATE(CDevicesSettingDlg, CFormView)

CDevicesSettingDlg::CDevicesSettingDlg()
	: CFormView(IDD_DIALOG_DEVICES_SETTING)
{
	wake_up_server_mode = getWakeUpServerMode();
}

CDevicesSettingDlg::~CDevicesSettingDlg()
{
}

void CDevicesSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_triggers);
	DDX_Control(pDX, IDC_LIST2, m_target_devices);
}

BEGIN_MESSAGE_MAP(CDevicesSettingDlg, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CDevicesSettingDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CDevicesSettingDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CDevicesSettingDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CDevicesSettingDlg diagnostics

#ifdef _DEBUG
void CDevicesSettingDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDevicesSettingDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDevicesSettingDlg message handlers


void CDevicesSettingDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();


	CRect rect_triggers;
	m_triggers.GetWindowRect(&rect_triggers);
	ScreenToClient(&rect_triggers);
	m_triggers.MoveWindow(rect_triggers.left, rect_triggers.top, 854, 850, TRUE);

	m_triggers.InsertColumn(1, TEXT("ID"), LVCFMT_CENTER, 400);
	m_triggers.InsertColumn(2, TEXT("Name"), LVCFMT_CENTER, 150);
	m_triggers.InsertColumn(3, TEXT("Type"), LVCFMT_CENTER, 150);
	m_triggers.InsertColumn(4, TEXT("Confirmed"), LVCFMT_CENTER, 150);
	m_triggers.SetExtendedStyle(m_triggers.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	CRect rect_targets;
	m_target_devices.GetWindowRect(&rect_targets);
	ScreenToClient(&rect_targets);
	m_target_devices.MoveWindow(rect_targets.left, rect_triggers.top, 654, 850, TRUE);

	m_target_devices.InsertColumn(1, TEXT("ID"), LVCFMT_CENTER, 250);
	m_target_devices.InsertColumn(2, TEXT("Name"), LVCFMT_CENTER, 250);
	m_target_devices.InsertColumn(3, TEXT("Type"), LVCFMT_CENTER, 150);
	m_target_devices.SetExtendedStyle(m_target_devices.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	//Create the ToolTip control
	if (!m_ToolTip.Create(this))
	{
		TRACE0("Unable to create the ToolTip!");
	}
	else
	{
		// Add tool tips to the controls, either by hard coded string 
		// or using the string table resource
		CWnd* tooltip_triggers = GetDlgItem(IDC_TOOLTIP_TRIGGERS);
		m_ToolTip.AddTool(tooltip_triggers, _T("Triggers: XXXXXXXX"));
		CWnd* tooltip_targets = GetDlgItem(IDC_TOOLTIP_TARGETS);
		m_ToolTip.AddTool(tooltip_targets, _T("Targets: XXXXXXXX"));

		m_ToolTip.Activate(TRUE);
	}

	checkWakeUpServerMode();
}

void CDevicesSettingDlg::checkWakeUpServerMode()
{
	if (wake_up_server_mode) {
		// Update the control after the HTTP response is checked
		SetDlgItemTextW(IDC_WAKE_UP_SERVER, TEXT("Wake Up Server: ON"));
		GetRequestTriggers();
		GetRequestTargetDevices();
	}
	else {
		SetDlgItemTextW(IDC_WAKE_UP_SERVER, TEXT("Wake Up Server: OFF"));
	}
}

void CDevicesSettingDlg::GetRequestTriggers()
{
	m_triggers.DeleteAllItems();
	std::string wake_up_server_url = CT2A(global_wake_up_server_url);
	cpr::Response r_triggers = cpr::Get(cpr::Url{ wake_up_server_url + "triggers" });
	nlohmann::json jsonList_triggers = nlohmann::json::parse(r_triggers.text);

	for (const auto& item : jsonList_triggers) {
		CString id = CString(item["id"].get<std::string>().c_str());
		CString name = CString(item["name"].get<std::string>().c_str());
		CString type = CString(item["type"].get<std::string>().c_str());
		BOOL confirmed = item["confirmed"].get<BOOL>();
		CString strConfirmed;
		strConfirmed.Format(_T("%s"), confirmed ? _T("True") : _T("False"));

		// Add the data to the list control
		int index = m_triggers.InsertItem(m_triggers.GetItemCount(), id);
		m_triggers.SetItemText(index, 1, name);
		m_triggers.SetItemText(index, 2, type);
		m_triggers.SetItemText(index, 3, strConfirmed);
	}
}

void CDevicesSettingDlg::GetRequestTargetDevices()
{
	m_target_devices.DeleteAllItems();
	std::string wake_up_server_url = CT2A(global_wake_up_server_url);
	cpr::Response r_target_devices = cpr::Get(cpr::Url{ wake_up_server_url + "target_devices" });
	nlohmann::json jsonList_target_devices = nlohmann::json::parse(r_target_devices.text);

	for (const auto& item : jsonList_target_devices) {
		CString id = CString(item["matter_id"].get<std::string>().c_str());
		CString name = CString(item["name"].get<std::string>().c_str());
		CString type = CString(item["type"].get<std::string>().c_str());

		// Add the data to the list control
		int index = m_target_devices.InsertItem(m_target_devices.GetItemCount(), id);
		m_target_devices.SetItemText(index, 1, name);
		m_target_devices.SetItemText(index, 2, type);
	}
}




BOOL CDevicesSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	m_ToolTip.RelayEvent(pMsg);

	return CFormView::PreTranslateMessage(pMsg);
}


void CDevicesSettingDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if (getWakeUpServerMode()) {
		CAddTrigger dlg;
		if (dlg.DoModal() == IDOK)
		{
			GetRequestTriggers();
		}
	}
	else {
		AfxMessageBox(TEXT("Wake Up Server has not been Connected!"));
	}
}


void CDevicesSettingDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	std::string Path = "C:\\Program Files\\wakeup_triggers";

	// Check if the folder exists
	if (!fs::exists(Path)) {
		// Create the folder if it doesn't exist
		if (!fs::create_directories(Path)) {
			std::cerr << "Error creating folder: " << Path << std::endl;
			/*return 1;*/ // Exit with an error code
		}
	}

	// Construct the file path
	CStringA cstringA(Path.c_str());
	CString cstring(cstringA);


	// Open the system file explorer at the specified folder
	ShellExecute(NULL, _T("explore"), cstring, NULL, NULL, SW_SHOWNORMAL);
}


void CDevicesSettingDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	GetRequestTriggers();
	GetRequestTargetDevices();
}
