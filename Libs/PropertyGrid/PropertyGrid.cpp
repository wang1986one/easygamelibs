// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被改写。


#include "stdafx.h"
#include "PropertyGrid.h"

namespace PropertyGrid
{
/////////////////////////////////////////////////////////////////////////////
// CMSFlexGrid

IMPLEMENT_DYNCREATE(CPropertyGrid, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CMSFlexGrid 属性

/////////////////////////////////////////////////////////////////////////////
// CMSFlexGrid 操作

void CPropertyGrid::Init()
{
	SetAllowUserResizing(flexResizeColumns);
	SetScrollBars(flexScrollBarNone);		
	Clear();

	CDC * pDC=GetDC();
	int Height = MulDiv((int)GetFontSize(),pDC->GetDeviceCaps(LOGPIXELSY), 72);
	ReleaseDC(pDC);

	HFONT hFont=CreateFont(Height,0,0,0,FW_NORMAL ,GetFontItalic(),GetFontUnderline(),
		GetFontStrikethru(),DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,GetFontName());


	m_Inputer.Create(this,CFont::FromHandle(hFont));
	
}

void CPropertyGrid::AddProperty(PROPERTY_INFO * pInfo)
{	
	int Row=GetRows();
	SetRows(Row+1);
	SetRow(Row);
	SetCol(0);
	SetCellAlignment(flexAlignCenterCenter);
	SetText((LPCTSTR)pInfo->Name);
	SetCol(1);
	switch(pInfo->PropertyType)
	{
	case PROT_NUMBER:
	case PROT_SPIN_NUMBER:
		SetCellAlignment(flexAlignLeftCenter);
		break;
	default:
		SetCellAlignment(flexAlignLeftCenter);
		break;
	}
	pInfo->BindRow=Row;
	SetText((LPCTSTR)pInfo->Value);	
	SetColWidth(0,1000);
	SetColWidth(1,1600);
	m_PropertyList.Add(*pInfo);
}

void CPropertyGrid::SetProperty(PROPERTY_INFO * pInfo,bool AddWhileNotExist)
{
	PROPERTY_INFO * pPropertyInfo=GetPropertyByID(pInfo->ID);
	if(pPropertyInfo)
	{
		int Row=pPropertyInfo->BindRow;
		*pPropertyInfo=*pInfo;
		pPropertyInfo->BindRow=Row;
		int CurRow=GetRow();
		int CurCol=GetCol();
		SetRow(Row);
		SetCol(1);
		SetText((LPCTSTR)pPropertyInfo->Value);
		SetRow(CurRow);
		SetCol(CurCol);
	}
	else if(AddWhileNotExist)
	{
		AddProperty(pInfo);
	}
}

void CPropertyGrid::SetPropertyValue(int Index,CPropertyValue Value)
{
	if(Index>=0&&Index<m_PropertyList.GetCount())
	{
		m_PropertyList[Index].Value=Value;
		int CurRow=GetRow();
		int CurCol=GetCol();
		SetRow(m_PropertyList[Index].BindRow);
		SetCol(1);
		SetText(m_PropertyList[Index].Value.GetString());
		SetRow(CurRow);
		SetCol(CurCol);
	}
}

void CPropertyGrid::SetPropertyValueByID(int ID,CPropertyValue Value)
{
	for(int i=0;i<m_PropertyList.GetCount();i++)
	{
		if(m_PropertyList[i].ID==ID)
		{
			m_PropertyList[i].Value=Value;
			int CurRow=GetRow();
			int CurCol=GetCol();
			SetRow(m_PropertyList[i].BindRow);
			SetCol(1);
			SetText(m_PropertyList[i].Value.GetString());
			SetRow(CurRow);
			SetCol(CurCol);
		}
	}
}

void CPropertyGrid::SetPropertyValue(LPCTSTR Name,CPropertyValue Value)
{
	for(int i=0;i<m_PropertyList.GetCount();i++)
	{
		if(m_PropertyList[i].Name==Name)
		{
			m_PropertyList[i].Value=Value;
			int CurRow=GetRow();
			int CurCol=GetCol();
			SetRow(m_PropertyList[i].BindRow);
			SetCol(1);
			SetText(m_PropertyList[i].Value.GetString());
			SetRow(CurRow);
			SetCol(CurCol);
		}
	}
}

PROPERTY_INFO * CPropertyGrid::GetPropertyByName(LPCTSTR Name)
{
	for(int i=0;i<m_PropertyList.GetCount();i++)
	{
		if(m_PropertyList[i].Name==Name)
			return &(m_PropertyList[i]);
	}
	return NULL;
}

PROPERTY_INFO * CPropertyGrid::GetPropertyByID(int ID)
{
	for(int i=0;i<m_PropertyList.GetCount();i++)
	{
		if(m_PropertyList[i].ID==ID)
			return &(m_PropertyList[i]);
	}
	return NULL;
}


void CPropertyGrid::OnFinishEdit(bool IsChanged)
{
	if(m_CurEditRow<=0||m_CurEditRow>m_PropertyList.GetCount())
		return;
	PROPERTY_INFO * pInfo=&(m_PropertyList[m_CurEditRow-1]);

	PROPERTY_CHANGE_INFO ChangeInfo;

	ChangeInfo.pInfo=pInfo;

	GetParent()->SendMessage(WM_PROPERTY_GRID_EDIT_END,m_CurEditRow-1,(LPARAM)&pInfo);

	if(pInfo->PropertyType==PROT_CUSTOM)
	{
		if(IsChanged)
			GetParent()->SendMessage(WM_PROPERTY_GRID_VALUE_CHANGED,m_CurEditRow-1,(LPARAM)&ChangeInfo);
	}
	else
	{
		int CurRow=GetRow();
		int CurCol=GetCol();
		SetRow(m_CurEditRow);
		SetCol(m_CurEditCol);

		ChangeInfo.NewValue=m_Inputer.GetInputText();
		ChangeInfo.OldValue=pInfo->Value;
		pInfo->Value=m_Inputer.GetInputText();	
		if((CString)pInfo->Value!=GetText())
		{
			SetText((LPCTSTR)pInfo->Value);
			if(GetParent())
			{
				GetParent()->SendMessage(WM_PROPERTY_GRID_VALUE_CHANGED,m_CurEditRow-1,(LPARAM)&ChangeInfo);
			}
		}

		SetRow(CurRow);
		SetCol(CurCol);		
	}
	m_CurEditRow=0;
	m_CurEditCol=0;
}

BEGIN_EVENTSINK_MAP(CPropertyGrid, CWnd)
	ON_EVENT_REFLECT(CPropertyGrid, DISPID_CLICK, OnClickMsFlexGrid, VTS_NONE)	
END_EVENTSINK_MAP()



void CPropertyGrid::OnClickMsFlexGrid()
{	
	
	
	m_CurEditRow=GetRow();
	m_CurEditCol=GetCol();
	
	if(m_CurEditRow<=0||m_CurEditRow>m_PropertyList.GetCount())
		return;

	

	PROPERTY_INFO * pInfo=&(m_PropertyList[m_CurEditRow-1]);

	int t;
	int l;
	int h;
	int w;
	t=GetCellTop()/15-3;
	l=GetCellLeft()/15-3;
	h=GetCellHeight()/15;
	w=GetCellWidth()/15;

	m_Inputer.MoveWindow(l,t,w,h);
	m_Inputer.StartInput(pInfo->PropertyType,pInfo->Define,pInfo->Value.GetString(),pInfo->pConfigDialog);	
	GetParent()->SendMessage(WM_PROPERTY_GRID_EDIT_START,m_CurEditRow-1,(LPARAM)&pInfo);
}

}