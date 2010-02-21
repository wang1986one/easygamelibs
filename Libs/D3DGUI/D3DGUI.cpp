#include "StdAfx.h"
#include "D3DGUI.h"

#if defined(USE_CRT_DETAIL_NEW) && defined(_DEBUG)
#undef new
#endif

#include <fstream>
#include <strstream>

#if defined(USE_CRT_DETAIL_NEW) && defined(_DEBUG)
#define new new( __FILE__, __LINE__ )
#endif

namespace D3DGUI{

IMPLEMENT_CLASS_INFO_STATIC(CD3DGUI,CNameObject);


CD3DGUI::CD3DGUI(IUIObjectCreator * pObjectCreator,HWND hParentWnd):CNameObject()
{
	m_pObjectCreator=pObjectCreator;
	m_ReadyToWork=false;
	m_ActiveWnd=NULL;
	m_RecentMouseWnd=NULL;
	m_CaptureAllWnd=NULL;
	m_hParentWnd=hParentWnd;
}

CD3DGUI::~CD3DGUI(void)
{
	Clear();
}



void CD3DGUI::AddWnd(CD3DWnd *pWnd)
{
	if(m_D3DWndList.Find(pWnd)==NULL)
		m_D3DWndList.InsertBefore(pWnd);
}

CD3DWnd * CD3DGUI::CreateWnd(CEasyRect rect)
{
	CD3DWnd * wnd;
	wnd=new CD3DWnd(this);
	wnd->SetRect(rect);	
	wnd->TopTo();
	return wnd;
}

CD3DButton * CD3DGUI::CreateButton(CEasyRect rect)
{
	CD3DButton * wnd;
	wnd=new CD3DButton(this);
	wnd->SetRect(rect);	
	wnd->TopTo();
	return wnd;
}

CD3DEdit * CD3DGUI::CreateEdit(CEasyRect rect)
{
	CD3DEdit * wnd;
	wnd=new CD3DEdit(this);
	wnd->SetRect(rect);	
	wnd->TopTo();
	return wnd;
}

CD3DScrollBar * CD3DGUI::CreateScrollBar(CEasyRect rect)
{
	CD3DScrollBar * wnd;
	wnd=new CD3DScrollBar(this);
	wnd->SetRect(rect);	
	wnd->TopTo();
	return wnd;
}

CD3DSimpleList * CD3DGUI::CreateSimpleList(CEasyRect rect)
{
	CD3DSimpleList * wnd;
	wnd=new CD3DSimpleList(this);
	wnd->SetRect(rect);	
	wnd->TopTo();
	return wnd;
}

CD3DCombo * CD3DGUI::CreateCombo(CEasyRect rect)
{
	CD3DCombo * wnd;
	wnd=new CD3DCombo(this);
	wnd->SetRect(rect);	
	wnd->TopTo();
	return wnd;
}

CD3DProgressBar * CD3DGUI::CreateProgressBar(CEasyRect rect)
{
	CD3DProgressBar * wnd;
	wnd=new CD3DProgressBar(this);
	wnd->SetRect(rect);	
	wnd->TopTo();
	return wnd;
}

BOOL CD3DGUI::HandleMessage(UINT msg, WPARAM wParam , LPARAM lParam)
{
	CD3DWnd * pWnd;		
	BOOL ActiveMessage=false;

	if(msg>=WM_D3DGUI_START)
		return false;
	if(m_ActiveWnd)
	{
		if(!m_ActiveWnd->IsVisible())
			ActiveWnd(m_ActiveWnd,false);
	}
		
	switch(msg)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDBLCLK:	
		ActiveMessage=true;		
	case WM_MOUSEMOVE:		
	case WM_MOUSEWHEEL:	
		{
			//找到窗口
			m_CurMouseX=LOWORD(lParam);
			m_CurMouseY=HIWORD(lParam);	
			pWnd=GetWndAtPos(m_CurMouseX,m_CurMouseY);			
			//处理MouseLeaveEnter消息
			if(m_RecentMouseWnd!=pWnd)
			{
				if(m_RecentMouseWnd)
					m_RecentMouseWnd->HandleMessage(m_RecentMouseWnd,WM_D3DGUI_MOUSE_LEAVE,wParam,lParam);
				m_RecentMouseWnd=pWnd;
				if(m_RecentMouseWnd)
					m_RecentMouseWnd->HandleMessage(m_RecentMouseWnd,WM_D3DGUI_MOUSE_ENTER,wParam,lParam);					
			}	
			//处理消息
			if(ActiveMessage)
			{
				CD3DWnd* pActiveWnd=pWnd;
				while(pActiveWnd&&(!pActiveWnd->CanGetFocus()))
					pActiveWnd=pActiveWnd->GetParent();
				ActiveWnd(pActiveWnd,true);					
			}
			if(pWnd)
			{				
				return pWnd->HandleMessage(pWnd,msg,wParam,lParam);				
			}
			return false;
		}
		break;
	case WM_KEYUP:
	case WM_KEYDOWN:
	case WM_CHAR:
	default:
		if(m_ActiveWnd)
			return m_ActiveWnd->HandleMessage(m_ActiveWnd,msg,wParam,lParam);
		break;
	}
	return false;
}

