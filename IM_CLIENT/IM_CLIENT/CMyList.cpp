// CMyList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IM_CLIENT.h"
#include "CMyList.h"


// CMyList


bool CMyList::Isint = true;
bool CMyList::method = false;


IMPLEMENT_DYNAMIC(CMyList, CListCtrl)

CMyList::CMyList()
{

}

CMyList::~CMyList()
{
}


BEGIN_MESSAGE_MAP(CMyList, CListCtrl)
END_MESSAGE_MAP()



// CMyList ��Ϣ�������



int CALLBACK CMyList::ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	DATA* pListCtrl = (DATA*)lParamSort;
	int Col = pListCtrl->SubItem;		//������к�;

										//�õ�����ǰ����;
	CString StrItem1 = (pListCtrl->pList)->GetItemText(lParam1, Col);
	CString StrItem2 = (pListCtrl->pList)->GetItemText(lParam2, Col);

	//�ַ�������;
	if (!Isint)
	{
		int nTemp = StrItem1.CompareNoCase(StrItem2);	//�Ա��ַ���;һ������0�����򷵻�-1;

		if (method)	//true����;
		{
			if (nTemp <= 0)
				return -1;
			else
				return 1;
		}
		else
		{
			if (nTemp >= 0)
				return -1;
			else
				return 1;
		}
	}
	//��������;
	else if (Isint)
	{
		//INT64 n1 = _wtoi64(StrItem1);
		//INT64 n2 = _wtoi64(StrItem2);
		INT64 n1 = _ttoi64(StrItem1);
		INT64 n2 = _ttoi64(StrItem2);
		if (method)
		{
			if (n1 <= n2)
				return -1;
			else
				return 1;
		}

		else
		{
			if (n1 >= n2)
				return -1;
			else
				return 1;
		}

	}
	return -1;	//-1��ʾ��һ���ڵڶ���ǰ��,0��ʾ�������;
}




void CMyList::AddColumn(DWORD dwCount, ...)
{
	//�����б������;
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	va_list vlist;
	va_start(vlist, dwCount);
	//ѭ����ȡ����;
	for (DWORD i = 0; i < dwCount; i++)
	{
		TCHAR * pName = va_arg(vlist, TCHAR*);
		int	nWidth = va_arg(vlist, int);

		//������;
		InsertColumn(i, pName, NULL, nWidth);
	}
	va_end(vlist);
}

void CMyList::AddItem(DWORD dwCount, ...)
{
	//��ȡ����;
	int nItemNum = GetItemCount();

	//��ʼ��;
	va_list vlist;
	va_start(vlist, dwCount);

	//�����;
	TCHAR* pText = va_arg(vlist, TCHAR*);
	InsertItem(nItemNum, pText);

	//����������;
	for (DWORD i = 1; i < dwCount; i++)
	{
		//��������;
		pText = va_arg(vlist, TCHAR*);
		SetItemText(nItemNum, i, pText);
	}

	va_end(vlist);
}

void CMyList::SortItemsPlus(int nColum, bool Isint /*= true*/)
{
	this->Isint = Isint;
	//��ȡ����;
	int nCount = GetItemCount();
	for (int i = 0; i < nCount; i++)
		SetItemData(i, i);

	DATA dData;
	dData.SubItem = nColum;
	dData.pList = this;

	//��������;
	method = !method;

	//����;
	SortItems(ListCompare, (LPARAM)&dData);
}



