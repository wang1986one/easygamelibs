/****************************************************************************/
/*                                                                          */
/*      文件名:    ObjectPropertyView.cpp                                   */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// ObjectPropertyView.cpp : 实现文件
//

#include "stdafx.h"
#include "D3DUIEdit.h"
#include "D3DUIEditDoc.h"
#include "ObjectPropertyView.h"
#include ".\objectpropertyview.h"

enum GUI_OBJECT_PROPERTY
{
	GUIOP_NONE,
	GUIOP_NAME,
	GUIOP_ID,
	GUIOP_X,
	GUIOP_Y,
	GUIOP_WIDTH,
	GUIOP_HEIGHT,		
	GUIOP_FONT,
	GUIOP_BKCOLOR,
	GUIOP_TEXT,
	GUIOP_PARENT,
	GUIOP_VISIBLE,
	GUIOP_ENABLE,
	GUIOP_CANRESIZE,
	GUIOP_CANDRAG,
	GUIOP_CANGETFOCUS,
	GUIOP_ISTOPMOST,
	GUIOP_TAB_INDEX,
	GUIOP_IS_TAB_CONTAINER,
	GUIOP_WND_TEXTURE,
	GUIOP_BUTTON_TEXTURE,
	GUIOP_EDIT_TEXTURE,
	GUIOP_EDIT_MULTILINE,
	GUIOP_EDIT_SHOWCARET,
	GUIOP_EDIT_CARETCOLOR,
	GUIOP_EDIT_AUTOWRAP,
	GUIOP_EDIT_READONLY,
	GUIOP_EDIT_SCROLLBAR,
	GUIOP_EDIT_SCROLLBARWIDTH,
	GUIOP_EDIT_ENABLEIME,
	GUIOP_EDIT_IS_ENCRYPTION,
	GUIOP_SLIST_TEXTURE,
	GUIOP_SLIST_LINESPACE,
	GUIOP_SLIST_MULTISELECT,
	GUIOP_SLIST_SCROLLBARWIDTH,
	GUIOP_SLIST_LOOPSCROLL,
	GUIOP_COMBO_TEXTURE,
	GUIOP_COMBO_LIST_HEIGHT,
	GUIOP_SCROLL_TEXTURE,
	GUIOP_SCROLL_STYLE,	
	GUIOP_SCROLL_RANGE,
	GUIOP_PROGRESS_STYLE,
	GUIOP_PROGRESS_WORKMODE,
	GUIOP_PROGRESS_POS,
	GUIOP_PROGRESS_MAXPOS,
	GUIOP_PROGRESS_BAR_TEXTURE,
	

};

// CObjectPropertyView

IMPLEMENT_DYNCREATE(CObjectPropertyView, CView)

CObjectPropertyView::CObjectPropertyView()
{	
	m_CurObject=NULL;	
}

CObjectPropertyView::~CObjectPropertyView()
{
}

BEGIN_MESSAGE_MAP(CObjectPropertyView, CView)
	ON_WM_SIZE()
	ON_MESSAGE(WM_PROPERTY_GRID_VALUE_CHANGED,OnPropertyChanged)
END_MESSAGE_MAP()


// CObjectPropertyView 绘图

void CObjectPropertyView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CObjectPropertyView 诊断

#ifdef _DEBUG
void CObjectPropertyView::AssertValid() const
{
	CView::AssertValid();
}

void CObjectPropertyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CObjectPropertyView 消息处理程序

void CObjectPropertyView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类

	CRect Rect;
	GetClientRect(&Rect);
	m_PropertyGrid.Create(Rect,this);
	m_PropertyGrid.ShowWindow(SW_SHOW);		
}

void CObjectPropertyView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(::IsWindow(m_PropertyGrid.GetSafeHwnd()))
	{
		RECT rect;

		GetClientRect(&rect);
		m_PropertyGrid.MoveWindow(&rect);
	}
}

