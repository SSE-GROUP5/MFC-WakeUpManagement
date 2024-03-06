#pragma once
#include "afxdialogex.h"


// CAddTarget dialog

class CAddTarget : public CDialogEx
{
	DECLARE_DYNAMIC(CAddTarget)

public:
	CAddTarget(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAddTarget();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADD_TARGET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
private:
	CEdit target_id;
	CEdit target_name;
	CEdit target_type;
};
