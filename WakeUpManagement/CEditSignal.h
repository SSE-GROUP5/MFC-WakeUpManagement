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
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
};
