/****************************************************************************/
/*                                                                          */
/*      �ļ���:    SizeBarManager.cpp                                       */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include ".\sizebarmanager.h"

CSizeBarManager::CSizeBarManager(void)
{
	m_IsControlPress=false;
}

CSizeBarManager::~CSizeBarManager(void)
{
}

void CSizeBarManager::AddObject(CD3DSizeBar * pObject)
{
	for(int i=0;i<(int)m_pSizeBarList.size();i++)
	{
		if(m_pSizeBarList[i]==pObject)
			return;
	}
	m_pSizeBarList.push_back(pObject);
}

void CSizeBarManager::DeleteObject(CD3DSizeBar * pObject)
{
	for(int i=0;i<(int)m_pSizeBarList.size();i++)
	{
		if(m_pSizeBarList[i]==pObject)
		{
			m_pSizeBarList.erase(m_pSizeBarList.begin()+i);
			return;
		}
	}
}

bool CSizeBarManager::AddSelectedObject(CD3DSizeBar * pObject)
{
	for(int i=0;i<(int)m_pSelectedSizeBarList.size();i++)
	{
		if(m_pSelectedSizeBarList[i]==pObject)
			return false;		
	}
	m_pSelectedSizeBarList.push_back(pObject);
	return true;
}


void CSizeBarManager::DeleteSelectedObject(CD3DSizeBar * pObject)
{
	for(int i=0;i<(int)m_pSelectedSizeBarList.size();i++)
	{
		if(m_pSelectedSizeBarList[i]==pObject)
		{
			m_pSelectedSizeBarList.erase(m_pSelectedSizeBarList.begin()+i);
			return;
		}
	}
}

void CSizeBarManager::DoMove(int dx,int dy)
{
	for(int i=0;i<GetSelectedObjectCount();i++)
	{
		CD3DSizeBar * pObj=GetSelectedObject(i);
		CRect Rect=pObj->GetRect();											
		Rect.OffsetRect(dx,dy);		
		pObj->SetRect(Rect);
		pObj->UpdateSizeToChild();

	}

}

void CSizeBarManager::DoResize(int dx,int dy)
{
	for(int i=0;i<GetSelectedObjectCount();i++)
	{
		CD3DSizeBar * pObj=GetSelectedObject(i);
		CRect Rect=pObj->GetRect();											
		Rect.right+=dx;
		Rect.bottom+=dy;
		pObj->SetRect(Rect);
		pObj->UpdateSizeToChild();

	}
}