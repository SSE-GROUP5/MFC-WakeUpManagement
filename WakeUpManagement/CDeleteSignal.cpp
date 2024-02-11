// CDeleteSignal.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "afxdialogex.h"
#include "CDeleteSignal.h"


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
	CDialogEx::OnOK();
}


BOOL CDeleteSignal::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	GetDlgItem(IDC_TEXT)->SetWindowText(TEXT("Controller id: ") + controller_id + 
										"\nController Name: " + controller_name);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
