/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ObjectView.h                                             */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "ObjectTreeView.h"
#include "ObjectPropertyView.h"

// CObjectView ��ͼ
class CD3DUIEditDoc;

class CObjectView : public CView
{
	DECLARE_DYNCREATE(CObjectView)

protected:
	CObjectView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CObjectView();
protected:
	CSplitterWnd m_wndSplitter;

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��ƴ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

public:
	inline CD3DUIEditDoc* GetDocument() const
	{ 
		return reinterpret_cast<CD3DUIEditDoc*>(m_pDocument); 
	}
protected:
	CObjectTreeView * m_pObjectTreeView;
	CObjectPropertyView * m_pObjectPropertyView;
};


