#pragma once
#include "stdafx.h"
#include <windows.h>
#include <map>

using namespace std;

#define BUFMSG 2048
#define BUFNAME  50


//头部;数据目的;定义必须和服务端一致;
enum CHATPURPOSE {
	REGISTER = 1,				//   注册
	LOGIN = 2,					//   登录
	SEARCHUSER = 3,				//   搜索
	ADDFRIEND = 4,				//	 添加好友
	ONE2ONE = 5,				//   1v1
	ONE2GROUP = 6,				//   1vN	
	MSGTEXT = 7,				//   文字消息;
	MSGOFFLINE = 8,				//   离线消息
	MSGIMAGE = 9,				//	 图片
	MSGVOICE = 10,				//	 语音
	MSGVIDEO = 11,				//   视频
	FILETRANS = 12,				//   文件传输
	UPDATEOL = 13,				//   更新在线状态
	UPDATEFRIEND = 14,			//   更新好友列表;
	UPDATEGROUP = 15,			//   更新群列表;
	UPDATEGROUPMEMBER = 16,		//   更新群列表;
	SEARCHGROUP = 17,			//   搜索群
	REPLOGIN = 18,				//   重复登录
	MSGRECORD = 19              //聊天记录
};


//聊天内容;

//注册;
typedef struct _CHATREGISTER
{
	CHAR szName[50];
	CHAR szPwd[50];
	//CHAR szSex[2];
}CHATREGISTER;

//登录
typedef struct _CHATLOGIN
{
	CHAR szName[50];
	CHAR szPwd[50];
}CHATLOGIN;

//搜索
typedef struct _CHATSEARCHUSER
{
	CHAR szUID[50];
	CHAR szName[50];
}CHATSEARCHUSER;


//添加好友
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

//更新在线信息
typedef struct _CHATUPDATAOL
{
	DWORD bOnline;
	DWORD dwLen;
	CHAR buf[50];
}CHATUPDATAOL;

//更新用户信息
typedef struct _CHATUPDATUSERINFO
{
	DWORD bOnline;
	DWORD dwLen;
	CHAR buf[50];
}CHATUPDATUSERINFO;

//获取群成员列表
typedef struct _ROUPMEMBER
{
	CHAR gid[50];
	CHAR mid[50];
}GROUPMEMBER;

//离线聊天记录
typedef struct _CHATMSGOFFLINE
{
	CHAR szFrom[50];
	CHAR szTo[50];
	CHAR szContent[1024];
}CHATMSGOFFLINE;


//好友云聊天记录
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


//语音消息

//视频消息
typedef struct _CHATVIDEO
{
	DWORD dwType;
	CHAR szUid[20];
	CHAR szContent[20];
	CHAR szFid[20];
}CHATVIDEO;

//文件传输
typedef struct _CHATFILE
{
	DWORD dwType;
	DWORD dwlength;
	CHAR szUid[50];
	CHAR szFid[50];
	CHAR szFileName[255];
	CHAR szFileContent[1024];
}CHATFILE;


//最终发送内容
typedef struct _CHATSEND
{
	CHATPURPOSE m_type;
	union {
		char buf[BUFMSG];
		CHATREGISTER reg;//注册
		CHATLOGIN log;//登录
		CHATSEARCHUSER su;//搜索用户
		CHATADDFRIEND adf;//添加好友
		CHATONE2ONE	o2o;//私聊
		CHATGROUP o2n;//群聊
		CHATUPDATAOL upol;//更新在线
		CHATUPDATUSERINFO upui;//更新用户
		CHATMSGOFFLINE mol;//离线消息
		CHATMSGRECORD mrc;//聊天记录
		GROUPMEMBER gml;//群成员信息
		CHATFILE file;//传文件
		CHATVIDEO video;//视频聊天
	}m_content;
}CHATSEND;

//套接字
class CClientSocket
{
public:
	CClientSocket();
	~CClientSocket();

	//初始化客户端;
	bool ConnectServer(char *szIP, WORD port);
	char * Recv();
	bool Send(CHATPURPOSE purpose, char* bufSend, DWORD dwLen);
	bool Close();
	bool StarServer(WORD port);
private:

	//发送各种消息
	void SendForRegister(char * bufSend, DWORD dwLen);			//注册
	void SendForLogin(char * bufSend, DWORD dwLen);				//登录
	void SendForSearchUser(char * bufSend, DWORD dwLen);		//搜索用户
	void SendForSearchGroup(char * bufSend, DWORD dwLen);		//搜索群
	void SendForOne2One(char * bufSend, DWORD dwLen);			//1v1
	void SendForAddFriend(char * bufSend, DWORD dwLen);			//添加好友
	void SendForGroup(char * bufSend, DWORD dwLen);				//1vN
	void SendUpdataGroup(char * bufSend, DWORD dwLen);			//获取群列表
	void SendUpdataGroupMember(char * bufSend, DWORD dwLen);	//获取群成员列表
	void SendUpdataFriend(char * bufSend, DWORD dwLen);			//获取好友列表
	void SendGetOfflinemessage(char * bufSend, DWORD dwLen);	//获取离线消息
	void SendGetMessageRecord(char * bufSend, DWORD dwLen);     //获取聊天信息
	void SendFileTrans(char * bufSend, DWORD dwLen);			//发送文件
	void SendChatVideo(char * bufSend, DWORD dwLen);			//发送视频聊天

public:

	HANDLE m_hThread = 0;					//线程句柄
	SOCKET m_sClient = NULL;				//套接字
	SOCKET m_sServer = NULL;				//作为服务端时套接字
	map<CString,CDialogEx*> m_map;			//窗口字典

	//当前用户名/ID/密码;
	char m_szName[BUFNAME] = {};
	char m_szUID[BUFNAME] = {};
	char m_szPWD[BUFNAME] = {};
	//接收消息;
	CHATSEND *m_pObjChatRecv;
private:
	char m_bufRecv[BUFMSG] = {};
};