BOOL CD3DGUI::SendMsgToParent(UINT msg, WPARAM wParam , LPARAM lParam)
{
	if(m_hParentWnd)
		return (BOOL)SendMessage(m_hParentWnd,msg,wParam,lParam);
	return false;
}

void CD3DGUI::Update()
{
	CD3DWnd * pWnd;
	LPVOID pos;
	pos=m_D3DWndList.GetHead();
	while(pos)
	{
		pWnd=m_D3DWndList.GetNextObject(pos);
		pWnd->Update();
	}
}

bool CD3DGUI::LeftWndToTop(CD3DWnd * pWnd,CD3DWnd * Before)
{
	LPVOID pos;

	pos=m_D3DWndList.Find(pWnd);
	if(pos==NULL)
		return false;
	if(Before)
	{
		LPVOID BeforePos=m_D3DWndList.Find(Before);
		if(BeforePos==NULL)
			return false;
		m_D3DWndList.Delete(pos);
		m_D3DWndList.InsertBefore(pWnd,BeforePos);
	}
	else
	{
		m_D3DWndList.Delete(pos);
		m_D3DWndList.InsertBefore(pWnd);
	}
	return true;
}

void CD3DGUI::ActiveWnd(CD3DWnd * pWnd, bool active)
{
	
	if(active)
	{
		if(pWnd==m_ActiveWnd)
			return;

		if(m_ActiveWnd)
			m_ActiveWnd->ActiveWnd(false);
		m_ActiveWnd=pWnd;
		if(m_ActiveWnd)
			m_ActiveWnd->ActiveWnd(true);		
	}
	else
	{
		LPVOID pos;
		
		pos=m_D3DWndList.Find(pWnd);
		pos=m_D3DWndList.GetNext(pos);
		if(pos)
		{
			CD3DWnd * wnd=m_D3DWndList.GetObject(pos);
			ActiveWnd(wnd,true);
		}	
		else
		{
			m_ActiveWnd=NULL;
		}
	}
}

CD3DWnd * CD3DGUI::GetWndAtPos(int x,int y)
{
	LPVOID pos;
	CD3DWnd * pWnd;

	if(m_CaptureAllWnd)
	{
		return m_CaptureAllWnd;
	}	
	pos=m_D3DWndList.GetHead();
	
	while(pos)
	{			
		pWnd=m_D3DWndList.GetNextObject(pos);
		if(pWnd->IsVisible())
		{
			CEasyRect rect=pWnd->GetWndRect();							
			if(rect.PtInRect(CEasyPoint(x,y)))
				return pWnd;
		}
		
	}
	return NULL;
}

LPVOID CD3DGUI::GetFirstWndPosAtPos(int x,int y)
{
	LPVOID pos;
	CD3DWnd * pWnd;

	pos=m_D3DWndList.GetHead();
	while(pos)
	{			
		pWnd=m_D3DWndList.GetNextObject(pos);
		if(pWnd->IsVisible())
		{
			CEasyRect rect=pWnd->GetWndRect();							
			if(rect.PtInRect(CEasyPoint(x,y)))
				return pos;
		}

	}
	return NULL;
}

CD3DWnd * CD3DGUI::GetNextWndAtPos(LPVOID & Pos,int x,int y)
{	
	CD3DWnd * pWnd;
	
	while(Pos)
	{			
		pWnd=m_D3DWndList.GetNextObject(Pos);
		if(pWnd->IsVisible())
		{
			CEasyRect rect=pWnd->GetWndRect();							
			if(rect.PtInRect(CEasyPoint(x,y)))
				return pWnd;
		}

	}
	return NULL;
}

bool CD3DGUI::AddRootWnd(CD3DWnd * child)
{	
	if(m_RootWndList.Find(child)==NULL)
	{
		m_RootWndList.InsertAfter(child);
		return true;
	}
	return false;
}

bool CD3DGUI::DelRootWnd(CD3DWnd * child)
{
	LPVOID pos=m_RootWndList.GetHead();
	LPVOID OldPos;
	while(pos)
	{
		OldPos=pos;
		if(m_RootWndList.GetNextObject(pos)==child)
		{
			m_RootWndList.Delete(OldPos);
			return true;
		}
	}
	return true;
}

bool CD3DGUI::DelWnd(CD3DWnd * pWnd)
{
	LPVOID pos;
		
	pos=m_D3DWndList.Find(pWnd);
	if(pos)
	{
		if(pWnd==m_ActiveWnd)
			ActiveWnd(m_ActiveWnd,false);
		if(pWnd==m_RecentMouseWnd)
			m_RecentMouseWnd=NULL;
		m_D3DWndList.Delete(pos);
		return true;
	}
	return false;
}

void CD3DGUI::Clear()
{
	CD3DWnd * pWnd;
	LPVOID pos;
	pos=m_RootWndList.GetHead();
	while(pos)
	{
		pWnd=m_RootWndList.GetNextObject(pos);
		SAFE_RELEASE(pWnd);
	}
	m_D3DWndList.Clear();	
}

