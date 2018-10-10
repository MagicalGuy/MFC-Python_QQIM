#pragma once
#include "CClientSocket.h"
#include "CMyList.h"

// CDlgFriend 对话框

class CDlgFriend : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFriend)

public:
	CDlgFriend(CClientSocket * client, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFriend();


	CClientSocket * m_client;
	afx_msg void OnNMDblclkListFriend(NMHDR *pNMHDR, LRESULT *pResult);
	void  UpdataFriend();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_FRIEND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CMyList m_FList;

	afx_msg void OnNMRClickFresh(NMHDR *pNMHDR, LRESULT *pResult);//右键菜单
	afx_msg void On32775_FreshInfo();		//刷新

};
