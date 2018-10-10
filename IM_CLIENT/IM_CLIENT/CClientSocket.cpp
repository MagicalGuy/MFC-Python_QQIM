#include "stdafx.h"
#include "CClientSocket.h"
#include "IM_MAIN.h"
#include "xxtea.h"
//Sock不进行重复检查；放到最上边;
#include <WinSock2.h>
#include <windows.h>
#include <WS2tcpip.h>
//载入套接字库;
#pragma comment(lib,"ws2_32.lib")


CClientSocket::CClientSocket(){}


CClientSocket::~CClientSocket(){}

//****************************************************************
// Brief 	: 初始化套接字
// Method	: ConnectServer
// FullName	: CClientSocket::ConnectServer
// Access	: public 
// Returns	: bool
// Parameter: char * szIP
// Parameter: WORD port
//****************************************************************

bool CClientSocket::ConnectServer(char *szIP, WORD port)
{
	//初始化套接字;
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return false;
	}

	//检查库版本
	if (LOBYTE(wsd.wVersion) != 2 || HIBYTE(wsd.wHighVersion) != 2)
	{
		WSACleanup();
		return false;
	}

	//创建套接字;
	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == INVALID_SOCKET)
	{
		WSACleanup();
		return false;
	}

	//设置服务器地址;
	SOCKADDR_IN serverAddr = {};
	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.s_addr = inet_addr(szIP);
	inet_pton(AF_INET,szIP,&(serverAddr.sin_addr.S_un.S_addr));
	serverAddr.sin_port = htons((short)port);

	//连接服务端
	int nRetValue = connect(m_sClient, (sockaddr*)&serverAddr, sizeof(SOCKADDR_IN));
	if (nRetValue == SOCKET_ERROR)
	{
		closesocket(m_sClient);
		m_sClient = NULL;
		WSACleanup();
		return false;
	}

	return true;
}

//****************************************************************
// Brief 	: 消息接收
// Method	: Recv
// FullName	: CClientSocket::Recv
// Access	: public 
// Returns	: char *
//****************************************************************

char * CClientSocket::Recv()
{
	//接收消息;
	CHATSEND ct = {};
	if (SOCKET_ERROR == recv(m_sClient, (char*)&ct, sizeof(CHATSEND), NULL))
	{
		printf("recv error!\n");
		return nullptr;
	}
	//转存数据
	m_pObjChatRecv = &ct;

	//分发消息
	switch (ct.m_type)
	{
	case REGISTER:
		return m_pObjChatRecv->m_content.reg.szName;
		break;
	case LOGIN:
		return m_pObjChatRecv->m_content.log.szName;
		break;
	case SEARCHUSER:
		break;
	case ADDFRIEND:
		break;
	case ONE2ONE:
		break;
	case ONE2GROUP:
		break;
	case MSGTEXT:
		break;
	case MSGOFFLINE:
		break;
	case MSGRECORD:
		break;
	case MSGIMAGE:
		break;
	case MSGVOICE:
		break;
	case MSGVIDEO:
		break;
	case FILETRANS:
		break;
	case UPDATEOL:
		break;
	case UPDATEFRIEND:
		return m_pObjChatRecv->m_content.buf;
		break;
	case UPDATEGROUP:
		return m_pObjChatRecv->m_content.buf;
		break;
	case UPDATEGROUPMEMBER:
		return m_pObjChatRecv->m_content.buf;
		break;
	default:
		break;
	}

	return nullptr;
}

//****************************************************************
// Brief 	: 发送消息
// Method	: Send
// FullName	: CClientSocket::Send
// Access	: public 
// Returns	: bool
// Parameter: CHATPURPOSE purpose
// Parameter: char * bufSend
// Parameter: DWORD dwLen
//****************************************************************

