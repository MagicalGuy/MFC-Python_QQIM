// IM_CLIENTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "IM_CLIENTDlg.h"
#include "IM_MAIN.h"
#include ".\UI\Button\ButtonEx.h"
#include ".\UI\Button\CheckButton.h"
#include ".\UI\Button\StatusButton.h"
#include ".\UI\Button\LinkButton.h"
#include ".\UI\Image\PhotoFrame.h"
#include ".\UI\Edit\EditEx.h"
#include ".\ui\Other\Area.h"
#include ".\UI\List\LoginList.h"
#include ".\UI\Menu\MenuEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//弹出框

#define					WM_LOGIN_EDIT_LIST					(WM_USER + 2)
#define					WM_LOGIN_STATUS_MENU				(WM_USER + 3)

//控件
#define					BT_ACCOUNT							2001
#define					BT_SETTING							2002
#define					BT_LOGIN							2003    //登录按钮
#define					BT_STATUS							2004
#define					BT_CHECK_PASSWORD					2005
#define					BT_CHECK_AUTO_LOGIN					2006
#define					BT_LINK_ACCOUNT						2007   //注册账号
#define					BT_LINK_PASSWORD					2008
#define					PIC_HEAD							2009
#define					EDIT_ACCOUNT						2010
#define					EDIT_PASSWORD						2011
#define					AREA_UP								2012
#define					AREA_DOWN							2013
#define					AREA_LINE							2014
#define					AREA_BUTTON							2015

//菜单 
#define					MENU_IMONLINE						1
#define					MENU_PNG_QME						2
#define					MENU_PNG_AWAY						3
#define					MENU_PNG_BUSY						4
#define					MENU_PNG_MUTE						5
#define					MENU_PNG_INVISIBLE					6


