#pragma once
#include "CClientSocket.h"

// CDlgUserInfo 对话框

class CDlgUserInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUserInfo)

public:
	CDlgUserInfo(CClientSocket * client, CString uid, CString uname, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgUserInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USERINFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();		//重写OK
	DECLARE_MESSAGE_MAP()
public:
	CString m_strUID;
	CString m_strUName;

	CClientSocket * m_client;
};
