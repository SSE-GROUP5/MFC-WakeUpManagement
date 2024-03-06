// CExportFhir.cpp : implementation file
//

#include "pch.h"
#include "WakeUpManagement.h"
#include "CExportFhir.h"


// CExportFhir

IMPLEMENT_DYNCREATE(CExportFhir, CFormView)

CExportFhir::CExportFhir()
	: CFormView(IDD_EXPORT_FHIR)
{

}

CExportFhir::~CExportFhir()
{
}

void CExportFhir::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CExportFhir, CFormView)
END_MESSAGE_MAP()


// CExportFhir diagnostics

#ifdef _DEBUG
void CExportFhir::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CExportFhir::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CExportFhir message handlers


void CExportFhir::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_Title_Font.CreatePointFont(180, _T("Calibri"));
	GetDlgItem(IDC_STATIC)->SetFont(&m_Title_Font);
	// TODO: Add your specialized code here and/or call the base class
}