CIM_CLIENTDlg::CIM_CLIENTDlg(CWnd* pParent /*=NULL*/)
	: CDlgBase(CIM_CLIENTDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIM_CLIENTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgBase::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CIM_CLIENTDlg, CDlgBase)
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_LOGIN_EDIT_LIST,OnMessageLoginList)
	ON_MESSAGE(WM_LOGIN_STATUS_MENU,OnMessageLoginMenu)
	ON_MESSAGE(BT_ACCOUNT,OnMessageButtomAccount)
	ON_MESSAGE(BT_STATUS,OnMessageButtomStatus)
	ON_MESSAGE(BT_CHECK_PASSWORD,OnMessageCheckPassWord)
	ON_MESSAGE(BT_CHECK_AUTO_LOGIN,OnMessageCheckAutoLogin)
	ON_MESSAGE(EDIT_ACCOUNT,OnMessageEditAccount)
	ON_MESSAGE(BT_LOGIN,OnMessageButtonLogin)
	ON_MESSAGE(BT_LINK_ACCOUNT, OnStnClickedTextReg)
	ON_MESSAGE(BT_LINK_PASSWORD,OnStnClickedTextFpw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CIM_CLIENTDlg 消息处理程序

BOOL CIM_CLIENTDlg::OnInitDialog()
{
	CDlgBase::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	srand(time(NULL)); 
	LoadImage(IDB_BITMAP_BACK + ((1 != (rand() % 4)) ? 0 : (rand() % 3)));

	return TRUE;
}

void CIM_CLIENTDlg::InitUI(CRect rcClient)
{
	CRect rcTemp;
	int nStartX = 0;
	int nStartY = 0;
	CControlBase * pControlBase = NULL;

	rcTemp.SetRect(0, rcClient.bottom - 190, rcClient.right, rcClient.bottom - 140);
	pControlBase = new CArea(GetSafeHwnd(), AREA_UP, rcTemp, 100, 40, RGB(255, 255, 255));
	m_vecArea.push_back(pControlBase);

	rcTemp.SetRect(0, rcClient.bottom - 140, rcClient.right, rcClient.bottom);
	pControlBase = new CArea(GetSafeHwnd(), AREA_DOWN, rcTemp, 40, 40, RGB(255, 255, 255));
	m_vecArea.push_back(pControlBase);

	rcTemp.SetRect(0, rcClient.bottom - 37, rcClient.right, rcClient.bottom - 36);
	pControlBase = new CArea(GetSafeHwnd(), AREA_LINE, rcTemp, 70, 70, RGB(128, 128, 128));
	m_vecArea.push_back(pControlBase);

	rcTemp.SetRect(0, rcClient.bottom - 36, rcClient.right, rcClient.bottom);
	pControlBase = new CArea(GetSafeHwnd(), AREA_BUTTON, rcTemp, 88, 88, RGB(128, 128, 128));
	m_vecArea.push_back(pControlBase);

	nStartX = rcClient.left + 7;
	nStartY = rcClient.bottom - 29;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 69, nStartY + 22);
	pControlBase = new CButtonEx(GetSafeHwnd(), BT_ACCOUNT, rcTemp, "多帐号");
	((CButtonEx *)pControlBase)->SetBitmap(IDB_BT_DEFAULT);
	((CButtonEx *)pControlBase)->SetSmallBitmap(IDB_PNG_SWITCH_BTN);
	m_vecControl.push_back(pControlBase);

	nStartX += 77;
	nStartY = rcClient.bottom - 29;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 69, nStartY + 22);
	pControlBase = new CButtonEx(GetSafeHwnd(), BT_SETTING, rcTemp, "设置");
	((CButtonEx *)pControlBase)->SetBitmap(IDB_BT_DEFAULT);
	m_vecControl.push_back(pControlBase);

	nStartX = rcClient.right - 76;
	nStartY = rcClient.bottom - 29;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 69, nStartY + 22);
	pControlBase = new CButtonEx(GetSafeHwnd(), BT_LOGIN, rcTemp, "登录");
	((CButtonEx *)pControlBase)->SetBitmap(IDB_BT_DEFAULT);
	m_vecControl.push_back(pControlBase);

	nStartX = rcClient.left + 16;
	nStartY = rcClient.top + 139;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 87, nStartY + 87);
	pControlBase = new CPhotoFrame(GetSafeHwnd(), PIC_HEAD, rcTemp);
	((CPhotoFrame *)pControlBase)->SetImageBitmap(IDB_PNG_HEAD_106);
	((CPhotoFrame *)pControlBase)->SetFrameBitmap(IDB_PNG_HEAD_BK);
	m_vecControl.push_back(pControlBase);
	

	nStartX = rcClient.left + 113;
	nStartY = rcClient.top + 210;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 35, nStartY + 22);
	pControlBase = new CStatusButton(GetSafeHwnd(), BT_STATUS, rcTemp);
	((CStatusButton *)pControlBase)->SetBitmap(IDB_PNG_STATUS_ARROW);
	((CStatusButton *)pControlBase)->SetStatusBitmap(IDB_PNG_IMONLINE);
	m_vecControl.push_back(pControlBase);


	nStartX = rcClient.left + 156;
	nStartY = rcClient.top + 213;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 70, nStartY + 17);
	pControlBase = new CCheckButton(GetSafeHwnd(), BT_CHECK_PASSWORD, rcTemp, "记住密码");
	((CCheckButton *)pControlBase)->SetBitmap(IDB_PNG_LONG_CHECK_BOX);
	m_vecControl.push_back(pControlBase);

	nStartX += 80;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 70, nStartY + 17);
	pControlBase = new CCheckButton(GetSafeHwnd(), BT_CHECK_AUTO_LOGIN, rcTemp, "自动登录");
	((CCheckButton *)pControlBase)->SetBitmap(IDB_PNG_LONG_CHECK_BOX);
	m_vecControl.push_back(pControlBase);

	nStartX = rcClient.right - 64;
	nStartY = rcClient.top + 150;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 48, nStartY + 13);
	pControlBase = new CLinkButton(GetSafeHwnd(), BT_LINK_ACCOUNT, rcTemp, "注册帐号", "");
	m_vecControl.push_back(pControlBase);

 	nStartY += 32;
 	rcTemp.SetRect(nStartX, nStartY, nStartX + 48, nStartY + 13);
 	pControlBase = new CLinkButton(GetSafeHwnd(), BT_LINK_PASSWORD, rcTemp, "找回密码", "");
 	m_vecControl.push_back(pControlBase);

	nStartX = rcClient.left + 112;
	nStartY = rcClient.top + 142;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 191, nStartY + 28);
	pControlBase = new CEditEx(GetSafeHwnd(), EDIT_ACCOUNT, rcTemp, "278162354");
	((CEditEx *)pControlBase)->SetBitmap(IDB_PNG_EDIT);
	((CEditEx *)pControlBase)->SetSmallBitmap(IDB_PNG_BT_DROP_DOWN);
	((CEditEx *)pControlBase)->SetRect(rcTemp);
	m_vecControl.push_back(pControlBase);	

	nStartY += 34;
	rcTemp.SetRect(nStartX, nStartY, nStartX + 191, nStartY + 28);
	//pControlBase = new CEditEx(GetSafeHwnd(), EDIT_PASSWORD, rcTemp, "", TRUE);
	pControlBase = new CEditEx(GetSafeHwnd(), EDIT_PASSWORD, rcTemp, "");
	((CEditEx *)pControlBase)->SetBitmap(IDB_PNG_EDIT);
	((CEditEx *)pControlBase)->SetSmallBitmap(IDB_PNG_BT_KEY_BOARD);
	((CEditEx *)pControlBase)->SetRect(rcTemp);
	m_vecControl.push_back(pControlBase);	

}

