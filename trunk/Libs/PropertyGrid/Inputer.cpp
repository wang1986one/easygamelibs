// Inputer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Inputer.h"
#include "PropertyGrid.h"

// CInputer
namespace PropertyGrid
{

IMPLEMENT_DYNAMIC(CInputer, CWnd)
CInputer::CInputer()
{
	m_Type=0;
	m_FuctionDialog=NULL;
}

CInputer::~CInputer()
{
}


BEGIN_MESSAGE_MAP(CInputer, CWnd)
	ON_BN_CLICKED(ID_FUCTION_BUTTON, OnBnClicked)
END_MESSAGE_MAP()



// CInputer ��Ϣ�������
BOOL CInputer::Create( CWnd* pParentWnd,CFont * pFont)
{
	// TODO: �ڴ����ר�ô����/����û���

	if(!CWnd::Create(NULL,"", WS_CHILD, CRect(0,0,0,0), pParentWnd,3384, NULL))
		return false;

	m_Edit.Create(WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL,CRect(0,0,0,0),this,11001);
	m_Edit.SetFont(pFont);
	m_Edit.ShowWindow(SW_HIDE);

	m_ComboBox.Create(WS_VISIBLE|WS_CHILD|CBS_DROPDOWNLIST,CRect(0,0,0,0),this,11002);
	m_ComboBox.SetFont(pFont);
	m_ComboBox.ShowWindow(SW_HIDE);

	m_Spin.Create(WS_VISIBLE|WS_CHILD|UDS_ALIGNRIGHT|UDS_SETBUDDYINT|UDS_NOTHOUSANDS,CRect(0,0,20,20),this,103883);
	m_Spin.ShowWindow(SW_HIDE);
	
	m_Button.Create("��",WS_VISIBLE|WS_CHILD|BS_NOTIFY|BS_PUSHBUTTON,CRect(0,0,20,20),this,ID_FUCTION_BUTTON);
	m_Button.ShowWindow(SW_HIDE);

	return true;
}

void CInputer::StartInput(int Type,CString& Define,CString& DefaultValue,CPropertyConfigDialog * pFuctionDialog)
{
	CRect Rect;

	m_Type=Type;
	m_FuctionDialog=pFuctionDialog;	
	switch(m_Type)
	{
	case PROT_NONE:
		break;
	case PROT_EDIT:
		{
			//���ñ༭���С
			GetWindowRect(&Rect);			
			ScreenToClient(&Rect);
			m_Edit.MoveWindow(&Rect);
			m_Edit.ShowWindow(SW_SHOW);
			m_Edit.SetWindowText((LPCTSTR)DefaultValue);
			m_Edit.SetFocus();
		}
		break;
	case PROT_NUMBER:
		{
			//���ñ༭���С
			GetWindowRect(&Rect);			
			ScreenToClient(&Rect);
			m_Edit.MoveWindow(&Rect);	
			m_Edit.ModifyStyle(0,ES_NUMBER);
			m_Edit.ShowWindow(SW_SHOW);
			m_Edit.SetWindowText((LPCTSTR)DefaultValue);
			m_Edit.SetFocus();
		}
		break;
	case PROT_SPIN_NUMBER:
		{
			//���ñ༭���С
			GetWindowRect(&Rect);			
			ScreenToClient(&Rect);
			m_Edit.MoveWindow(&Rect);	
			m_Edit.ModifyStyle(0,ES_NUMBER);
			m_Edit.ShowWindow(SW_SHOW);
			m_Edit.SetWindowText((LPCTSTR)DefaultValue);
			m_Edit.SetFocus();

			CSettingFile SettingStr;
			SettingStr.Load(Define);
			int Min=SettingStr.GetInteger("Define","Min",0);
			int Max=SettingStr.GetInteger("Define","Max",99999);
			m_Spin.ShowWindow(SW_SHOW);	
			m_Spin.SetBuddy(&m_Edit);
			m_Spin.SetRange32(Min,Max);	

			
		}
		break;
	case PROT_COMBO:
		{
		
			//������Ͽ��С
			GetWindowRect(&Rect);			
			ScreenToClient(&Rect);
			Rect.bottom=Rect.top+100;
			m_ComboBox.MoveWindow(&Rect);			

			m_ComboBox.ResetContent();
			CSettingFile SettingStr;
			SettingStr.Load((LPCTSTR)Define);
			int Count=SettingStr.GetInteger("Define","ItemCount",0);
			for(int i=0;i<Count;i++)
			{
				char key[20];
				sprintf_s(key,20,"Item%02d",i);

				CString Item=SettingStr.GetString("Define",key,"");
				m_ComboBox.InsertString(i,Item);
				if(Item==DefaultValue)
					m_ComboBox.SetCurSel(i);
				
			}
			m_ComboBox.ShowWindow(SW_SHOW);
			m_ComboBox.SetFocus();
		}

		break;
	case PROT_BOOL:
		{
			//������Ͽ��С
			GetWindowRect(&Rect);			
			ScreenToClient(&Rect);
			Rect.bottom=Rect.top+100;
			m_ComboBox.MoveWindow(&Rect);			

			m_ComboBox.ResetContent();
			m_ComboBox.InsertString(0,"��");
			m_ComboBox.InsertString(1,"��");
			if(DefaultValue=="��")
				m_ComboBox.SetCurSel(0);
			else
				m_ComboBox.SetCurSel(1);
			m_ComboBox.ShowWindow(SW_SHOW);
			m_ComboBox.SetFocus();
		}
		break;
	case PROT_COLOR:
		break;
	case PROT_FONT:
		break;
	case PROT_COLOREX:
		break;
	case PROT_DIRECT:
		break;
	case PROT_PATH:
		{			
			//���ñ༭���С
			GetWindowRect(&Rect);
			Rect.right-=Rect.Height();
			ScreenToClient(&Rect);
			m_Edit.MoveWindow(&Rect);	
			m_Edit.ShowWindow(SW_SHOW);
			m_Edit.SetWindowText((LPCTSTR)DefaultValue);
			m_Edit.SetFocus();

			//���ð�ť��С
			GetWindowRect(&Rect);
			Rect.left=Rect.right-Rect.Height();
			ScreenToClient(&Rect);
			m_Button.MoveWindow(&Rect);
			m_Button.ShowWindow(SW_SHOW);
			
		}
		break;
	case PROT_CUSTOM:
		{
			//���ñ༭���С
			GetWindowRect(&Rect);
			Rect.right-=Rect.Height();
			ScreenToClient(&Rect);
			m_Edit.MoveWindow(&Rect);	
			m_Edit.ShowWindow(SW_SHOW);
			m_Edit.SetWindowText((LPCTSTR)DefaultValue);
			m_Edit.EnableInput(false);
			m_Edit.SetFocus();

			//���ð�ť��С
			GetWindowRect(&Rect);
			Rect.left=Rect.right-Rect.Height();
			ScreenToClient(&Rect);
			m_Button.MoveWindow(&Rect);
			m_Button.ShowWindow(SW_SHOW);			
		}
		break;
	}	
	
	ShowWindow(SW_SHOW);
}

void CInputer::LostInputFocus(bool IsCheckMouse)
{
	if(IsCheckMouse)
	{	
		if(m_Spin.IsWindowVisible())
		{
			CPoint Point;
			GetCursorPos(&Point);
			CRect Rect;
			m_Spin.GetWindowRect(&Rect);
			if(Rect.PtInRect(Point))
				return;
		}
		if(m_Button.IsWindowVisible())
		{
			CPoint Point;
			GetCursorPos(&Point);
			CRect Rect;
			m_Button.GetWindowRect(&Rect);
			if(Rect.PtInRect(Point))
				return;
		}
		
		if(m_ComboBox.IsWindowVisible())
		{
			CPoint Point;
			GetCursorPos(&Point);
			CRect Rect;
			m_ComboBox.GetWindowRect(&Rect);
			if(Rect.PtInRect(Point))
				return;
		}

		if(m_Edit.IsWindowVisible())
		{
			CPoint Point;
			GetCursorPos(&Point);
			CRect Rect;
			m_Edit.GetWindowRect(&Rect);
			if(Rect.PtInRect(Point))
				return;
		}
	}
	if(m_Type==PROT_CUSTOM)
		EndInput(false);
	else
		EndInput(true);
}

void CInputer::EndInput(bool IsChanged)
{
	((CPropertyGrid *)GetParent())->OnFinishEdit(IsChanged);
	ShowWindow(SW_HIDE);
	m_Edit.ShowWindow(SW_HIDE);
	m_ComboBox.ShowWindow(SW_HIDE);
	m_Button.ShowWindow(SW_HIDE);
	m_Spin.ShowWindow(SW_HIDE);
	m_Edit.ModifyStyle(ES_NUMBER,0);
	m_Edit.EnableInput(true);

}

void CInputer::OnBnClicked()
{
	switch(m_Type)
	{
	case PROT_NONE:
		break;
	case PROT_EDIT:
		break;
	case PROT_NUMBER:
		break;
	case PROT_SPIN_NUMBER:
		break;
	case PROT_COMBO:
		break;
	case PROT_BOOL:
		break;
	case PROT_COLOR:
		break;
	case PROT_FONT:
		break;
	case PROT_COLOREX:
		break;
	case PROT_DIRECT:
		break;
	case PROT_PATH:
		{
			CFileDialog dlg(false);

			if(dlg.DoModal()==IDOK)
			{
				m_Edit.SetWindowText((LPCTSTR)dlg.GetPathName());				
			}
			
		}
		break;
	case PROT_CUSTOM:
		{
			if(m_FuctionDialog)
			{
				if(m_FuctionDialog->IsModual())
				{
					if(m_FuctionDialog->DoModal()==IDOK)
						EndInput(true);
					else
						EndInput(false);
				}
				else
				{
					//m_FuctionDialog->OnInitDialog();
					m_FuctionDialog->ShowWindow(SW_SHOW);
					EndInput(false);
				}
			}
		}
		break;
	}
	m_Edit.SetFocus();
}

CString CInputer::GetInputText()
{
	CString Text;
	switch(m_Type)
	{
	case PROT_NONE:
		break;
	case PROT_EDIT:		
	case PROT_NUMBER:		
	case PROT_SPIN_NUMBER:
	case PROT_COLOR:		
	case PROT_FONT:		
	case PROT_COLOREX:		
	case PROT_DIRECT:		
	case PROT_PATH:		
	case PROT_CUSTOM:
		m_Edit.GetWindowText(Text);
		break;
	case PROT_COMBO:		
	case PROT_BOOL:
		m_ComboBox.GetWindowText(Text);
		break;
	
		
	}

	
	
	return Text;
}

}

