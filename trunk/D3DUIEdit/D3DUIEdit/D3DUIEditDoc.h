/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DUIEditDoc.h                                           */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// D3DUIEditDoc.h :  CD3DUIEditDoc ��Ľӿ�
//
#include "ObjectView.h"

#include "D3DView.h"
#pragma once

class CD3DUIEditDoc : public CDocument
{
protected: // �������л�����
	CD3DUIEditDoc();
	DECLARE_DYNCREATE(CD3DUIEditDoc)

// ����
public:

// ����
public:

// ��д
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CD3DUIEditDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:	
	void Init();

protected:
	CD3DGUI *				m_pGUI;
	CObjectTreeView *		m_pObjectTreeView;
	CObjectPropertyView *	m_pObjectPropertyView;
	CD3DViewPort *			m_pViewPort;
	CLASS_INFO				m_CurNewObject;

public:
	CLASS_INFO& GetCurNewObject()
	{
		return m_CurNewObject;
	}

	void SetCurNewObject(CLASS_INFO&  ClassInfo)
	{
		m_CurNewObject=ClassInfo;
	}

	void AddGUIObject(CD3DWnd * pWnd);

	CObjectTreeView * GetObjectTreeView()
	{
		return m_pObjectTreeView;
	}

	CObjectPropertyView * GetObjectPropertyView()
	{
		return m_pObjectPropertyView;
	}

	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);



	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);	
	afx_msg void OnNewWnd();
	afx_msg void OnUpdateNewWnd(CCmdUI *pCmdUI);
	afx_msg void OnNewButton();
	afx_msg void OnUpdateNewButton(CCmdUI *pCmdUI);
	afx_msg void OnNewEdit();
	afx_msg void OnUpdateNewEdit(CCmdUI *pCmdUI);
	afx_msg void OnNewSimplelist();
	afx_msg void OnUpdateNewSimplelist(CCmdUI *pCmdUI);
	afx_msg void OnNewCombo();
	afx_msg void OnUpdateNewCombo(CCmdUI *pCmdUI);
	afx_msg void OnNewScroll();
	afx_msg void OnUpdateNewScroll(CCmdUI *pCmdUI);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnAlignLeft();
	afx_msg void OnAlignRight();
	afx_msg void OnAlignTop();
	afx_msg void OnAlignBotton();
	afx_msg void OnVcenter();
	afx_msg void OnHcenter();
	afx_msg void OnHarrange();
	afx_msg void OnVarrange();
	afx_msg void OnSameWidth();
	afx_msg void OnSameHeight();
	afx_msg void OnSameSize();
	afx_msg void OnSetParent();
	afx_msg void OnNewProgressbar();
	afx_msg void OnUpdateNewProgressbar(CCmdUI *pCmdUI);
	afx_msg void OnEditDelete();
	afx_msg void OnEditMoveLeft();
	afx_msg void OnEditMoveRight();
	afx_msg void OnEditMoveUp();
	afx_msg void OnEditMoveDown();
	afx_msg void OnEditHorizontalShrink();
	afx_msg void OnEditHorizontalStrengthen();
	afx_msg void OnEditVerticalShrink();
	afx_msg void OnEditVerticalStrengthen();
};


