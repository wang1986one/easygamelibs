/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DCombo.cpp                                             */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include ".\d3dcombo.h"

namespace D3DGUI{


enum COMBO_INTER_CONTROL
{
	CIC_EB=4729,
	CIC_LIST,
};

IMPLEMENT_CLASS_INFO(CD3DCombo,CD3DEdit);

CD3DCombo::CD3DCombo():CD3DEdit()
{
	m_pExpandButton=NULL;
	m_pComboList=NULL;
	m_ComboListOrginHeight=0;
	InitWnd(NULL);

}

CD3DCombo::CD3DCombo(CD3DGUI * pGUI):CD3DEdit(pGUI)
{
	m_pExpandButton=NULL;
	m_pComboList=NULL;
	m_ComboListOrginHeight=0;
	InitWnd(pGUI);	
}

CD3DCombo::~CD3DCombo(void)
{
	
}

void CD3DCombo::InitWnd(CD3DGUI *  pGUI)
{
	m_IsReadyOnly=true;	
	m_IsListShow=false;
	m_CanDrag=false;
	if(pGUI)
	{
		m_pExpandButton=pGUI->CreateButton(CEasyRect(0,0,0,0));
		m_pExpandButton->SetParent(this);
		m_pExpandButton->SetName("ComboEB");
		m_pExpandButton->SetID(CIC_EB);
		m_pExpandButton->SetInternal(true);
		//m_pExpandButton->EnableFocus(false);

		m_pComboList=pGUI->CreateSimpleList(CEasyRect(0,0,0,0));	
		m_pComboList->SetParent(this);
		m_pComboList->SetName("ComboList");
		m_pComboList->SetID(CIC_LIST);
		m_pComboList->SetVisible(false);
		m_pComboList->SetInternal(true);
		//m_pComboList->EnableFocus(false);
	}

	m_ComboListOrginHeight=m_pComboList->GetWndRect().Height();
}


BOOL CD3DCombo::OnMessage(CD3DWnd * pWnd,UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(CD3DEdit::OnMessage(pWnd,msg,wParam,lParam))	
		return true;


	switch(msg)
	{
	case WM_D3DGUI_BUTTON_CLICK:
		if(wParam==CIC_EB)
		{
			m_IsListShow=!m_IsListShow;
			m_pComboList->SetVisible(m_IsListShow);
			if(m_pComboList->IsVisible())
			{
				CEasyRect Rect=GetWndRect();
				ScreenToClient(&Rect);

				int Height=m_pComboList->GetItemCount()*m_pComboList->GetItemHeight();

				if(Height<0)
					Height=m_pComboList->GetItemHeight();

				Height+=m_pComboList->GetBorder(RECT_TOP)+m_pComboList->GetBorder(RECT_BOTTOM);

				if(Height>m_ComboListOrginHeight)
					Height=m_ComboListOrginHeight;

				Rect.top=Rect.bottom;
				Rect.bottom=Rect.top+Height;
				m_pComboList->SetRect(Rect);
				m_pComboList->SetFocus();				
			}
			return true;
		}
		break;
	case WM_D3DGUI_LOST_FOCUS:
		if(wParam==CIC_LIST)
		{
			CEasyRect Rect=m_pExpandButton->GetWndRect();
			if(!Rect.PtInRect(CEasyPoint(m_pGUI->GetCurMouseX(),m_pGUI->GetCurMouseY())))
			{
				m_IsListShow=false;
				m_pComboList->SetVisible(m_IsListShow);				
			}
			return true;
		}
		break;
	case WM_D3DGUI_SIMPLE_LIST_SELCHANGE:
		if(wParam==CIC_LIST)
		{
			int Index=m_pComboList->GetFirstSelectedItem();
			if(Index>=0)
			{
				CEasyString Text;
				m_pComboList->GetItemText(Index,Text);
				SetText(Text);
				HandleMessage(this,WM_D3DGUI_COMBO_SELCHANGE,(WPARAM)GetID(),Index);
			}
			return true;
		}
		break;
	case WM_D3DGUI_SIMPLE_LIST_LDBCLICK:
		if(wParam==CIC_LIST)
		{
			m_IsListShow=false;
			m_pComboList->SetVisible(m_IsListShow);		
		}
		break;
	}
	return false;
}

void CD3DCombo::SetVisible(bool IsVisible)
{
	CD3DWnd::SetVisible(IsVisible);
	if(IsVisible)
		m_pComboList->SetVisible(m_IsListShow);
	else
		m_pComboList->SetVisible(false);
}

CEasyRect CD3DCombo::GetCenterRect()
{
	CEasyRect rect=m_WndRect;

	rect.top+=m_Borders[RECT_TOP];
	rect.bottom-=m_Borders[RECT_BOTTOM];
	rect.left+=m_Borders[RECT_LEFT];
	rect.right-=m_Borders[RECT_RIGHT];

	if(m_pScrollBar&&m_IsMultiLine)
		rect.right-=m_ScrollBarWidth;

	rect.right-=rect.Height();
	return rect;
}


void CD3DCombo::UpdateRects()
{
	if(!m_IsVisible)
		return;

	CD3DEdit::UpdateRects();

	
	CEasyRect Rect=GetClientRect();		

	Rect.left=Rect.right;	

	if(m_pScrollBar&&m_IsMultiLine)
		Rect.left+=m_ScrollBarWidth;

	Rect.right=Rect.left+Rect.Height();

	m_pExpandButton->SetRect(Rect);
}


void CD3DCombo::SelectItem(int Index)
{
	m_pComboList->ClearAllSelect();
	m_pComboList->SelectItem(Index);
	Index=m_pComboList->GetFirstSelectedItem();
	if(Index>=0)
	{
		CEasyString Text;
		m_pComboList->GetItemText(Index,Text);
		SetText(Text);
	}
}

int CD3DCombo::AddItem(LPCTSTR ItemText)
{
	return m_pComboList->AddItem(ItemText);
}

bool CD3DCombo::InsertItem(int Index,LPCTSTR ItemText)
{
	return m_pComboList->InsertItem(Index,ItemText);
}

bool CD3DCombo::DeleteItem(int Index)
{
	return m_pComboList->DeleteItem(Index);
}

void CD3DCombo::DeleteAllItem()
{
	m_pComboList->DeleteAllItem();
}

int CD3DCombo::GetItemCount()
{
	return m_pComboList->GetItemCount();
}
bool CD3DCombo::SetItemText(int Index,LPCTSTR Text)
{
	return m_pComboList->SetItemText(Index,Text);
}
bool CD3DCombo::GetItemText(int Index,CEasyString& Text)
{
	return m_pComboList->GetItemText(Index,Text);
}
bool CD3DCombo::SetItemData(int Index,LPVOID pData)
{
	return m_pComboList->SetItemData(Index,pData);
}
LPVOID CD3DCombo::GetItemData(int Index)
{
	return m_pComboList->GetItemData(Index);
}

int CD3DCombo::GetFirstSelectedItem()
{
	return m_pComboList->GetFirstSelectedItem();
}
int CD3DCombo::GetNextSelectedItem(int Index)
{
	return m_pComboList->GetNextSelectedItem(Index);
}

void CD3DCombo::SaveToXml(xml_node * pXMLNode)
{
	xml_node Wnd=pXMLNode->append_child(node_element,"Combo");
	Wnd.append_attribute("Name",(LPCTSTR)GetName());
	Wnd.append_attribute("ID",(long)GetID());
	Wnd.append_attribute("IsInternal",IsInternal());

	xml_node Behavior=Wnd.append_child(node_element,"Behavior");
	SaveBehaviorToXML(Behavior);
	Behavior.append_attribute("IsMultiLine",m_IsMultiLine);
	Behavior.append_attribute("ShowCaret",m_IsShowCaret);
	Behavior.append_attribute("CaretColor",(long)m_CaretColor);
	Behavior.append_attribute("AutoWrap",m_AutoWrap);
	Behavior.append_attribute("ReadOnly",m_IsReadyOnly);
	Behavior.append_attribute("Encryption",m_IsEncryption);
	Behavior.append_attribute("EnableScrollBar",m_IsEnabledScrollBar);
	Behavior.append_attribute("ScrollBarWidth",(long)m_ScrollBarWidth);

	xml_node Frame=Wnd.append_child(node_element,"Frame");
	SaveFrameToXML(Frame);
	xml_node Borders=Wnd.append_child(node_element,"Borders");
	SaveBorderToXML(Borders);
	xml_node Text=Wnd.append_child(node_element,"Text");
	SaveTextToXML(Text);
	xml_node Font=Wnd.append_child(node_element,"Font");
	SaveFontToXML(Font);
	if(m_pTexture)
	{	
		xml_node Texture=Wnd.append_child(node_element,"Texture");
		SaveTextureToXML(Texture);
	}

	if(m_ChildWndList.GetCount())
	{
		xml_node Childs=Wnd.append_child(node_element,"Childs");
		SaveChildsToXml(Childs);
	}
}

bool CD3DCombo::LoadFromXml(xml_node * pXMLNode)
{
	if(_strnicmp(pXMLNode->name(),"Combo",7)!=0)
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
			if(pXMLNode->child(i).has_attribute("IsMultiLine"))
				SetMultiLine((bool)pXMLNode->child(i).attribute("IsMultiLine"));
			if(pXMLNode->child(i).has_attribute("ShowCaret"))
				EnableCaret((bool)pXMLNode->child(i).attribute("ShowCaret"));
			if(pXMLNode->child(i).has_attribute("CaretColor"))
				SetCaretColor((long)pXMLNode->child(i).attribute("CaretColor"));
			if(pXMLNode->child(i).has_attribute("AutoWrap"))
				EnableAutoWrap((bool)pXMLNode->child(i).attribute("AutoWrap"));
			if(pXMLNode->child(i).has_attribute("ReadOnly"))
				SetReadOnly((bool)pXMLNode->child(i).attribute("ReadOnly"));
			if(pXMLNode->child(i).has_attribute("Encryption"))
				SetEncryption((bool)pXMLNode->child(i).attribute("Encryption"));
			if(pXMLNode->child(i).has_attribute("EnableScrollBar"))
				EnableScrollBar((bool)pXMLNode->child(i).attribute("EnableScrollBar"));
			if(pXMLNode->child(i).has_attribute("ScrollBarWidth"))
				SetScrollBarWidth((long)pXMLNode->child(i).attribute("ScrollBarWidth"));
		}
		else if(_strnicmp(pXMLNode->child(i).name(),"Frame",6)==0)
		{
			LoadFrameFromXML(pXMLNode->child(i));
		}
		else if(_strnicmp(pXMLNode->child(i).name(),"Borders",8)==0)
		{
			LoadBorderFromXML(pXMLNode->child(i));
		}
		else if(_strnicmp(pXMLNode->child(i).name(),"Text",5)==0)
		{
			LoadTextFromXML(pXMLNode->child(i));
		}
		else if(_strnicmp(pXMLNode->child(i).name(),"Font",5)==0)
		{
			LoadFontFromXML(pXMLNode->child(i));
		}
		else if(_strnicmp(pXMLNode->child(i).name(),"Texture",8)==0)
		{
			LoadTextureFromXML(pXMLNode->child(i));
		}


	}
	HandleMessage(this,WM_D3DGUI_WND_LOADED,GetID(),(LPARAM)this);

	//装载子窗口
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
			m_ChildWndList[i]->IsKindOf(GET_CLASS_INFO(CD3DScrollBar))&&
			(strcmp(m_ChildWndList[i]->GetName(),"ED_ScrollBar")==0)&&
			m_ChildWndList[i]!=m_pScrollBar)
		{
			CD3DScrollBar * pScrollBar=(CD3DScrollBar *)m_ChildWndList[i];
			SAFE_RELEASE(m_pScrollBar);
			m_pScrollBar=pScrollBar;
		}
		else if(m_ChildWndList[i]->IsInternal()&&
			m_ChildWndList[i]->IsKindOf(GET_CLASS_INFO(CD3DButton))&&
			(strcmp(m_ChildWndList[i]->GetName(),"ComboEB")==0)&&
			m_ChildWndList[i]!=m_pExpandButton)
		{
			CD3DButton * pButton=(CD3DButton *)m_ChildWndList[i];
			SAFE_RELEASE(m_pExpandButton);
			m_pExpandButton=pButton;
		}
		else if(m_ChildWndList[i]->IsInternal()&&
			m_ChildWndList[i]->IsKindOf(GET_CLASS_INFO(CD3DSimpleList))&&
			(strcmp(m_ChildWndList[i]->GetName(),"ComboList")==0)&&
			m_ChildWndList[i]!=m_pComboList)
		{
			CD3DSimpleList * pWnd=(CD3DSimpleList *)m_ChildWndList[i];
			SAFE_RELEASE(m_pComboList);
			m_pComboList=pWnd;
			m_pComboList->SetVisible(false);
			m_ComboListOrginHeight=m_pComboList->GetWndRect().Height();
		}
		
	}	
	TopChild(true);
	HandleMessage(this,WM_D3DGUI_CHILD_LOADED,GetID(),(LPARAM)this);
	
	
	return true;
}

/////////////////////////////////////////


void CD3DCombo::GetMiniSize(int& Width,int& Height)
{
	//检测窗口大小
	int LMinWidth,RMinWidth;

	LMinWidth=m_Borders[RECT_LEFT];
	if(LMinWidth<m_Borders[RECT_TOP_LEFT])
		LMinWidth=m_Borders[RECT_TOP_LEFT];
	if(LMinWidth<m_Borders[RECT_BOTTOM_LEFT])
		LMinWidth=m_Borders[RECT_BOTTOM_LEFT];

	RMinWidth=m_Borders[RECT_RIGHT];
	if(RMinWidth<m_Borders[RECT_TOP_RIGHT])
		RMinWidth=m_Borders[RECT_TOP_RIGHT];
	if(RMinWidth<m_Borders[RECT_BOTTOM_RIGHT])
		RMinWidth=m_Borders[RECT_BOTTOM_RIGHT];

	CEasyRect Rect=GetCenterRect();
	
	int MinWidth,MinHeight;
	m_pExpandButton->GetMiniSize(MinWidth,MinHeight);

	if(m_pScrollBar&&m_IsMultiLine)
		Width=LMinWidth+RMinWidth+MinWidth+Rect.Height()+m_ScrollBarWidth;
	else
		Width=LMinWidth+RMinWidth+MinWidth+Rect.Height();
	Height=m_Borders[RECT_TOP]+m_Borders[RECT_BOTTOM]+MinHeight;
	

}

}