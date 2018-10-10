
// IM_CLIENTDlg.h : ͷ�ļ�
//

#pragma once
#include ".\UI\Dialog\DlgBase.h"
#include "CClientSocket.h"
#include "CDlgRegister.h"

// CIM_CLIENTDlg �Ի���
class CIM_CLIENTDlg : public CDlgBase{
// ����
public:
	CIM_CLIENTDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_QQ_DIALOG };

	CClientSocket m_client;
	CDlgRegister m_cReg;		 //ע��



	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	afx_msg LRESULT OnMessageButtonLogin(WPARAM wParam, LPARAM lParam);//�����¼��ť
	afx_msg LRESULT OnStnClickedTextReg(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnStnClickedTextFpw(WPARAM wParam, LPARAM lParam);

};
