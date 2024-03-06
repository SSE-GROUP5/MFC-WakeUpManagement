// CAddTarget.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "afxdialogex.h"
#include "CAddTarget.h"
#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>

extern CString global_wake_up_server_url;

// CAddTarget dialog

IMPLEMENT_DYNAMIC(CAddTarget, CDialogEx)

CAddTarget::CAddTarget(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADD_TARGET, pParent)
{

}

CAddTarget::~CAddTarget()
{
}

void CAddTarget::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, target_id);
	DDX_Control(pDX, IDC_EDIT2, target_name);
	DDX_Control(pDX, IDC_EDIT3, target_type);
}


BEGIN_MESSAGE_MAP(CAddTarget, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddTarget::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddTarget message handlers


void CAddTarget::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString edit_target_id;
	CString edit_target_name;
	CString edit_target_type;

	target_id.GetWindowTextW(edit_target_id);
	target_name.GetWindowTextW(edit_target_name);
	target_type.GetWindowTextW(edit_target_type);

	std::string str_edit_target_id = CT2A(edit_target_id);
	std::string std_edit_target_name = CT2A(edit_target_name);
	std::string std_edit_target_type = CT2A(edit_target_type);

	if (!str_edit_target_id.empty() && !std_edit_target_name.empty() && !std_edit_target_type.empty()) {
		std::string wake_up_server_url = CT2A(global_wake_up_server_url);
		cpr::Response response = cpr::Post(cpr::Url{ wake_up_server_url + "target_devices" },
			cpr::Header{ {"Content-Type", "application/json"} },
			cpr::Body{ "{ \"matter_id\": \"" + str_edit_target_id +
						"\", \"name\": \"" + std_edit_target_name +
						"\", \"type\": \"" + std_edit_target_type + "\" }" });

		if (response.status_code == 201) {
			CDialogEx::OnOK();
			AfxMessageBox(_T("Successly added!"), MB_ICONINFORMATION | MB_OK);
		}
		else {
			auto json_error = nlohmann::json::parse(response.text);
			std::string error_message = json_error["message"];
			CString m_error_message(error_message.c_str());

			CString m_error_status_code;
			m_error_status_code.Format(_T("%d Error: "), response.status_code);

			AfxMessageBox(m_error_status_code + m_error_message + "\n ", MB_ICONERROR | MB_OK);
		}
	}
	else {
		AfxMessageBox(TEXT("Input cannot be empty!"), MB_ICONERROR | MB_OK);
	}
}
