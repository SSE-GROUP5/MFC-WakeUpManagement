#pragma once
#include "afxdialogex.h"


// CDeleteSignal dialog

class CDeleteSignal : public CDialogEx
{
	DECLARE_DYNAMIC(CDeleteSignal)

public:
	CDeleteSignal(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDeleteSignal();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DELETE_SIGNAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	CString controller_id;
	CString controller_name;
	virtual BOOL OnInitDialog();
};
