#pragma once


// CPatientRecordDlg form view

class CPatientRecordDlg : public CFormView
{
	DECLARE_DYNCREATE(CPatientRecordDlg)

protected:
	CPatientRecordDlg();           // protected constructor used by dynamic creation
	virtual ~CPatientRecordDlg();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PATIENT_RECORD };
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
	afx_msg void OnBnClickedButton1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	CFont m_Title_Font;
	CFont m_Table_Font;
	CEdit first_name;
	CEdit last_name;
	CEdit gosh_id;
	CListCtrl m_patient_record;
	void getRequestPatient();
	CToolTipCtrl m_ToolTip;
	void checkWakeUpServerMode();
	bool wake_up_server_mode;
};


