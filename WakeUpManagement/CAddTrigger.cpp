// CAddTrigger.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "afxdialogex.h"
#include "CAddTrigger.h"
#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>

extern CString global_wake_up_server_url;

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
	DDX_Control(pDX, IDC_EDIT1, trigger_wake_up_server_url);
	DDX_Control(pDX, IDC_EDIT4, trigger_zmq_url);
	DDX_Control(pDX, IDC_COMBO1, trigger_type);
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
		std::string wake_up_server_url = CT2A(global_wake_up_server_url);
		cpr::Response response = cpr::Post(cpr::Url{ wake_up_server_url + "triggers" },
				cpr::Header{ {"Content-Type", "application/json"} },
				cpr::Body{ "{ \"name\": \"" + str_edit_trigger_name +
							"\", \"type\": \"" + std_edit_trigger_type + "\" }" });

			if (response.status_code == 201) {
				auto json_response = nlohmann::json::parse(response.text);
				std::string trigger_id = json_response["id"];
				std::string folderPath = "C:\\Program Files\\wakeup_triggers\\" + std_edit_trigger_type;

				// Check if the folder exists
				if (!fs::exists(folderPath)) {
					// Create the folder if it doesn't exist
					if (!fs::create_directories(folderPath)) {
						std::cerr << "Error creating folder: " << folderPath << std::endl;
						/*return 1;*/ // Exit with an error code
					}
				}

				// Construct the file path
				std::string filePath = folderPath + "\\env_trigger.txt";
				std::ofstream file(filePath, std::ios::trunc);

				if (file.is_open()) {
					file << "ID=" << trigger_id << "\n"
						 << "WAKEUP_SERVER_URL=" << std_edit_trigger_wake_up_server_url << "\n"
						 << "ZMQ_SERVER=" << std_edit_trigger_zmq_url << "\n";

					file.close();
					CDialogEx::OnOK();
					AfxMessageBox(_T("Successly added!"), MB_ICONINFORMATION | MB_OK);
				}
				else {
					// Failed to open the file
					AfxMessageBox(TEXT("Failed to Open env_trigger.txt!"), MB_ICONERROR | MB_OK);
				}
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


BOOL CAddTrigger::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	trigger_wake_up_server_url.SetWindowText(_T("http://localhost:5001"));
	trigger_zmq_url.SetWindowText(_T("tcp://127.0.0.1:5556"));
	trigger_type.AddString(_T("blink_detect"));
	trigger_type.AddString(_T("morse_vision"));
	trigger_type.AddString(_T("morse_audio"));
	trigger_type.AddString(_T("fall_detection"));
	trigger_type.AddString(_T("fall_upper_detection"));
	trigger_type.AddString(_T("whisper"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