bool CClientSocket::Send(CHATPURPOSE purpose, char* bufSend, DWORD dwLen)
{
	//分发消息
	switch (purpose)
	{
	case REGISTER:
		SendForRegister(bufSend, dwLen);
		break;
	case LOGIN:
		SendForLogin(bufSend, dwLen);
		break;
	case SEARCHUSER:
		SendForSearchUser(bufSend, dwLen);
		break;
	case  SEARCHGROUP:
		SendForSearchGroup(bufSend, dwLen);
		break;
	case ADDFRIEND:
		SendForAddFriend(bufSend, dwLen);
		break;
	case ONE2ONE:
		SendForOne2One(bufSend, dwLen);
		break;
	case ONE2GROUP:
		SendForGroup(bufSend, dwLen);
		break;
	case MSGTEXT:
		break;
	case MSGOFFLINE:
		SendGetOfflinemessage(bufSend, dwLen);
		break;
	case MSGRECORD:
		SendGetMessageRecord(bufSend, dwLen);
		break;
	case MSGIMAGE:
		break;
	case MSGVOICE:
		break;
	case MSGVIDEO:
		SendChatVideo(bufSend, dwLen);
		break;
	case FILETRANS:
		SendFileTrans(bufSend, dwLen);
		break;
	case UPDATEOL:
		break;
	case UPDATEFRIEND:
		SendUpdataFriend(bufSend, dwLen);
		break;
	case UPDATEGROUP:
		SendUpdataGroup(bufSend, dwLen);
		break;
	case UPDATEGROUPMEMBER:
		SendUpdataGroupMember(bufSend, dwLen);
		break;
	default:
		break;
	}

	return true;
}

//关闭套接字
bool CClientSocket::Close()
{
	closesocket(m_sClient);
	WSACleanup();
	return true;
}


//****************************************************************
// Brief 	: 启动服务端
// Method	: StarServer
// FullName	: CClientSocket::StarServer
// Access	: public 
// Returns	: bool
//****************************************************************

bool CClientSocket::StarServer(WORD port)
{
	//初始化套接字;
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return false;
	}

	//检查库版本
	if (LOBYTE(wsd.wVersion) != 2 || HIBYTE(wsd.wHighVersion) != 2)
	{
		WSACleanup();
		return false;
	}

	//创建套接字;
	m_sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sServer == INVALID_SOCKET)
	{
		WSACleanup();
		return false;
	}

	//设置服务器地址;
	SOCKADDR_IN serverAddr = {};
	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//inet_pton(AF_INET,szIP,&(serverAddr.sin_addr.S_un.S_addr));
	inet_pton(AF_INET, "192.168.3.184", &(serverAddr.sin_addr.S_un.S_addr));
	serverAddr.sin_port = htons((short)port);

	//绑定
	int nRet = bind(m_sServer, (sockaddr*)&serverAddr, sizeof(sockaddr_in));
	if (nRet == SOCKET_ERROR)
	{
		closesocket(m_sServer);
		m_sServer = NULL;
		WSACleanup();
		return false;
	}

	//监听消息
	int nRet2 = listen(m_sServer, SOMAXCONN);

	if (nRet == SOCKET_ERROR)
	{
		closesocket(m_sServer);
		m_sServer = NULL;
		WSACleanup();
		return false;
	}

	return true;

}

