/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DUIEditDoc.cpp                                         */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// D3DUIEditDoc.cpp :  CD3DUIEditDoc ���ʵ��
//

#include "stdafx.h"
#include "D3DUIEdit.h"

#include "D3DUIEditDoc.h"
#include ".\d3duieditdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CD3DUIEditDoc

IMPLEMENT_DYNCREATE(CD3DUIEditDoc, CDocument)

BEGIN_MESSAGE_MAP(CD3DUIEditDoc, CDocument)	
	ON_COMMAND(ID_NEW_WND, OnNewWnd)
	ON_UPDATE_COMMAND_UI(ID_NEW_WND, OnUpdateNewWnd)
	ON_COMMAND(ID_NEW_BUTTON, OnNewButton)
	ON_UPDATE_COMMAND_UI(ID_NEW_BUTTON, OnUpdateNewButton)
	ON_COMMAND(ID_NEW_EDIT, OnNewEdit)
	ON_UPDATE_COMMAND_UI(ID_NEW_EDIT, OnUpdateNewEdit)
	ON_COMMAND(ID_NEW_SIMPLELIST, OnNewSimplelist)
	ON_UPDATE_COMMAND_UI(ID_NEW_SIMPLELIST, OnUpdateNewSimplelist)
	ON_COMMAND(ID_NEW_COMBO, OnNewCombo)
	ON_UPDATE_COMMAND_UI(ID_NEW_COMBO, OnUpdateNewCombo)
	ON_COMMAND(ID_NEW_SCROLL, OnNewScroll)
	ON_UPDATE_COMMAND_UI(ID_NEW_SCROLL, OnUpdateNewScroll)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_ALIGN_LEFT, OnAlignLeft)
	ON_COMMAND(ID_ALIGN_RIGHT, OnAlignRight)
	ON_COMMAND(ID_ALIGN_TOP, OnAlignTop)
	ON_COMMAND(ID_ALIGN_BOTTON, OnAlignBotton)
	ON_COMMAND(ID_VCENTER, OnVcenter)
	ON_COMMAND(ID_HCENTER, OnHcenter)
	ON_COMMAND(ID_HARRANGE, OnHarrange)
	ON_COMMAND(ID_VARRANGE, OnVarrange)
	ON_COMMAND(ID_SAME_WIDTH, OnSameWidth)
	ON_COMMAND(ID_SAME_HEIGHT, OnSameHeight)
	ON_COMMAND(ID_SAME_SIZE, OnSameSize)
	ON_COMMAND(ID_SET_PARENT, OnSetParent)
	ON_COMMAND(ID_NEW_PROGRESSBAR, OnNewProgressbar)
	ON_UPDATE_COMMAND_UI(ID_NEW_PROGRESSBAR, OnUpdateNewProgressbar)
	ON_COMMAND(ID_EDIT_DELETE, &CD3DUIEditDoc::OnEditDelete)
	ON_COMMAND(ID_EDIT_MOVE_LEFT, &CD3DUIEditDoc::OnEditMoveLeft)
	ON_COMMAND(ID_EDIT_MOVE_RIGHT, &CD3DUIEditDoc::OnEditMoveRight)
	ON_COMMAND(ID_EDIT_MOVE_UP, &CD3DUIEditDoc::OnEditMoveUp)
	ON_COMMAND(ID_EDIT_MOVE_DOWN, &CD3DUIEditDoc::OnEditMoveDown)
	ON_COMMAND(ID_EDIT_HORIZONTAL_SHRINK, &CD3DUIEditDoc::OnEditHorizontalShrink)
	ON_COMMAND(ID_EDIT_HORIZONTAL_STRENGTHEN, &CD3DUIEditDoc::OnEditHorizontalStrengthen)
	ON_COMMAND(ID_EDIT_VERTICAL_SHRINK, &CD3DUIEditDoc::OnEditVerticalShrink)
	ON_COMMAND(ID_EDIT_VERTICAL_STRENGTHEN, &CD3DUIEditDoc::OnEditVerticalStrengthen)
END_MESSAGE_MAP()


