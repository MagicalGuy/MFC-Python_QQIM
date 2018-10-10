#pragma once
#include "afxwin.h"


// CDlgChatRecord 对话框

class CDlgChatRecord : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgChatRecord)

public:
	//CDlgChatRecord(CWnd* pParent = NULL);   // 标准构造函数
	CDlgChatRecord(CString fid, CWnd* pParent = NULL);
	virtual ~CDlgChatRecord();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATRECORD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ceRecord;//聊天记录控件
	CString m_strRecordText; //绑定变量
	CString m_fid;				//好友ID
};
