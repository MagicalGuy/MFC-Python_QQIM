#pragma once
#include "stdafx.h"

// CMyTab

class CMyTab : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTab)

public:
	CMyTab();
	virtual ~CMyTab();
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);	//ѡ��л�;s	
	void MyInserItem(DWORD dwCount, ...);							//����ѡ���ǩ;	
	void MyInsertChild(DWORD dwCount, ...);							//����ӶԻ���;	
	DWORD m_dwCount;												//�����ӶԻ�����Ϣ;
	CDialogEx* m_pWnd[5];											//�Ӵ�������;
	
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	
};


