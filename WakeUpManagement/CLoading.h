#pragma once
#include "afxdialogex.h"


// CLoading dialog

class CLoading : public CDialogEx
{
	DECLARE_DYNAMIC(CLoading)

public:
	CLoading(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CLoading();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOADING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CStatic m_staticMessage;

public:
	virtual BOOL OnInitDialog();
};