void CIM_CLIENTDlg::OnSize(CRect rcClient)
{

}

HCURSOR CIM_CLIENTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//登录账户列表
LRESULT CIM_CLIENTDlg::OnMessageLoginList(WPARAM wParam, LPARAM lParam)
{
	if(m_pWndPopup)
	{
		CLoginList *pLoginList = (CLoginList *)m_pWndPopup;
		if(DELETE_ITEM == wParam)
		{
			pLoginList->DeleteItem(lParam);
		}
		else if(SELECT_ITEM == wParam)
		{
			CPhotoFrame *pPhotoFrame = (CPhotoFrame *)GetControl(PIC_HEAD);
			if(pPhotoFrame)
			{
				UINT nResourceID = 0;
				CString strImageFile;
				if(pLoginList->GetItemImage(lParam, nResourceID, strImageFile))
				{
					if(!pPhotoFrame->SetImageBitmap(strImageFile))
					{
						pPhotoFrame->SetImageBitmap(nResourceID);
					}
					InvalidateRect(pPhotoFrame->GetRect());					
				}
				
				
				CEditEx *pEditEx = (CEditEx *)GetControl(EDIT_ACCOUNT);
				if(pEditEx)
				{
					CString strNumber;
					pLoginList->GetItemNumber(lParam, strNumber);
					pEditEx->SetTitle(strNumber);
					InvalidateRect(pEditEx->GetRect());		
				}
				CloseDlgPopup();
				
			}
		}
	}
	return 0;
}

//点击登录状态
LRESULT CIM_CLIENTDlg::OnMessageLoginMenu(WPARAM wParam, LPARAM lParam)
{
	if(m_pWndPopup)
	{
		CStatusButton *pStatusButton = (CStatusButton *)GetControl(BT_STATUS);
		if(pStatusButton)
		{
			if(MENU_IMONLINE == lParam) pStatusButton->SetStatusBitmap(IDB_PNG_IMONLINE);			
			else if(MENU_PNG_QME == lParam) pStatusButton->SetStatusBitmap(IDB_PNG_QME);				
			else if(MENU_PNG_AWAY == lParam) pStatusButton->SetStatusBitmap(IDB_PNG_AWAY);	
			else if(MENU_PNG_BUSY == lParam) pStatusButton->SetStatusBitmap(IDB_PNG_BUSY);	
			else if(MENU_PNG_MUTE == lParam) pStatusButton->SetStatusBitmap(IDB_PNG_MUTE);	
			else if(MENU_PNG_INVISIBLE == lParam) pStatusButton->SetStatusBitmap(IDB_PNG_INVISIBLE);	

			InvalidateRect(pStatusButton->GetRect());		
		}
		CloseDlgPopup();
	}
	return 0;
}


//单账号多账号按钮
LRESULT CIM_CLIENTDlg::OnMessageButtomAccount(WPARAM wParam, LPARAM lParam)
{
	if(BUTTOM_UP == lParam)
	{
		CButtonEx *pButtonAccount = (CButtonEx *)GetControl(BT_ACCOUNT);
		if(pButtonAccount)
		{
			pButtonAccount->SetTitle("多帐号" == pButtonAccount->GetTitle() ? "单帐号" : "多帐号");
		}
	}

	return 0;
}


//点击切换登录状态
LRESULT CIM_CLIENTDlg::OnMessageButtomStatus(WPARAM wParam, LPARAM lParam)
{
	if(BUTTOM_UP == lParam)
	{
		CStatusButton *pStatusButton = (CStatusButton *)GetControl(BT_STATUS);
		if(pStatusButton)
		{
			CMenuEx *pMenu = new CMenuEx;
			pMenu->SetBackBitmap(IDB_PNG_MENU);
			CRect rcClient = pStatusButton->GetRect();

			rcClient.top = rcClient.bottom + 1;
			rcClient.left -= 4;
			OpenDlgPopup(pMenu, rcClient, WM_LOGIN_STATUS_MENU);	
		}
	}

	return 0;
}

