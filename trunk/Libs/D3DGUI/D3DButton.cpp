#include "StdAfx.h"
#include "D3DButton.h"
#include "D3DGUI.h"

namespace D3DGUI{

IMPLEMENT_CLASS_INFO(CD3DButton,CD3DWnd);

CD3DButton::CD3DButton():CD3DWnd()
{
	m_CurStatus=BUTTON_STATUS_NORMAL;
	m_CanDrag=false;
	m_CanResize=false;	
}

CD3DButton::CD3DButton(CD3DGUI * pGUI):CD3DWnd(pGUI)
{
	m_CurStatus=BUTTON_STATUS_NORMAL;
	m_CanDrag=false;
	m_CanResize=false;	
}

CD3DButton::~CD3DButton(void)
{
	Destory();	
}

void CD3DButton::Destory()
{
	for(int i=0;i<BUTTON_STATUS_MAX;i++)
	{
		SAFE_RELEASE(m_pStatusInfo[i].pTexture);
	}
	m_pTexture=NULL;
	CD3DWnd::Destory();
}

BOOL CD3DButton::OnMessage(CD3DWnd * pWnd,UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(CD3DWnd::OnMessage(pWnd,msg,wParam,lParam))	
		return true;
	switch(msg)
	{	
	case WM_LBUTTONDOWN:
		m_CurStatus=BUTTON_STATUS_PRESS;
		UpdateTextureRect();
		HandleMessage(this,WM_D3DGUI_BUTTON_DOWN,(WPARAM)GetID(),lParam);
		break;
	case WM_LBUTTONUP:	
		m_CurStatus=BUTTON_STATUS_ACTIVE;
		UpdateTextureRect();
		HandleMessage(this,WM_D3DGUI_BUTTON_UP,(WPARAM)GetID(),lParam);
		HandleMessage(this,WM_D3DGUI_BUTTON_CLICK,(WPARAM)GetID(),lParam);
		break;
	case WM_D3DGUI_MOUSE_ENTER:
		m_CurStatus=BUTTON_STATUS_ACTIVE;
		UpdateTextureRect();
		break;
	case WM_D3DGUI_MOUSE_LEAVE:
		if(m_CurStatus==BUTTON_STATUS_PRESS)
			HandleMessage(this,WM_D3DGUI_BUTTON_UP,(WPARAM)GetID(),lParam);
		m_CurStatus=BUTTON_STATUS_NORMAL;
		UpdateTextureRect();
		break;
	}
	return false;
	
}

bool CD3DButton::SetTexture(int Status,LPCTSTR TextureFileName,RECT& TextureRect,FLOAT_POINT TextOffset)
{
	if(Status<0||Status>=BUTTON_STATUS_MAX)
		return false;
	IUITexture * pTexture=m_pGUI->GetObjectCreator()->CreateTextureFromFile(TextureFileName);
	if(pTexture)
	{	
		SAFE_RELEASE(m_pStatusInfo[Status].pTexture);
		m_pStatusInfo[Status].pTexture=pTexture;
		m_pStatusInfo[Status].TextRect=TextureRect;		
		m_pStatusInfo[Status].TextOffset=TextOffset;				
		m_pTexture=pTexture;
		CaculateUV(Status);
		return UpdateTextureRect();
	}

	return false;
}

bool CD3DButton::GetTexture(int Status,CEasyString& TextureFileName,RECT& TextureRect,FLOAT_POINT& TextOffset)
{
	if(Status<0||Status>=BUTTON_STATUS_MAX)
		return false;
	if(m_pStatusInfo[Status].pTexture)
		TextureFileName=m_pStatusInfo[Status].pTexture->GetName();
	TextureRect=m_pStatusInfo[Status].TextRect;
	TextOffset=m_pStatusInfo[Status].TextOffset;
	return true;
}

void CD3DButton::SetBorders(WIN_BORDERS& Borders)
{
	memcpy(m_Borders,Borders,sizeof(WIN_BORDERS));
	for(int i=0;i<BUTTON_STATUS_MAX;i++)
		CaculateUV(i);
	UpdateRects();
	UpdateTextureRect();
}


void CD3DButton::SetButtonStatus(int Status)
{
	m_CurStatus=Status;
	UpdateTextureRect();
}

void CD3DButton::EnableWindow(bool IsEnable)
{
	m_IsEnable=IsEnable;
	if(m_IsEnable)
		m_CurStatus=BUTTON_STATUS_NORMAL;
	else
		m_CurStatus=BUTTON_STATUS_DISBALE;
	UpdateTextureRect();
}



bool CD3DButton::UpdateTextureRect()
{	
	if(m_pTexture==NULL)
		return false;	

	for(int i=0;i<RECT_TEXT;i++)
	{
		if(m_Borders[i])
		{
			FLOAT_RECT UVRect;
			if(m_pWndRects[i]==NULL)
			{
				m_pWndRects[i]=CreateRect();
				RebuildOrder();				
			}
			((IUIWndRect *)m_pWndRects[i])->SetTexture(m_pStatusInfo[m_CurStatus].pTexture);
			((IUIWndRect *)m_pWndRects[i])->SetUV(&(m_pStatusInfo[m_CurStatus].UVRect[i]));
		}
	}
	UpdateText();
	return true;
}

void CD3DButton::UpdateText()
{
	if(!m_IsVisible)
		return;

	CEasyRect r=GetClientRect();
	ClientToScreen(&r);

	FLOAT_RECT Rect=r;

	Rect.left+=m_pStatusInfo[m_CurStatus].TextOffset.x;
	Rect.right+=m_pStatusInfo[m_CurStatus].TextOffset.x;
	Rect.top+=m_pStatusInfo[m_CurStatus].TextOffset.y;
	Rect.bottom+=m_pStatusInfo[m_CurStatus].TextOffset.y;

	if(!m_WndText.IsEmpty())
	{
		if(m_pWndRects[RECT_TEXT]==NULL)
		{
			m_pWndRects[RECT_TEXT]=m_pGUI->GetObjectCreator()->CreateTextRect(&Rect,&m_LogFont);
			RebuildOrder();
			if(m_pWndRects[RECT_TEXT])
			{
				m_FontAlign=DT_CENTER|DT_VCENTER;
				((IUITextRect *)m_pWndRects[RECT_TEXT])->EnableUpdate(false);
				((IUITextRect *)m_pWndRects[RECT_TEXT])->SetFont(&m_LogFont);
				((IUITextRect *)m_pWndRects[RECT_TEXT])->SetColor(m_FontColor);
				((IUITextRect *)m_pWndRects[RECT_TEXT])->SetAlign(m_FontAlign);
				((IUITextRect *)m_pWndRects[RECT_TEXT])->SetShadowMode(m_FontShadowMode);
				((IUITextRect *)m_pWndRects[RECT_TEXT])->SetShadowColor(m_FontShadowColor);
				((IUITextRect *)m_pWndRects[RECT_TEXT])->SetShadowWidth(m_FontShadowWidth);
				((IUITextRect *)m_pWndRects[RECT_TEXT])->EnableUpdate(true);
			}
		}
	}
	if(m_pWndRects[RECT_TEXT])
	{
		m_pWndRects[RECT_TEXT]->SetRect(&Rect);		
	}
}

void CD3DButton::SaveToXml(xml_node * pXMLNode)
{
	xml_node Wnd=pXMLNode->append_child(node_element,"Button");
	Wnd.append_attribute("Name",(LPCTSTR)GetName());
	Wnd.append_attribute("ID",(long)GetID());
	Wnd.append_attribute("IsInternal",IsInternal());

	xml_node Behavior=Wnd.append_child(node_element,"Behavior");
	SaveBehaviorToXML(Behavior);
	xml_node Frame=Wnd.append_child(node_element,"Frame");
	SaveFrameToXML(Frame);
	xml_node Borders=Wnd.append_child(node_element,"Borders");
	SaveBorderToXML(Borders);
	xml_node Text=Wnd.append_child(node_element,"Text");
	SaveTextToXML(Text);
	xml_node Font=Wnd.append_child(node_element,"Font");
	SaveFontToXML(Font);
	xml_node Texture=Wnd.append_child(node_element,"Texture");
	SaveTextureToXML(Texture);
	
	if(m_ChildWndList.size()>0)
	{
		xml_node Childs=Wnd.append_child(node_element,"Childs");
		SaveChildsToXml(Childs);
	}

}

bool CD3DButton::LoadFromXml(xml_node * pXMLNode)
{
	if(_strnicmp(pXMLNode->name(),"Button",7)!=0)
		return false;
	if(pXMLNode->has_attribute("Name"))
		SetName(pXMLNode->attribute("Name").getvalue().c_str());

	if(pXMLNode->has_attribute("ID"))
		SetID((long)pXMLNode->attribute("ID"));

	if(pXMLNode->has_attribute("IsInternal"))
		SetInternal((bool)pXMLNode->attribute("IsInternal"));


	for(int i=0;i<(int)pXMLNode->children();i++)
	{
		if(_strnicmp(pXMLNode->child(i).name(),"Behavior",6)==0)
		{
			LoadBehaviorFromXML(pXMLNode->child(i));
		}		
		else if(_strnicmp(pXMLNode->child(i).name(),"Frame",6)==0)
		{
			LoadFrameFromXML(pXMLNode->child(i));
		}
		else if(_strnicmp(pXMLNode->child(i).name(),"Borders",6)==0)
		{
			LoadBorderFromXML(pXMLNode->child(i));
		}
		else if(_strnicmp(pXMLNode->child(i).name(),"Text",6)==0)
		{
			LoadTextFromXML(pXMLNode->child(i));
		}
		else if(_strnicmp(pXMLNode->child(i).name(),"Font",6)==0)
		{
			LoadFontFromXML(pXMLNode->child(i));
		}
		else if(_strnicmp(pXMLNode->child(i).name(),"Texture",6)==0)
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
	TopChild();
	HandleMessage(this,WM_D3DGUI_CHILD_LOADED,GetID(),(LPARAM)this);
	return true;
}

void CD3DButton::CaculateUV(int Status)
{
	if(m_pStatusInfo[Status].pTexture)
	{
		m_pTexture=m_pStatusInfo[Status].pTexture;
		m_TextureRect=m_pStatusInfo[Status].TextRect;
	
		m_pStatusInfo[Status].UVRect[RECT_CENTER].left=m_pTexture->X2U(m_TextureRect.left+m_Borders[RECT_LEFT]);
		m_pStatusInfo[Status].UVRect[RECT_CENTER].top=m_pTexture->Y2V(m_TextureRect.top+m_Borders[RECT_TOP]);
		m_pStatusInfo[Status].UVRect[RECT_CENTER].right=m_pTexture->X2U(m_TextureRect.right-m_Borders[RECT_RIGHT]);
		m_pStatusInfo[Status].UVRect[RECT_CENTER].bottom=m_pTexture->Y2V(m_TextureRect.bottom-m_Borders[RECT_BOTTOM]);

		m_pStatusInfo[Status].UVRect[RECT_TOP].left=m_pTexture->X2U(m_TextureRect.left+m_Borders[RECT_TOP_LEFT]);
		m_pStatusInfo[Status].UVRect[RECT_TOP].top=m_pTexture->Y2V(m_TextureRect.top);
		m_pStatusInfo[Status].UVRect[RECT_TOP].right=m_pTexture->X2U(m_TextureRect.right-m_Borders[RECT_TOP_RIGHT]);
		m_pStatusInfo[Status].UVRect[RECT_TOP].bottom=m_pTexture->Y2V(m_TextureRect.top+m_Borders[RECT_TOP]);

		m_pStatusInfo[Status].UVRect[RECT_BOTTOM].left=m_pTexture->X2U(m_TextureRect.left+m_Borders[RECT_TOP_LEFT]);
		m_pStatusInfo[Status].UVRect[RECT_BOTTOM].top=m_pTexture->Y2V(m_TextureRect.bottom-m_Borders[RECT_BOTTOM]);
		m_pStatusInfo[Status].UVRect[RECT_BOTTOM].right=m_pTexture->X2U(m_TextureRect.right-m_Borders[RECT_TOP_RIGHT]);
		m_pStatusInfo[Status].UVRect[RECT_BOTTOM].bottom=m_pTexture->Y2V(m_TextureRect.bottom);

		m_pStatusInfo[Status].UVRect[RECT_LEFT].left=m_pTexture->X2U(m_TextureRect.left);
		m_pStatusInfo[Status].UVRect[RECT_LEFT].top=m_pTexture->Y2V(m_TextureRect.top+m_Borders[RECT_TOP]);
		m_pStatusInfo[Status].UVRect[RECT_LEFT].right=m_pTexture->X2U(m_TextureRect.left+m_Borders[RECT_LEFT]);
		m_pStatusInfo[Status].UVRect[RECT_LEFT].bottom=m_pTexture->Y2V(m_TextureRect.bottom-m_Borders[RECT_BOTTOM]);

		m_pStatusInfo[Status].UVRect[RECT_RIGHT].left=m_pTexture->X2U(m_TextureRect.right-m_Borders[RECT_RIGHT]);
		m_pStatusInfo[Status].UVRect[RECT_RIGHT].top=m_pTexture->Y2V(m_TextureRect.top+m_Borders[RECT_TOP]);
		m_pStatusInfo[Status].UVRect[RECT_RIGHT].right=m_pTexture->X2U(m_TextureRect.right);
		m_pStatusInfo[Status].UVRect[RECT_RIGHT].bottom=m_pTexture->Y2V(m_TextureRect.bottom-m_Borders[RECT_BOTTOM]);

		m_pStatusInfo[Status].UVRect[RECT_TOP_LEFT].left=m_pTexture->X2U(m_TextureRect.left);
		m_pStatusInfo[Status].UVRect[RECT_TOP_LEFT].top=m_pTexture->Y2V(m_TextureRect.top);
		m_pStatusInfo[Status].UVRect[RECT_TOP_LEFT].right=m_pTexture->X2U(m_TextureRect.left+m_Borders[RECT_TOP_LEFT]);
		m_pStatusInfo[Status].UVRect[RECT_TOP_LEFT].bottom=m_pTexture->Y2V(m_TextureRect.top+m_Borders[RECT_TOP]);

		m_pStatusInfo[Status].UVRect[RECT_TOP_RIGHT].left=m_pTexture->X2U(m_TextureRect.right-m_Borders[RECT_TOP_RIGHT]);
		m_pStatusInfo[Status].UVRect[RECT_TOP_RIGHT].top=m_pTexture->Y2V(m_TextureRect.top);
		m_pStatusInfo[Status].UVRect[RECT_TOP_RIGHT].right=m_pTexture->X2U(m_TextureRect.right);
		m_pStatusInfo[Status].UVRect[RECT_TOP_RIGHT].bottom=m_pTexture->Y2V(m_TextureRect.top+m_Borders[RECT_TOP]);

		m_pStatusInfo[Status].UVRect[RECT_BOTTOM_LEFT].left=m_pTexture->X2U(m_TextureRect.left);
		m_pStatusInfo[Status].UVRect[RECT_BOTTOM_LEFT].top=m_pTexture->Y2V(m_TextureRect.bottom-m_Borders[RECT_BOTTOM]);
		m_pStatusInfo[Status].UVRect[RECT_BOTTOM_LEFT].right=m_pTexture->X2U(m_TextureRect.left+m_Borders[RECT_BOTTOM_LEFT]);
		m_pStatusInfo[Status].UVRect[RECT_BOTTOM_LEFT].bottom=m_pTexture->Y2V(m_TextureRect.bottom);

		m_pStatusInfo[Status].UVRect[RECT_BOTTOM_RIGHT].left=m_pTexture->X2U(m_TextureRect.right-m_Borders[RECT_BOTTOM_RIGHT]);
		m_pStatusInfo[Status].UVRect[RECT_BOTTOM_RIGHT].top=m_pTexture->Y2V(m_TextureRect.bottom-m_Borders[RECT_BOTTOM]);
		m_pStatusInfo[Status].UVRect[RECT_BOTTOM_RIGHT].right=m_pTexture->X2U(m_TextureRect.right);
		m_pStatusInfo[Status].UVRect[RECT_BOTTOM_RIGHT].bottom=m_pTexture->Y2V(m_TextureRect.bottom);
					
			
	}
}

void CD3DButton::SaveTextureToXML(xml_node& Texture)
{
	xml_node Status;
	if(m_pStatusInfo[BUTTON_STATUS_NORMAL].pTexture)
	{
		Status=Texture.append_child(node_element,"Normal");
		Status.append_attribute("TextureFile",(LPCTSTR)m_pStatusInfo[BUTTON_STATUS_NORMAL].pTexture->GetName());
		Status.append_attribute("RectLeft",m_pStatusInfo[BUTTON_STATUS_NORMAL].TextRect.left);
		Status.append_attribute("RectTop",m_pStatusInfo[BUTTON_STATUS_NORMAL].TextRect.top);
		Status.append_attribute("RectRight",m_pStatusInfo[BUTTON_STATUS_NORMAL].TextRect.right);
		Status.append_attribute("RectBottom",m_pStatusInfo[BUTTON_STATUS_NORMAL].TextRect.bottom);
		Status.append_attribute("TextOffsetX",(long)m_pStatusInfo[BUTTON_STATUS_NORMAL].TextOffset.x);
		Status.append_attribute("TextOffsetY",(long)m_pStatusInfo[BUTTON_STATUS_NORMAL].TextOffset.y);
	}
	if(m_pStatusInfo[BUTTON_STATUS_ACTIVE].pTexture)
	{
		Status=Texture.append_child(node_element,"Active");
		Status.append_attribute("TextureFile",(LPCTSTR)m_pStatusInfo[BUTTON_STATUS_ACTIVE].pTexture->GetName());
		Status.append_attribute("RectLeft",m_pStatusInfo[BUTTON_STATUS_ACTIVE].TextRect.left);
		Status.append_attribute("RectTop",m_pStatusInfo[BUTTON_STATUS_ACTIVE].TextRect.top);
		Status.append_attribute("RectRight",m_pStatusInfo[BUTTON_STATUS_ACTIVE].TextRect.right);
		Status.append_attribute("RectBottom",m_pStatusInfo[BUTTON_STATUS_ACTIVE].TextRect.bottom);
		Status.append_attribute("TextOffsetX",(long)m_pStatusInfo[BUTTON_STATUS_ACTIVE].TextOffset.x);
		Status.append_attribute("TextOffsetY",(long)m_pStatusInfo[BUTTON_STATUS_ACTIVE].TextOffset.y);
	}
	if(m_pStatusInfo[BUTTON_STATUS_PRESS].pTexture)
	{
		Status=Texture.append_child(node_element,"Press");
		Status.append_attribute("TextureFile",(LPCTSTR)m_pStatusInfo[BUTTON_STATUS_PRESS].pTexture->GetName());
		Status.append_attribute("RectLeft",m_pStatusInfo[BUTTON_STATUS_PRESS].TextRect.left);
		Status.append_attribute("RectTop",m_pStatusInfo[BUTTON_STATUS_PRESS].TextRect.top);
		Status.append_attribute("RectRight",m_pStatusInfo[BUTTON_STATUS_PRESS].TextRect.right);
		Status.append_attribute("RectBottom",m_pStatusInfo[BUTTON_STATUS_PRESS].TextRect.bottom);
		Status.append_attribute("TextOffsetX",(long)m_pStatusInfo[BUTTON_STATUS_PRESS].TextOffset.x);
		Status.append_attribute("TextOffsetY",(long)m_pStatusInfo[BUTTON_STATUS_PRESS].TextOffset.y);
	}
	if(m_pStatusInfo[BUTTON_STATUS_DISBALE].pTexture)
	{
		Status=Texture.append_child(node_element,"Disable");
		Status.append_attribute("TextureFile",(LPCTSTR)m_pStatusInfo[BUTTON_STATUS_DISBALE].pTexture->GetName());
		Status.append_attribute("RectLeft",m_pStatusInfo[BUTTON_STATUS_DISBALE].TextRect.left);
		Status.append_attribute("RectTop",m_pStatusInfo[BUTTON_STATUS_DISBALE].TextRect.top);
		Status.append_attribute("RectRight",m_pStatusInfo[BUTTON_STATUS_DISBALE].TextRect.right);
		Status.append_attribute("RectBottom",m_pStatusInfo[BUTTON_STATUS_DISBALE].TextRect.bottom);
		Status.append_attribute("TextOffsetX",(long)m_pStatusInfo[BUTTON_STATUS_DISBALE].TextOffset.x);
		Status.append_attribute("TextOffsetY",(long)m_pStatusInfo[BUTTON_STATUS_DISBALE].TextOffset.y);
	}
}

void CD3DButton::LoadTextureFromXML(xml_node& Texture)
{	
	for(int i=0;i<(int)Texture.children();i++)
	{
		CEasyString TexFile;
		CEasyRect Rect;
		FLOAT_POINT TextOffset;
		TexFile=Texture.child(i).attribute("TextureFile").getvalue().c_str();
		Rect.left=Texture.child(i).attribute("RectLeft");
		Rect.top=Texture.child(i).attribute("RectTop");
		Rect.right=Texture.child(i).attribute("RectRight");
		Rect.bottom=Texture.child(i).attribute("RectBottom");
		TextOffset.x=(double)Texture.child(i).attribute("TextOffsetX");
		TextOffset.y=(double)Texture.child(i).attribute("TextOffsetY");
		if(_strnicmp(Texture.child(i).name(),"Normal",7)==0)
		{
			SetTexture(BUTTON_STATUS_NORMAL,TexFile,Rect,TextOffset);
		}
		else if(_strnicmp(Texture.child(i).name(),"Active",7)==0)
		{
			SetTexture(BUTTON_STATUS_ACTIVE,TexFile,Rect,TextOffset);
		}
		else if(_strnicmp(Texture.child(i).name(),"Press",6)==0)
		{
			SetTexture(BUTTON_STATUS_PRESS,TexFile,Rect,TextOffset);
		}
		else if(_strnicmp(Texture.child(i).name(),"Disable",8)==0)
		{
			SetTexture(BUTTON_STATUS_DISBALE,TexFile,Rect,TextOffset);
		}
		
	}
}

void CD3DButton::PickResource(CNameObjectSet * pObjectSet,UINT Param)
{
	for(int i=0;i<BUTTON_STATUS_MAX;i++)
	{
		if(m_pStatusInfo[i].pTexture)
		{
			m_pStatusInfo[i].pTexture->PickResource(pObjectSet,Param);
		}
	}		

	for(int i=0;i<(int)m_ChildWndList.size();i++)
	{
		m_ChildWndList[i]->PickResource(pObjectSet,Param);
	}	
}

}