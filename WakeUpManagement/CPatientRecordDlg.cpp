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
}

BEGIN_MESSAGE_MAP(CPatientRecordDlg, CFormView)
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

	m_patient_record.InsertColumn(0, TEXT("Wake Up System ID"), LVCFMT_LEFT, 500);
	m_patient_record.InsertColumn(1, TEXT("First Name"), LVCFMT_LEFT, 245);
	m_patient_record.InsertColumn(2, TEXT("Last Name"), LVCFMT_LEFT, 245);
	m_patient_record.InsertColumn(3, TEXT("Gosh ID"), LVCFMT_LEFT, 250);

	//CString str[] = { TEXT("Wake Up System ID"), TEXT("First Name"), TEXT("Last Name"), TEXT("Gosh ID") };
	//for (int i = 0; i < 4; i++) {
	//	//title
	//	m_patient_record.InsertColumn(i, str[i], LVCFMT_LEFT, 200);
	//}

	cpr::Response r = cpr::Get(cpr::Url{ "http://localhost:5001/users" });
	nlohmann::json jsonList = nlohmann::json::parse(r.text);

	for (const auto& item : jsonList) {
		CString id = CString(item["id"].get<std::string>().c_str());
		CString first_name = CString(item["first_name"].get<std::string>().c_str());
		CString last_name = CString(item["last_name"].get<std::string>().c_str());
		CString gosh_id = CString(item["gosh_id"].get<std::string>().c_str());

		// Add the data to the list control
		int index = m_patient_record.InsertItem(m_patient_record.GetItemCount(), id);
		m_patient_record.SetItemText(index, 1, first_name);
		m_patient_record.SetItemText(index, 2, last_name);
		m_patient_record.SetItemText(index, 3, gosh_id);
	}

	//property (show table lines)
	m_patient_record.SetExtendedStyle(m_patient_record.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

}
