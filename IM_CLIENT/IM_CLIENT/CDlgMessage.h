#pragma once
#include "CMyList.h"
#include "CClientSocket.h"
#include <locale>
#include <vector>
using std::vector;


// CDlgMessage �Ի���

class CDlgMessage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMessage)

public:
	CDlgMessage(CClientSocket * client, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMessage();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MSG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CMyList m_LastChatList;

	CClientSocket * m_client;

	vector<CString> m_vecChatList;		//�����б�VEC

	void UpDataChatList(CString fid);	//���������б�

	void SaveChatList();				//�����б�
	void ReadChatList();				//��ȡ�б�

	afx_msg void OnNMDblclkLastchat(NMHDR *pNMHDR, LRESULT *pResult);
};
