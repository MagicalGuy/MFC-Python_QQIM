// CDlgSendFile.cpp : 实现文件
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "CDlgSendFile.h"
#include "afxdialogex.h"


// CDlgSendFile 对话框

IMPLEMENT_DYNAMIC(CDlgSendFile, CDialogEx)


CDlgSendFile::CDlgSendFile(CClientSocket * client, CString fid, bool server, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SENDFILE, pParent)
	, m_strFilePath(_T(""))
{
	m_client = client;
	m_fid = fid;
	IsServer = server;
}



CDlgSendFile::~CDlgSendFile()
{
}

void CDlgSendFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILEPATH, m_strFilePath);
}


BOOL CDlgSendFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	if (IsServer)
	{
		//创建接收线程;
		m_hFile = AfxBeginThread(ThreadFile, this);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDlgSendFile::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CString str = "FileTrans";
	if (m_client->m_map.find(str) == m_client->m_map.end())
	{


	}
	else
	{
		m_client->m_map.erase(str);
	}
	CDialogEx::OnCancel();
}

void CDlgSendFile::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	TCHAR szPatch[MAX_PATH] = { 0 };

	UINT nCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (UINT idx = 0; idx < nCount; idx++)
	{
		DragQueryFile(hDropInfo, idx, szPatch, MAX_PATH);
		SetDlgItemText(IDC_FILEPATH, szPatch);
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

void CDlgSendFile::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//发送请求
	//获取文件名
	//CStringA filename = CW2A(m_strFilePath.GetBuffer(), CP_THREAD_ACP);
	CStringA filename = CT2A(m_strFilePath.GetBuffer(), CP_THREAD_ACP);
	int i = 0;
	while (i != -1)
	{
		i = filename.Find("\\");
		filename.Delete(0, i + 1);
	}

	m_strFileName = filename;

	filename += ":" + m_fid;

	m_client->Send(FILETRANS, filename.GetBuffer(), 1);
}

LRESULT CDlgSendFile::OnMyMessage2(WPARAM wParam, LPARAM lParam)
{
	CHATSEND* ct = (CHATSEND*)wParam;

	//接收文件
	if (lParam == 2)
	{
		m_strFileName = ct->m_content.file.szFileName;

		//创建接收线程;
		m_hFile = AfxBeginThread(ThreadFile, this);
		return 0;
	}
	//发送文件
	else if (lParam == 1)
	{
		m_hFileSend = AfxBeginThread(ThreadFileSend, this);
		return 0;
	}
	//刷新进度条
	else
	{
		UpdateData(FALSE);
	}

	return -1;
}



