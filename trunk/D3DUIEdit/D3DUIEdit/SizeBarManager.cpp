/****************************************************************************/
/*                                                                          */
/*      文件名:    SizeBarManager.cpp                                       */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
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