#pragma once
#include "afxdialogex.h"


// CEditSignal dialog

class CEditSignal : public CDialogEx
{
	DECLARE_DYNAMIC(CEditSignal)

public:
	CEditSignal(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CEditSignal();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CToolTipCtrl m_ToolTip;
	CFont m_Title_Font;
	CComboBox cb_trigger_name;
	CComboBox cb_trigger_action;
	CEdit editBox_trigger_value;
	CComboBox cb_target_id;
	CComboBox cb_target_action;
	void GetRequestForTriggersCombo();
	void GetRequestForTargetsCombo();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo3();
	CString trigger_name;
	CString trigger_action;
	CString trigger_value;
	CString target_id;
	CString target_action;
	CString user_id;
};
