#pragma once
#include "CMyList.h"
#include "afxwin.h"
#include"CClientSocket.h"


// CDlgGroupChat 对话框

class CDlgGroupChat : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGroupChat)

public:
	CDlgGroupChat(CClientSocket * client, CString &gid, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgGroupChat();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GROUPCHAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	CMyList m_gChatList;//群成员列表


	CEdit m_ceRecv;
	CString m_strGRecv;
	CString m_strGSend;


	CString m_gid;				//群ID
	CClientSocket * m_client;

	void UpdataGroupMember();			//更新群成员
	void RecvMemberList(char * uid);	//接收群成员

	afx_msg void OnBnClickedGsend();	//发送
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);	//右键菜单
	afx_msg void On32771_AddFrined();	//添加好友
	afx_msg void On32772_ShowInfo();	//查看资料

	virtual void OnOK();
};
