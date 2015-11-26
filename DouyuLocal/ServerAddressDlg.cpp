// ServerAddressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DouyuLocal.h"
#include "ServerAddressDlg.h"
#include "afxdialogex.h"

#include "DouyuLocal.h"


// CServerAddressDlg dialog

IMPLEMENT_DYNAMIC(CServerAddressDlg, CDialog)

CServerAddressDlg::CServerAddressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerAddressDlg::IDD, pParent)
{
	m_svnf1 = 0;
	m_svnf2 = 0;
	m_svnf3 = 0;
	m_svnf4 = 0;
}

CServerAddressDlg::~CServerAddressDlg()
{
}

void CServerAddressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ipctrl);
}


BEGIN_MESSAGE_MAP(CServerAddressDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CServerAddressDlg::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CServerAddressDlg message handlers


void CServerAddressDlg::OnBnClickedOk()
{
	m_ipctrl.GetAddress(m_svnf1, m_svnf2, m_svnf3, m_svnf4);
	CDialog::OnOK();
}


BOOL CServerAddressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_xASSERT(m_svnf1 >= 0 && m_svnf1 <= 255);
	_xASSERT(m_svnf2 >= 0 && m_svnf2 <= 255);
	_xASSERT(m_svnf3 >= 0 && m_svnf3 <= 255);
	_xASSERT(m_svnf4 >= 0 && m_svnf4 <= 255);

	m_ipctrl.SetAddress(m_svnf1, m_svnf2, m_svnf3, m_svnf4);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CServerAddressDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}
