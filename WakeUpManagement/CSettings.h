#pragma once



// CSettings form view

class CSettings : public CFormView
{
	DECLARE_DYNCREATE(CSettings)

protected:
	CSettings();           // protected constructor used by dynamic creation
	virtual ~CSettings();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGS };
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