//注册
void CClientSocket::SendForRegister(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { REGISTER };
	char* pwd = nullptr;
	//分割数据
	strtok_s(bufSend, ":", &pwd);

	memcpy_s(ct.m_content.reg.szName, pwd - bufSend, bufSend, pwd - bufSend);
	memcpy_s(ct.m_content.reg.szPwd, strlen(pwd), pwd, strlen(pwd));
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

//登录
void CClientSocket::SendForLogin(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { LOGIN };
	char * pwd = nullptr;
	//分割数据
	strtok_s(bufSend, ":", &pwd);
	memcpy_s(ct.m_content.log.szName, pwd - bufSend, bufSend, pwd - bufSend);
	memcpy_s(ct.m_content.log.szPwd, strlen(pwd), pwd, strlen(pwd));
	send(m_sClient, (char *)&ct, sizeof(ct), NULL);
}

//搜索用户
void CClientSocket::SendForSearchUser(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { SEARCHUSER };
	memcpy_s(ct.m_content.su.szUID, strlen(bufSend), bufSend, strlen(bufSend));
	send(m_sClient, (char *)&ct, sizeof(ct), NULL);
}

//搜索群
void CClientSocket::SendForSearchGroup(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { SEARCHGROUP };
	memcpy_s(ct.m_content.su.szUID, strlen(bufSend), bufSend, strlen(bufSend));
	send(m_sClient, (char *)&ct, sizeof(ct), NULL);
}

//1v1
void CClientSocket::SendForOne2One(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { ONE2ONE };
	char fid[BUFNAME] = {};
	_itoa_s(dwLen, fid, 20, 10);
	memcpy_s(ct.m_content.o2o.szUid, strlen(m_szUID), m_szUID, strlen(m_szUID));
	memcpy_s(ct.m_content.o2o.szFid, strlen(fid), fid, strlen(fid));
	memcpy_s(ct.m_content.o2o.szUname, strlen(m_szName), m_szName, strlen(m_szName));

	//加密聊天信息
	string Content = bufSend;
	string key = m_szUID;
	string cipherText = xxtea_encrypt(Content, key);
	memcpy_s(ct.m_content.o2o.szContent, cipherText.length(), cipherText.c_str(), cipherText.length());
	send(m_sClient, (char *)&ct, sizeof(ct), NULL);
}

//添加好友
void CClientSocket::SendForAddFriend(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { ADDFRIEND };
	ct.m_content.adf.dwAdd = dwLen;
	memcpy_s(ct.m_content.adf.szName, strlen(m_szUID), m_szUID, strlen(m_szUID));
	memcpy_s(ct.m_content.adf.szFrd, strlen(bufSend), bufSend, strlen(bufSend));
	send(m_sClient, (char *)&ct, sizeof(ct), NULL);
}

//1vN
void CClientSocket::SendForGroup(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { ONE2GROUP };
	char gid[BUFNAME] = {};
	_itoa_s(dwLen, gid, 20, 10);
	memcpy_s(ct.m_content.o2n.szGID, strlen(gid), gid, strlen(gid));
	memcpy_s(ct.m_content.o2n.szName, strlen(m_szName), m_szName, strlen(m_szName));
	memcpy_s(ct.m_content.o2n.szContent, strlen(bufSend), bufSend, strlen(bufSend));
	send(m_sClient, (char *)&ct, sizeof(ct), NULL);
}

//请求群列表
void CClientSocket::SendUpdataGroup(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { UPDATEGROUP };
	if (bufSend != NULL) {
		memcpy_s(ct.m_content.buf, strlen(bufSend), bufSend, strlen(bufSend));
	}
	send(m_sClient, (char *)&ct, sizeof(ct), NULL);
}

//请求群成员列表
void CClientSocket::SendUpdataGroupMember(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { UPDATEGROUPMEMBER };
	if(bufSend!=NULL){
	memcpy_s(ct.m_content.buf, strlen(bufSend), bufSend, strlen(bufSend));
	}
	send(m_sClient, (char *)&ct, sizeof(ct), NULL);
}

//请求好友列表
void CClientSocket::SendUpdataFriend(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { UPDATEFRIEND };
	if (bufSend != NULL) {
		memcpy_s(ct.m_content.buf, strlen(bufSend), bufSend, strlen(bufSend));
	}
	send(m_sClient, (char *)&ct, sizeof(ct), NULL);
}

//请求离线消息
void CClientSocket::SendGetOfflinemessage(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { MSGOFFLINE };
	memcpy_s(ct.m_content.buf, strlen(bufSend), bufSend, strlen(bufSend));
	send(m_sClient, (char *)&ct, sizeof(ct), NULL);
}

//请求聊天记录
void CClientSocket::SendGetMessageRecord(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { MSGRECORD };
	//memcpy_s(ct.m_content.buf, strlen(bufSend), bufSend, strlen(bufSend));
	send(m_sClient, (char *)&ct, sizeof(ct), NULL);
}

//文件传输
void CClientSocket::SendFileTrans(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { FILETRANS };
	ct.m_content.file.dwType = dwLen;
	if (dwLen == 1)
	{
		char* fid = nullptr;
		//分割数据
		strtok_s(bufSend, ":", &fid);

		memcpy_s(ct.m_content.file.szUid, strlen(m_szUID), m_szUID, strlen(m_szUID));
		memcpy_s(ct.m_content.file.szFileName, strlen(bufSend), bufSend, strlen(bufSend));
		memcpy_s(ct.m_content.file.szFid, strlen(fid), fid, strlen(fid));

		send(m_sClient, (char *)&ct, sizeof(ct), NULL);
	}
	else if (dwLen == 2 || dwLen == 3)
	{
		memcpy_s(ct.m_content.file.szUid, strlen(m_szUID), m_szUID, strlen(m_szUID));
		memcpy_s(ct.m_content.file.szFid, strlen(bufSend), bufSend, strlen(bufSend));
		send(m_sClient, (char *)&ct, sizeof(ct), NULL);
	}

}

void CClientSocket::SendChatVideo(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { MSGVIDEO };
	ct.m_content.video.dwType = dwLen;
	memcpy_s(ct.m_content.video.szUid, strlen(m_szUID), m_szUID, strlen(m_szUID));
	memcpy_s(ct.m_content.video.szFid, strlen(bufSend), bufSend, strlen(bufSend));
	send(m_sClient, (char *)&ct, sizeof(ct), NULL);
}

