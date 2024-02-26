#pragma once
#include "afxdialogex.h"


// CAddTrigger dialog

class CAddTrigger : public CDialogEx
{
	DECLARE_DYNAMIC(CAddTrigger)

public:
	CAddTrigger(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAddTrigger();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADD_TRIGGER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
private:
	CEdit trigger_name;
	CEdit trigger_wake_up_server_url;
	CEdit trigger_zmq_url;
	CComboBox trigger_type;
public:
	virtual BOOL OnInitDialog();
private:
};
