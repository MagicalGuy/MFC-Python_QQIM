#pragma once
#include "stdafx.h"
#include <windows.h>
#include <map>

using namespace std;

#define BUFMSG 2048
#define BUFNAME  50


//ͷ��;����Ŀ��;�������ͷ����һ��;
enum CHATPURPOSE {
	REGISTER = 1,				//   ע��
	LOGIN = 2,					//   ��¼
	SEARCHUSER = 3,				//   ����
	ADDFRIEND = 4,				//	 ��Ӻ���
	ONE2ONE = 5,				//   1v1
	ONE2GROUP = 6,				//   1vN	
	MSGTEXT = 7,				//   ������Ϣ;
	MSGOFFLINE = 8,				//   ������Ϣ
	MSGIMAGE = 9,				//	 ͼƬ
	MSGVOICE = 10,				//	 ����
	MSGVIDEO = 11,				//   ��Ƶ
	FILETRANS = 12,				//   �ļ�����
	UPDATEOL = 13,				//   ��������״̬
	UPDATEFRIEND = 14,			//   ���º����б�;
	UPDATEGROUP = 15,			//   ����Ⱥ�б�;
	UPDATEGROUPMEMBER = 16,		//   ����Ⱥ�б�;
	SEARCHGROUP = 17,			//   ����Ⱥ
	REPLOGIN = 18,				//   �ظ���¼
	MSGRECORD = 19              //�����¼
};


//��������;

//ע��;
typedef struct _CHATREGISTER
{
	CHAR szName[50];
	CHAR szPwd[50];
	//CHAR szSex[2];
}CHATREGISTER;

//��¼
typedef struct _CHATLOGIN
{
	CHAR szName[50];
	CHAR szPwd[50];
}CHATLOGIN;

//����
typedef struct _CHATSEARCHUSER
{
	CHAR szUID[50];
	CHAR szName[50];
}CHATSEARCHUSER;


//��Ӻ���
typedef struct _CHATADDFRIEND
{
	DWORD dwAdd;
	CHAR szName[50];
	CHAR szFrd[50];
}CHATADDFRIEND;

//1v1
typedef struct _CHATONE2ONE
{
	CHAR szUid[20];
	CHAR szUname[20];
	CHAR szFid[20];
	CHAR szTime[30];
	CHAR szContent[1024];
}CHATONE2ONE;

//1vN
typedef struct _CHATGROUP
{
	CHAR szGID[20];
	CHAR szTime[30];
	CHAR szName[20];
	CHAR szContent[1024];
}CHATGROUP;

//����������Ϣ
typedef struct _CHATUPDATAOL
{
	DWORD bOnline;
	DWORD dwLen;
	CHAR buf[50];
}CHATUPDATAOL;

//�����û���Ϣ
typedef struct _CHATUPDATUSERINFO
{
	DWORD bOnline;
	DWORD dwLen;
	CHAR buf[50];
}CHATUPDATUSERINFO;

//��ȡȺ��Ա�б�
typedef struct _ROUPMEMBER
{
	CHAR gid[50];
	CHAR mid[50];
}GROUPMEMBER;

//���������¼
typedef struct _CHATMSGOFFLINE
{
	CHAR szFrom[50];
	CHAR szTo[50];
	CHAR szContent[1024];
}CHATMSGOFFLINE;


//�����������¼
typedef struct _CHATMSGRECORD {
	//CHAR szFrom[50];
	//CHAR szTo[50];
	//CHAR szContent[1024];
	CHAR szUid[20];
	CHAR szUname[20];
	CHAR szFid[20];
	CHAR szTime[30];
	CHAR szContent[1024];

}CHATMSGRECORD;


//������Ϣ

//��Ƶ��Ϣ
typedef struct _CHATVIDEO
{
	DWORD dwType;
	CHAR szUid[20];
	CHAR szContent[20];
	CHAR szFid[20];
}CHATVIDEO;

//�ļ�����
typedef struct _CHATFILE
{
	DWORD dwType;
	DWORD dwlength;
	CHAR szUid[50];
	CHAR szFid[50];
	CHAR szFileName[255];
	CHAR szFileContent[1024];
}CHATFILE;


//���շ�������
typedef struct _CHATSEND
{
	CHATPURPOSE m_type;
	union {
		char buf[BUFMSG];
		CHATREGISTER reg;//ע��
		CHATLOGIN log;//��¼
		CHATSEARCHUSER su;//�����û�
		CHATADDFRIEND adf;//��Ӻ���
		CHATONE2ONE	o2o;//˽��
		CHATGROUP o2n;//Ⱥ��
		CHATUPDATAOL upol;//��������
		CHATUPDATUSERINFO upui;//�����û�
		CHATMSGOFFLINE mol;//������Ϣ
		CHATMSGRECORD mrc;//�����¼
		GROUPMEMBER gml;//Ⱥ��Ա��Ϣ
		CHATFILE file;//���ļ�
		CHATVIDEO video;//��Ƶ����
	}m_content;
}CHATSEND;

//�׽���
class CClientSocket
{
public:
	CClientSocket();
	~CClientSocket();

	//��ʼ���ͻ���;
	bool ConnectServer(char *szIP, WORD port);
	char * Recv();
	bool Send(CHATPURPOSE purpose, char* bufSend, DWORD dwLen);
	bool Close();
	bool StarServer(WORD port);
private:

	//���͸�����Ϣ
	void SendForRegister(char * bufSend, DWORD dwLen);			//ע��
	void SendForLogin(char * bufSend, DWORD dwLen);				//��¼
	void SendForSearchUser(char * bufSend, DWORD dwLen);		//�����û�
	void SendForSearchGroup(char * bufSend, DWORD dwLen);		//����Ⱥ
	void SendForOne2One(char * bufSend, DWORD dwLen);			//1v1
	void SendForAddFriend(char * bufSend, DWORD dwLen);			//��Ӻ���
	void SendForGroup(char * bufSend, DWORD dwLen);				//1vN
	void SendUpdataGroup(char * bufSend, DWORD dwLen);			//��ȡȺ�б�
	void SendUpdataGroupMember(char * bufSend, DWORD dwLen);	//��ȡȺ��Ա�б�
	void SendUpdataFriend(char * bufSend, DWORD dwLen);			//��ȡ�����б�
	void SendGetOfflinemessage(char * bufSend, DWORD dwLen);	//��ȡ������Ϣ
	void SendGetMessageRecord(char * bufSend, DWORD dwLen);     //��ȡ������Ϣ
	void SendFileTrans(char * bufSend, DWORD dwLen);			//�����ļ�
	void SendChatVideo(char * bufSend, DWORD dwLen);			//������Ƶ����

public:

	HANDLE m_hThread = 0;					//�߳̾��
	SOCKET m_sClient = NULL;				//�׽���
	SOCKET m_sServer = NULL;				//��Ϊ�����ʱ�׽���
	map<CString,CDialogEx*> m_map;			//�����ֵ�

	//��ǰ�û���/ID/����;
	char m_szName[BUFNAME] = {};
	char m_szUID[BUFNAME] = {};
	char m_szPWD[BUFNAME] = {};
	//������Ϣ;
	CHATSEND *m_pObjChatRecv;
private:
	char m_bufRecv[BUFMSG] = {};
};