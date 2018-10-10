// CDlgSendFile.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "CDlgSendFile.h"
#include "afxdialogex.h"


// CDlgSendFile �Ի���

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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	if (IsServer)
	{
		//���������߳�;
		m_hFile = AfxBeginThread(ThreadFile, this);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgSendFile::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	//��������
	//��ȡ�ļ���
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

	//�����ļ�
	if (lParam == 2)
	{
		m_strFileName = ct->m_content.file.szFileName;

		//���������߳�;
		m_hFile = AfxBeginThread(ThreadFile, this);
		return 0;
	}
	//�����ļ�
	else if (lParam == 1)
	{
		m_hFileSend = AfxBeginThread(ThreadFileSend, this);
		return 0;
	}
	//ˢ�½�����
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

	//���������
	sockaddr_in addClient = {};
	int nSize = sizeof(sockaddr_in);
	SOCKET sClient = accept(FileTrans->m_fileserver.m_sServer, (sockaddr*)&addClient, &nSize);
	if (sClient == INVALID_SOCKET)
	{

		FileTrans->MessageBox("�����ļ�ʧ��");
		return -1;
	}

	//�����ļ���Ϣ
	CHATSEND ct = {};
	if (SOCKET_ERROR == recv(sClient, (char*)&ct, sizeof(CHATSEND), NULL))
	{
		FileTrans->MessageBox("�ļ��ͻ������ӶϿ�");
		::SendMessage(FileTrans->m_hWnd, WM_CLOSE, NULL, NULL);
		return -1;
	}

	int nLength = ct.m_content.file.dwlength;

	//д���ļ�
	CString sFilename = FileTrans->m_strFileName;
	CFile file;

	if (file.Open(sFilename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		int total = 0;
		int nRet = 0;

		char * filebuf = new char[nLength];
		//ѭ������
		while (total < nLength)
		{

			char* buf = new char[1024 * 100];
			nRet = recv(sClient, buf, 1024 * 100, NULL);

			if (SOCKET_ERROR == nRet)
			{
				FileTrans->MessageBox("�����ļ�����");
				::SendMessage(FileTrans->m_hWnd, WM_CLOSE, NULL, NULL);
				return -1;
			}

			memcpy_s(filebuf + total, nRet, buf, nRet);

			total += nRet;

			delete[] buf;
			buf = nullptr;

			//ˢ�½�����
			float progress = total;
			progress /= nLength;
			progress *= 100;
//			FileTrans->m_scFileProgress.SetPos(progress);
			//::SendMessage(FileTrans->m_hWnd, WM_USER + 101, NULL, progress);

			//�������
			if (total >= nLength)
			{
				file.Write(filebuf, nLength);
//				FileTrans->m_scFileProgress.SetPos(100);

				closesocket(FileTrans->m_fileserver.m_sServer);
				WSACleanup();
				FileTrans->MessageBox("�����ļ��ɹ�");
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
	//���ӷ����
	if (!FileTrans->m_fileclient.ConnectServer(addr.GetBuffer(), 1235))
	{
		FileTrans->MessageBox("�����ļ������ʧ�ܣ�", "Error", MB_OK | MB_ICONWARNING);
		return -1;
	}


	CFile file;

	if (file.Open(FileTrans->m_strFilePath, CFile::modeRead | CFile::typeBinary))
	{
		//�����ļ�
		CHATSEND ct = { FILETRANS };
		ct.m_content.file.dwlength = file.GetLength();
		int nLength = file.GetLength();
		if (file.GetLength() > 1024 * 1024 * 10)
		{
			FileTrans->MessageBox("�ļ�����");
			return -1;
		}

		//�����ļ���Ϣ
		int nRet = 0;
		nRet = send(FileTrans->m_fileclient.m_sClient, (char*)&ct, sizeof(CHATSEND), NULL);

		if (SOCKET_ERROR == nRet)
		{
			FileTrans->MessageBox("�ļ������������");
			::SendMessage(FileTrans->m_hWnd, WM_CLOSE, NULL, NULL);
			return -1;
		}

		Sleep(10);
		file.Seek(0, CFile::begin);
		int total = 0;
		int num = 0;


		//ѭ������
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
				FileTrans->MessageBox("�ļ������������");
				::SendMessage(FileTrans->m_hWnd, WM_CLOSE, NULL, NULL);
				return -1;
			}

			total += nRet;
			delete[] buf;
			buf = nullptr;

			//ˢ�½�����
			float progress = total;
			progress /= nLength;
			progress *= 100;
//			FileTrans->m_scFileProgress.SetPos(progress);

			//::SendMessage(FileTrans->m_hWnd, WM_USER + 101, NULL, progress);

			Sleep(100);

		}

//		FileTrans->m_scFileProgress.SetPos(100);

		file.Close();

		FileTrans->MessageBox("�ļ����ͳɹ�");
		FileTrans->m_fileclient.Close();
		return 0;
	}
	else
	{
		FileTrans->MessageBox("��ȡ�ļ�����");
		return -1;
	}
	return 0;
}





BEGIN_MESSAGE_MAP(CDlgSendFile, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgSendFile::OnBnClickedButton1)
	ON_MESSAGE(WM_MY_MESSAGE2, OnMyMessage2)
END_MESSAGE_MAP()


// CDlgSendFile ��Ϣ�������
