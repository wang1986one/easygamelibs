/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DProgressBar.cpp                                       */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include ".\d3dprogressbar.h"

namespace D3DGUI{

enum PROGRESS_BAR_BAR_WND_ID
{
	PB_BAR_WIN,
};

IMPLEMENT_CLASS_INFO(CD3DProgressBar,CD3DWnd);

CD3DProgressBar::CD3DProgressBar(void):CD3DWnd()
{
	m_pBarWnd=NULL;	
	InitWnd(NULL);
}

CD3DProgressBar::CD3DProgressBar(CD3DGUI * pGUI):CD3DWnd(pGUI)
{
	m_pBarWnd=NULL;	
	InitWnd(pGUI);
}

CD3DProgressBar::~CD3DProgressBar(void)
{
}

void CD3DProgressBar::InitWnd(CD3DGUI *  pGUI)
{
	SAFE_RELEASE(m_pBarWnd);
	m_CanDrag=false;
	m_CanResize=false;
	m_MaxPos=100;
	m_CurPos=50;
	m_WorkMode=PBW_STRETCH;
	m_Style=PROGRESS_BAR_HORIZONTAL;
	if(pGUI)
	{
		CEasyRect ClientRect=GetClientRect();
		m_pBarWnd=pGUI->CreateWnd(ClientRect);	
		m_pBarWnd->SetParent(this);
		m_pBarWnd->SetName("PBBarWnd");
		m_pBarWnd->SetID(PB_BAR_WIN);
		m_pBarWnd->SetInternal(true);
		m_pBarWnd->EnableFocus(false);
		UpdateProgessBar();
	}
}

void CD3DProgressBar::SetWorkMode(int Mode)
{
	if(m_pBarWnd)
	{
		m_WorkMode=Mode;
		if(m_WorkMode==PBW_CUT)
		{			
			m_pBarWnd->GetTexture(m_BarWndOrgTextureName,m_BarWndOrgTextureRect);
		}
		else
		{			
			if(!m_BarWndOrgTextureName.IsEmpty())
				m_pBarWnd->SetTexture(m_BarWndOrgTextureName,m_BarWndOrgTextureRect);
		}
		UpdateProgessBar();
	}
}

void CD3DProgressBar::SetProgressStyle(int Style)
{
	m_Style=Style;
	if(m_pBarWnd)
	{		
		UpdateProgessBar();
	}
}

void CD3DProgressBar::SetPos(unsigned __int64 Pos)
{
	m_CurPos=Pos;	
	UpdateProgessBar();
}

void CD3DProgressBar::SetMaxPos(unsigned __int64 MaxPos)
{
	m_MaxPos=MaxPos;
	UpdateProgessBar();
}

unsigned __int64 CD3DProgressBar::GetPosByXY(int x,int y)
{
	CEasyRect ClientRect=GetClientRect();
	double Ratio;
	if(m_Style==PROGRESS_BAR_HORIZONTAL)
		Ratio=(double)x/ClientRect.Width();
	else
		Ratio=(double)y/ClientRect.Height();
	return (unsigned __int64)(m_MaxPos*Ratio);
}

void CD3DProgressBar::UpdateProgessBar()
{
	if(m_pBarWnd)
	{
		FLOAT Ratio;

		if(m_CurPos<0)
			m_CurPos=0;
		if(m_MaxPos<0)
			m_MaxPos=0;
		if(m_CurPos>m_MaxPos)
			m_CurPos=m_MaxPos;
		CEasyRect ClientRect=GetClientRect();
		if(m_MaxPos)
			Ratio=(FLOAT)m_CurPos/m_MaxPos;
		else
			Ratio=1.0f;

		if(m_Style==PROGRESS_BAR_HORIZONTAL)
			ClientRect.right=(int)(ClientRect.left+ClientRect.Width()*Ratio);
		else
			ClientRect.top=(int)(ClientRect.bottom-ClientRect.Height()*Ratio);

		if(m_WorkMode==PBW_CUT)
		{
			CEasyRect TextureRect=m_BarWndOrgTextureRect;

			if(m_Style==PROGRESS_BAR_HORIZONTAL)
				TextureRect.right=(int)(TextureRect.left+TextureRect.Width()*Ratio);
			else
				TextureRect.top=(int)(TextureRect.bottom-TextureRect.Height()*Ratio);

			m_pBarWnd->SetTexture(m_BarWndOrgTextureName,TextureRect);
		}
		m_pBarWnd->SetRect(ClientRect);
	}
}

void CD3DProgressBar::UpdateRects()
{
	CD3DWnd::UpdateRects();
	UpdateProgessBar();
}

void CD3DProgressBar::SaveToXml(xml_node * pXMLNode)
{
	xml_node Wnd=pXMLNode->append_child(node_element,"ProgressBar");
	Wnd.append_attribute("Name",(LPCTSTR)GetName());
	Wnd.append_attribute("ID",(long)GetID());
	Wnd.append_attribute("IsInternal",IsInternal());

	xml_node Behavior=Wnd.append_child(node_element,"Behavior");
	SaveBehaviorToXML(Behavior);	
	Behavior.append_attribute("Style",(long)m_Style);
	Behavior.append_attribute("WorkMode",(long)m_WorkMode);
	Behavior.append_attribute("CurPos",(long)m_CurPos);
	Behavior.append_attribute("MaxPos",(long)m_MaxPos);

	xml_node Frame=Wnd.append_child(node_element,"Frame");
	SaveFrameToXML(Frame);	
	xml_node Borders=Wnd.append_child(node_element,"Borders");
	SaveBorderToXML(Borders);	
	if(m_pTexture)
	{	
		xml_node Texture=Wnd.append_child(node_element,"Texture");
		SaveTextureToXML(Texture);
	}

	if((int)m_ChildWndList.GetCount())
	{
		xml_node Childs=Wnd.append_child(node_element,"Childs");
		SaveChildsToXml(Childs);
	}

}

bool CD3DProgressBar::LoadFromXml(xml_node * pXMLNode)
{
	if(_strnicmp(pXMLNode->name(),"ProgressBar",10)!=0)
		return false;
	if(pXMLNode->has_attribute("Name"))
		SetName(pXMLNode->attribute("Name").getvalue().c_str());

	if(pXMLNode->has_attribute("ID"))
		SetID((long)pXMLNode->attribute("ID"));

	if(pXMLNode->has_attribute("IsInternal"))
		SetInternal((bool)pXMLNode->attribute("IsInternal"));


	for(int i=0;i<(int)pXMLNode->children();i++)
	{
		if(_strnicmp(pXMLNode->child(i).name(),"Behavior",9)==0)
		{
			LoadBehaviorFromXML(pXMLNode->child(i));
			if(pXMLNode->child(i).has_attribute("Style"))
				SetProgressStyle((long)pXMLNode->child(i).attribute("Style"));
			if(pXMLNode->child(i).has_attribute("WorkMode"))
				SetWorkMode((long)pXMLNode->child(i).attribute("WorkMode"));
			if(pXMLNode->child(i).has_attribute("MaxPos"))
				SetMaxPos((long)pXMLNode->child(i).attribute("MaxPos"));
			if(pXMLNode->child(i).has_attribute("CurPos"))
				SetPos((long)pXMLNode->child(i).attribute("CurPos"));
			
		}
		else if(_strnicmp(pXMLNode->child(i).name(),"Frame",6)==0)
		{
			LoadFrameFromXML(pXMLNode->child(i));
		}
		else if(_strnicmp(pXMLNode->child(i).name(),"Borders",8)==0)
		{
			LoadBorderFromXML(pXMLNode->child(i));
		}		
		else if(_strnicmp(pXMLNode->child(i).name(),"Texture",8)==0)
		{
			LoadTextureFromXML(pXMLNode->child(i));
		}
	}
	HandleMessage(this,WM_D3DGUI_WND_LOADED,GetID(),(LPARAM)this);

	//װ���Ӵ���
	for(int i=(int)pXMLNode->children()-1;i>=0;i--)
	{
		if(_strnicmp(pXMLNode->child(i).name(),"Childs",7)==0)
		{
			LoadChildsFromXml(pXMLNode->child(i));
			break;
		}
	}

	for(int i=(int)m_ChildWndList.GetCount()-1;i>=0;i--)
	{
		if(m_ChildWndList[i]->IsInternal()&&
			m_ChildWndList[i]->IsKindOf(GET_CLASS_INFO(CD3DWnd))&&
			(strcmp(m_ChildWndList[i]->GetName(),"PBBarWnd")==0)&&
			m_ChildWndList[i]!=m_pBarWnd)
		{
			CD3DWnd * pWnd=(CD3DWnd *)m_ChildWndList[i];
			SAFE_RELEASE(m_pBarWnd);
			m_pBarWnd=pWnd;
		}		
	}
	TopChild(true);
	HandleMessage(this,WM_D3DGUI_CHILD_LOADED,GetID(),(LPARAM)this);
	return true;
}

}