// CD3DUIEditDoc ����/����

CD3DUIEditDoc::CD3DUIEditDoc()
{
	// TODO: �ڴ����һ���Թ������
	m_CurNewObject=GET_CLASS_INFO(CNameObject);
	m_pViewPort=NULL;
	m_pGUI=NULL;
}

CD3DUIEditDoc::~CD3DUIEditDoc()
{
}

BOOL CD3DUIEditDoc::OnNewDocument()
{
	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)
	Init();
	return TRUE;
}




// CD3DUIEditDoc ���л�

void CD3DUIEditDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CD3DUIEditDoc ���

#ifdef _DEBUG
void CD3DUIEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CD3DUIEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CD3DUIEditDoc ����


void CD3DUIEditDoc::Init()
{
	POSITION pos=GetFirstViewPosition();
	while(pos)
	{
		CView * pView=GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CD3DView)))
		{			
			m_pGUI=((CD3DView *)pView)->GetGUI();
			m_pViewPort=((CD3DView *)pView)->GetViewPort();		
			
		}
		if(pView->IsKindOf(RUNTIME_CLASS(CObjectTreeView)))
		{
			m_pObjectTreeView=(CObjectTreeView *)pView;
		}
		if(pView->IsKindOf(RUNTIME_CLASS(CObjectPropertyView)))
		{
			m_pObjectPropertyView=(CObjectPropertyView *)pView;
		}
	}
}

BOOL CD3DUIEditDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	CString newName = lpszPathName;
	if (newName.IsEmpty())
	{

		CFileDialog Dlg(false,_T("*.UI"),m_strTitle,OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST,
			_T("UI Script Files(*.UI)|*.UI|Universal System Object Files (*.USO)|*.USO|All Files(*.*)|*.*||"));
		if(Dlg.DoModal()==IDOK)
		{
			newName=Dlg.GetPathName();
		}
		else
		{
			return FALSE;
		}	
	}

	CWaitCursor wait;

	if (!OnSaveDocument(newName))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			TRY
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE(traceAppMsg, 0, "Warning: failed to delete file after failed SaveAs.\n");
				do 
				{ 
					if(e) 
					{ 
						e->Delete(); 
					} 
				} while (0);
			}
			END_CATCH_ALL
		}
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
		SetPathName(newName);

	return TRUE;        // success
}

BOOL CD3DUIEditDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	

	// TODO:  �ڴ������ר�õĴ�������
	Init();
	m_pGUI->Clear();
	CEasyString FileName=lpszPathName;
	FileName.MakeUpper();
	if(FileName.Find(_T(".USO"))>=0)
	{
		CUSOResourceManager USOFile;
		CUSOGUIObjectCreateFilter GUIFilter(m_pGUI);
		CUSOD3DObjectCreateFilter D3DFilter(m_pViewPort->GetRender()->GetDevice());

		USOFile.AddObjectCreateFilter(&GUIFilter);
		USOFile.AddObjectCreateFilter(&D3DFilter);
		if(USOFile.Import(lpszPathName))
		{
			for(int i=0;i<USOFile.GetObjectCount();i++)
			{
				USOFile.GetObjectByIndex(i)->AddUseRef();
			}
			return true;
		}
		else
		{
			AfxMessageBox(_T("�ļ���ʽ����"));
		}

	}
	else
	{	
		CEasyString UIPath=GetPathDirectory(MakeFullPath(lpszPathName));
		CD3DTexture::AddPath(UIPath);
		if(!m_pGUI->LoadFromFile(lpszPathName))
		{
			AfxMessageBox(_T("�ļ���ʽ����"));
			return false;
		}
		return true;
	}
	return false;
}


void CD3DUIEditDoc::OnNewWnd()
{
	// TODO: �ڴ���������������
	if(m_CurNewObject==GET_CLASS_INFO(CD3DWnd))
		m_CurNewObject=GET_CLASS_INFO(CNameObject);
	else
		m_CurNewObject=GET_CLASS_INFO(CD3DWnd);
}

