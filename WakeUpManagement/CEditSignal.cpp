// CEditSignal.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "afxdialogex.h"
#include "CEditSignal.h"


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
