#pragma once
#include "CMyList.h"
#include "CClientSocket.h"

// CDlgSearch 对话框

class CDlgSearch : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSearch)

public:
	CDlgSearch(CClientSocket * client, bool isuid, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSearch();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEARCH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnCancel();				//屏蔽ESC
	DECLARE_MESSAGE_MAP()
public:
	CString m_strid;
	CMyList m_ListSearch;

	CClientSocket * m_client;
	bool m_bIsUid;						  //是否搜索用户ID

	afx_msg void OnBnClickedOk();			//点击确定
	afx_msg void OnNMRClickSearchlist(NMHDR *pNMHDR, LRESULT *pResult);//右键菜单
	afx_msg void On32771_AddFriend();		//添加好友
	afx_msg void On32772_ShowUserInfo();	//查看资料

};
