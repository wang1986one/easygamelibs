/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DSizeBar.cpp                                           */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include ".\d3dsizebar.h"
#include "ObjectPropertyView.h"
#include "ObjectTreeView.h"
#include ".\sizebarmanager.h"


IMPLEMENT_CLASS_INFO(CD3DSizeBar,CD3DWnd);

CD3DSizeBar::CD3DSizeBar():CD3DWnd()
{		
	m_pObjectTreeView=NULL;
	m_pObjectPropertyView=NULL;
	m_pManager=NULL;

	m_IsControlPress=false;
	m_IsShiftPress=false;
}

CD3DSizeBar::CD3DSizeBar(CD3DGUI *  pGUI):CD3DWnd(pGUI)
{	
	m_pObjectTreeView=NULL;
	m_pObjectPropertyView=NULL;
	m_pManager=NULL;

	m_IsControlPress=false;
	m_IsShiftPress=false;
}

CD3DSizeBar::~CD3DSizeBar(void)
{
	if(GetTreeItem())
		m_pObjectTreeView->GetTreeCtrl().DeleteItem(GetTreeItem());
	if(m_pObjectPropertyView)
		m_pObjectPropertyView->SetGUIObject(NULL);
	if(m_pManager)
		m_pManager->DeleteObject(this);
}

void CD3DSizeBar::SetManager(CSizeBarManager * pManager)
{
	m_pManager=pManager;
	m_pManager->AddObject(this);
}

BOOL CD3DSizeBar::OnMessage(CD3DWnd * pWnd,UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(CD3DWnd::OnMessage(pWnd,msg,wParam,lParam))	
		return true;
	switch(msg)
	{
	case WM_LBUTTONUP:
	case WM_D3DGUI_GET_FOCUS:
		PrintSystemLog(0,_T("%s获得焦点"),
			GetChild()->GetName());
		if(pWnd==this||pWnd==GetChild())
		{
			if(m_pManager->AddSelectedObject(this))
			{
				
				SetVisible(true);
				
				SetBKColor(0xffffffff);
				if(m_pObjectTreeView)
					m_pObjectTreeView->GetTreeCtrl().SelectItem(GetTreeItem());				
			}
		}
		break;
	case WM_D3DGUI_LOST_FOCUS:
		PrintSystemLog(0,_T("%s失去焦点"),
			GetChild()->GetName());
		if(pWnd==this||pWnd==GetChild())
		{
			if(m_pManager->IsControlPress())
			{
				SetBKColor(0x80ffffff);
			}
			else
			{
				SetVisible(false);
				m_pManager->ClearSelectedObject();
			}
		}
		break;
	case WM_D3DGUI_RESIZING:
		{
			UpdateSizeToChild();				
			return true;
		}
		break;
	case WM_D3DGUI_RESIZED:
		{
			if(m_pObjectPropertyView)
				m_pObjectPropertyView->UpdateObjectRect();
			return true;
		}
		break;
	case WM_D3DGUI_EDIT_TEXT_CHANGED:
		{
			if(m_pObjectPropertyView)
				m_pObjectPropertyView->UpdateObjectText();
			return true;
		}
		break;	
	}
	return false;
}

BOOL CD3DSizeBar::HandleMessage(CD3DWnd * pWnd,UINT msg, WPARAM wParam, LPARAM lParam)
{

	if(!m_IsEnable)
		return false;

	if(!OnMessage(pWnd,msg,wParam,lParam))
	{
		if(m_pParentWnd!=NULL) 
			return m_pParentWnd->HandleMessage(pWnd,msg,wParam,lParam);
		else if(msg>WM_D3DGUI_START)
			return m_pGUI->SendMsgToParent(msg,wParam,lParam);
		else
			return false;
	}
	else 
		return true;

}

CD3DWnd * CD3DSizeBar::GetChild()
{
	//POSITION Pos=m_ChildWndList.GetHeadPosition();

	if(m_ChildWndList.GetCount()<=0)
		return NULL;
	return m_ChildWndList[0];
}

void CD3DSizeBar::GetMiniSize(int& Width,int& Height)
{
	CD3DWnd::GetMiniSize(Width,Height);

	if(GetChild())
	{
		int ChildWidth,ChildHeight;
		GetChild()->GetMiniSize(ChildWidth,ChildHeight);
		Width+=ChildWidth;
		Height+=ChildHeight;
	}

}

