#pragma once
#include "CMyList.h"
#include "CClientSocket.h"

// CDlgGroup 对话框

class CDlgGroup : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGroup)

public:
	//CDlgGroup(CWnd* pParent = NULL);   // 标准构造函数
	CDlgGroup(CClientSocket * client, CWnd* pParent = NULL);
	virtual ~CDlgGroup();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_GROUP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CMyList m_GList;

	CClientSocket * m_client;
	void UpdataGroup();			//更新群列表

	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
