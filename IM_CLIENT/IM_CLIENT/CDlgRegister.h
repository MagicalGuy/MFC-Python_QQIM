#pragma once
#include "CClientSocket.h"

// CDlgRegister 对话框

class CDlgRegister : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRegister)

public:
	CDlgRegister(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRegister();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_REG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CClientSocket m_client;
	CString m_strUsername;
	CString m_strPWD1;
	CString m_strPWD2;
	afx_msg void OnBnClickedButton1();
};
