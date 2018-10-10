#pragma once
#include "CClientSocket.h"
#include "afxwin.h"

// CDlgOne2One 对话框

class CDlgOne2One : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgOne2One)

public:
	CDlgOne2One(CClientSocket * client, CString &fid, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgOne2One();

	CClientSocket * m_client;


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_1TO1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnCancel();							//屏蔽ESC键
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

public:
	CString m_fid;
	CEdit m_ceFRecv;
	CString m_strFSend;
	CString m_strFRecv;

	afx_msg void OnBnClickedFsend();		//发送消息
	afx_msg void OnBnClickedChatrecord();	//查看本地聊天记录
	afx_msg void OnBnClickedSendfile();		//发送文件
	afx_msg void OnBnClickedVideo();		//视频聊天
	virtual void OnOK();
	afx_msg void OnBnClickedChatrecord1();//查看云记录

};
