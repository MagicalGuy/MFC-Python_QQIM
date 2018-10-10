#pragma once
#include "CClientSocket.h"
#include "CMyTab.h"
#include "afxwin.h"

// IM_MAIN �Ի���
#define WM_MY_MESSAGE WM_USER+100
class IM_MAIN : public CDialogEx
{
	DECLARE_DYNAMIC(IM_MAIN)

public:
	IM_MAIN(CClientSocket * client, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~IM_MAIN();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHAT_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	HANDLE m_hRecv = 0;							//�߳̾��
	static UINT  ThreadRecv(LPVOID lpParam);	//���ջص�����;
	
	CClientSocket * m_client;

	DECLARE_MESSAGE_MAP()
public:
	CMyTab m_tabctrl;
	CStatic m_staMyImg;
	CStatic m_staMyName;


	BOOL m_bLogin = FALSE;//��¼���컹����������


	CRect m_rect;
	ATL::CImage m_image;
	HBITMAP m_hBitmap;
	void drawimg();



	afx_msg void OnBnDoubleclickedAdd();
	afx_msg void OnStnClickedMyimg();
//	afx_msg void On32784_Set();


	//������Ϣ;
	CHATSEND *m_pObjChatRecv;

	void AddFrinend(CHATSEND& ct);	   //��Ӻ�����Ϣ
	void OneToGroup(CHATSEND& ct);	   //1vN��Ϣ
	void OneToOne(CHATSEND& ct);	   //1v1��Ϣ
	void SearchUser(CHATSEND& ct);	   //�����û���Ϣ
	void SearchGroup(CHATSEND& ct);	   //����Ⱥ��Ϣ
	void FileTrans(CHATSEND& ct);		//�ļ�����
	void ChatVideo(CHATSEND& ct);		//��Ƶ����
	void ChatRecord(CHATSEND& ct);      //�����¼


	afx_msg void OnBnClickedAdd();	   //������
	afx_msg void On32773SearchUser();  //�����û�
	afx_msg void On32774SearchGroup(); //����Ⱥ
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);	//�����Զ�����Ϣ�������촰��
	afx_msg void OnPaint();

};
