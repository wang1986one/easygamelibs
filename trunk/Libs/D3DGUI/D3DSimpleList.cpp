/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DSimpleList.cpp                                        */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include ".\d3dsimplelist.h"

namespace D3DGUI{

#define LINE_GROW	8

static LPCTSTR SELECTED_RECT_FX=	
	"texture TexLay0 < string name = \"test.jpg\"; >;"
	"texture TexLay1 < string name = \"test1.jpg\"; >;"
	"technique tec0"
	"{"
	"    pass p0"
	"    {"
	"		MultiSampleAntialias = FALSE;"
	"		Lighting=false;"
	"		zenable = false;"
	"		zwriteenable = false;"
	"		CullMode = ccw;"
	"		fogenable = false;"	
	"		AlphaTestEnable = false;"
	"		AlphaBlendEnable = true;"
	"		SrcBlend = InvDestColor;"
	"		DestBlend = InvDestColor;"		
	"     	ColorOp[0] = SelectArg1;"	
	"       ColorArg1[0] = Diffuse;"      	
	"       AlphaOp[0] = SelectArg1;"	
	"       AlphaArg1[0] = diffuse;"
	"		ColorOp[1] = disable;"
	"		AlphaOp[1] = disable;"	
	"    }"
	"}";


IMPLEMENT_CLASS_INFO(CD3DSimpleList,CD3DWnd);

CD3DSimpleList::CD3DSimpleList():CD3DWnd()
{
	m_pScrollBar=NULL;	
	InitWnd(NULL);
}

CD3DSimpleList::CD3DSimpleList(CD3DGUI * pGUI):CD3DWnd(pGUI)
{	
	m_pScrollBar=NULL;	
	InitWnd(pGUI);	
}

CD3DSimpleList::~CD3DSimpleList(void)
{
}

void CD3DSimpleList::InitWnd(CD3DGUI *  pGUI)
{
	m_LineSpace=0;
	m_LineHeight=17;
	m_FirstVisibleLine=0;


	SAFE_DELETE_ARRAY(m_pWndRects);
	m_WndRectCount=RECT_TEXT+LINE_GROW*6;
	m_pWndRects=new LPIBASERECT[m_WndRectCount];
	ZeroMemory(m_pWndRects,sizeof(LPIBASERECT)*m_WndRectCount);
	m_TextRectStart=RECT_TEXT;
	m_TextRectCount=LINE_GROW*3;
	m_SelectRectStart=m_TextRectStart+m_TextRectCount;
	m_SelectRectCount=LINE_GROW*3;



	m_ClientCurWidth=0;
	m_ClientCurHeight=0;

	m_ScrollBarWidth=16;

	

	m_IsControlPress=false;
	m_IsShiftPress=false;
	m_ShiftSelectStart=0;
	m_RecentSelect=-1;
	m_AllowMutliSelect=false;


	m_FontAlign=DT_LEFT|DT_VCENTER;

	m_CanDrag=false;

	SAFE_RELEASE(m_pScrollBar);

	if(m_pGUI)
	{
		m_pScrollBar=m_pGUI->CreateScrollBar(CEasyRect(0,0,0,0));
		m_pScrollBar->SetParent(this);
		m_pScrollBar->SetVisible(false);
		m_pScrollBar->SetName("SL_ScrollBar");
		m_pScrollBar->SetInternal(true);
		m_pScrollBar->EnableFocus(false);
	}

	
}


BOOL CD3DSimpleList::OnMessage(CD3DWnd * pWnd,UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	if(CD3DWnd::OnMessage(pWnd,msg,wParam,lParam))	
		return true;

	switch(msg)
	{
	case WM_KEYDOWN:
		{			
			switch(wParam)
			{
			case VK_CONTROL:
				m_IsControlPress=true;
				return true;
			case VK_SHIFT:
				if(!m_IsShiftPress)
				{
					m_IsShiftPress=true;
					m_ShiftSelectStart=m_RecentSelect;
				}				
				return true;
			case VK_UP:
				{
					int Select=GetFirstSelectedItem();
					if(Select>0)
					{
						ClearAllSelect();
						Select--;
						SelectItem(Select);
						MakeItemVisible(Select);
						HandleMessage(this,WM_D3DGUI_SIMPLE_LIST_SELCHANGE,(WPARAM)GetID(),Select);
						m_RecentSelect=Select;
					}
				}
				return true;
			case VK_DOWN:
				{
					int Select=GetFirstSelectedItem();
					if(Select<GetItemCount()-1)
					{
						ClearAllSelect();
						Select++;
						SelectItem(Select);
						MakeItemVisible(Select);
						HandleMessage(this,WM_D3DGUI_SIMPLE_LIST_SELCHANGE,(WPARAM)GetID(),Select);
						m_RecentSelect=Select;
					}
				}
				return true;
			}
		}
		break;
	case WM_KEYUP:
		{			
			switch(wParam)
			{
			case VK_CONTROL:
				m_IsControlPress=false;
				return true;
			case VK_SHIFT:
				m_IsShiftPress=false;
				m_ShiftSelectStart=0;
				return true;			
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{		
			int xPos=LOWORD(lParam);
			int yPos=HIWORD(lParam);
			int index=XYToItemIndex(xPos,yPos);
			if(index>=0)
			{
				if(m_IsShiftPress)
				{
					ClearAllSelect();
					SelectItemRange(m_ShiftSelectStart,index);
				}
				else
				{
					if(!m_IsControlPress&&!m_AllowMutliSelect)
						ClearAllSelect();
					SelectItem(index);
				}
				if(m_RecentSelect!=index)
				{					
					HandleMessage(this,WM_D3DGUI_SIMPLE_LIST_SELCHANGE,(WPARAM)GetID(),index);
					m_RecentSelect=index;
				}
				
			}
		}
		return true;
	case WM_LBUTTONUP:
		{
			int xPos=LOWORD(lParam);
			int yPos=HIWORD(lParam);
			int index=XYToItemIndex(xPos,yPos);
			if(index>=0)
			{
				HandleMessage(this,WM_D3DGUI_SIMPLE_LIST_LCLICK,(WPARAM)GetID(),index);
			}
		}
		return true;
	case WM_LBUTTONDBLCLK:
		{
			int xPos=LOWORD(lParam);
			int yPos=HIWORD(lParam);
			int index=XYToItemIndex(xPos,yPos);
			if(index>=0)
			{
				HandleMessage(this,WM_D3DGUI_SIMPLE_LIST_LDBCLICK,(WPARAM)GetID(),index);
			}
		}
		return true;
	case WM_MOUSEWHEEL:
		{
			int Delta = -GET_WHEEL_DELTA_WPARAM(wParam)/WHEEL_DELTA;
			Scroll(Delta);
		}
		return true;
	case WM_D3DGUI_SCROLL_BAR_SCROLL:
		ScrollTo((int)lParam);		
		return true;
	}	
	return false;
}

CEasyRect CD3DSimpleList::GetCenterRect()
{
	CEasyRect rect=m_WndRect;

	rect.top+=m_Borders[RECT_TOP];
	rect.bottom-=m_Borders[RECT_BOTTOM];
	rect.left+=m_Borders[RECT_LEFT];
	rect.right-=m_Borders[RECT_RIGHT];

	if(m_pScrollBar->IsVisible())
		rect.right-=m_ScrollBarWidth;
	return rect;
}

void CD3DSimpleList::SetFont(LOGFONT * pLogFont)
{
	if(pLogFont)
	{
		m_LogFont=*pLogFont;
		m_LineHeight=(m_LogFont.lfHeight+m_FontShadowWidth*2+1)*m_FontScale;
		m_WantUpdateFont=true;	
		UpdateFont();
	}
}

void CD3DSimpleList::SetFontAlign(DWORD Align)
{
	m_FontAlign=Align;
	m_WantUpdateFont=true;
	UpdateFont();
}

void CD3DSimpleList::SetFontSahdowWidth(DWORD ShadowWidth)
{
	m_FontShadowWidth=ShadowWidth;
	m_LineHeight=(m_LogFont.lfHeight+m_FontShadowWidth*2+1)*m_FontScale;
	m_WantUpdateFont=true;
	UpdateFont();
}

void CD3DSimpleList::SetFontScale(FLOAT Scale)
{
	m_FontScale=Scale;
	m_LineHeight=(m_LogFont.lfHeight+m_FontShadowWidth*2+1)*m_FontScale;
	m_WantUpdateFont=true;
	UpdateFont();
}

void CD3DSimpleList::SetVisible(bool IsVisible)
{
	CD3DWnd::SetVisible(IsVisible);	
	CheckScroll();
	if(IsVisible)
		UpdateText();
}

void CD3DSimpleList::GetMiniSize(int& Width,int& Height)
{
	//��ⴰ�ڴ�С
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

	Width=LMinWidth+RMinWidth+m_ScrollBarWidth;
	if(m_pScrollBar->IsVisible())
	{
		int MinWidth,MinHeight;
		m_pScrollBar->GetMiniSize(MinWidth,MinHeight);
		Height=m_Borders[RECT_TOP]+m_Borders[RECT_BOTTOM]+MinHeight;
	}
	else
	{
		Height=m_Borders[RECT_TOP]+m_Borders[RECT_BOTTOM];
	}

}


void CD3DSimpleList::UpdateRects()
{
	if(!m_IsVisible)
		return;

	CEasyRect ClientRect=GetClientRect();	
	ClientToScreen(&ClientRect);


	//������Ҫ�Ŀ�������
	m_CurVisibleLineCount=(int)floor((FLOAT)ClientRect.Height()/(m_LineHeight+m_LineSpace));

	//������Ҫ�������־�����
	if(m_CurVisibleLineCount>GetTextRectCount())
		GrowTextRect(m_CurVisibleLineCount-GetTextRectCount()+LINE_GROW);

	CheckScroll();

	CD3DWnd::UpdateRects();

	if(m_pScrollBar->IsVisible())
	{
		CEasyRect Rect=GetClientRect();		
		Rect.left=Rect.right;
		Rect.right+=m_ScrollBarWidth;
		m_pScrollBar->SetRect(Rect);
	}
}

void CD3DSimpleList::UpdateText()
{
	if(!m_IsVisible)
		return;

	CEasyRect ClientRect=GetClientRect();	
	ClientToScreen(&ClientRect);

	FLOAT_RECT Rect(ClientRect);
	

	
	for(int i=0;i<GetTextRectCount();i++)
	{
		if(TextRect(i))
			TextRect(i)->SetVisible(false);
	}		

	
	//��������
	for(int i=0;i<(int)m_LineInfos.GetCount()-m_FirstVisibleLine;i++)
	{
		
		if(TextRect(i)==NULL)
		{
			TextRect(i)=CreateTextRect();
			RebuildOrder();			
		}			
		if(TextRect(i))
		{
			Rect.bottom=Rect.top+m_LineHeight;
			if(Rect.bottom<=(FLOAT)ClientRect.bottom)
			{
				TextRect(i)->EnableUpdate(false);
				TextRect(i)->SetRect(&Rect);
				
				TextRect(i)->SetTextW((LPCWSTR)m_LineInfos[i+m_FirstVisibleLine].Text,
					m_LineInfos[i+m_FirstVisibleLine].Text.GetLength());
				TextRect(i)->EnableUpdate(true);

				Rect.top=Rect.bottom+m_LineSpace;	
				TextRect(i)->SetVisible(true);
			}
			else
				break;

		}				
		
	}
	m_ClientCurWidth=ClientRect.Width();
	m_ClientCurHeight=ClientRect.Height();
	UpdateSelect();	
}

void CD3DSimpleList::UpdateSelect()
{
	if(!m_IsVisible)
		return;	


	for(int i=0;i<GetSelectRectCount();i++)
	{
		if(SelectRect(i))
			SelectRect(i)->SetVisible(false);
	}	

	CEasyRect ClientRect=GetClientRect();	
	ClientToScreen(&ClientRect);

	FLOAT_RECT Rect(ClientRect);

	for(int i=0;i<(int)m_LineInfos.GetCount()-m_FirstVisibleLine;i++)
	{			
		Rect.bottom=Rect.top+m_LineHeight;
		if(Rect.bottom<=(FLOAT)ClientRect.bottom)
		{
			if(m_LineInfos[i+m_FirstVisibleLine].IsSelected)
			{
				if(SelectRect(i)==NULL)
				{
					SelectRect(i)=CreateRect();
					RebuildOrder();			
					SelectRect(i)->SetEffectMode(D3DGUI_EFFECT_INVERSE_COLOR);
				}
				if(SelectRect(i))
				{
					SelectRect(i)->SetRect(&Rect);
					SelectRect(i)->SetVisible(true);
				}
			}
			Rect.top=Rect.bottom+m_LineSpace;	
		}
		else
			break;
	}
}


void CD3DSimpleList::SetScrollBarWidth(int Width)
{
	m_ScrollBarWidth=Width;
	UpdateRects();
}

//void CD3DSimpleList::EnableScrollBar(bool Enable)
//{
//	if(Enable)
//	m_pScrollBar->SetVisible(true);
//	UpdateRects();		
//}


void CD3DSimpleList::ScrollTo(int Line)
{
	m_FirstVisibleLine=Line;

	if(m_FirstVisibleLine>(int)m_LineInfos.GetCount()-m_CurVisibleLineCount)
		m_FirstVisibleLine=(int)m_LineInfos.GetCount()-m_CurVisibleLineCount;
	if(m_FirstVisibleLine<0)
		m_FirstVisibleLine=0;	

	if(m_pScrollBar)
	{
		if(m_pScrollBar->GetScrollPos()!=m_FirstVisibleLine)
			m_pScrollBar->SetScrollPos(m_FirstVisibleLine);
	}
	UpdateText();
}

int CD3DSimpleList::XYToItemIndex(int x,int y)
{
	for(int i=0;i<GetTextRectCount();i++)
	{
		if(TextRect(i))
		{
			if(TextRect(i)->IsVisible())
			{
				FLOAT_RECT Rect=TextRect(i)->GetRect();
				if(Rect.IsIn((FLOAT)x,(FLOAT)y))
				{
					return i+m_FirstVisibleLine;
				}
			}
		}
	}
	return -1;
}

void CD3DSimpleList::SelectItem(int Index)
{
	if(!m_AllowMutliSelect)
		ClearAllSelect();
	if(Index>=0&&Index<(int)m_LineInfos.GetCount())
	{
		m_LineInfos[Index].IsSelected=true;		
		UpdateSelect();
	}	
}

void CD3DSimpleList::SelectItemRange(int Start,int End)
{
	if(!m_AllowMutliSelect)
	{
		SelectItem(End);
		return;
	}

	if(Start>End)
	{
		int temp=End;
		End=Start;
		Start=temp;
	}
	if(Start<0)
		Start=0;
	if(End>=(int)m_LineInfos.GetCount())
		End=(int)m_LineInfos.GetCount()-1;
	
	for(int i=Start;i<=End;i++)
	{
		m_LineInfos[i].IsSelected=true;
	}
	UpdateSelect();
}

void CD3DSimpleList::ClearAllSelect()
{
	for(int i=0;i<(int)m_LineInfos.GetCount();i++)
	{
		m_LineInfos[i].IsSelected=false;
	}
	UpdateSelect();
}

int CD3DSimpleList::AddItem(LPCTSTR ItemText)
{
	SIMPLE_LIST_LINE_INFO Info;
	Info.Text=ItemText;
	m_LineInfos.Add(Info);
	CheckScroll();
	UpdateText();
	return (int)m_LineInfos.GetCount()-1;
}

bool CD3DSimpleList::InsertItem(int Index,LPCTSTR ItemText)
{
	SIMPLE_LIST_LINE_INFO Info;
	Info.Text=ItemText;
	
	if(m_LineInfos.Insert(Index,Info))
	{
		CheckScroll();
		UpdateText();
		return true;
	}
	else
	{
		return false;
	}	
}

bool CD3DSimpleList::DeleteItem(int Index)
{
	if(m_LineInfos.Delete(Index))
	{
		CheckScroll();
		UpdateText();
		return true;
	}
	return false;
}

void CD3DSimpleList::DeleteAllItem()
{
	m_LineInfos.Clear();
	CheckScroll();
	UpdateText();
}

int CD3DSimpleList::GetItemCount()
{
	return (int)m_LineInfos.GetCount();
}

int CD3DSimpleList::GetFirstSelectedItem()
{
	for(int i=0;i<(int)m_LineInfos.GetCount();i++)
	{
		if(m_LineInfos[i].IsSelected)
			return i;
	}
	return -1;
}

int CD3DSimpleList::GetNextSelectedItem(int Index)
{
	for(int i=Index+1;i<(int)m_LineInfos.GetCount();i++)
	{
		if(m_LineInfos[i].IsSelected)
			return i;
	}
	return -1;
}

bool CD3DSimpleList::SetItemText(int Index,LPCTSTR Text)
{
	if(Index>=0&&Index<(int)m_LineInfos.GetCount())
	{
		m_LineInfos[Index].Text=Text;
		return true;
	}
	return false;
}

bool CD3DSimpleList::GetItemText(int Index,CEasyString& Text)
{
	if(Index>=0&&Index<(int)m_LineInfos.GetCount())
	{		
		Text=m_LineInfos[Index].Text;
		return true;
	}
	return false;
}

bool CD3DSimpleList::SetItemData(int Index,LPVOID pData)
{
	if(Index>=0&&Index<(int)m_LineInfos.GetCount())
	{		
		m_LineInfos[Index].pItemData=pData;
		return true;
	}
	return false;
}

LPVOID CD3DSimpleList::GetItemData(int Index)
{
	if(Index>=0&&Index<(int)m_LineInfos.GetCount())
	{		
		return m_LineInfos[Index].pItemData;
	}
	return NULL;
}

void CD3DSimpleList::SaveToXml(xml_node * pXMLNode)
{
	xml_node Wnd=pXMLNode->append_child(node_element,"SimpleList");
	Wnd.append_attribute("Name",(LPCTSTR)GetName());
	Wnd.append_attribute("ID",(long)GetID());
	Wnd.append_attribute("IsInternal",IsInternal());

	xml_node Behavior=Wnd.append_child(node_element,"Behavior");
	SaveBehaviorToXML(Behavior);
	Behavior.append_attribute("LineSpace",(long)m_LineSpace);
	Behavior.append_attribute("IsMultiSelect",m_AllowMutliSelect);
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

bool CD3DSimpleList::LoadFromXml(xml_node * pXMLNode)
{
	if(_strnicmp(pXMLNode->name(),"SimpleList",11)!=0)
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
			if(pXMLNode->child(i).has_attribute("LineSpace"))
				SetLineSpace((long)pXMLNode->child(i).attribute("LineSpace"));
			if(pXMLNode->child(i).has_attribute("IsMultiSelect"))
				SetAllowMutliSelect((bool)pXMLNode->child(i).attribute("IsMultiSelect"));
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

	//װ���Ӵ���
	for(int i=(int)pXMLNode->children()-1;i>=0;i--)
	{
		if(_strnicmp(pXMLNode->child(i).name(),"Childs",7)==0)
		{
			LoadChildsFromXml(pXMLNode->child(i));
			break;
		}
	}
	//ʶ���ڲ�����
	for(int i=(int)m_ChildWndList.GetCount()-1;i>=0;i--)
	{
		if(m_ChildWndList[i]->IsInternal()&&
			m_ChildWndList[i]->IsKindOf(GET_CLASS_INFO(CD3DScrollBar))&&
			(strcmp(m_ChildWndList[i]->GetName(),"SL_ScrollBar")==0)&&
			m_ChildWndList[i]!=m_pScrollBar)
		{
			CD3DScrollBar * pScrollBar=(CD3DScrollBar *)m_ChildWndList[i];
			SAFE_RELEASE(m_pScrollBar);
			m_pScrollBar=pScrollBar;
		}
	}
	TopChild(true);
	HandleMessage(this,WM_D3DGUI_CHILD_LOADED,GetID(),(LPARAM)this);
	return true;
}

void CD3DSimpleList::MakeItemVisible(int Index)
{
	if(Index<0||Index>=GetItemCount())
		return;

	CEasyRect ClientRect=GetClientRect();	
	ClientToScreen(&ClientRect);
	int TotalVisibleLine=ClientRect.Height()/m_LineHeight;
	if(Index<m_FirstVisibleLine)
	{
		ScrollTo(Index);
	}
	else if(Index>m_FirstVisibleLine+TotalVisibleLine-1)
	{
		ScrollTo(Index-TotalVisibleLine+1);
	}
}
////////////////////////////////////////////////////////
bool CD3DSimpleList::UpdateFont()
{
	if(m_WantUpdateFont)
	{
		for(int i=0;i<GetTextRectCount();i++)
		{			
			if(TextRect(i))
			{
				TextRect(i)->EnableUpdate(false);
				TextRect(i)->SetFont(&m_LogFont);
				TextRect(i)->SetColor(m_FontColor);
				TextRect(i)->SetAlign(m_FontAlign);
				TextRect(i)->SetShadowMode(m_FontShadowMode);
				TextRect(i)->SetShadowColor(m_FontShadowColor);
				TextRect(i)->SetShadowWidth(m_FontShadowWidth);	
				TextRect(i)->SetCharSpace(m_FontCharSpace);	
				TextRect(i)->SetLineSpace(m_FontLineSpace);
				TextRect(i)->SetScale(m_FontScale);
				m_FontCharSpace=TextRect(i)->GetCharSpace();
				m_FontLineSpace=TextRect(i)->GetLineSpace();
				TextRect(i)->EnableUpdate(true);				
				UpdateRects();
			}
		}
		m_WantUpdateFont=false;
		return true;
	}
	return false;
}

void CD3DSimpleList::GrowTextRect(int Grow)
{

	int NewCount=m_WndRectCount+Grow*2;
	

	LPIBASERECT *pNewRects=new LPIBASERECT[NewCount];
	ZeroMemory(pNewRects,sizeof(LPIBASERECT)*NewCount);
	//memcpy(pNewRects,m_pWndRects,sizeof(LPIBASERECT)*(m_WndRectCount-1));
	memcpy(pNewRects,m_pWndRects,sizeof(LPIBASERECT)*m_SelectRectStart);
	memcpy(pNewRects+m_SelectRectStart+Grow,m_pWndRects+m_SelectRectStart,sizeof(LPIBASERECT)*m_SelectRectCount);
	

	SAFE_DELETE_ARRAY(m_pWndRects);
	m_pWndRects=pNewRects;
	m_WndRectCount=NewCount;	

	m_TextRectStart=RECT_TEXT;
	m_TextRectCount+=Grow;
	m_SelectRectStart=m_TextRectStart+m_TextRectCount;
	m_SelectRectCount+=Grow;

}

void CD3DSimpleList::CheckScroll()
{
	if(!m_IsVisible)
		return;

	if((int)m_LineInfos.GetCount()>m_CurVisibleLineCount)
	{
		if(!m_pScrollBar->IsVisible())
		{
			m_pScrollBar->SetVisible(true);
			CEasyRect Rect=GetClientRect();		
			Rect.left=Rect.right;
			Rect.right+=m_ScrollBarWidth;
			m_pScrollBar->SetRect(Rect);
		}
		m_pScrollBar->SetMaxScrollPos((int)m_LineInfos.GetCount()-m_CurVisibleLineCount);
		ScrollTo(m_FirstVisibleLine);
		
	}
	else
	{
		m_FirstVisibleLine=0;
		m_pScrollBar->SetVisible(false);
	}
}

}