void CD3DUIEditDoc::OnUpdateNewWnd(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_CurNewObject==GET_CLASS_INFO(CD3DWnd))
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CD3DUIEditDoc::OnNewButton()
{
	// TODO: �ڴ���������������
	if(m_CurNewObject==GET_CLASS_INFO(CD3DButton))
		m_CurNewObject=GET_CLASS_INFO(CNameObject);
	else
		m_CurNewObject=GET_CLASS_INFO(CD3DButton);
}

void CD3DUIEditDoc::OnUpdateNewButton(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_CurNewObject==GET_CLASS_INFO(CD3DButton))
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CD3DUIEditDoc::OnNewEdit()
{
	// TODO: �ڴ���������������
	if(m_CurNewObject==GET_CLASS_INFO(CD3DEdit))
		m_CurNewObject=GET_CLASS_INFO(CNameObject);
	else
		m_CurNewObject=GET_CLASS_INFO(CD3DEdit);
}

void CD3DUIEditDoc::OnUpdateNewEdit(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_CurNewObject==GET_CLASS_INFO(CD3DEdit))
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CD3DUIEditDoc::OnNewSimplelist()
{
	// TODO: �ڴ���������������
	if(m_CurNewObject==GET_CLASS_INFO(CD3DSimpleList))
		m_CurNewObject=GET_CLASS_INFO(CNameObject);
	else
		m_CurNewObject=GET_CLASS_INFO(CD3DSimpleList);
}

void CD3DUIEditDoc::OnUpdateNewSimplelist(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_CurNewObject==GET_CLASS_INFO(CD3DSimpleList))
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CD3DUIEditDoc::OnNewCombo()
{
	// TODO: �ڴ���������������
	if(m_CurNewObject==GET_CLASS_INFO(CD3DCombo))
		m_CurNewObject=GET_CLASS_INFO(CNameObject);
	else
		m_CurNewObject=GET_CLASS_INFO(CD3DCombo);
}

void CD3DUIEditDoc::OnUpdateNewCombo(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_CurNewObject==GET_CLASS_INFO(CD3DCombo))
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CD3DUIEditDoc::OnNewScroll()
{
	// TODO: �ڴ���������������
	if(m_CurNewObject==GET_CLASS_INFO(CD3DScrollBar))
		m_CurNewObject=GET_CLASS_INFO(CNameObject);
	else
		m_CurNewObject=GET_CLASS_INFO(CD3DScrollBar);
}

void CD3DUIEditDoc::OnUpdateNewScroll(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_CurNewObject==GET_CLASS_INFO(CD3DScrollBar))
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CD3DUIEditDoc::OnNewProgressbar()
{
	// TODO: �ڴ���������������	
	if(m_CurNewObject==GET_CLASS_INFO(CD3DProgressBar))
		m_CurNewObject=GET_CLASS_INFO(CNameObject);
	else
		m_CurNewObject=GET_CLASS_INFO(CD3DProgressBar);
}

void CD3DUIEditDoc::OnUpdateNewProgressbar(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_CurNewObject==GET_CLASS_INFO(CD3DProgressBar))
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CD3DUIEditDoc::AddGUIObject(CD3DWnd * pWnd)
{
	if(m_pObjectTreeView)
		m_pObjectTreeView->AddGUIObject(pWnd);
	if(pWnd->IsKindOf(GET_CLASS_INFO(CD3DSizeBar)))
		((CD3DSizeBar *)pWnd)->SetObjectPropertyView(m_pObjectPropertyView);
}
BOOL CD3DUIEditDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: �ڴ����ר�ô����/����û���
	CEasyString SaveFileName=lpszPathName;
	SaveFileName.MakeUpper();
	if(SaveFileName.Find(_T(".USO"))>=0)
	{	
		return m_pGUI->SaveToUSOFile(lpszPathName);
	}
	else
	{	
		return m_pGUI->SaveToFile(lpszPathName);
	}
	//return CDocument::OnSaveDocument(lpszPathName);
}

