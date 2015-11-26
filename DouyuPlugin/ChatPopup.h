#pragma once

#define WCH_BUFFERSIZE 1024

#define DYPLUGIN_EXPORT
#include "plugin.h"

class ChatPopupElement;

class ChatPopupElement : public IPluginElement
{
public:
	HWND m_hwnd;
	wchar_t m_wch[WCH_BUFFERSIZE];

public:
	virtual bool Open(char* env) {return true;}
	virtual void OnChatMessage(int id, wchar_t* msg, wchar_t* sendername, int deserveLev);
	virtual void OnGiftMessage(int id, wchar_t* sendername, wchar_t* giftname, int giftnum);
	virtual void OnDonaterMessage(int id, wchar_t* senderName, int sliver);

	void RecordMessage(CString strmsg);

public:
	ChatPopupElement();
	~ChatPopupElement();

};


class ChatPopupPlugin : public IPlugin
{
public:
	ChatPopupElement* m_elem;

	ChatPopupPlugin() {m_elem = NULL;}
	~ChatPopupPlugin() {if (m_elem) delete m_elem;}
	//获取插件名称
	virtual char* GetName()
	{
		return "ChatPopupPlugin";
	}

	//创建插件接口
	virtual IPluginElement* CreateElement()
	{
		if (m_elem == NULL)
		{
			m_elem = new ChatPopupElement();
		}

		return dynamic_cast<IPluginElement*>(m_elem);

	}
};