void CObjectPropertyView::SetGUIObject(CD3DWnd * pObject)
{

	if(pObject&&pObject->IsKindOf(GET_CLASS_INFO(CD3DSizeBar)))
	{
		m_CurObject=((CD3DSizeBar *)pObject)->GetChild();
	}
	else
	{
		m_CurObject=pObject;
	}
	if(m_CurObject)
	{
		m_DlgWndTextureConfig.SetGUIObject(m_CurObject);
		m_DlgButtonTextureConfig.SetGUIObject(m_CurObject);
		m_DlgFontConfig.SetGUIObject(m_CurObject);
		m_DlgScrollBarTextureConfig.SetGUIObject(m_CurObject);
		m_DlgEditTextureConfig.SetGUIObject(m_CurObject);
		m_DlgSimpleListTextureConfig.SetGUIObject(m_CurObject);
		m_DlgComboTextureConfig.SetGUIObject(m_CurObject);
		m_DlgProgressBarTextureConfig.SetGUIObject(m_CurObject);
	}
	RefreshObjectProperty();
}

void CObjectPropertyView::RefreshObjectProperty()
{
	m_PropertyGrid.Clear();
	if(m_CurObject==NULL)
		return;

	CD3DWnd * pWnd=m_CurObject;

	if(pWnd==NULL)
		return;

	CD3DSizeBar * pSizeBar=NULL;

	if(m_CurObject->GetParent())
	{
		if(m_CurObject->GetParent()->IsKindOf(GET_CLASS_INFO(CD3DSizeBar)))
			pSizeBar=(CD3DSizeBar *)m_CurObject->GetParent();
	}

	PROPERTY_INFO info;	
	CEasyString Temp;
	CRect Rect;

	if(pSizeBar)
	{
		Rect=pSizeBar->GetCenterRect();
	}
	else
	{
		Rect=pWnd->GetRect();
	}
	

	m_PropertyGrid.SetRedraw(false);

	if(!pWnd->IsInternal())
	{
		info.PropertyType=PROT_EDIT;
		info.Name="名称";
		info.ID=GUIOP_NAME;
		info.Value=pWnd->GetName();
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_NUMBER;
		info.Name="ID";
		info.ID=GUIOP_ID;
		info.Value=pWnd->GetID();
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_NUMBER;
		info.Name="X";
		info.ID=GUIOP_X;
		info.Value=Rect.left;
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_NUMBER;
		info.Name="Y";
		info.ID=GUIOP_Y;
		info.Value=Rect.top;
		m_PropertyGrid.AddProperty(&info);	
	}

	info.PropertyType=PROT_NUMBER;
	info.Name="宽度";
	info.ID=GUIOP_WIDTH;
	info.Value=Rect.Width();
	m_PropertyGrid.AddProperty(&info);

	info.PropertyType=PROT_NUMBER;
	info.Name="高度";
	info.ID=GUIOP_HEIGHT;
	info.Value=Rect.Height();
	m_PropertyGrid.AddProperty(&info);
	

	info.PropertyType=PROT_CUSTOM;
	info.Name="字体";
	info.ID=GUIOP_FONT;
	info.Value="<编辑>";
	info.pConfigDialog=&m_DlgFontConfig;
	m_PropertyGrid.AddProperty(&info);

	info.PropertyType=PROT_CUSTOM;
	info.Name="背景色";
	info.ID=GUIOP_BKCOLOR;
	info.Value="<编辑>";
	info.pConfigDialog=&m_DlgColorConfig;
	m_DlgColorConfig.m_D3DColor=pWnd->GetBKColor();
	m_PropertyGrid.AddProperty(&info);

	info.PropertyType=PROT_EDIT;
	info.Name="文本";
	info.ID=GUIOP_TEXT;
	pWnd->GetText(Temp);
	info.Value=Temp;
	m_PropertyGrid.AddProperty(&info);

	info.PropertyType=PROT_NONE;
	info.Name="父窗口";
	info.ID=GUIOP_PARENT;
	if(pWnd->GetParent()->GetParent())
		info.Value=pWnd->GetParent()->GetParent()->GetName();
	else
		info.Value="无";
	m_PropertyGrid.AddProperty(&info);

	if(pWnd->IsInternal())
	{
		info.PropertyType=PROT_NONE;
		info.Name="可见性";
		info.ID=GUIOP_VISIBLE;
		info.Value=pWnd->IsVisible();
		m_PropertyGrid.AddProperty(&info);
	}
	else
	{
		info.PropertyType=PROT_BOOL;
		info.Name="可见性";
		info.ID=GUIOP_VISIBLE;
		info.Value=pWnd->IsVisible();
		m_PropertyGrid.AddProperty(&info);


		info.PropertyType=PROT_BOOL;
		info.Name="可激活";
		info.ID=GUIOP_ENABLE;
		info.Value=pWnd->IsEnable();
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_BOOL;
		info.Name="可缩放";
		info.ID=GUIOP_CANRESIZE;
		info.Value=pWnd->CanResize();
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_BOOL;
		info.Name="可拖动";
		info.ID=GUIOP_CANDRAG;
		info.Value=pWnd->CanDrag();
		m_PropertyGrid.AddProperty(&info);	

		info.PropertyType=PROT_BOOL;
		info.Name="可获得焦点";
		info.ID=GUIOP_CANGETFOCUS;
		info.Value=pWnd->CanGetFocus();
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_BOOL;
		info.Name="总是在前面";
		info.ID=GUIOP_ISTOPMOST;
		info.Value=pWnd->IsTopmost();
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_NUMBER;
		info.Name="Tab顺序";
		info.ID=GUIOP_TAB_INDEX;
		info.Value=pWnd->GetTabIndex();
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_BOOL;
		info.Name="Tab容器";
		info.ID=GUIOP_IS_TAB_CONTAINER;
		info.Value=pWnd->IsTabContainer();
		m_PropertyGrid.AddProperty(&info);
		
	}
	

	

	
	if(pWnd->GetClassInfo()==GET_CLASS_INFO(CD3DWnd))
	{
		info.PropertyType=PROT_CUSTOM;
		info.Name="纹理";
		info.ID=GUIOP_WND_TEXTURE;
		info.Value="<编辑>";
		info.pConfigDialog=&m_DlgWndTextureConfig;		
		m_PropertyGrid.AddProperty(&info);		
	}
	if(pWnd->GetClassInfo()==GET_CLASS_INFO(CD3DButton))
	{
		info.PropertyType=PROT_CUSTOM;
		info.Name="纹理";
		info.ID=GUIOP_BUTTON_TEXTURE;
		info.Value="<编辑>";
		info.pConfigDialog=&m_DlgButtonTextureConfig;
		m_PropertyGrid.AddProperty(&info);
	}
	if(pWnd->GetClassInfo()==GET_CLASS_INFO(CD3DEdit))
	{
		info.PropertyType=PROT_CUSTOM;
		info.Name="纹理";
		info.ID=GUIOP_EDIT_TEXTURE;
		info.Value="<编辑>";
		info.pConfigDialog=&m_DlgEditTextureConfig;
		m_PropertyGrid.AddProperty(&info);
	}
	if(pWnd->GetClassInfo()==GET_CLASS_INFO(CD3DCombo))
	{
		info.PropertyType=PROT_CUSTOM;
		info.Name="纹理";
		info.ID=GUIOP_COMBO_TEXTURE;
		info.Value="<编辑>";
		info.pConfigDialog=&m_DlgComboTextureConfig;
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_NUMBER;
		info.Name="列表高度";
		info.ID=GUIOP_COMBO_LIST_HEIGHT;
		info.Value=((CD3DCombo *)pWnd)->GetComboListHeight();
		m_PropertyGrid.AddProperty(&info);

		

	}
	if(pWnd->IsKindOf(GET_CLASS_INFO(CD3DEdit)))
	{

		info.PropertyType=PROT_BOOL;
		info.Name="多行文本";
		info.ID=GUIOP_EDIT_MULTILINE;
		info.Value=((CD3DEdit *)pWnd)->IsMultiLine();
		m_PropertyGrid.AddProperty(&info);	

		info.PropertyType=PROT_BOOL;
		info.Name="启用光标";
		info.ID=GUIOP_EDIT_SHOWCARET;
		info.Value=((CD3DEdit *)pWnd)->IsCaretShow();
		m_PropertyGrid.AddProperty(&info);	

		info.PropertyType=PROT_CUSTOM;
		info.Name="光标颜色";
		info.ID=GUIOP_EDIT_CARETCOLOR;
		info.Value="<编辑>";
		info.pConfigDialog=&m_DlgColorConfig;
		m_DlgColorConfig.m_D3DColor=((CD3DEdit *)pWnd)->GetCaretColor();
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_BOOL;
		info.Name="自动换行";
		info.ID=GUIOP_EDIT_AUTOWRAP;
		info.Value=((CD3DEdit *)pWnd)->IsAutoWrap();
		m_PropertyGrid.AddProperty(&info);
		
		info.PropertyType=PROT_BOOL;
		info.Name="只读";
		info.ID=GUIOP_EDIT_READONLY;
		info.Value=((CD3DEdit *)pWnd)->IsReadyOnly();
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_BOOL;
		info.Name="加密显示";
		info.ID=GUIOP_EDIT_IS_ENCRYPTION;
		info.Value=((CD3DEdit *)pWnd)->IsEncryption();
		m_PropertyGrid.AddProperty(&info);
	

		info.PropertyType=PROT_BOOL;
		info.Name="开启滚动条";
		info.ID=GUIOP_EDIT_SCROLLBAR;
		info.Value=((CD3DEdit *)pWnd)->IsScrollBarEnabled();
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_NUMBER;
		info.Name="滚动条宽度";
		info.ID=GUIOP_EDIT_SCROLLBARWIDTH;
		info.Value=((CD3DEdit *)pWnd)->GetScrollBarWidth();
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_BOOL;
		info.Name="是否支持输入法";
		info.ID=GUIOP_EDIT_ENABLEIME;
		info.Value=((CD3DEdit *)pWnd)->IsIMEEnabled();
		m_PropertyGrid.AddProperty(&info);
	}
	if(pWnd->GetClassInfo()==GET_CLASS_INFO(CD3DSimpleList))
	{
		info.PropertyType=PROT_CUSTOM;
		info.Name="纹理";
		info.ID=GUIOP_SLIST_TEXTURE;
		info.Value="<编辑>";
		info.pConfigDialog=&m_DlgSimpleListTextureConfig;
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_NUMBER;
		info.Name="行间距";
		info.ID=GUIOP_SLIST_LINESPACE;
		info.Value=((CD3DSimpleList *)pWnd)->GetLineSpace();
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_BOOL;
		info.Name="多重选择";
		info.ID=GUIOP_SLIST_MULTISELECT;
		info.Value=((CD3DSimpleList *)pWnd)->IsAllowMutliSelect();
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_NUMBER;
		info.Name="滚动条宽度";
		info.ID=GUIOP_SLIST_SCROLLBARWIDTH;
		info.Value=((CD3DSimpleList *)pWnd)->GetScrollBarWidth();
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_BOOL;
		info.Name="循环滚动";
		info.ID=GUIOP_SLIST_LOOPSCROLL;
		info.Value=((CD3DSimpleList *)pWnd)->IsLoopScroll();
		m_PropertyGrid.AddProperty(&info);

		
	}	
	if(pWnd->GetClassInfo()==GET_CLASS_INFO(CD3DScrollBar))
	{
		info.PropertyType=PROT_CUSTOM;
		info.Name="纹理";
		info.ID=GUIOP_SCROLL_TEXTURE;
		info.Value="<编辑>";
		info.pConfigDialog=&m_DlgScrollBarTextureConfig;
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_COMBO;
		info.Name="类型";
		info.ID=GUIOP_SCROLL_STYLE;
		info.Define="[Define]\r\nItemCount=2\r\nItem00=纵向\r\nItem01=横向\r\n";
		if(((CD3DScrollBar *)pWnd)->GetScrollStyle()==SCROLL_BAR_VERTICAL)
			info.Value="纵向";
		else
			info.Value="横向";
		m_PropertyGrid.AddProperty(&info);		

		info.PropertyType=PROT_NUMBER;
		info.Name="滚动范围";
		info.ID=GUIOP_SCROLL_RANGE;
		info.Value=((CD3DScrollBar *)pWnd)->GetMaxScrollPos();
		m_PropertyGrid.AddProperty(&info);	
	}
	if(pWnd->GetClassInfo()==GET_CLASS_INFO(CD3DProgressBar))
	{
		

		info.PropertyType=PROT_COMBO;
		info.Name="类型";
		info.ID=GUIOP_PROGRESS_STYLE;
		info.Define="[Define]\r\nItemCount=2\r\nItem00=纵向\r\nItem01=横向\r\n";
		if(((CD3DProgressBar *)pWnd)->GetProgressStyle()==SCROLL_BAR_VERTICAL)
			info.Value="纵向";
		else
			info.Value="横向";
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_COMBO;
		info.Name="工作模式";
		info.ID=GUIOP_PROGRESS_WORKMODE;
		info.Define="[Define]\r\nItemCount=2\r\nItem00=拉伸\r\nItem01=剪切\r\n";
		if(((CD3DProgressBar *)pWnd)->GetWorkMode()==PBW_STRETCH)
			info.Value="拉伸";
		else
			info.Value="剪切";
		m_PropertyGrid.AddProperty(&info);	

		info.PropertyType=PROT_NUMBER;
		info.Name="当前进度";
		info.ID=GUIOP_PROGRESS_POS;
		info.Value=((CD3DProgressBar *)pWnd)->GetPos();
		m_PropertyGrid.AddProperty(&info);	

		info.PropertyType=PROT_NUMBER;
		info.Name="最大进度";
		info.ID=GUIOP_PROGRESS_MAXPOS;
		info.Value=((CD3DProgressBar *)pWnd)->GetMaxPos();
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_CUSTOM;
		info.Name="纹理";
		info.ID=GUIOP_PROGRESS_BAR_TEXTURE;
		info.Value="<编辑>";
		info.pConfigDialog=&m_DlgProgressBarTextureConfig;		
		m_PropertyGrid.AddProperty(&info);


	}
	m_PropertyGrid.SetRedraw(true);
}

void CObjectPropertyView::UpdateObjectRect()
{
	if(m_CurObject==NULL)
		return;

	CD3DWnd * pWnd=m_CurObject;

	if(pWnd==NULL)
		return;

	CD3DSizeBar * pSizeBar=NULL;

	if(m_CurObject->GetParent())
	{
		if(m_CurObject->GetParent()->IsKindOf(GET_CLASS_INFO(CD3DSizeBar)))
			pSizeBar=(CD3DSizeBar *)m_CurObject->GetParent();
	}

	if(pSizeBar==NULL)
		return;

	PROPERTY_INFO info;	
	CRect Rect;

	Rect=pSizeBar->GetCenterRect();
	

	info.PropertyType=PROT_NUMBER;
	info.Name="X";
	info.ID=GUIOP_X;
	info.Value=Rect.left;
	m_PropertyGrid.SetProperty(&info);

	info.PropertyType=PROT_NUMBER;
	info.Name="Y";
	info.ID=GUIOP_Y;
	info.Value=Rect.top;
	m_PropertyGrid.SetProperty(&info);

	info.PropertyType=PROT_NUMBER;
	info.Name="宽度";
	info.ID=GUIOP_WIDTH;
	info.Value=Rect.Width();
	m_PropertyGrid.SetProperty(&info);

	info.PropertyType=PROT_NUMBER;
	info.Name="高度";
	info.ID=GUIOP_HEIGHT;
	info.Value=Rect.Height();
	m_PropertyGrid.SetProperty(&info);
}