UINT CDlgSendFile::ThreadFile(LPVOID lpParam)
{

	CDlgSendFile* FileTrans = (CDlgSendFile*)lpParam;

	FileTrans->m_fileserver.StarServer(1235);

	//启动服务端
	sockaddr_in addClient = {};
	int nSize = sizeof(sockaddr_in);
	SOCKET sClient = accept(FileTrans->m_fileserver.m_sServer, (sockaddr*)&addClient, &nSize);
	if (sClient == INVALID_SOCKET)
	{

		FileTrans->MessageBox("接收文件失败");
		return -1;
	}

	//接收文件信息
	CHATSEND ct = {};
	if (SOCKET_ERROR == recv(sClient, (char*)&ct, sizeof(CHATSEND), NULL))
	{
		FileTrans->MessageBox("文件客户端连接断开");
		::SendMessage(FileTrans->m_hWnd, WM_CLOSE, NULL, NULL);
		return -1;
	}

	int nLength = ct.m_content.file.dwlength;

	//写入文件
	CString sFilename = FileTrans->m_strFileName;
	CFile file;

	if (file.Open(sFilename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		int total = 0;
		int nRet = 0;

		char * filebuf = new char[nLength];
		//循环接收
		while (total < nLength)
		{

			char* buf = new char[1024 * 100];
			nRet = recv(sClient, buf, 1024 * 100, NULL);

			if (SOCKET_ERROR == nRet)
			{
				FileTrans->MessageBox("接收文件出错");
				::SendMessage(FileTrans->m_hWnd, WM_CLOSE, NULL, NULL);
				return -1;
			}

			memcpy_s(filebuf + total, nRet, buf, nRet);

			total += nRet;

			delete[] buf;
			buf = nullptr;

			//刷新进度条
			float progress = total;
			progress /= nLength;
			progress *= 100;
//			FileTrans->m_scFileProgress.SetPos(progress);
			//::SendMessage(FileTrans->m_hWnd, WM_USER + 101, NULL, progress);

			//接收完成
			if (total >= nLength)
			{
				file.Write(filebuf, nLength);
//				FileTrans->m_scFileProgress.SetPos(100);

				closesocket(FileTrans->m_fileserver.m_sServer);
				WSACleanup();
				FileTrans->MessageBox("接收文件成功");
				break;
			}
		}

		file.Close();
	}

	return 0;
}




UINT CDlgSendFile::ThreadFileSend(LPVOID lpParam)
{
	CDlgSendFile* FileTrans = (CDlgSendFile*)lpParam;
	//CStringA addr = CW2A(FileTrans->m_frindIP.GetBuffer(), CP_THREAD_ACP);
	CStringA addr = CT2A(FileTrans->m_frindIP.GetBuffer(), CP_THREAD_ACP);
	//连接服务端
	if (!FileTrans->m_fileclient.ConnectServer(addr.GetBuffer(), 1235))
	{
		FileTrans->MessageBox("连接文件服务端失败！", "Error", MB_OK | MB_ICONWARNING);
		return -1;
	}


	CFile file;

	if (file.Open(FileTrans->m_strFilePath, CFile::modeRead | CFile::typeBinary))
	{
		//处理文件
		CHATSEND ct = { FILETRANS };
		ct.m_content.file.dwlength = file.GetLength();
		int nLength = file.GetLength();
		if (file.GetLength() > 1024 * 1024 * 10)
		{
			FileTrans->MessageBox("文件过大！");
			return -1;
		}

		//发送文件信息
		int nRet = 0;
		nRet = send(FileTrans->m_fileclient.m_sClient, (char*)&ct, sizeof(CHATSEND), NULL);

		if (SOCKET_ERROR == nRet)
		{
			FileTrans->MessageBox("文件传输网络错误");
			::SendMessage(FileTrans->m_hWnd, WM_CLOSE, NULL, NULL);
			return -1;
		}

		Sleep(10);
		file.Seek(0, CFile::begin);
		int total = 0;
		int num = 0;


		//循环发送
		while (total < nLength)
		{

			char* buf = new char[1024 * 100];
			num = file.Read(buf, 1024 * 100);
			if (num == 0)
			{
				break;
			}
			nRet = send(FileTrans->m_fileclient.m_sClient, buf, num, NULL);

			if (SOCKET_ERROR == nRet)
			{
				FileTrans->MessageBox("文件传输网络错误");
				::SendMessage(FileTrans->m_hWnd, WM_CLOSE, NULL, NULL);
				return -1;
			}

			total += nRet;
			delete[] buf;
			buf = nullptr;

			//刷新进度条
			float progress = total;
			progress /= nLength;
			progress *= 100;
//			FileTrans->m_scFileProgress.SetPos(progress);

			//::SendMessage(FileTrans->m_hWnd, WM_USER + 101, NULL, progress);

			Sleep(100);

		}

//		FileTrans->m_scFileProgress.SetPos(100);

		file.Close();

		FileTrans->MessageBox("文件发送成功");
		FileTrans->m_fileclient.Close();
		return 0;
	}
	else
	{
		FileTrans->MessageBox("读取文件错误");
		return -1;
	}
	return 0;
}





BEGIN_MESSAGE_MAP(CDlgSendFile, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgSendFile::OnBnClickedButton1)
	ON_MESSAGE(WM_MY_MESSAGE2, OnMyMessage2)
END_MESSAGE_MAP()


// CDlgSendFile 消息处理程序
