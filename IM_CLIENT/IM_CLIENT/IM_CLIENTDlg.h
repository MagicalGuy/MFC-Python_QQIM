
// IM_CLIENTDlg.h : 头文件
//

#pragma once
#include ".\UI\Dialog\DlgBase.h"
#include "CClientSocket.h"
#include "CDlgRegister.h"

// CIM_CLIENTDlg 对话框
class CIM_CLIENTDlg : public CDlgBase{
// 构造
public:
	CIM_CLIENTDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_QQ_DIALOG };

	CClientSocket m_client;
	CDlgRegister m_cReg;		 //注册



	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

	virtual void InitUI(CRect rcClient);
	virtual void OnSize(CRect rcClient);

	afx_msg LRESULT OnMessageLoginList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageLoginMenu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageButtomAccount(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageButtomStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageCheckPassWord(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageCheckAutoLogin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageEditAccount(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageButtonLogin(WPARAM wParam, LPARAM lParam);//点击登录按钮
	afx_msg LRESULT OnStnClickedTextReg(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnStnClickedTextFpw(WPARAM wParam, LPARAM lParam);

};
