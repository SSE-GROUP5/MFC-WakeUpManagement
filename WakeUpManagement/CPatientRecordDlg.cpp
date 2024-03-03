// CPatientRecordDlg.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "CPatientRecordDlg.h"
#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>

// CPatientRecordDlg

IMPLEMENT_DYNCREATE(CPatientRecordDlg, CFormView)

CPatientRecordDlg::CPatientRecordDlg()
	: CFormView(IDD_DIALOG_PATIENT_RECORD)
{

}

CPatientRecordDlg::~CPatientRecordDlg()
{
}

void CPatientRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_patient_record);
	DDX_Control(pDX, IDC_EDIT1, first_name);
	DDX_Control(pDX, IDC_EDIT2, last_name);
	DDX_Control(pDX, IDC_EDIT3, gosh_id);
}

BEGIN_MESSAGE_MAP(CPatientRecordDlg, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CPatientRecordDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPatientRecordDlg diagnostics

#ifdef _DEBUG
void CPatientRecordDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPatientRecordDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPatientRecordDlg message handlers


void CPatientRecordDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// Creates a 12-point-Courier-font
	m_Title_Font.CreatePointFont(120, _T("Calibri"));
	GetDlgItem(IDC_STATIC)->SetFont(&m_Title_Font);


	m_Table_Font.CreatePointFont(100, _T("Calibri"));
	m_patient_record.SetFont(&m_Table_Font);

	// TODO: Add your specialized code here and/or call the base class

	m_patient_record.InsertColumn(1, TEXT("First Name"), LVCFMT_CENTER, 215);
	m_patient_record.InsertColumn(2, TEXT("Last Name"), LVCFMT_CENTER, 215);
	m_patient_record.InsertColumn(3, TEXT("Gosh ID"), LVCFMT_CENTER, 210);
	m_patient_record.SetExtendedStyle(m_patient_record.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//Create the ToolTip control
	if (!m_ToolTip.Create(this))
	{
		TRACE0("Unable to create the ToolTip!");
	}
	else
	{
		// Add tool tips to the controls, either by hard coded string 
		// or using the string table resource
		CWnd* tooltip_gosh_id = GetDlgItem(IDC_TOOLTIP_GOSH_ID);
		m_ToolTip.AddTool(tooltip_gosh_id, _T("Gosh ID should start with XXXXX"));
		m_ToolTip.Activate(TRUE);
	}

	std::thread(&CPatientRecordDlg::checkWakeUpServerMode, this).detach();

}

void CPatientRecordDlg::checkWakeUpServerMode()
{
	if (getWakeUpServerMode()) {
		// Update the control after the HTTP response is checked
		SetDlgItemTextW(IDC_WAKE_UP_SERVER, TEXT("Wake Up Server: ON"));
		getRequestPatient();
	}
	else {
		SetDlgItemTextW(IDC_WAKE_UP_SERVER, TEXT("Wake Up Server: OFF"));
	}
}

BOOL CPatientRecordDlg::getWakeUpServerMode()
{
	cpr::Response r = cpr::Get(cpr::Url{ "http://localhost:5001" });
	return (r.status_code == 200);
}


//BOOL CPatientRecordDlg::getWakeUpServerMode()
//{
//	cpr::Response r = cpr::Get(cpr::Url{ "http://localhost:5001" });
//	if (r.status_code == 200) {
//		GetDlgItem(IDC_WAKE_UP_SERVER)->SetWindowTextW(TEXT("Wake Up Server: ON"));
//		return true;
//	}
//	else {
//		GetDlgItem(IDC_WAKE_UP_SERVER)->SetWindowTextW(TEXT("Wake Up Server: OFF"));
//	}
//
//	return false;
//}

void CPatientRecordDlg::getRequestPatient()
{
	m_patient_record.DeleteAllItems();
	cpr::Response r = cpr::Get(cpr::Url{ "http://localhost:5001/users" });
	nlohmann::json jsonList = nlohmann::json::parse(r.text);

	for (const auto& item : jsonList) {
		CString first_name = CString(item["first_name"].get<std::string>().c_str());
		CString last_name = CString(item["last_name"].get<std::string>().c_str());
		CString gosh_id = CString(item["gosh_id"].get<std::string>().c_str());

		// Add the data to the list control
		int index = m_patient_record.InsertItem(m_patient_record.GetItemCount(), first_name);
		m_patient_record.SetItemText(index, 1, last_name);
		m_patient_record.SetItemText(index, 2, gosh_id);
	}
}


void CPatientRecordDlg::OnBnClickedButton1()
{
	CString edit_first_name;
	CString edit_last_name;
	CString edit_gosh_id;

	first_name.GetWindowTextW(edit_first_name);
	last_name.GetWindowTextW(edit_last_name);
	gosh_id.GetWindowTextW(edit_gosh_id);

	boolean postRequest = true;
	for (int i = 0; i < m_patient_record.GetItemCount(); ++i) {
		if (m_patient_record.GetItemText(i, 1) == edit_first_name &&
			m_patient_record.GetItemText(i, 2) == edit_last_name &&
			m_patient_record.GetItemText(i, 3) == edit_gosh_id) {
			// Item already exists, handle accordingly (e.g., show a message)
			MessageBox(TEXT("The patient already has been recorded!"));
			postRequest = false;
			break;
		}
	}

	if (postRequest) {
		std::string str_edit_first_name = CT2A(edit_first_name);
		std::string std_edit_last_name = CT2A(edit_last_name);
		std::string std_edit_gosh_id = CT2A(edit_gosh_id);

		cpr::Response response = cpr::Post(cpr::Url{ "http://localhost:5001/users" },
			cpr::Header{ {"Content-Type", "application/json"} },
			cpr::Body{ "{ \"first_name\": \"" + str_edit_first_name +
					   "\", \"last_name\": \"" + std_edit_last_name +
					   "\", \"gosh_id\": \"" + std_edit_gosh_id + "\" }" });

		if (response.status_code == 200) {
			getRequestPatient();
			AfxMessageBox(_T("Successly added!"), MB_ICONINFORMATION | MB_OK);
		}
		else {
			//auto json_error = nlohmann::json::parse(response.text);
			std::string error_message = response.text;
			CString m_error_message(error_message.c_str());

			CString m_error_status_code;
			m_error_status_code.Format(_T("%d Error: "), response.status_code);

			AfxMessageBox(m_error_status_code + m_error_message + "\n " + "Please check your input!", MB_ICONERROR | MB_OK);
		}
	}
}


BOOL CPatientRecordDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	m_ToolTip.RelayEvent(pMsg);
	return CFormView::PreTranslateMessage(pMsg);
}
