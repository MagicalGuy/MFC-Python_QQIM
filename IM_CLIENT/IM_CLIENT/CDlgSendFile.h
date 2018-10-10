#pragma once
#include "CClientSocket.h"

#define WM_MY_MESSAGE2 WM_USER+101
// CDlgSendFile 对话框

class CDlgSendFile : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSendFile)

public:
	//CDlgSendFile(CWnd* pParent = NULL);   // 标准构造函数

	CDlgSendFile(CClientSocket * client, CString fid, bool server, CWnd* pParent = NULL);


	virtual ~CDlgSendFile();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENDFILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	CString m_fid;							   //好友ID
	CString m_frindIP;						   //好友IP
	CString m_strFilePath;



	CString m_strFileName;					   //文件名
	//CProgressCtrl m_scFileProgress;			   //进度条

	DWORD m_dwFileSize;						   //文件尺寸

	bool IsServer;							   //是否作为服务端接收
	CClientSocket * m_client;				   //主进程soket
	CClientSocket m_fileclient;				   //发送客户端
	CClientSocket m_fileserver;				   //接收服务端


	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT OnMyMessage2(WPARAM wParam, LPARAM lParam);	//接收自定义消息创建聊天窗口


	HANDLE m_hFile = 0;							//线程句柄
	static UINT  ThreadFile(LPVOID lpParam);	//接收文件;


	HANDLE m_hFileSend = 0;							//线程句柄
	static UINT  ThreadFileSend(LPVOID lpParam);	//发送文件;

};
