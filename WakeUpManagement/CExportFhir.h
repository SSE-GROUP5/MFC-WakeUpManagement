#pragma once



// CExportFhir form view

class CExportFhir : public CFormView
{
	DECLARE_DYNCREATE(CExportFhir)

protected:
	CExportFhir();           // protected constructor used by dynamic creation
	virtual ~CExportFhir();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPORT_FHIR };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
private:
	CFont m_Title_Font;
};


