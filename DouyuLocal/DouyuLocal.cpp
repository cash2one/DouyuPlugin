
// DouyuLocal.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "DouyuLocal.h"
#include "DouyuLocalDlg.h"

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

// CDouyuLocalApp

BEGIN_MESSAGE_MAP(CDouyuLocalApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDouyuLocalApp ����

CDouyuLocalApp::CDouyuLocalApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDouyuLocalApp ����

CDouyuLocalApp theApp;


// CDouyuLocalApp ��ʼ��

BOOL CDouyuLocalApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CDouyuLocalDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

