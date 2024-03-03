#pragma once



// CDevicesSettingDlg form view

class CDevicesSettingDlg : public CFormView
{
	DECLARE_DYNCREATE(CDevicesSettingDlg)

protected:
	CDevicesSettingDlg();           // protected constructor used by dynamic creation
	virtual ~CDevicesSettingDlg();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DEVICES_SETTING };
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton1();
private:
	CListCtrl m_triggers;
	CListCtrl m_target_devices;
	CFont m_Table_Font;
	void GetRequestTriggers();
	void GetRequestTargetDevices();
	CToolTipCtrl m_ToolTip;
	void checkWakeUpServerMode();
public:
	afx_msg void OnBnClickedButton4();
};


