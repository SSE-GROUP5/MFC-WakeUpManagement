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

	m_patient_record.InsertColumn(1, TEXT("First Name"), LVCFMT_LEFT, 320);
	m_patient_record.InsertColumn(2, TEXT("Last Name"), LVCFMT_LEFT, 320);
	m_patient_record.SetExtendedStyle(m_patient_record.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	getRequestPatient();
}

void CPatientRecordDlg::getRequestPatient()
{
	m_patient_record.DeleteAllItems();
	cpr::Response r = cpr::Get(cpr::Url{ "http://localhost:5001/users" });
	nlohmann::json jsonList = nlohmann::json::parse(r.text);

	for (const auto& item : jsonList) {
		CString first_name = CString(item["first_name"].get<std::string>().c_str());
		CString last_name = CString(item["last_name"].get<std::string>().c_str());

		// Add the data to the list control
		int index = m_patient_record.InsertItem(m_patient_record.GetItemCount(), first_name);
		m_patient_record.SetItemText(index, 1, last_name);
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
