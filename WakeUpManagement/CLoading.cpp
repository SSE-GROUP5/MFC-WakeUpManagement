// CLoading.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "afxdialogex.h"
#include "CLoading.h"


// CLoading dialog

IMPLEMENT_DYNAMIC(CLoading, CDialogEx)

CLoading::CLoading(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOADING, pParent)
{
    
}

CLoading::~CLoading()
{
}

void CLoading::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE, m_staticMessage);
}


BEGIN_MESSAGE_MAP(CLoading, CDialogEx)
END_MESSAGE_MAP()


 //CLoading message handlers
BOOL CLoading::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Initialize message and dots count
    m_staticMessage.SetWindowText(_T("Reaching Wake Up Server Internet......"));

    return TRUE;
}
