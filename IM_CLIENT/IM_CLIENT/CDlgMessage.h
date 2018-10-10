#pragma once
#include "CMyList.h"
#include "CClientSocket.h"
#include <locale>
#include <vector>
using std::vector;


// CDlgMessage 对话框

class CDlgMessage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMessage)

public:
	CDlgMessage(CClientSocket * client, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMessage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MSG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CMyList m_LastChatList;

	CClientSocket * m_client;

	vector<CString> m_vecChatList;		//聊天列表VEC

	void UpDataChatList(CString fid);	//更新聊天列表

	void SaveChatList();				//保存列表
	void ReadChatList();				//读取列表

	afx_msg void OnNMDblclkLastchat(NMHDR *pNMHDR, LRESULT *pResult);
};
