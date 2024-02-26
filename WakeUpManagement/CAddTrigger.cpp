// CAddTrigger.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "afxdialogex.h"
#include "CAddTrigger.h"
#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>


// CAddTrigger dialog

IMPLEMENT_DYNAMIC(CAddTrigger, CDialogEx)

CAddTrigger::CAddTrigger(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADD_TRIGGER, pParent)
{

}

CAddTrigger::~CAddTrigger()
{
}

void CAddTrigger::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, trigger_name);
	DDX_Control(pDX, IDC_EDIT3, trigger_type);
	DDX_Control(pDX, IDC_EDIT1, trigger_wake_up_server_url);
	DDX_Control(pDX, IDC_EDIT4, trigger_zmq_url);
}


BEGIN_MESSAGE_MAP(CAddTrigger, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddTrigger::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddTrigger message handlers


void CAddTrigger::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString edit_trigger_name;
	CString edit_trigger_type;
	CString edit_trigger_wake_up_server_url;
	CString edit_trigger_zmq_url;

	trigger_name.GetWindowTextW(edit_trigger_name);
	trigger_type.GetWindowTextW(edit_trigger_type);
	trigger_wake_up_server_url.GetWindowTextW(edit_trigger_wake_up_server_url);
	trigger_zmq_url.GetWindowTextW(edit_trigger_zmq_url);


	std::string str_edit_trigger_name = CT2A(edit_trigger_name);
	std::string std_edit_trigger_type = CT2A(edit_trigger_type);
	std::string std_edit_trigger_wake_up_server_url = CT2A(edit_trigger_wake_up_server_url);
	std::string std_edit_trigger_zmq_url = CT2A(edit_trigger_zmq_url);

	if (!str_edit_trigger_name.empty() && !std_edit_trigger_type.empty()) {
		cpr::Response response = cpr::Post(cpr::Url{ "http://localhost:5001/triggers" },
				cpr::Header{ {"Content-Type", "application/json"} },
				cpr::Body{ "{ \"name\": \"" + str_edit_trigger_name +
							"\", \"type\": \"" + std_edit_trigger_type + "\" }" });

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
	else {
		MessageBox(TEXT("Input cannot be null!"));
	}

	

}
