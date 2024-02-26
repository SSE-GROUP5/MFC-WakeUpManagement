// CDeleteSignal.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "afxdialogex.h"
#include "CDeleteSignal.h"
#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>


// CDeleteSignal dialog

IMPLEMENT_DYNAMIC(CDeleteSignal, CDialogEx)

CDeleteSignal::CDeleteSignal(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DELETE_SIGNAL, pParent)
{

}

CDeleteSignal::~CDeleteSignal()
{
}

void CDeleteSignal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDeleteSignal, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDeleteSignal::OnBnClickedOk)
END_MESSAGE_MAP()


// CDeleteSignal message handlers


void CDeleteSignal::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	std::string str_trigger_name = CT2A(trigger_name);
	std::string std_trigger_action = CT2A(trigger_action);
	int int_trigger_value = _ttoi(trigger_value);
	std::string std_target_id = CT2A(target_id);
	std::string std_target_action = CT2A(target_action);
	std::string std_user_id = CT2A(user_id);

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
				trigger_value == get_trigger_value &&
				target_id == CString(item["target_id"].get<std::string>().c_str()) &&
				target_action == CString(item["target_action"].get<std::string>().c_str()) &&
				(user_id.IsEmpty() && item["user_id"].is_null() || user_id == CString(item["user_id"].get<std::string>().c_str())))
			{
				std::string signal_id = item["id"].get<std::string>();
				response_delete = cpr::Delete(cpr::Url{ "http://localhost:5001/signals/" + signal_id },
									cpr::Header{ {"Content-Type", "application/json"} });
				if (response_delete.status_code == 200) {
					CDialogEx::OnOK();
					AfxMessageBox(_T("Successly deleted!"), MB_ICONINFORMATION | MB_OK);
				}
				else {
					auto json_error = nlohmann::json::parse(response_delete.text);
					std::string error_message = json_error["message"];
					CString m_error_message(error_message.c_str());

					CString m_error_status_code;
					m_error_status_code.Format(_T("%d Error: "), response_delete.status_code);

					AfxMessageBox(m_error_status_code + m_error_message + "\n ", MB_ICONERROR | MB_OK);
				}
				break;
			}
		}
	}
}


BOOL CDeleteSignal::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_Title_Font.CreatePointFont(120, _T("Calibri"));
	GetDlgItem(IDC_STATIC)->SetFont(&m_Title_Font);
	GetDlgItem(IDC_TEXT)->ModifyStyle(0, SS_CENTER);
	GetDlgItem(IDC_TEXT)->SetWindowText(TEXT("Trigger Name: ") + trigger_name +
										"\n\nTrigger Action: " + trigger_action +
										"\n\nTrigger Value: " + trigger_value +
										"\n\nTarget ID: " + target_id +
										"\n\nTarget Action: " + target_action);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
