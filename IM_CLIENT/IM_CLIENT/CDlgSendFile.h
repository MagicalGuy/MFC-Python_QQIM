#pragma once
#include "CClientSocket.h"

#define WM_MY_MESSAGE2 WM_USER+101
// CDlgSendFile �Ի���

class CDlgSendFile : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSendFile)

public:
	//CDlgSendFile(CWnd* pParent = NULL);   // ��׼���캯��

	CDlgSendFile(CClientSocket * client, CString fid, bool server, CWnd* pParent = NULL);


	virtual ~CDlgSendFile();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENDFILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	CString m_fid;							   //����ID
	CString m_frindIP;						   //����IP
	CString m_strFilePath;



	CString m_strFileName;					   //�ļ���
	//CProgressCtrl m_scFileProgress;			   //������

	DWORD m_dwFileSize;						   //�ļ��ߴ�

	bool IsServer;							   //�Ƿ���Ϊ����˽���
	CClientSocket * m_client;				   //������soket
	CClientSocket m_fileclient;				   //���Ϳͻ���
	CClientSocket m_fileserver;				   //���շ����


	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT OnMyMessage2(WPARAM wParam, LPARAM lParam);	//�����Զ�����Ϣ�������촰��


	HANDLE m_hFile = 0;							//�߳̾��
	static UINT  ThreadFile(LPVOID lpParam);	//�����ļ�;


	HANDLE m_hFileSend = 0;							//�߳̾��
	static UINT  ThreadFileSend(LPVOID lpParam);	//�����ļ�;

};
