#pragma once
#include "afxdialogex.h"


// CAddSignal dialog

class CAddSignal : public CDialogEx
{
	DECLARE_DYNAMIC(CAddSignal)

public:
	CAddSignal(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAddSignal();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADD_SIGNAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	CString user_id;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnBnClickedOk();
private:
	void GetRequestForTriggersCombo();
	void GetRequestForTargetsCombo();
	CFont m_Title_Font;
	CComboBox cb_trigger_name;
	CComboBox cb_trigger_action;
	CEdit editBox_trigger_value;
	CComboBox cb_target_id;
	CComboBox cb_target_action;
	CToolTipCtrl m_ToolTip;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
