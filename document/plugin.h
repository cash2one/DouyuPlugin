#ifndef __LIVEPLUGIN_H__
#define __LIVEPLUGIN_H__

#include <stdint.h>

//����汾�ţ����˰汾�Ų��������°汾��ʱ���������ʧ��
#define PLUGIN_VERSION		20151109

//ͼ�����ظ�ʽ�������ڴ�ģ������(������Ϊ�ڴ���������)
enum PluginPixelFormat
{
	PPF_NONE,
	PPF_ARGB32,   //B0 B1 B2 B3 B4 B5 B6 B7 G0 G1 G2 G3 G4 G5 G6 G7 R0 R1 R2 R3 R4 R5 R6 R7 A0 A1 A2 A3 A4 A5 A6 A7
	PPF_BGRA32,   //R0 R1 R2 R3 R4 R5 R6 R7 G0 G1 G2 G3 G4 G5 G6 G7 B0 B1 B2 B3 B4 B5 B6 B7 A0 A1 A2 A3 A4 A5 A6 A7
	PPF_I420,     //YUV 420 12bit ƽ���ʽ����ΪY��U��V����ƽ�� Y0 Y1 Y2 Y3  U0 U1   V0 V1
	PPF_NV12,     //YUV 420 12bit ƽ���ʽ����ΪY��UV����ƽ��   Y0 Y1 Y2 Y3  U0V0 U1V1
	PPF_RGB565,   //B0 B1 B2 B3 B4 G0 G1 G2 G3 G4 G5 R0 R1 R2 R3 R4
	PPF_RGB555,   //B0 B1 B2 B3 B4 G0 G1 G2 G3 G4 R0 R1 R2 R3 R4 A0
	PPF_YUY2,     //YUV 422 16bit �����ʽ��ֻ��һ��ƽ�� Y0U0Y1V0 Y2U1Y3V1
	PPF_UYVY,     //YUV 422 16bit �����ʽ��ֻ��һ��ƽ�� U0Y0V0Y1 U1Y2V1Y3
	PPF_YVYU,	  //YUV 422 16bit �����ʽ��ֻ��һ��ƽ�� Y0V0Y1U0 Y2V1Y3U1
	PPF_YV12,     //YUV 420 12bit ƽ���ʽ����ΪY��U��V����ƽ�� Y0 Y1 Y2 Y3  V0 V1   U0 U1
};

//��Ƶ��ʽ
enum PluginAudioFormat
{
	PAF_NONE,
	PAF_S16,
	PAF_S32,
	PAF_FLOAT,
};

//ͼ��Դ����
enum ImageSourceType
{
	IST_CAMERA,									//����ͷ����
	IST_IMAGE,									//ͼƬ����
	IST_VIDEO,									//��Ƶ�ļ�����
	IST_PLUGIN,									//���ͼ������
};

typedef struct _PluginElementParam
{
	int videoWidth;								//ͼ����
	int videoHeight;							//ͼ��߶�
	PluginPixelFormat pf;						//���ظ�ʽ
	int pitch[3];								//�о�

	PluginAudioFormat af;		
	int audioSampleRate;						//��Ƶ������
	int audioChannels;							//��Ƶ������
	int audioFramesize;							//һ�β�����
	int audioSound;								//����ֵ

	int pid;									//���Դid
	float dstTop;								//��ʾ�����󶥵�Y����ֵ,[0,1]��һ������
	float dstLeft;								//��ʾ�����󶥵�X����ֵ
	float dstWidth;								//��ʾ������
	float dstHeight;							//��ʾ����߶�
}PluginElementParam;

//������Ӧ�ü̳�ʵ�����в��ֽӿ�
class IPluginElement
{
public:
	virtual bool Open(char* env){return false;}

	//��ȡͼ��
	virtual bool ReadVideo(uint8_t* pixels,int bufsize){return false;}

	//��ȡ��Ƶ
	virtual bool ReadAudio(uint8_t* pcm,int bufsize){return false;}

