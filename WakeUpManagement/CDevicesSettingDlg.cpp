// CDevicesSettingDlg.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "CDevicesSettingDlg.h"


// CDevicesSettingDlg

IMPLEMENT_DYNCREATE(CDevicesSettingDlg, CFormView)

CDevicesSettingDlg::CDevicesSettingDlg()
	: CFormView(IDD_DIALOG_DEVICES_SETTING)
{

}

CDevicesSettingDlg::~CDevicesSettingDlg()
{
}

void CDevicesSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_controllers);
	DDX_Control(pDX, IDC_LIST2, m_matter_devices);
}

BEGIN_MESSAGE_MAP(CDevicesSettingDlg, CFormView)
END_MESSAGE_MAP()


// CDevicesSettingDlg diagnostics

#ifdef _DEBUG
void CDevicesSettingDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDevicesSettingDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDevicesSettingDlg message handlers


void CDevicesSettingDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	MoveWindow(200, 0, 1000, 700);

	m_Table_Font.CreatePointFont(100, _T("Calibri"));
	m_controllers.SetFont(&m_Table_Font);
	m_matter_devices.SetFont(&m_Table_Font);

	m_controllers.InsertColumn(0, TEXT("No."), LVCFMT_LEFT, 50);
	m_controllers.InsertColumn(1, TEXT("Controller Name"), LVCFMT_LEFT, 280);

	for (int i = 0; i < 8; i++) {
		int j = 0;
		CString number;
		number.Format(TEXT("%d"), i);
		CString name;
		name.Format(TEXT("Controller_%d"), i);
		m_controllers.InsertItem(i, number);
		m_controllers.SetItemText(i, ++j, name);
	}

	//property (show table lines)
	m_controllers.SetExtendedStyle(m_controllers.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_matter_devices.InsertColumn(0, TEXT("No."), LVCFMT_LEFT, 50);
	m_matter_devices.InsertColumn(1, TEXT("Matter Devices Name"), LVCFMT_LEFT, 280);

	for (int i = 0; i < 8; i++) {
		int j = 0;
		CString number;
		number.Format(TEXT("%d"), i);
		CString name;
		name.Format(TEXT("Matter Device_%d"), i);
		m_matter_devices.InsertItem(i, number);
		m_matter_devices.SetItemText(i, ++j, name);
	}

	//property (show table lines)
	m_matter_devices.SetExtendedStyle(m_matter_devices.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}
