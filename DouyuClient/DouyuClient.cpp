
// DouyuClient.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "DouyuClient.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/*ȫ�ֺ���*/
void xASSERT(bool expression, int line, CString filepath)
{
	if (!expression)
	{
		CString str;
		str.Format(_T("%s \r\nLine %d"), filepath, line);
		MessageBox(NULL, str, _T("_xASSERT"), MB_OK | MB_ICONERROR);
	}
}


bool safesend(SOCKET socket, char* pbuf, int len)
{
	int datalen = 0;
	while (len > 0)
	{
		datalen = send(socket, pbuf, len, 0);
		if (datalen == SOCKET_ERROR) break;
		len -= datalen;
		pbuf += datalen;
	}

	if (len != 0) return false;
	return true;
}

bool saferecv(SOCKET socket, char* pbuf, int len)
{
	int datalen = 0;
	while (len > 0)
	{
		datalen = recv(socket, pbuf, len, 0);
		if (datalen <= 0) break;
		len -= datalen;
		pbuf += datalen;

	}

	if (len != 0) return false;
	return true;
}

// CDouyuClientApp

BEGIN_MESSAGE_MAP(CDouyuClientApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CDouyuClientApp::OnAppAbout)
END_MESSAGE_MAP()


// CDouyuClientApp ����

CDouyuClientApp::CDouyuClientApp()
{
	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("DouyuClient.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CDouyuClientApp ����

CDouyuClientApp theApp;


// CDouyuClientApp ��ʼ��

BOOL CDouyuClientApp::InitInstance()
{
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// ���������ؿ�ܼ�����Դ
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);





	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

int CDouyuClientApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	return CWinApp::ExitInstance();
}

// CDouyuClientApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CDouyuClientApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDouyuClientApp ��Ϣ�������



