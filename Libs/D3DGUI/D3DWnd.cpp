/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DWnd.cpp                                               */
/*      ��������:  2009��09��25��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      ��������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ������������κ���ҵ�ͷ���ҵ������������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include "d3dwnd.h"

#if defined(USE_CRT_DETAIL_NEW) && defined(_DEBUG)
#undef new
#endif

#include <strstream>

#if defined(USE_CRT_DETAIL_NEW) && defined(_DEBUG)
#define new new( __FILE__, __LINE__ )
#endif

namespace D3DGUI{


IMPLEMENT_CLASS_INFO(CD3DWnd,CNameObject);

CD3DWnd::CD3DWnd():CNameObject()
{
	m_pWndRects=NULL;
	m_pTexture=NULL;
	m_pParentWnd=NULL;
	m_pGUI=NULL;
	InitWnd(NULL);
}

CD3DWnd::CD3DWnd(CD3DGUI *  pGUI):CNameObject()
{
	m_pWndRects=NULL;
	m_pTexture=NULL;
	m_pParentWnd=NULL;
	m_pGUI=NULL;
	InitWnd(pGUI);
}

void CD3DWnd::InitWnd(CD3DGUI *  pGUI)
{
	Destory();

	m_pGUI=pGUI; 

	if(m_pGUI)
	{
		m_pGUI->AddWnd(this);
		m_pGUI->AddRootWnd(this);		
	}

	m_pParentWnd=NULL;

	m_IsDragging=false;	


	m_WndRectCount=RECT_MAX;

	
	m_pWndRects=new LPIBASERECT[m_WndRectCount];
	ZeroMemory(m_pWndRects,sizeof(LPIBASERECT)*m_WndRectCount);

	m_pTexture=NULL;


	m_WndRect=CEasyRect(0,0,100,100);

	ZeroMemory(&m_Borders,sizeof(m_Borders));	

	ZeroMemory(&m_LogFont,sizeof(m_LogFont));

	strcpy_s(m_LogFont.lfFaceName,LF_FACESIZE,"����");
	m_LogFont.lfHeight=16;
	m_LogFont.lfWidth=0;
	m_LogFont.lfEscapement=0;
	m_LogFont.lfOrientation=0;
	m_LogFont.lfWeight=400;
	m_LogFont.lfItalic=false;
	m_LogFont.lfUnderline=false;
	m_LogFont.lfStrikeOut=false;
	m_LogFont.lfCharSet=DEFAULT_CHARSET;
	m_LogFont.lfOutPrecision=OUT_DEFAULT_PRECIS;
	m_LogFont.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	m_LogFont.lfQuality=DEFAULT_QUALITY;
	m_LogFont.lfPitchAndFamily=DEFAULT_PITCH | FF_DONTCARE;

	m_FontColor=0xffffffff;
	m_FontAlign=DT_WORDBREAK;
	m_FontShadowMode=0;
	m_FontShadowColor=0xff000000;
	m_FontShadowWidth=0;
	m_FontCharSpace=0;
	m_FontLineSpace=0;


	m_CanDrag=true;
	m_CanResize=false;
	m_Resizing=0;	

	m_IsCaptureAll=false;
	m_IsVisible=true;
	m_IsActive=false;

	m_IsEnable=true;
	m_CanGetFocus=true;
	m_IsTopmost=false;

	m_BKColor=0xffffffff;

	m_WantUpdateFont=false;
	m_IsInternal=false;	

	m_WndData=0;
}

void CD3DWnd::Destory()
{
	//�Ӹ���������ɾ���Լ�

	if(m_pParentWnd)
	{
		m_pParentWnd->DelChildWnd(this);
	}
	else
	{
		if(m_pGUI)
			m_pGUI->DelRootWnd(this);		
	}

	//ɾ���Ӵ���
	for(int i=(int)m_ChildWndList.size()-1;i>=0;i--)	
	{			
		CD3DWnd * pWnd=m_ChildWndList[i];
		SAFE_RELEASE(pWnd);
	}
	m_ChildWndList.clear();

	
	if(m_pWndRects)
	{
		for(int i=0;i<m_WndRectCount;i++)
		{
			SAFE_RELEASE(m_pWndRects[i]);
		}
		m_WndRectCount=0;
	}
	SAFE_DELETE_ARRAY(m_pWndRects);
	SAFE_RELEASE(m_pTexture);

	if(m_pGUI)
		m_pGUI->DelWnd(this);

	CNameObject::Destory();
}



CD3DWnd::~CD3DWnd(void)
{	
	Destory();
}


bool CD3DWnd::SetTexture(LPCTSTR TextureFileName,RECT& TextureRect)
{
	SAFE_RELEASE(m_pTexture);
	m_pTexture=m_pGUI->GetObjectCreator()->CreateTextureFromFile(TextureFileName);
	if(m_pTexture)
	{	
		m_TextureRect=TextureRect;
		return UpdateTextureRect();
	}

	return false;
	
}

void CD3DWnd::GetTexture(CEasyString& TextureFileName,RECT& TextureRect)
{
	if(m_pTexture)
		TextureFileName=m_pTexture->GetName();
	TextureRect=m_TextureRect;
}

void CD3DWnd::Move(int x, int y)
{	
	CEasyRect OldRect=m_WndRect;	
	if(m_WndRect.left!=x||m_WndRect.top!=y)
	{
		m_WndRect.MoveToXY(x,y);
		UpdateRects();		
	}
}

void CD3DWnd::Resize(int width, int height)
{
	m_WndRect.right=m_WndRect.left+width;
	m_WndRect.bottom=m_WndRect.top+height;
	UpdateRects();
}

void CD3DWnd::SetRect(RECT& rect)
{
	m_WndRect=rect;	
	UpdateRects();
}

CEasyRect CD3DWnd::GetRect()
{
	return m_WndRect;
}

CEasyRect CD3DWnd::GetCenterRect()
{
	CEasyRect rect=m_WndRect;

	rect.top+=m_Borders[RECT_TOP];
	rect.bottom-=m_Borders[RECT_BOTTOM];
	rect.left+=m_Borders[RECT_LEFT];
	rect.right-=m_Borders[RECT_RIGHT];
	
	return rect;
}

CEasyRect CD3DWnd::GetWndRect()
{
	CEasyRect Rect=GetRect();
	if(m_pParentWnd)
		m_pParentWnd->ClientToScreen(&Rect);
	return Rect;
}

CEasyRect CD3DWnd::GetClientRect()
{
	CEasyRect Rect=GetCenterRect();	
	Rect.right-=Rect.left;
	Rect.bottom-=Rect.top;
	Rect.left=0;
	Rect.top=0;
	return Rect;
}

void CD3DWnd::SetBorders(WIN_BORDERS& Borders)
{
	memcpy(m_Borders,Borders,sizeof(WIN_BORDERS));
	UpdateRects();
	UpdateTextureRect();
}

void CD3DWnd::GetBorders(WIN_BORDERS& Borders)
{
	memcpy(Borders,m_Borders,sizeof(WIN_BORDERS));
}

BOOL CD3DWnd::OnMessage(CD3DWnd * pWnd,UINT msg, WPARAM wParam, LPARAM lParam)
{	
	CEasyRect rect;
	int xPos,yPos;	


	switch(msg)
	{
	case WM_LBUTTONDOWN:
		xPos=LOWORD(lParam);
		yPos=HIWORD(lParam);			

		if(m_CanDrag)
		{
			if(m_pWndRects[RECT_TOP])
			{
				rect=m_pWndRects[RECT_TOP]->GetRect();			
				if(rect.PtInRect(CEasyPoint(xPos,yPos)))
				{
					CEasyRect WndRect=GetRect();				
					m_IsDragging=true;
					m_DraggingStartMousePoint.x=xPos;
					m_DraggingStartMousePoint.y=yPos;
					m_DraggingStartPoint.x=WndRect.left;
					m_DraggingStartPoint.y=WndRect.top;			
					SetCaptureAll(true);
					return true;
				}
			}
		}
		if(m_CanResize)
		{
			if(m_pWndRects[RECT_RIGHT])
			{
				rect=m_pWndRects[RECT_RIGHT]->GetRect();			
				if(rect.PtInRect(CEasyPoint(xPos,yPos)))
				{
					CEasyRect WndRect=GetRect();			
					m_Resizing=2;
					m_DraggingStartMousePoint.x=xPos;
					m_DraggingStartMousePoint.y=yPos;
					m_DraggingStartPoint.x=WndRect.left;
					m_DraggingStartPoint.y=WndRect.top;	
					m_DraggingStartSize.cx=WndRect.Width();
					m_DraggingStartSize.cy=WndRect.Height();
					SetCaptureAll(true);
					return true;
				}
			}
			if(m_pWndRects[RECT_BOTTOM])
			{
				rect=m_pWndRects[RECT_BOTTOM]->GetRect();		
				if(rect.PtInRect(CEasyPoint(xPos,yPos)))
				{
					CEasyRect WndRect=GetRect();			
					m_Resizing=4;
					m_DraggingStartMousePoint.x=xPos;
					m_DraggingStartMousePoint.y=yPos;
					m_DraggingStartPoint.x=WndRect.left;
					m_DraggingStartPoint.y=WndRect.top;	
					m_DraggingStartSize.cx=WndRect.Width();
					m_DraggingStartSize.cy=WndRect.Height();
					SetCaptureAll(true);
					return true;
				}
			}
			if(m_pWndRects[RECT_LEFT])
			{
				rect=m_pWndRects[RECT_LEFT]->GetRect();
				if(rect.PtInRect(CEasyPoint(xPos,yPos)))
				{
					CEasyRect WndRect=GetRect();			
					m_Resizing=8;
					m_DraggingStartMousePoint.x=xPos;
					m_DraggingStartMousePoint.y=yPos;
					m_DraggingStartPoint.x=WndRect.left;
					m_DraggingStartPoint.y=WndRect.top;	
					m_DraggingStartSize.cx=WndRect.Width();
					m_DraggingStartSize.cy=WndRect.Height();
					SetCaptureAll(true);
					return true;
				}
			}
			if(m_pWndRects[RECT_TOP_LEFT])
			{
				rect=m_pWndRects[RECT_TOP_LEFT]->GetRect();
				if(rect.PtInRect(CEasyPoint(xPos,yPos)))
				{
					CEasyRect WndRect=GetRect();			
					m_Resizing=1|8;
					m_DraggingStartMousePoint.x=xPos;
					m_DraggingStartMousePoint.y=yPos;
					m_DraggingStartPoint.x=WndRect.left;
					m_DraggingStartPoint.y=WndRect.top;	
					m_DraggingStartSize.cx=WndRect.Width();
					m_DraggingStartSize.cy=WndRect.Height();
					SetCaptureAll(true);
					return true;
				}
			}
			if(m_pWndRects[RECT_TOP_RIGHT])
			{
				rect=m_pWndRects[RECT_TOP_RIGHT]->GetRect();
				if(rect.PtInRect(CEasyPoint(xPos,yPos)))
				{
					CEasyRect WndRect=GetRect();			
					m_Resizing=1|2;
					m_DraggingStartMousePoint.x=xPos;
					m_DraggingStartMousePoint.y=yPos;
					m_DraggingStartPoint.x=WndRect.left;
					m_DraggingStartPoint.y=WndRect.top;	
					m_DraggingStartSize.cx=WndRect.Width();
					m_DraggingStartSize.cy=WndRect.Height();
					SetCaptureAll(true);
					return true;
				}
			}
			if(m_pWndRects[RECT_BOTTOM_LEFT])
			{
				rect=m_pWndRects[RECT_BOTTOM_LEFT]->GetRect();
				if(rect.PtInRect(CEasyPoint(xPos,yPos)))
				{
					CEasyRect WndRect=GetRect();			
					m_Resizing=4|8;
					m_DraggingStartMousePoint.x=xPos;
					m_DraggingStartMousePoint.y=yPos;
					m_DraggingStartPoint.x=WndRect.left;
					m_DraggingStartPoint.y=WndRect.top;	
					m_DraggingStartSize.cx=WndRect.Width();
					m_DraggingStartSize.cy=WndRect.Height();
					SetCaptureAll(true);
					return true;
				}
			}
			if(m_pWndRects[RECT_BOTTOM_RIGHT])
			{
				rect=m_pWndRects[RECT_BOTTOM_RIGHT]->GetRect();
				if(rect.PtInRect(CEasyPoint(xPos,yPos)))
				{
					CEasyRect WndRect=GetRect();			
					m_Resizing=4|2;
					m_DraggingStartMousePoint.x=xPos;
					m_DraggingStartMousePoint.y=yPos;
					m_DraggingStartPoint.x=WndRect.left;
					m_DraggingStartPoint.y=WndRect.top;	
					m_DraggingStartSize.cx=WndRect.Width();
					m_DraggingStartSize.cy=WndRect.Height();
					SetCaptureAll(true);
					return true;
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		if(m_IsDragging||m_Resizing)
		{
			m_IsDragging=false;				
			m_Resizing=0;
			SetCaptureAll(false);
			HandleMessage(this,WM_D3DGUI_RESIZED,GetID(),(LPARAM)this);
			return true;		
		}
		break;
	case WM_MOUSEMOVE:
		xPos=(short)(LOWORD(lParam));
		yPos=(short)(HIWORD(lParam));
		if(m_IsDragging)
		{				
			POINT Pos;
			Pos.x = (xPos-m_DraggingStartMousePoint.x)+m_DraggingStartPoint.x; 
			Pos.y = (yPos-m_DraggingStartMousePoint.y)+m_DraggingStartPoint.y;			//if(m_pParentWnd)
			//	m_pParentWnd->ScreenToClient(&Pos);
			Move(Pos.x,Pos.y);			
			HandleMessage(this,WM_D3DGUI_RESIZING,GetID(),(LPARAM)this);
			return true;
		}
		if(m_Resizing>0)
		{
			CEasyRect WndRect=GetRect();
			int dx,dy;
			dx=xPos-m_DraggingStartMousePoint.x;
			dy=yPos-m_DraggingStartMousePoint.y;
			if(dx||dy)
			{				
				if(m_Resizing&1)
				{
					WndRect.top=m_DraggingStartPoint.y+dy;
					WndRect.bottom=WndRect.top+m_DraggingStartSize.cy-dy;
				}
				if(m_Resizing&2)
				{
					WndRect.right=WndRect.left+m_DraggingStartSize.cx+dx;
				}
				if(m_Resizing&4)
				{
					WndRect.bottom=WndRect.top+m_DraggingStartSize.cy+dy;
				}
				if(m_Resizing&8)
				{
					WndRect.left=m_DraggingStartPoint.x+dx;
					WndRect.right=WndRect.left+m_DraggingStartSize.cx-dx;			
				}			
				SetRect(WndRect);		
				HandleMessage(this,WM_D3DGUI_RESIZING,GetID(),(LPARAM)this);
			}			
			return true;
		}
		break;
	}
	return false;
}

BOOL CD3DWnd::HandleMessage(CD3DWnd * pWnd,UINT msg, WPARAM wParam, LPARAM lParam)
{
	bool CanProcess=false;
	bool Processed=false;
	if(msg>WM_D3DGUI_START&&msg<WM_D3DGUI_SYSTEM_MSG_END)
	{
		CanProcess=true;	
	}
	else
	{
		if(m_IsVisible&&m_IsEnable)
			CanProcess=true;
	}
	if(CanProcess)
		Processed=OnMessage(pWnd,msg,wParam,lParam);

	if(!Processed)
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


CD3DWnd * CD3DWnd::GetParent(void)
{
	return m_pParentWnd;
}

void CD3DWnd::SetParent(CD3DWnd * parent,bool IsUpdateParent)
{
	if(parent)
	{	
		if(IsUpdateParent)
		{
			if(IsUpdateParent)

			if(m_pParentWnd)
				m_pParentWnd->DelChildWnd(this);
			else
			{			
				m_pGUI->DelRootWnd(this);
			}
			parent->AddChildWnd(this);
		}
		m_pParentWnd=parent;
		TopTo(m_pParentWnd);
	}
	else
	{
		if(m_pParentWnd)
		{
			if(IsUpdateParent)
			{
				m_pParentWnd->DelChildWnd(this);			
				m_pGUI->AddRootWnd(this);
			}
			m_pParentWnd=NULL;			
		}		
	}
	UpdateRects();
}

void CD3DWnd::SetFont(LOGFONT * pLogFont)
{
	if(pLogFont)
	{
		m_LogFont=*pLogFont;
		UpdateFont();
	}
}
void CD3DWnd::SetFontColor(DWORD Color)
{
	m_FontColor=Color;
	m_WantUpdateFont=true;
	UpdateFont();
}

void CD3DWnd::SetFontAlign(DWORD Align)
{
	m_FontAlign=Align;
	m_WantUpdateFont=true;
	UpdateFont();
}

void CD3DWnd::SetFontShadowMode(DWORD ShadowMode)
{
	m_FontShadowMode=ShadowMode;
	m_WantUpdateFont=true;
	UpdateFont();
}

void CD3DWnd::SetFontShadowColor(DWORD ShadowColor)
{
	m_FontShadowColor=ShadowColor;
	m_WantUpdateFont=true;
	UpdateFont();
}

void CD3DWnd::SetFontShadowWidth(DWORD ShadowWidth)
{
	m_FontShadowWidth=ShadowWidth;
	m_WantUpdateFont=true;
	UpdateFont();
}
void CD3DWnd::SetFontCharSpace(int Space)
{
	m_FontCharSpace=Space;
	m_WantUpdateFont=true;
	UpdateFont();
}
void CD3DWnd::SetFontLineSpace(int Space)
{
	m_FontLineSpace=Space;
	m_WantUpdateFont=true;
	UpdateFont();
}

LOGFONT * CD3DWnd::GetFont()
{
	return &m_LogFont;
}

DWORD CD3DWnd::GetFontColor()
{
	return m_FontColor;
}

DWORD CD3DWnd::GetFontAlign()
{
	return m_FontAlign;
}

DWORD CD3DWnd::GetFontShadowMode()
{
	return m_FontShadowMode;
}

DWORD CD3DWnd::GetFontShadowColor()
{
	return m_FontShadowColor;
}

DWORD CD3DWnd::GetFontShadowWidth()
{
	return m_FontShadowWidth;
}

int CD3DWnd::GetFontCharSpace()
{
	return m_FontCharSpace;
}
int CD3DWnd::GetFontLineSpace()
{
	return m_FontLineSpace;
}


void CD3DWnd::SetText(LPCTSTR Text)
{
	m_WndText=Text;
	UpdateText();
	IUITextRect * pTextRect=GetTextRect();
	if(pTextRect)
	{		
		pTextRect->SetTextW((LPCWSTR)m_WndText);		
	}
}

void CD3DWnd::SetTextW(LPCWSTR Text)
{
	m_WndText=Text;
	UpdateText();
	IUITextRect * pTextRect=GetTextRect();
	if(pTextRect)
	{		
		pTextRect->SetTextW((LPCWSTR)m_WndText);		
	}
}

void CD3DWnd::SetBKColor(DWORD BKColor)
{
	m_BKColor=BKColor;
	for(int i=RECT_TOP;i<RECT_CENTER;i++)
	{
		if(m_pWndRects[i])
			((IUIWndRect *)m_pWndRects[i])->SetColor(m_BKColor);
	}	
}


void CD3DWnd::ClientToScreen(RECT * rect)
{
	CEasyRect WndRect=GetCenterRect();

	rect->left+=WndRect.left;
	rect->right+=WndRect.left;
	rect->top+=WndRect.top;
	rect->bottom+=WndRect.top;
	if(m_pParentWnd!=NULL) 
		m_pParentWnd->ClientToScreen(rect);
}

void CD3DWnd::ScreenToClient(RECT * rect)
{
	CEasyRect WndRect=GetCenterRect();

	rect->left-=WndRect.left;
	rect->right-=WndRect.left;
	rect->top-=WndRect.top;
	rect->bottom-=WndRect.top;
	if(m_pParentWnd!=NULL) 
		m_pParentWnd->ScreenToClient(rect);
}

void CD3DWnd::ClientToScreen(POINT * point)
{
	CEasyRect WndRect=GetCenterRect();

	point->x+=WndRect.left;
	point->y+=WndRect.top;
	if(m_pParentWnd!=NULL) 
		m_pParentWnd->ClientToScreen(point);
}

void CD3DWnd::ScreenToClient(POINT * point)
{
	CEasyRect WndRect=GetCenterRect();

	point->x-=WndRect.left;
	point->y-=WndRect.top;
	if(m_pParentWnd!=NULL) 
		m_pParentWnd->ScreenToClient(point);
}


void CD3DWnd::ActiveWnd(bool bActive)
{
	//m_pGUI->ActiveWnd(this,bActive);
	m_IsActive=bActive;
	if(m_IsActive)
	{
		TopParent();
		HandleMessage(this,WM_D3DGUI_GET_FOCUS,GetID(),(LPARAM)this);
	}
	else
	{
		HandleMessage(this,WM_D3DGUI_LOST_FOCUS,GetID(),(LPARAM)this);
	}
}


void CD3DWnd::UpdateRects()
{
	CEasyRect rect;

	if(!m_IsVisible)
		return;
	

	int MinWidth,MinHeight;

	GetMiniSize(MinWidth,MinHeight);
	if(m_WndRect.Width()<MinWidth)
		m_WndRect.right=m_WndRect.left+MinWidth;

	if(m_WndRect.Height()<MinHeight)
		m_WndRect.bottom=m_WndRect.top+MinHeight;
	
	rect=GetCenterRect();

	if(m_pParentWnd)
		m_pParentWnd->ClientToScreen(&rect);

	if(rect.Width()||rect.Height())
	{
		if(m_pWndRects[RECT_CENTER]==NULL)
		{
			m_pWndRects[RECT_CENTER]=CreateRect();		
			RebuildOrder();
		}
		m_pWndRects[RECT_CENTER]->SetRect(&FLOAT_RECT(rect));			
	}	
	
		
	
	rect=m_WndRect;
	if(m_pParentWnd)
		m_pParentWnd->ClientToScreen(&rect);		
	for(int i=RECT_TOP;i<RECT_CENTER;i++)
	{
		if(m_Borders[i])
		{
			if(m_pWndRects[i]==NULL)
			{
				m_pWndRects[i]=CreateRect();
				RebuildOrder();		
			}
			FLOAT_RECT FRect;
			switch(i)
			{
			case RECT_TOP:				
				FRect.left		=(FLOAT)rect.left+m_Borders[RECT_TOP_LEFT];
				FRect.top		=(FLOAT)rect.top;
				FRect.right		=(FLOAT)rect.right-m_Borders[RECT_TOP_RIGHT];
				FRect.bottom	=(FLOAT)rect.top+m_Borders[RECT_TOP];
				break;
			case RECT_BOTTOM:
				FRect.left		=(FLOAT)rect.left+m_Borders[RECT_TOP_LEFT];
				FRect.top		=(FLOAT)rect.bottom-m_Borders[RECT_BOTTOM];
				FRect.right		=(FLOAT)rect.right-m_Borders[RECT_TOP_RIGHT];
				FRect.bottom	=(FLOAT)rect.bottom;
				break;
			case RECT_LEFT:
				FRect.left		=(FLOAT)rect.left;
				FRect.top		=(FLOAT)rect.top+m_Borders[RECT_TOP];
				FRect.right		=(FLOAT)rect.left+m_Borders[RECT_LEFT];
				FRect.bottom	=(FLOAT)rect.bottom-m_Borders[RECT_BOTTOM];
				break;
			case RECT_RIGHT:
				FRect.left		=(FLOAT)rect.right-m_Borders[RECT_RIGHT];
				FRect.top		=(FLOAT)rect.top+m_Borders[RECT_TOP];
				FRect.right		=(FLOAT)rect.right;
				FRect.bottom	=(FLOAT)rect.bottom-m_Borders[RECT_BOTTOM];
				break;
			case RECT_TOP_LEFT:
				FRect.left		=(FLOAT)rect.left;
				FRect.top		=(FLOAT)rect.top;
				FRect.right		=(FLOAT)rect.left+m_Borders[RECT_TOP_LEFT];
				FRect.bottom	=(FLOAT)rect.top+m_Borders[RECT_TOP];
				break;
			case RECT_TOP_RIGHT:
				FRect.left		=(FLOAT)rect.right-m_Borders[RECT_TOP_RIGHT];
				FRect.top		=(FLOAT)rect.top;
				FRect.right		=(FLOAT)rect.right;
				FRect.bottom	=(FLOAT)rect.top+m_Borders[RECT_TOP];
				break;
			case RECT_BOTTOM_LEFT:
				FRect.left		=(FLOAT)rect.left;
				FRect.top		=(FLOAT)rect.bottom-m_Borders[RECT_BOTTOM];
				FRect.right		=(FLOAT)rect.left+m_Borders[RECT_BOTTOM_LEFT];
				FRect.bottom	=(FLOAT)rect.bottom;
				break;
			case RECT_BOTTOM_RIGHT:
				FRect.left		=(FLOAT)rect.right-m_Borders[RECT_BOTTOM_RIGHT];
				FRect.top		=(FLOAT)rect.bottom-m_Borders[RECT_BOTTOM];
				FRect.right		=(FLOAT)rect.right;
				FRect.bottom	=(FLOAT)rect.bottom;
				break;			
			}
			m_pWndRects[i]->SetRect(&FRect);			
		}
	}
	UpdateText();
	UpdateChildRects();
}

bool CD3DWnd::UpdateTextureRect()
{		
	if(m_pTexture==NULL)
		return false;	
	for(int i=0;i<RECT_TEXT;i++)
	{
		if(m_Borders[i])
		{
			FLOAT_RECT UVRect;
			CEasyRect TextureRect;
			if(m_pWndRects[i]==NULL)
			{
				m_pWndRects[i]=CreateRect();
				RebuildOrder();
			}
			((IUIWndRect *)m_pWndRects[i])->SetTexture(m_pTexture);
			switch(i)
			{
			case RECT_CENTER:
				TextureRect.left	=m_TextureRect.left+m_Borders[RECT_LEFT];
				TextureRect.top		=m_TextureRect.top+m_Borders[RECT_TOP];
				TextureRect.right	=m_TextureRect.right-m_Borders[RECT_RIGHT];
				TextureRect.bottom	=m_TextureRect.bottom-m_Borders[RECT_BOTTOM];
				break;
			case RECT_TOP:
				TextureRect.left	=m_TextureRect.left+m_Borders[RECT_TOP_LEFT];
				TextureRect.top		=m_TextureRect.top;
				TextureRect.right	=m_TextureRect.right-m_Borders[RECT_TOP_RIGHT];
				TextureRect.bottom	=m_TextureRect.top+m_Borders[RECT_TOP];
				break;
			case RECT_BOTTOM:
				TextureRect.left	=m_TextureRect.left+m_Borders[RECT_TOP_LEFT];
				TextureRect.top		=m_TextureRect.bottom-m_Borders[RECT_BOTTOM];
				TextureRect.right	=m_TextureRect.right-m_Borders[RECT_TOP_RIGHT];
				TextureRect.bottom	=m_TextureRect.bottom;
				break;
			case RECT_LEFT:
				TextureRect.left	=m_TextureRect.left;
				TextureRect.top		=m_TextureRect.top+m_Borders[RECT_TOP];
				TextureRect.right	=m_TextureRect.left+m_Borders[RECT_LEFT];
				TextureRect.bottom	=m_TextureRect.bottom-m_Borders[RECT_BOTTOM];
				break;
			case RECT_RIGHT:
				TextureRect.left	=m_TextureRect.right-m_Borders[RECT_RIGHT];
				TextureRect.top		=m_TextureRect.top+m_Borders[RECT_TOP];
				TextureRect.right	=m_TextureRect.right;
				TextureRect.bottom	=m_TextureRect.bottom-m_Borders[RECT_BOTTOM];
				break;
			case RECT_TOP_LEFT:
				TextureRect.left	=m_TextureRect.left;
				TextureRect.top		=m_TextureRect.top;
				TextureRect.right	=m_TextureRect.left+m_Borders[RECT_TOP_LEFT];
				TextureRect.bottom	=m_TextureRect.top+m_Borders[RECT_TOP];
				break;
			case RECT_TOP_RIGHT:
				TextureRect.left		=m_TextureRect.right-m_Borders[RECT_TOP_RIGHT];
				TextureRect.top		=m_TextureRect.top;
				TextureRect.right	=m_TextureRect.right;
				TextureRect.bottom	=m_TextureRect.top+m_Borders[RECT_TOP];
				break;
			case RECT_BOTTOM_LEFT:
				TextureRect.left	=m_TextureRect.left;
				TextureRect.top		=m_TextureRect.bottom-m_Borders[RECT_BOTTOM];
				TextureRect.right	=m_TextureRect.left+m_Borders[RECT_BOTTOM_LEFT];
				TextureRect.bottom	=m_TextureRect.bottom;
				break;
			case RECT_BOTTOM_RIGHT:
				TextureRect.left	=m_TextureRect.right-m_Borders[RECT_BOTTOM_RIGHT];
				TextureRect.top		=m_TextureRect.bottom-m_Borders[RECT_BOTTOM];
				TextureRect.right	=m_TextureRect.right;
				TextureRect.bottom	=m_TextureRect.bottom;
				break;			
			}
			UVRect.left=m_pTexture->X2U(TextureRect.left);
			UVRect.right=m_pTexture->X2U(TextureRect.right);
			UVRect.top=m_pTexture->Y2V(TextureRect.top);
			UVRect.bottom=m_pTexture->Y2V(TextureRect.bottom);
			((IUIWndRect *)m_pWndRects[i])->SetUV(&UVRect);
		}
	}
	return true;
}

void CD3DWnd::UpdateText()
{
	if(!m_IsVisible)
		return;

	CEasyRect Rect=GetClientRect();
	ClientToScreen(&Rect);

	if(!m_WndText.IsEmpty())
	{
		if(m_pWndRects[RECT_TEXT]==NULL)		
		{
			m_pWndRects[RECT_TEXT]=CreateTextRect();
			RebuildOrder();
		}
		if(m_pWndRects[RECT_TEXT])
			m_pWndRects[RECT_TEXT]->SetRect(&FLOAT_RECT(Rect));
	}
	
	
}

void CD3DWnd::Update()
{
	//UpdateFont();
}

void CD3DWnd::SaveToXml(xml_node * pXMLNode)
{
	xml_node Wnd=pXMLNode->append_child(node_element,"Window");
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
	if(m_pTexture)
	{	
		xml_node Texture=Wnd.append_child(node_element,"Texture");
		SaveTextureToXML(Texture);
	}
	
	if(m_ChildWndList.size()>0)
	{
		xml_node Childs=Wnd.append_child(node_element,"Childs");
		SaveChildsToXml(Childs);
	}

}

bool CD3DWnd::LoadFromXml(xml_node * pXMLNode)
{
	if(_strnicmp(pXMLNode->name(),"Window",7)!=0)
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
	TopChild();
	HandleMessage(this,WM_D3DGUI_CHILD_LOADED,GetID(),(LPARAM)this);
	
	return true;
}

bool CD3DWnd::LoadFromFile(LPCTSTR FileName,LPCTSTR WndName)
{
	pug::xml_parser Xml;

	Xml.parse_file(FileName,pug::parse_trim_attribute);
	xml_node GUI=Xml.document();
	if(!GUI.moveto_child("D3DGUI"))
		return false;
	CEasyString UIPath=GetPathDirectory(MakeFullPath(FileName));
	m_pGUI->GetObjectCreator()->SetTextureDir(UIPath);

	for(int i=0;i<(int)GUI.children();i++)
	{
		if(WndName==NULL)
			return LoadFromXml(&GUI.child(i));
		if(GUI.child(i).attribute("Name").getvalue()==WndName)
			return LoadFromXml(&GUI.child(i));
	}
	return false;
}



//bool CD3DWnd::ToUSOFile(CUSOFile * pUSOFile,UINT Param)
//{	
//
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	pug::xml_parser Xml;
//
//	Xml.create();
//	xml_node Doc;
//	Doc = Xml.document();	
//	
//	SaveToXml(&Doc);	
//
//	std::strstream os;		
//
//	os << Xml.document();
//	os<<'\0';
//	os.freeze();
//
//	CEasyString Script=os.str();
//
//	Script.Replace("\n","\r\n");
//
//	STORAGE_STRUCT Data;
//
//	strncpy_0(Data.ObjectHead.Type,USO_FILE_MAX_TYPE_LEN,GetClassInfo().ClassName,USO_FILE_MAX_TYPE_LEN);	
//	strncpy_0(Data.ObjectHead.Name,USO_FILE_MAX_OBJECT_NAME,GetName(),USO_FILE_MAX_OBJECT_NAME);	
//	Data.ScriptStrLen=Script.GetLength()+1;
//	Data.ObjectHead.Size=sizeof(STORAGE_STRUCT)+Data.ScriptStrLen;	
//	
//
//	pFile->Write(&Data,sizeof(STORAGE_STRUCT));
//	pFile->Write((LPSTR)((LPCTSTR)Script),Data.ScriptStrLen);	
//
//	return true;
//}
//
//bool CD3DWnd::FromUSOFile(CUSOFile * pUSOFile,UINT Param)
//{
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	STORAGE_STRUCT * pData;
//	BYTE * pBuff;
//	char * pScript;
//	UINT Size;
//
//	pFile->Read(&Size,sizeof(UINT));
//	pBuff=new BYTE[Size];
//	pFile->Read(pBuff+sizeof(UINT),Size-sizeof(UINT));
//	pData=(STORAGE_STRUCT *)pBuff;
//	pScript=(char *)pBuff+sizeof(STORAGE_STRUCT);
//	pData->ObjectHead.Size=Size;
//
//
//	if((!GetClassInfo().IsKindOf(pData->ObjectHead.Type))||
//		pData->ObjectHead.Size<sizeof(STORAGE_STRUCT))
//	{	
//		delete[] pBuff;
//		return false;
//	}
//
//	
//
//	pData->ObjectHead.Name[USO_FILE_MAX_OBJECT_NAME-1]=0;
//	SetName(pData->ObjectHead.Name);
//
//	pug::xml_parser Xml;
//	Xml.parse(pScript,pug::parse_trim_attribute);	
//	xml_node Doc=Xml.document();
//
//	if(Doc.children()>0)
//	{
//		if(LoadFromXml(&Doc.child(0)))
//		{	
//			delete[] pBuff;
//			return true;
//		}
//	}
//	delete[] pBuff;
//	return false;
//	
//}

void CD3DWnd::PickResource(CNameObjectSet * pObjectSet,UINT Param)
{
	if(m_pTexture)
	{
		m_pTexture->PickResource(pObjectSet,Param);
	}

	for(int i=0;i<(int)m_ChildWndList.size();i++)
	{
		m_ChildWndList[i]->PickResource(pObjectSet,Param);
	}	
}

void CD3DWnd::OnLoaded()
{
}

bool CD3DWnd::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CNameObject::ToSmartStruct(Packet,pUSOFile,Param))
		return false;

	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_GUI_SCRIPT,(LPCTSTR)m_TempScript,m_TempScript.GetLength()));

	return true;
}
bool CD3DWnd::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CNameObject::FromSmartStruct(Packet,pUSOFile,Param))
		return false;

	LPCTSTR szScript=Packet.GetMember(SST_GUI_SCRIPT);
	if(szScript)
	{
		pug::xml_parser Xml;
		Xml.parse((TCHAR *)szScript,pug::parse_trim_attribute);	
		xml_node Doc=Xml.document();

		if(Doc.children()>0)
		{
			if(LoadFromXml(&Doc.child(0)))
			{	
				return false;
			}
		}
	}
	return true;
}

UINT CD3DWnd::GetSmartStructSize(UINT Param)
{
	UINT Size=CNameObject::GetSmartStructSize(Param);

	pug::xml_parser Xml;

	Xml.create();
	xml_node Doc;
	Doc = Xml.document();	
	
	SaveToXml(&Doc);	

	std::strstream os;		

	os << Xml.document();
	os<<'\0';
	os.freeze();

	m_TempScript=os.str();

	m_TempScript.Replace("\n","\r\n");

	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(m_TempScript.GetLength());

	return Size;
}

//////////////////////////////////////////////////////



void CD3DWnd::SaveChildsToXml(xml_node& Childs)
{
	for(int i=0;i<(int)m_ChildWndList.size();i++)
	{
		m_ChildWndList[i]->SaveToXml(&Childs);
	}
}

void CD3DWnd::SaveFrameToXML(xml_node& Frame)
{
	CEasyRect Rect=GetRect();
	Frame.append_attribute("X",(long)Rect.left);
	Frame.append_attribute("Y",(long)Rect.top);
	Frame.append_attribute("Width",(long)Rect.Width());
	Frame.append_attribute("Height",(long)Rect.Height());
	Frame.append_attribute("BKColor",(long)GetBKColor());
}
void CD3DWnd::SaveBorderToXML(xml_node& Borders)
{
	Borders.append_attribute("Top",(long)m_Borders[RECT_TOP]);
	Borders.append_attribute("Bottom",(long)m_Borders[RECT_BOTTOM]);
	Borders.append_attribute("Left",(long)m_Borders[RECT_LEFT]);
	Borders.append_attribute("Right",(long)m_Borders[RECT_RIGHT]);
	Borders.append_attribute("TopLeft",(long)m_Borders[RECT_TOP_LEFT]);
	Borders.append_attribute("TopRight",(long)m_Borders[RECT_TOP_RIGHT]);
	Borders.append_attribute("BottomLeft",(long)m_Borders[RECT_BOTTOM_LEFT]);
	Borders.append_attribute("BottomRight",(long)m_Borders[RECT_BOTTOM_RIGHT]);
}
void CD3DWnd::SaveTextToXML(xml_node& Text)
{
	CEasyString WndText;
	GetText(WndText);
	WndText.Replace("\r\n","[&RetuRn]");
	WndText.Replace("\"","[&InveRtedComMa]");
	Text.append_attribute("Text",(LPCTSTR)WndText);
}
void CD3DWnd::SaveFontToXML(xml_node& Font)
{
	Font.append_attribute("FaceName",m_LogFont.lfFaceName);
	Font.append_attribute("Height",(long)m_LogFont.lfHeight);
	Font.append_attribute("Width",(long)m_LogFont.lfWidth);
	Font.append_attribute("Weight",(long)m_LogFont.lfWeight);
	Font.append_attribute("CharSet",(long)m_LogFont.lfCharSet);
	Font.append_attribute("Italic",(bool)m_LogFont.lfItalic);
	Font.append_attribute("OutPrecision",(long)m_LogFont.lfOutPrecision);
	Font.append_attribute("PitchAndFamily",(long)m_LogFont.lfPitchAndFamily);
	Font.append_attribute("Quality",(long)m_LogFont.lfQuality);
	Font.append_attribute("FontColor",(long)m_FontColor);
	Font.append_attribute("Align",(long)m_FontAlign);
	Font.append_attribute("ShadowMode",(long)m_FontShadowMode);
	Font.append_attribute("ShadowColor",(long)m_FontShadowColor);
	Font.append_attribute("ShadowWidth",(long)m_FontShadowWidth);
	Font.append_attribute("CharSpace",(long)m_FontCharSpace);
	Font.append_attribute("LineSpace",(long)m_FontLineSpace);
}
void CD3DWnd::SaveTextureToXML(xml_node& Texture)
{
	Texture.append_attribute("TextureFile",(LPCTSTR)m_pTexture->GetName());
	Texture.append_attribute("RectLeft",m_TextureRect.left);
	Texture.append_attribute("RectTop",m_TextureRect.top);
	Texture.append_attribute("RectRight",m_TextureRect.right);
	Texture.append_attribute("RectBottom",m_TextureRect.bottom);
}

void CD3DWnd::SaveBehaviorToXML(xml_node& Behavior)
{
	Behavior.append_attribute("Visible",m_IsVisible);
	Behavior.append_attribute("Enable",m_IsEnable);
	Behavior.append_attribute("CanDrag",m_CanDrag);
	Behavior.append_attribute("CanResize",m_CanResize);
	Behavior.append_attribute("CanGetFocus",m_CanGetFocus);
	Behavior.append_attribute("IsTopmost",m_IsTopmost);
	
}

void CD3DWnd::LoadChildsFromXml(xml_node& Childs)
{
	m_pGUI->LoadFromXML(&Childs,this);
}

void CD3DWnd::LoadFrameFromXML(xml_node& Frame)
{
	if(Frame.has_attribute("X"))
	{
		CEasyRect Rect(0,0,0,0);
		Rect.left=(long)Frame.attribute("X");
		Rect.top=(long)Frame.attribute("Y");
		Rect.right=Rect.left+(long)Frame.attribute("Width");
		Rect.bottom=Rect.top+(long)Frame.attribute("Height");
		SetRect(Rect);
	}
	if(Frame.has_attribute("BKColor"))
		SetBKColor((long)Frame.attribute("BKColor"));
}

void CD3DWnd::LoadBorderFromXML(xml_node& Borders)
{
	WIN_BORDERS WndBorders;

	WndBorders[RECT_TOP]=(long)Borders.attribute("Top");
	WndBorders[RECT_BOTTOM]=(long)Borders.attribute("Bottom");
	WndBorders[RECT_LEFT]=(long)Borders.attribute("Left");
	WndBorders[RECT_RIGHT]=(long)Borders.attribute("Right");
	WndBorders[RECT_TOP_LEFT]=(long)Borders.attribute("TopLeft");
	WndBorders[RECT_TOP_RIGHT]=(long)Borders.attribute("TopRight");
	WndBorders[RECT_BOTTOM_LEFT]=(long)Borders.attribute("BottomLeft");
	WndBorders[RECT_BOTTOM_RIGHT]=(long)Borders.attribute("BottomRight");

	SetBorders(WndBorders);
}

void CD3DWnd::LoadTextFromXML(xml_node& Text)
{
	CEasyString WndText=Text.attribute("Text").getvalue().c_str();
	WndText.Replace("[&RetuRn]","\r\n");
	WndText.Replace("[&InveRtedComMa]","\"");
	SetText(WndText);
}

void CD3DWnd::LoadFontFromXML(xml_node& Font)
{
	LOGFONT LogFont;
	ZeroMemory(&LogFont,sizeof(LogFont));

	if(Font.has_attribute("FaceName"))
	{
		strncpy_0(LogFont.lfFaceName,LF_FACESIZE,Font.attribute("FaceName").getvalue().c_str(),LF_FACESIZE);
		LogFont.lfHeight=(long)Font.attribute("Height");
		LogFont.lfWidth=(long)Font.attribute("Width");
		LogFont.lfWeight=(long)Font.attribute("Weight");
		LogFont.lfCharSet=(long)Font.attribute("CharSet");
		LogFont.lfItalic=(bool)Font.attribute("Italic");
		LogFont.lfOutPrecision=(long)Font.attribute("OutPrecision");
		LogFont.lfPitchAndFamily=(long)Font.attribute("PitchAndFamily");
		LogFont.lfQuality=(long)Font.attribute("Quality");
		SetFont(&LogFont);
	}
	if(Font.has_attribute("FontColor"))
		SetFontColor((long)Font.attribute("FontColor"));
	if(Font.has_attribute("Align"))
		SetFontAlign((long)Font.attribute("Align"));
	if(Font.has_attribute("ShadowMode"))
		SetFontShadowMode((long)Font.attribute("ShadowMode"));
	if(Font.has_attribute("ShadowColor"))
		SetFontShadowColor((long)Font.attribute("ShadowColor"));
	if(Font.has_attribute("ShadowWidth"))
		SetFontShadowWidth((long)Font.attribute("ShadowWidth"));
	if(Font.has_attribute("CharSpace"))
		SetFontCharSpace((long)Font.attribute("CharSpace"));
	if(Font.has_attribute("LineSpace"))
		SetFontLineSpace((long)Font.attribute("LineSpace"));
}

void CD3DWnd::LoadTextureFromXML(xml_node& Texture)
{	
	CEasyString TexFile;
	CEasyRect Rect;
	TexFile=Texture.attribute("TextureFile").getvalue().c_str();
	Rect.left=Texture.attribute("RectLeft");
	Rect.top=Texture.attribute("RectTop");
	Rect.right=Texture.attribute("RectRight");
	Rect.bottom=Texture.attribute("RectBottom");
	SetTexture(TexFile,Rect);
}

void CD3DWnd::LoadBehaviorFromXML(xml_node& Behavior)
{
	if(Behavior.has_attribute("Visible"))
		SetVisible((bool)Behavior.attribute("Visible"));
	if(Behavior.has_attribute("Enable"))
		EnableWindow((bool)Behavior.attribute("Enable"));
	if(Behavior.has_attribute("CanDrag"))
		EnableDrag((bool)Behavior.attribute("CanDrag"));
	if(Behavior.has_attribute("CanResize"))
		EnableResize((bool)Behavior.attribute("CanResize"));
	if(Behavior.has_attribute("CanGetFocus"))
		EnableFocus((bool)Behavior.attribute("CanGetFocus"));
	if(Behavior.has_attribute("IsTopmost"))
		SetTopmost((bool)Behavior.attribute("IsTopmost"));

	
}
 
bool CD3DWnd::AddChildWnd(CD3DWnd * child)
{	
	for(int i=0;i<(int)m_ChildWndList.size();i++)
	{
		if(m_ChildWndList[i]==child)
			return false;
	}
	m_ChildWndList.push_back(child);
	child->UpdateRects();
	child->TopTo(this);
	return true;
}

bool CD3DWnd::DelChildWnd(CD3DWnd * child)
{
	for(int i=0;i<(int)m_ChildWndList.size();i++)
	{
		
		if(m_ChildWndList[i]==child)
		{
			m_ChildWndList.erase(m_ChildWndList.begin()+i);
			return true;
		}
	}
	return true;
}

bool CD3DWnd::UpdateFont()
{
	if(m_WantUpdateFont)
	{		
		IUITextRect * pTextRect=GetTextRect();
		if(pTextRect)		
		{
			pTextRect->EnableUpdate(false);
			pTextRect->SetFont(&m_LogFont);
			pTextRect->SetColor(m_FontColor);
			pTextRect->SetAlign(m_FontAlign);
			pTextRect->SetShadowMode(m_FontShadowMode);
			pTextRect->SetShadowColor(m_FontShadowColor);
			pTextRect->SetShadowWidth(m_FontShadowWidth);		
			pTextRect->SetCharSpace(m_FontCharSpace);
			pTextRect->SetLineSpace(m_FontLineSpace);
			m_FontCharSpace=pTextRect->GetCharSpace();
			m_FontLineSpace=pTextRect->GetLineSpace();
			pTextRect->EnableUpdate(true);
		}
		m_WantUpdateFont=false;
		return true;
	}
	return false;
}

void CD3DWnd::UpdateChildRects()
{
	for(int i=0;i<(int)m_ChildWndList.size();i++)
	{		
		m_ChildWndList[i]->UpdateRects();
	}
}

void CD3DWnd::TopTo(CD3DWnd * Before)
{
	IUIBaseRect * pBottomRect;
	if(Before)
		pBottomRect=Before->GetTopRect();
	else
		pBottomRect=NULL;
	for(int i=0;i<m_WndRectCount;i++)
	{
		if(m_pWndRects[i])
		{
			m_pWndRects[i]->TopTo(pBottomRect);
			pBottomRect=m_pWndRects[i];
		}
	}	
	m_pGUI->LeftWndToTop(this,Before);
	TopChild();
}

IUIBaseRect * CD3DWnd::GetTopRect()
{
	for(int i=m_WndRectCount-1;i>=0;i--)
	{
		if(m_pWndRects[i])
			return m_pWndRects[i];
	}
	return NULL;
}

void CD3DWnd::TopChild()
{
	for(int i=0;i<(int)m_ChildWndList.size();i++)
	{		
		m_ChildWndList[i]->TopTo(this);
	}
}


void CD3DWnd::SetVisible(bool IsVisible)
{
	if(m_IsVisible!=IsVisible)
	{	
		m_IsVisible=IsVisible;
		for(int i=0;i<m_WndRectCount;i++)
		{
			if(m_pWndRects[i])
				m_pWndRects[i]->SetVisible(IsVisible);
		}
		if(m_IsVisible)
		{
			UpdateRects();
			UpdateText();
			UpdateFont();
		}
		HandleMessage(this,WM_D3DGUI_VISIBLE_CHANGE,(WPARAM)m_IsVisible,0);
		for(int i=0;i<(int)m_ChildWndList.size();i++)
		{
			m_ChildWndList[i]->SetVisible(IsVisible);
		}
	}
}

void CD3DWnd::SetCaptureAll(bool IsCaptureAll)
{
	m_IsCaptureAll=IsCaptureAll;
	if(IsCaptureAll)
		m_pGUI->SetCaptureAllWnd(this);
	else
		m_pGUI->SetCaptureAllWnd(NULL);
}

void CD3DWnd::SetFocus()
{
	m_pGUI->ActiveWnd(this,true);
}

IUIWndRect * CD3DWnd::CreateRect()
{
	IUIWndRect * pWndRect=m_pGUI->GetObjectCreator()->CreateWndRect(NULL);
	if(pWndRect)
	{
		pWndRect->SetColor(m_BKColor);	
		pWndRect->SetVisible(m_IsVisible);
	}
	return pWndRect;
}

IUITextRect * CD3DWnd::CreateTextRect()
{
	IUITextRect * pText=m_pGUI->GetObjectCreator()->CreateTextRect(NULL,NULL);
	
	if(pText)
	{
		pText->EnableUpdate(false);
		pText->SetFont(&m_LogFont);
		pText->SetColor(m_FontColor);
		pText->SetAlign(m_FontAlign);
		pText->SetShadowMode(m_FontShadowMode);
		pText->SetShadowColor(m_FontShadowColor);
		pText->SetShadowWidth(m_FontShadowWidth);	
		pText->SetCharSpace(m_FontCharSpace);
		pText->SetLineSpace(m_FontLineSpace);
		pText->EnableUpdate(true);
		pText->SetVisible(m_IsVisible);
	}	
	return pText;
}

void CD3DWnd::RebuildOrder()
{
	int BottomIndex=0;
	for(int i=0;i<m_WndRectCount;i++)
	{
		if(m_pWndRects[i])
		{
			BottomIndex=i;
			break;
		}
	}
	for(int i=BottomIndex+1;i<m_WndRectCount;i++)
	{
		if(m_pWndRects[i])
		{
			m_pWndRects[i]->TopTo(m_pWndRects[BottomIndex]);
			BottomIndex=i;
		}
	}		
	TopChild();
}

void CD3DWnd::TopParent()
{
	if(m_pParentWnd)
		m_pParentWnd->TopParent();
	TopTo();
}

void CD3DWnd::GetMiniSize(int& Width,int& Height)
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

	Width=LMinWidth+RMinWidth;
	Height=m_Borders[RECT_TOP]+m_Borders[RECT_BOTTOM];


	
}

CD3DWnd * CD3DWnd::GetChildWndByID(int ID)
{
	for(int i=0;i<(int)m_ChildWndList.size();i++)
	{
		if(m_ChildWndList[i]->GetID()==ID)
			return m_ChildWndList[i];
	}
	return NULL;
}
CD3DWnd * CD3DWnd::GetChildWndByName(LPCTSTR Name)
{
	for(int i=0;i<(int)m_ChildWndList.size();i++)
	{
		if(strcmp(m_ChildWndList[i]->GetName(),Name)==0)
			return m_ChildWndList[i];
	}
	return NULL;
}

bool CD3DWnd::IsChild(CD3DWnd * pWnd)
{
	for(int i=0;i<(int)m_ChildWndList.size();i++)
	{
		if(m_ChildWndList[i]==pWnd)
			return true;
	}
	return false;
}

bool CD3DWnd::IsChildRecursion(CD3DWnd * pWnd)
{
	for(int i=0;i<(int)m_ChildWndList.size();i++)
	{
		if(m_ChildWndList[i]==pWnd)
			return true;
	}
	for(int i=0;i<(int)m_ChildWndList.size();i++)
	{
		if(m_ChildWndList[i]->IsChildRecursion(pWnd))
			return true;
	}
	return false;
}

IUITextRect * CD3DWnd::GetTextRect()
{
	if(m_pWndRects[RECT_TEXT]==NULL)
	{
		UpdateText();		
	}
	return (IUITextRect *)m_pWndRects[RECT_TEXT];
}

//CNameObject::STORAGE_STRUCT * CD3DWnd::USOCreateHead(UINT Param)
//{
//	STORAGE_STRUCT * pHead=new STORAGE_STRUCT;
//	ZeroMemory(pHead,sizeof(STORAGE_STRUCT));
//	pHead->Size=sizeof(STORAGE_STRUCT);
//	return pHead;
//}
//
//int CD3DWnd::USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	int HeadSize=CNameObject::USOWriteHead(pHead,pUSOFile,Param);
//	if(HeadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	
//	pug::xml_parser Xml;
//
//	Xml.create();
//	xml_node Doc;
//	Doc = Xml.document();	
//	
//	SaveToXml(&Doc);	
//
//	std::strstream os;		
//
//	os << Xml.document();
//	os<<'\0';
//	os.freeze();
//
//	m_TempScript=os.str();
//
//	m_TempScript.Replace("\n","\r\n");
//
//	
//	pLocalHead->ScriptStrLen=m_TempScript.GetLength()+1;
//	pLocalHead->Size+=pLocalHead->ScriptStrLen;	
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//bool CD3DWnd::USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	if(!CNameObject::USOWriteData(pHead,pUSOFile,Param))
//		return false;
//
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	if(!m_TempScript.IsEmpty())
//	{
//		UINT WriteSize=(UINT)pFile->Write((LPSTR)((LPCTSTR)m_TempScript),pLocalHead->ScriptStrLen);
//		m_TempScript.Clear();
//		if(WriteSize<pLocalHead->ScriptStrLen)
//			return false;
//	}
//	return true;
//}
//
//int CD3DWnd::USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{	
//	int ReadSize=CNameObject::USOReadHead(pHead,pUSOFile,Param);
//	if(ReadSize<0)
//		return -1;	
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//int CD3DWnd::USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param)
//{
//	int ReadSize=CNameObject::USOReadData(pHead,pUSOFile,pData,DataSize,Param);
//
//	pData+=ReadSize;
//	DataSize-=ReadSize;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	if(pLocalHead->ScriptStrLen)
//	{
//		pug::xml_parser Xml;
//		Xml.parse((TCHAR *)pData,pug::parse_trim_attribute);	
//		xml_node Doc=Xml.document();
//
//		if(Doc.children()>0)
//		{
//			if(LoadFromXml(&Doc.child(0)))
//			{	
//				return -1;
//			}
//		}
//
//		ReadSize+=pLocalHead->ScriptStrLen;
//	}
//	return ReadSize;
//}

}