	//���ٲ��
	virtual bool Destroy(){return false;}

	//��ȡ����
	virtual bool GetParameters(PluginElementParam* param){return false;}

	//��������
	virtual void SetConfig(){}

	//֪ͨ��ʼֱ��
	virtual void OnStartLive(){}

	//ֱֹ֪ͨͣ��
	virtual void OnStopLive(){}

	//��Ļ��Ϣ����Ϣ���ݣ������û����û����ڵȼ�
	virtual void OnChatMessage(int id,wchar_t* msg,wchar_t* sendername,int deserveLev){}

	//����������Ϣ:�û�id���û��ǳƣ�����������

	virtual void OnDonaterMessage(int id,wchar_t* senderName,int sliver){}

	//������Ϣ���û�ID���û��ǳƣ��������������͵ȼ�
	virtual void OnDeserveMessage(int id,wchar_t* senderName,int num,int lev){}

	//�û����뷿����Ϣ��
	virtual void OnUserEnterMessage(wchar_t* senderName,int lev){}

	//��ȡ��ǰ״̬
	virtual char* GetCurrentStatus(){return NULL;}

	//ͼ��Դ���ͣ�������һЩ��Ƶ����
	//dataid:capturedata�ı�ʾ��
	//pixels:���أ����Ϊ��YUV ƽ�����ͣ���ֻ��pixels[0]��Ч
	//pitch:�о࣬���Ϊ��YUVƽ�����ͣ���ֻ��pitch[0]��Ч
	//width:ͼ����
	//height:ͼ��߶�
	//fmt:��������
	//type:����Դ����
	virtual bool OnCaptureData(int dataid,uint8_t* pixels[3],int pitch[3],int width,int height,PluginPixelFormat fmt,ImageSourceType type){return false;}

	//�Ƿ���ʾ���
	virtual void ShowPlugin(bool show){}

	//������Ϣ
	//id���û�id
	//sendername:�������ǳ�
	//giftname:��������
	//giftnum:���͵���������
	virtual void OnGiftMessage(int id,wchar_t* sendername,wchar_t* giftname,int giftnum){};

	//����������Ϣ����Ҫ�Լ�����
	virtual void OnNetworkMessage(const wchar_t* msg){};


	//�ַ�windowsϵͳ��Ϣ
	virtual void OnWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){}

};

//������Ӧ�ü̳�ʵ�ָýӿ�
class IPlugin
{
public:
	//��ȡ�������
	virtual char* GetName() = 0;

	virtual IPluginElement* CreateElement() = 0;
};

//������������л���
class IPluginContext
{
public:
	//���͵�Ļ������Ϣ
	//����msg:UNICODE����
	virtual bool SendChatMessage(wchar_t* msg) = 0;

	//��ȡ��½��
	virtual bool GetUserName(char* name) = 0;

	//��ȡ��½���û�ID
	virtual bool GetUserID(int* id) = 0;

	//�Ƿ�����ֱ��
	virtual bool IsBroadcasting(int* broad) = 0;

	//��ȡLiveEncoderָ��
	virtual class ILiveEncoder* GetLiveEncoder() = 0;

	//��ȡ����ӿ�ָ�룬����������صĶ����߼�
	virtual class ILiveencoderNetService* GetNetService() = 0;
};


#ifndef DYPLUGIN_EXPORT
#define DYPLUGIN_LOAD extern "C" __declspec(dllimport)
#else
#define DYPLUGIN_LOAD extern "C" __declspec(dllexport)
#endif

//���ز��
DYPLUGIN_LOAD IPlugin* dyLoadPlugin(IPluginContext* context);

//ж�ز��
DYPLUGIN_LOAD void dyUnloadPlugin(IPlugin* plugin);

//����������ȷ������İ汾�ţ������߲���Ҫ�Դ˺��������κε��޸�
DYPLUGIN_LOAD inline int GetPluginVersion()
{
	return PLUGIN_VERSION;
}

#endif