void CObjectPropertyView::UpdateObjectText()
{
	if(m_CurObject==NULL)
		return;

	CD3DWnd * pWnd=m_CurObject;

	if(pWnd==NULL)
		return;

	PROPERTY_INFO info;
	CEasyString Temp;

	info.PropertyType=PROT_EDIT;
	info.Name="文本";
	info.ID=GUIOP_TEXT;
	pWnd->GetText(Temp);
	info.Value=Temp;
	m_PropertyGrid.SetProperty(&info);
}

LRESULT CObjectPropertyView::OnPropertyChanged( WPARAM wParam, LPARAM lParam)
{
	PROPERTY_CHANGE_INFO * pChangeInfo=(PROPERTY_CHANGE_INFO *)lParam;
	PROPERTY_INFO * pInfo=pChangeInfo->pInfo;

	if(m_CurObject==NULL)
		return TRUE;

	CD3DWnd * pWnd=m_CurObject;

	if(pWnd==NULL)
		return TRUE;

	CD3DSizeBar * pSizeBar=NULL;

	if(m_CurObject->GetParent())
	{
		if(m_CurObject->GetParent()->IsKindOf(GET_CLASS_INFO(CD3DSizeBar)))
			pSizeBar=(CD3DSizeBar *)m_CurObject->GetParent();
	}

	
	switch(pInfo->ID)
	{	
	case GUIOP_NAME:		
		if(pSizeBar)
		{
			pWnd->SetName(pInfo->Value);
			pSizeBar->UpdateTreeItem();
		}
		break;
	case GUIOP_ID:
		if(pSizeBar)
		{
			pWnd->SetID(pInfo->Value);
		}
		break;
	case GUIOP_X:
		if(pSizeBar)
		{
			CRect Rect=pSizeBar->GetCenterRect();
			int dx=(int)(pInfo->Value)-Rect.left;
			Rect=pSizeBar->GetRect();
			Rect.left+=dx;
			Rect.right+=dx;
			pSizeBar->SetRect(Rect);
			pSizeBar->UpdateSizeToChild();
		}
		break;
	case GUIOP_Y:
		if(pSizeBar)
		{
			CRect Rect=pSizeBar->GetCenterRect();
			int dy=(int)(pInfo->Value)-Rect.top;
			Rect=pSizeBar->GetRect();
			Rect.top+=dy;
			Rect.bottom+=dy;
			pSizeBar->SetRect(Rect);
			pSizeBar->UpdateSizeToChild();
		}
		break;
	case GUIOP_WIDTH:
		if(pSizeBar)
		{
			CRect Rect=pSizeBar->GetCenterRect();
			int Border=Rect.Width();
			Rect=pSizeBar->GetRect();			
			Border=Rect.Width()-Border;
			Rect.right=Rect.left+(int)(pInfo->Value)+Border;
			pSizeBar->SetRect(Rect);
			pSizeBar->UpdateSizeToChild();
			UpdateObjectRect();
		}
		break;
	case GUIOP_HEIGHT:
		if(pSizeBar)
		{
			CRect Rect=pSizeBar->GetCenterRect();
			int Border=Rect.Height();
			Rect=pSizeBar->GetRect();			
			Border=Rect.Height()-Border;	
			Rect.bottom=Rect.top+(int)(pInfo->Value)+Border;
			pSizeBar->SetRect(Rect);
			pSizeBar->UpdateSizeToChild();
			UpdateObjectRect();
		}
		else
		{
			CRect Rect=pWnd->GetRect();
			Rect.bottom=Rect.top+(int)(pInfo->Value);
			pWnd->SetRect(Rect);
			if(pWnd->GetParent()&&pWnd->GetParent()->IsKindOf(GET_CLASS_INFO(CD3DCombo)))
			{
				((CD3DCombo *)pWnd->GetParent())->SetComboListHeight(pInfo->Value);
			}
		}
		break;	
	}
	
	switch(pInfo->ID)
	{
	case GUIOP_FONT:
		break;
	case GUIOP_BKCOLOR:
		pWnd->SetBKColor(m_DlgColorConfig.m_D3DColor);
		break;
	case GUIOP_TEXT:
		pWnd->SetText(pInfo->Value);
		break;
	case GUIOP_PARENT:
		break;
	case GUIOP_VISIBLE:
		pWnd->SetVisible(pInfo->Value);
		break;
	case GUIOP_ENABLE:
		pWnd->EnableWindow(pInfo->Value);
		break;
	case GUIOP_CANRESIZE:
		pWnd->EnableResize(pInfo->Value);
		break;
	case GUIOP_CANDRAG:
		pWnd->EnableDrag(pInfo->Value);
		break;
	case GUIOP_CANGETFOCUS:
		pWnd->EnableFocus(pInfo->Value);
		break;
	case GUIOP_ISTOPMOST:
		pWnd->SetTopmost(pInfo->Value);
		break;
	case GUIOP_TAB_INDEX:
		pWnd->SetTabIndex(pInfo->Value);
		break;
	case GUIOP_IS_TAB_CONTAINER:
		pWnd->EnableTabContainer(pInfo->Value);
		break;
	case GUIOP_WND_TEXTURE:
		break;	
	case GUIOP_BUTTON_TEXTURE:
		break;
	case GUIOP_EDIT_TEXTURE:
		break;	
	case GUIOP_EDIT_MULTILINE:
		((CD3DEdit *)pWnd)->SetMultiLine(pInfo->Value);
		break;
	case GUIOP_EDIT_SHOWCARET:
		((CD3DEdit *)pWnd)->EnableCaret(pInfo->Value);
		break;
	case GUIOP_EDIT_CARETCOLOR:
		((CD3DEdit *)pWnd)->SetCaretColor(m_DlgColorConfig.m_D3DColor);
		break;
	case GUIOP_EDIT_AUTOWRAP:
		((CD3DEdit *)pWnd)->EnableAutoWrap(pInfo->Value);
		break;
	case GUIOP_EDIT_READONLY:
		((CD3DEdit *)pWnd)->SetReadOnly(pInfo->Value);
		break;
	case GUIOP_EDIT_SCROLLBAR:
		((CD3DEdit *)pWnd)->EnableScrollBar(pInfo->Value);
		break;
	case GUIOP_EDIT_SCROLLBARWIDTH:
		((CD3DEdit *)pWnd)->SetScrollBarWidth(pInfo->Value);
		break;
	case GUIOP_EDIT_ENABLEIME:
		((CD3DEdit *)pWnd)->EnableIME(pInfo->Value);
		break;
	case GUIOP_EDIT_IS_ENCRYPTION:
		((CD3DEdit *)pWnd)->SetEncryption(pInfo->Value);
		break;
	case GUIOP_SLIST_TEXTURE:
		break;	
	case GUIOP_SLIST_LINESPACE:
		((CD3DSimpleList *)pWnd)->SetLineSpace(pInfo->Value);
		break;
	case GUIOP_SLIST_MULTISELECT:
		((CD3DSimpleList *)pWnd)->SetAllowMutliSelect(pInfo->Value);
		break;
	case GUIOP_SLIST_SCROLLBARWIDTH:
		((CD3DSimpleList *)pWnd)->SetScrollBarWidth(pInfo->Value);
		break;
	case GUIOP_SLIST_LOOPSCROLL:
		((CD3DSimpleList *)pWnd)->EnableLoopScroll(pInfo->Value);
		break;
	case GUIOP_COMBO_TEXTURE:
		break;	
	case GUIOP_COMBO_LIST_HEIGHT:
		((CD3DCombo *)pWnd)->SetComboListHeight(pInfo->Value);
		break;
	case GUIOP_SCROLL_TEXTURE:
		break;	
	case GUIOP_SCROLL_STYLE:
		{
			if(((CString)pInfo->Value)=="纵向")
				((CD3DScrollBar *)pWnd)->SetScrollStyle(SCROLL_BAR_VERTICAL);
			else
				((CD3DScrollBar *)pWnd)->SetScrollStyle(SCROLL_BAR_HORIZONTAL);
		}
		break;	
	case GUIOP_SCROLL_RANGE:
		((CD3DScrollBar *)pWnd)->SetMaxScrollPos((int)(pInfo->Value));
		break;
	case GUIOP_PROGRESS_STYLE:
		{
			if(((CString)pInfo->Value)=="纵向")
				((CD3DProgressBar *)pWnd)->SetProgressStyle(PROGRESS_BAR_VERTICAL);
			else
				((CD3DProgressBar *)pWnd)->SetProgressStyle(PROGRESS_BAR_HORIZONTAL);
		}
		break;
	case GUIOP_PROGRESS_WORKMODE:
		{
			if(((CString)pInfo->Value)=="拉伸")
				((CD3DProgressBar *)pWnd)->SetWorkMode(PBW_STRETCH);
			else
				((CD3DProgressBar *)pWnd)->SetWorkMode(PBW_CUT);
		}
		break;
	case GUIOP_PROGRESS_POS:
		((CD3DProgressBar *)pWnd)->SetPos((int)(pInfo->Value));
		break;
	case GUIOP_PROGRESS_MAXPOS:
		((CD3DProgressBar *)pWnd)->SetMaxPos((int)(pInfo->Value));
		break;
	case GUIOP_PROGRESS_BAR_TEXTURE:
		break;
	}
	return TRUE;
}