//点击记住密码单选框
LRESULT CIM_CLIENTDlg::OnMessageCheckPassWord(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
//点击自动登录单选框
LRESULT CIM_CLIENTDlg::OnMessageCheckAutoLogin(WPARAM wParam, LPARAM lParam)
{
	if(BUTTOM_UP == lParam)
	{
		CCheckButton *pCheckButtonAutoLogin = (CCheckButton *)GetControl(BT_CHECK_AUTO_LOGIN);
		if(pCheckButtonAutoLogin)
		{
			if(pCheckButtonAutoLogin->GetCheck())
			{
				CCheckButton *pCheckButtonPassWord = (CCheckButton *)GetControl(BT_CHECK_PASSWORD);
				if(pCheckButtonPassWord)
				{
					pCheckButtonPassWord->SetCheck(true);
				}
			}
		}
	}
	
	return 0;
}

//点击账户切换列表
LRESULT CIM_CLIENTDlg::OnMessageEditAccount(WPARAM wParam, LPARAM lParam)
{
	if(BUTTOM_DOWN == lParam)
	{
		CEditEx *pEditEx = (CEditEx *)GetControl(EDIT_ACCOUNT);
		if(pEditEx)
		{
			if(pEditEx->GetLButtonDown())
			{
				CLoginList *pLoginList = new CLoginList;
				pLoginList->SetHeadBitmap(IDB_PNG_LOGIN_LIST_HEAD);
				pLoginList->SetDeleteBitmap(IDB_PNG_LOGIN_LIST_CLOSE);
				CRect rcClient = pEditEx->GetRect();
				rcClient.top = rcClient.bottom;
				OpenDlgPopup(pLoginList, rcClient, WM_LOGIN_EDIT_LIST);	
			}
			else
			{

			}
		}	
	}
	return 0;
}

LRESULT CIM_CLIENTDlg::OnMessageButtonLogin(WPARAM wParam, LPARAM lParam)
{
	if (BUTTOM_UP == lParam)
	{
		CButtonEx *pButtonLogin = (CButtonEx *)GetControl(BT_LOGIN);
		if (pButtonLogin)
		{			
			CEditEx *pEditEx = (CEditEx *)GetControl(EDIT_ACCOUNT);
			CString m_strUID = pEditEx->GetTitle();
			//MessageBox(m_strUID);
			CEditEx *pEditEx2 = (CEditEx *)GetControl(EDIT_PASSWORD);
			CString m_strPWD = pEditEx2->GetTitle();
			//CString m_strPWD;
			//this->GetDlgItemTextA(EDIT_PASSWORD, m_strPWD);
			//CString m_strPWD;
			//GetDlgItemText(EDIT_PASSWORD,m_strPWD);

			//MessageBox(m_strPWD);
			// TODO: 在此添加控件通知处理程序代码
			//UpdateData(TRUE);
			if (m_strUID.IsEmpty() ||
				m_strPWD.IsEmpty())
			{
				MessageBox("用户名/密码不能为空");
				return 0;
			}

			if (!m_client.ConnectServer("192.168.3.184", 1234))
			{
				MessageBox("连接服务器失败！", "Error", MB_OK | MB_ICONWARNING);
				return 0;
			}

			//用户名，密码
			CString strSend = m_strUID;
			strSend += ":" + m_strPWD;
			CStringA str = CT2A(strSend.GetBuffer(), CP_THREAD_ACP);
			m_client.Send(LOGIN, str.GetBuffer(), str.GetLength() + 1);

			char *ret = m_client.Recv();
			if (!strcmp(ret, "登录失败！"))
			{
				MessageBox("账号/密码错误");
				m_client.Close();
				return 0;
			}

			//登录成功;//获取用户名;
			CStringA strShowName = CT2A(m_strUID.GetBuffer(), CP_THREAD_ACP);
			CStringA strPWD = CT2A(m_strPWD.GetBuffer(), CP_THREAD_ACP);
			strcpy_s(m_client.m_szName, ret);
			strcpy_s(m_client.m_szUID, strShowName.GetBuffer());
			strcpy_s(m_client.m_szPWD, strPWD.GetBuffer());
			ShowWindow(SW_HIDE);
			//显示聊天窗口;
			IM_MAIN dlgChat(&m_client);
			dlgChat.DoModal();

			m_client.Close();

			EndDialog(0);

		}
	}

	return 0;
}

LRESULT CIM_CLIENTDlg::OnStnClickedTextReg(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加控件通知处理程序代码

	//CLinkButton* PRegister = (CLinkButton*)GetControl(BT_LINK_ACCOUNT);
	if (BUTTOM_UP == lParam)
	{
		m_cReg.DoModal();
	}
	return 0;
}

LRESULT CIM_CLIENTDlg::OnStnClickedTextFpw(WPARAM wParam, LPARAM lParam)
{
	if (BUTTOM_UP == lParam)
	{
		MessageBox("请联系系统管理员");
	}
	return 0;
}