CD3DWnd * CD3DGUI::GetWndByName(LPCTSTR Name)
{
	LPVOID pos=m_D3DWndList.GetHead();
	
	while(pos)
	{
		CD3DWnd * pWnd=m_D3DWndList.GetNextObject(pos);
		if(strcmp(pWnd->GetName(),Name)==0)
			return pWnd;
	}
	return NULL;
}

CD3DWnd * CD3DGUI::GetWndByID(UINT ID)
{
	LPVOID pos=m_D3DWndList.GetHead();

	while(pos)
	{
		CD3DWnd * pWnd=m_D3DWndList.GetNextObject(pos);
		if(pWnd->GetID()==ID)
			return pWnd;
	}
	return NULL;
}

bool CD3DGUI::SaveToFile(LPCTSTR FileName)
{
	pug::xml_parser Xml;

	Xml.create();
	xml_node Doc;
	Doc = Xml.document();
	xml_node pi = Doc.append_child(node_pi);
	pi.name(_T("xml"));
	pi.attribute(_T("version")) = _T("1.0");
	pi.attribute(_T("encoding")) = _T("gb2312");
	xml_node GUI=Doc.append_child(node_element,"D3DGUI");

	SaveToXML(&GUI);

	std::ofstream os;	
	os.open(FileName, std::ios::trunc);	
	if (os.is_open())
	{
		os << Xml.document();
		os.close();
		return true;
	}
	else
	{		
		return false;
	}
}

void CD3DGUI::SaveToXML(xml_node * pXMLNode)
{
	CD3DWnd * pWnd;
	LPVOID pos;
	pos=m_RootWndList.GetHead();
	while(pos)
	{
		pWnd=m_RootWndList.GetNextObject(pos);
		pWnd->SaveToXml(pXMLNode);			
	}
}

bool CD3DGUI::SaveToUSO(CUSOFile * pUSOFile)
{
	CD3DWnd * pWnd;
	LPVOID pos;
	pos=m_RootWndList.GetHead();
	while(pos)
	{
		pWnd=m_RootWndList.GetNextObject(pos);
		pUSOFile->AddObject(pWnd);					
	}
	return true;
}

bool CD3DGUI::SaveToUSOFile(LPCTSTR FileName)
{
	CUSOFile USOFile;
	if(SaveToUSO(&USOFile))
	{
		return USOFile.Save(FileName);
	}
	return false;
}

bool CD3DGUI::LoadFromFile(LPCTSTR FileName)
{
	pug::xml_parser Xml;

	Xml.parse_file(FileName,pug::parse_trim_attribute);
	xml_node GUI=Xml.document();
	if(!GUI.moveto_child("D3DGUI"))
		return false;
	return LoadFromXML(&(GUI));
}

bool CD3DGUI::LoadFromXML(xml_node * pXMLNode,CD3DWnd * pParent)
{
	CD3DWnd * pWnd;
	for(int i=0;i<(int)pXMLNode->children();i++)
	{
		pWnd=NULL;
		if(_strnicmp(pXMLNode->child(i).name(),"Window",6)==0)
		{
			pWnd=CreateWnd(CEasyRect(0,0,0,0));
			pWnd->SetParent(pParent);
			pWnd->LoadFromXml(&(pXMLNode->child(i)));
		}
		else if(_strnicmp(pXMLNode->child(i).name(),"Button",6)==0)
		{
			pWnd=CreateButton(CEasyRect(0,0,0,0));
			pWnd->SetParent(pParent);
			pWnd->LoadFromXml(&(pXMLNode->child(i)));
		}
		else if(_strnicmp(pXMLNode->child(i).name(),"Edit",6)==0)
		{
			pWnd=CreateEdit(CEasyRect(0,0,0,0));
			pWnd->SetParent(pParent);
			pWnd->LoadFromXml(&(pXMLNode->child(i)));
		}
		else if(_strnicmp(pXMLNode->child(i).name(),"ScrollBar",6)==0)
		{
			pWnd=CreateScrollBar(CEasyRect(0,0,0,0));
			pWnd->SetParent(pParent);
			pWnd->LoadFromXml(&(pXMLNode->child(i)));
		}
		else if(_strnicmp(pXMLNode->child(i).name(),"SimpleList",6)==0)
		{
			pWnd=CreateSimpleList(CEasyRect(0,0,0,0));
			pWnd->SetParent(pParent);
			pWnd->LoadFromXml(&(pXMLNode->child(i)));
		}
		else if(_strnicmp(pXMLNode->child(i).name(),"Combo",6)==0)
		{
			pWnd=CreateCombo(CEasyRect(0,0,0,0));
			pWnd->SetParent(pParent);
			pWnd->LoadFromXml(&(pXMLNode->child(i)));
		}	
		else if(_strnicmp(pXMLNode->child(i).name(),"ProgressBar",12)==0)
		{
			pWnd=CreateProgressBar(CEasyRect(0,0,0,0));
			pWnd->SetParent(pParent);
			pWnd->LoadFromXml(&(pXMLNode->child(i)));
		}
	}
	return true;
}

}
