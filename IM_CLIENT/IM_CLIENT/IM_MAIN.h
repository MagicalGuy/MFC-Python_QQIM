#pragma once
#include "CClientSocket.h"
#include "CMyTab.h"
#include "afxwin.h"

// IM_MAIN 对话框
#define WM_MY_MESSAGE WM_USER+100
class IM_MAIN : public CDialogEx
{
	DECLARE_DYNAMIC(IM_MAIN)

public:
	IM_MAIN(CClientSocket * client, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~IM_MAIN();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHAT_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	HANDLE m_hRecv = 0;							//线程句柄
	static UINT  ThreadRecv(LPVOID lpParam);	//接收回调函数;
	
	CClientSocket * m_client;

	DECLARE_MESSAGE_MAP()
public:
	CMyTab m_tabctrl;
	CStatic m_staMyImg;
	CStatic m_staMyName;


	BOOL m_bLogin = FALSE;//登录聊天还是匿名聊天


	CRect m_rect;
	ATL::CImage m_image;
	HBITMAP m_hBitmap;
	void drawimg();



	afx_msg void OnBnDoubleclickedAdd();
	afx_msg void OnStnClickedMyimg();
//	afx_msg void On32784_Set();


	//接收消息;
	CHATSEND *m_pObjChatRecv;

	void AddFrinend(CHATSEND& ct);	   //添加好友消息
	void OneToGroup(CHATSEND& ct);	   //1vN消息
	void OneToOne(CHATSEND& ct);	   //1v1消息
	void SearchUser(CHATSEND& ct);	   //搜索用户消息
	void SearchGroup(CHATSEND& ct);	   //搜索群消息
	void FileTrans(CHATSEND& ct);		//文件传输
	void ChatVideo(CHATSEND& ct);		//视频聊天
	void ChatRecord(CHATSEND& ct);      //聊天记录


	afx_msg void OnBnClickedAdd();	   //点击添加
	afx_msg void On32773SearchUser();  //搜索用户
	afx_msg void On32774SearchGroup(); //搜索群
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);	//接收自定义消息创建聊天窗口
	afx_msg void OnPaint();

};
