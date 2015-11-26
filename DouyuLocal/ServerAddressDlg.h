#pragma once
#include "afxcmn.h"


// CServerAddressDlg dialog

class CServerAddressDlg : public CDialog
{
	DECLARE_DYNAMIC(CServerAddressDlg)

public:
	BYTE m_svnf1, m_svnf2, m_svnf3, m_svnf4;

public:
	CServerAddressDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CServerAddressDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SERVERADDRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CIPAddressCtrl m_ipctrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
};