void CD3DUIEditDoc::OnCloseDocument()
{
	// TODO: �ڴ����ר�ô����/����û���
	m_pGUI->Clear();
	CDocument::OnCloseDocument();
}

void CD3DUIEditDoc::OnEditCut()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
		m_pViewPort->Cut();
}

void CD3DUIEditDoc::OnEditCopy()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
		m_pViewPort->Copy();
}

void CD3DUIEditDoc::OnEditPaste()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
		m_pViewPort->Paste();
}

void CD3DUIEditDoc::OnAlignLeft()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
		m_pViewPort->AlignLeft();
}

void CD3DUIEditDoc::OnAlignRight()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
		m_pViewPort->AlignRight();
}

void CD3DUIEditDoc::OnAlignTop()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
		m_pViewPort->AlignTop();
}

void CD3DUIEditDoc::OnAlignBotton()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
		m_pViewPort->AlignBottom();
}

void CD3DUIEditDoc::OnVcenter()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
		m_pViewPort->VCenter();
}

void CD3DUIEditDoc::OnHcenter()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
		m_pViewPort->HCenter();
}

void CD3DUIEditDoc::OnHarrange()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
		m_pViewPort->HArrange();
}

void CD3DUIEditDoc::OnVarrange()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
		m_pViewPort->VArrange();
}

void CD3DUIEditDoc::OnSameWidth()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
		m_pViewPort->SameWidth();
}

void CD3DUIEditDoc::OnSameHeight()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
		m_pViewPort->SameHeight();
}

void CD3DUIEditDoc::OnSameSize()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
		m_pViewPort->SameSize();
}

void CD3DUIEditDoc::OnSetParent()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
		m_pViewPort->SetParent();
}



void CD3DUIEditDoc::OnEditDelete()
{
	// TODO: �ڴ���������������
	
	{
		if(m_pViewPort)
			m_pViewPort->GetSizeBarManager()->DeleteAllSelectedObject();
	}
}

void CD3DUIEditDoc::OnEditMoveLeft()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
	{
		m_pViewPort->GetSizeBarManager()->DoMove(-1,0);
		if(m_pObjectPropertyView)
			m_pObjectPropertyView->UpdateObjectRect();
	}
}

void CD3DUIEditDoc::OnEditMoveRight()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
	{
		m_pViewPort->GetSizeBarManager()->DoMove(1,0);
		if(m_pObjectPropertyView)
			m_pObjectPropertyView->UpdateObjectRect();
	}
}

void CD3DUIEditDoc::OnEditMoveUp()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
	{
		m_pViewPort->GetSizeBarManager()->DoMove(0,-1);
		if(m_pObjectPropertyView)
			m_pObjectPropertyView->UpdateObjectRect();
	}
}

void CD3DUIEditDoc::OnEditMoveDown()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
	{
		m_pViewPort->GetSizeBarManager()->DoMove(0,1);
		if(m_pObjectPropertyView)
			m_pObjectPropertyView->UpdateObjectRect();
	}
}

void CD3DUIEditDoc::OnEditHorizontalShrink()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
	{
		m_pViewPort->GetSizeBarManager()->DoResize(-1,0);
		if(m_pObjectPropertyView)
			m_pObjectPropertyView->UpdateObjectRect();
	}
}

void CD3DUIEditDoc::OnEditHorizontalStrengthen()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
	{
		m_pViewPort->GetSizeBarManager()->DoResize(1,0);
		if(m_pObjectPropertyView)
			m_pObjectPropertyView->UpdateObjectRect();
	}
}

void CD3DUIEditDoc::OnEditVerticalShrink()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
	{
		m_pViewPort->GetSizeBarManager()->DoResize(0,-1);
		if(m_pObjectPropertyView)
			m_pObjectPropertyView->UpdateObjectRect();
	}
}

void CD3DUIEditDoc::OnEditVerticalStrengthen()
{
	// TODO: �ڴ���������������
	if(m_pViewPort)
	{
		m_pViewPort->GetSizeBarManager()->DoResize(0,1);
		if(m_pObjectPropertyView)
			m_pObjectPropertyView->UpdateObjectRect();
	}
}
