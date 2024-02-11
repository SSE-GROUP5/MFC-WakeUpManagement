// CDevicesSettingDlg.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "CDevicesSettingDlg.h"
#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>




// CDevicesSettingDlg

IMPLEMENT_DYNCREATE(CDevicesSettingDlg, CFormView)

CDevicesSettingDlg::CDevicesSettingDlg()
	: CFormView(IDD_DIALOG_DEVICES_SETTING)
{

}

CDevicesSettingDlg::~CDevicesSettingDlg()
{
}

void CDevicesSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_controllers);
	DDX_Control(pDX, IDC_LIST2, m_matter_devices);
}

BEGIN_MESSAGE_MAP(CDevicesSettingDlg, CFormView)
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

	m_Table_Font.CreatePointFont(100, _T("Calibri"));
	m_controllers.SetFont(&m_Table_Font);
	m_matter_devices.SetFont(&m_Table_Font);

	m_controllers.InsertColumn(1, TEXT("ID"), LVCFMT_CENTER, 400);
	m_controllers.InsertColumn(2, TEXT("Name"), LVCFMT_CENTER, 150);
	m_controllers.InsertColumn(3, TEXT("Type"), LVCFMT_CENTER, 150);
	//m_controllers.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_controllers.SetExtendedStyle(m_controllers.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	GetRequestControllers();

	m_matter_devices.InsertColumn(1, TEXT("ID"), LVCFMT_CENTER, 250);
	m_matter_devices.InsertColumn(2, TEXT("Name"), LVCFMT_CENTER, 150);
	m_matter_devices.InsertColumn(3, TEXT("Type"), LVCFMT_CENTER, 150);
	m_matter_devices.SetExtendedStyle(m_matter_devices.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	GetRequestMatterDevices();


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
}

void CDevicesSettingDlg::GetRequestControllers()
{
	cpr::Response r_controllers = cpr::Get(cpr::Url{ "http://localhost:5001/triggers" });
	nlohmann::json jsonList_controllers = nlohmann::json::parse(r_controllers.text);

	for (const auto& item : jsonList_controllers) {
		CString id = CString(item["id"].get<std::string>().c_str());
		CString name = CString(item["name"].get<std::string>().c_str());
		CString type = CString(item["type"].get<std::string>().c_str());

		// Add the data to the list control
		int index = m_controllers.InsertItem(m_controllers.GetItemCount(), id);
		m_controllers.SetItemText(index, 1, name);
		m_controllers.SetItemText(index, 2, type);
	}
}

void CDevicesSettingDlg::GetRequestMatterDevices()
{
	cpr::Response r_matter_devices = cpr::Get(cpr::Url{ "http://localhost:5001/target_devices" });
	nlohmann::json jsonList_matter_devices = nlohmann::json::parse(r_matter_devices.text);

	for (const auto& item : jsonList_matter_devices) {
		CString id = CString(item["matter_id"].get<std::string>().c_str());
		CString name = CString(item["name"].get<std::string>().c_str());
		CString type = CString(item["type"].get<std::string>().c_str());

		// Add the data to the list control
		int index = m_matter_devices.InsertItem(m_matter_devices.GetItemCount(), id);
		m_matter_devices.SetItemText(index, 1, name);
		m_matter_devices.SetItemText(index, 2, type);
	}
}




BOOL CDevicesSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	m_ToolTip.RelayEvent(pMsg);

	return CFormView::PreTranslateMessage(pMsg);
}
