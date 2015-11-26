
#include "ChatPopup.h"
#include "resource.h"

#include <wchar.h>

IPluginContext* g_pPluginContect = NULL;

ChatPopupElement::ChatPopupElement()
{
	m_hwnd = NULL;
	m_wch[0] = L'\0';
}

ChatPopupElement::~ChatPopupElement()
{

}

//�Զ���л������Ϣ
void ChatPopupElement::OnGiftMessage(int id, wchar_t* sendername, wchar_t* giftname, int giftnum)
{
	CString strmsg;
	strmsg.Format(L"%d(,)%s(,)gift(,)%s(,)%d", id, sendername, giftname, giftnum);
	RecordMessage(strmsg);
}

//��Ļ��Ϣ����Ϣ���ݣ������û����û����ڵȼ�
void ChatPopupElement::OnChatMessage(int id, wchar_t* msg, wchar_t* sendername, int deserveLev)
{
	CString strmsg;
	strmsg.Format(L"%d(,)%s(,)chat(,)%s", id, sendername, msg);
	RecordMessage(strmsg);
}

void ChatPopupElement::OnDonaterMessage(int id, wchar_t* senderName, int sliver)
{
	/*
	CString strname, strmsg;
	strname = L"����";
	strmsg.Format(L"лл[%s]�͵�%d������ !", senderName, sliver);

	RecordMessage(strname, strmsg);
	*/
}

void ChatPopupElement::RecordMessage(CString strmsg)
{
	if (m_hwnd == NULL)
	{
		m_hwnd = FindWindow(NULL, L"DouyuPluginClient");
		if (m_hwnd == NULL)
		{
			return;
		}
	}

	/*
	if (StrCmp(strmsg, m_wch) == 0)
		return;
	*/

	StrCpy(m_wch, strmsg);

	COPYDATASTRUCT copyData = { 0 };
	copyData.lpData = (PVOID)m_wch;
	copyData.cbData = sizeof(wchar_t) * WCH_BUFFERSIZE;
	copyData.dwData = 0;

	BOOL ret = ::SendMessage(m_hwnd, WM_COPYDATA, (WPARAM)0, (LPARAM)&copyData);
	if (ret != TRUE)
	{
		m_hwnd = NULL;
	}
}

//DLL��������
DYPLUGIN_LOAD IPlugin* dyLoadPlugin(IPluginContext* context)
{
	g_pPluginContect = context;
	return dynamic_cast<IPlugin*>(new ChatPopupPlugin);
}

//ж�ز��
DYPLUGIN_LOAD void dyUnloadPlugin(IPlugin* plugin)
{

}