#pragma once



// CWakeUpSettingDlg form view

class CWakeUpSettingDlg : public CFormView
{
	DECLARE_DYNCREATE(CWakeUpSettingDlg)

protected:
	CWakeUpSettingDlg();           // protected constructor used by dynamic creation
	virtual ~CWakeUpSettingDlg();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WAKE_UP_SETTING };
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
	CListCtrl m_wake_up_setting_list;
private:
	CFont m_Title_Font;
private:
	CFont m_Table_Font;
public:
	afx_msg void OnBnClickedButton1();
};


