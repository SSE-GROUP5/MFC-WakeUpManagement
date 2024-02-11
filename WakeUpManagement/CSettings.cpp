// CSettings.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "CSettings.h"


// CSettings

IMPLEMENT_DYNCREATE(CSettings, CFormView)

CSettings::CSettings()
	: CFormView(IDD_SETTINGS)
{

}

CSettings::~CSettings()
{
}

void CSettings::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSettings, CFormView)
END_MESSAGE_MAP()


// CSettings diagnostics

#ifdef _DEBUG
void CSettings::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSettings::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSettings message handlers


void CSettings::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_Title_Font.CreatePointFont(200, _T("Calibri"));
	GetDlgItem(IDC_STATIC)->SetFont(&m_Title_Font);
	// TODO: Add your specialized code here and/or call the base class
}