void CD3DSizeBar::UpdateRects()
{
	CD3DWnd::UpdateRects();
	UpdateChildRects();
}

void CD3DSizeBar::UpdateSizeToChild()
{
	CRect Rect=GetClientRect();
	GetChild()->SetRect(Rect);		
}


void CD3DSizeBar::SaveToXml(xml_node * pXMLNode)
{
	CRect Rect=GetChild()->GetRect();
	GetChild()->SetRect(GetCenterRect());
	GetChild()->SetParent(GetParent(),false);
	GetChild()->SaveToXml(pXMLNode);
	GetChild()->SetParent(this,false);
	GetChild()->SetRect(Rect);
}

bool CD3DSizeBar::ToSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param)
{
	CRect Rect=GetChild()->GetRect();
	GetChild()->SetRect(GetCenterRect());
	GetChild()->SetParent(GetParent(),false);
	bool Ret=GetChild()->ToSmartStruct(Packet,pResourceManager,Param);
	GetChild()->SetParent(this,false);
	GetChild()->SetRect(Rect);
	return Ret;
}

UINT CD3DSizeBar::GetSmartStructSize(UINT Param)
{
	CRect Rect=GetChild()->GetRect();
	GetChild()->SetRect(GetCenterRect());
	GetChild()->SetParent(GetParent(),false);
	UINT Ret=GetChild()->GetSmartStructSize(Param);
	GetChild()->SetParent(this,false);
	GetChild()->SetRect(Rect);
	return Ret;
}

void CD3DSizeBar::PickResource(CUSOResourceManager * pResourceManager,UINT Param)
{
	CRect Rect=GetChild()->GetRect();
	GetChild()->SetRect(GetCenterRect());
	GetChild()->SetParent(GetParent(),false);
	GetChild()->PickResource(pResourceManager,Param);
	GetChild()->SetParent(this,false);
	GetChild()->SetRect(Rect);	
}

void CD3DSizeBar::ClearSelect()
{
	PrintSystemLog(0,_T("%s被隐藏"),
		GetChild()->GetName());
	SetVisible(false);
}



void CD3DSizeBar::SetParent(CD3DWnd * parent,bool IsUpdateParent)
{
	CD3DWnd::SetParent(parent,IsUpdateParent);
	
	if(m_pObjectTreeView)
	{
		TVINSERTSTRUCT ItemInfo;
		TCHAR buff[1024];

		ZeroMemory(&ItemInfo,sizeof(ItemInfo));
		
		CD3DSizeBar * pParentSizeBar=NULL;
		if(parent)
			pParentSizeBar=(CD3DSizeBar *)(parent->GetParent());
		if(pParentSizeBar)
			ItemInfo.hParent=pParentSizeBar->GetTreeItem();
		else
			ItemInfo.hParent=m_pObjectTreeView->GetParentItem(NULL);
		ItemInfo.hInsertAfter=TVI_LAST;
		ItemInfo.item.hItem=GetTreeItem();
		ItemInfo.item.mask=TVIF_HANDLE|TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_STATE|TVIF_TEXT;
		ItemInfo.item.pszText=buff;
		ItemInfo.item.cchTextMax=1024;
		m_pObjectTreeView->GetTreeCtrl().GetItem(&(ItemInfo.item));
		m_pObjectTreeView->GetTreeCtrl().DeleteItem(GetTreeItem());
		SetTreeItem(m_pObjectTreeView,m_pObjectTreeView->GetTreeCtrl().InsertItem(&ItemInfo));
		m_pObjectTreeView->GetTreeCtrl().Expand(ItemInfo.hParent,TVE_EXPAND);
	}
	
}

void CD3DSizeBar::UpdateTreeItem()
{
	if(m_pObjectTreeView)
	{
		m_pObjectTreeView->UpdateGUIObject(this);
	}
}

void CD3DSizeBar::SetVisible(bool IsVisible)
{
	if(GetChild()->IsInternal())
		m_IsVisible=false;
	else
		m_IsVisible=IsVisible;
	for(int i=0;i<m_WndRectCount;i++)
	{
		if(m_pWndRects[i])
			m_pWndRects[i]->SetVisible(IsVisible);
	}
	if(m_IsVisible)
	{
		UpdateRects();
	}
}